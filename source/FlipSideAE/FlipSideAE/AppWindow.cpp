#include <stdlib.h>
#include <be/app/Application.h>
#include <be/app/Message.h>
#include <be/interface/Alert.h>
#include <be/interface/Menu.h>
#include <be/interface/MenuBar.h>
#include <be/interface/MenuItem.h>
#include <be/kernel/fs_info.h>
#include <be/kernel/image.h>
#include <be/storage/Directory.h>
#include <be/storage/Entry.h>
#include <be/storage/File.h>
#include <be/storage/FindDirectory.h>
#include <be/storage/NodeMonitor.h>
#include <be/storage/Path.h>
#include <be/storage/Volume.h>
#include <be/storage/VolumeRoster.h>
#include <be/support/Debug.h>
#include <be/support/String.h>
#include <AEEncoder/AEEncoder.h>
#include "AppDefs.h"
#include "AppView.h"
#include "AppWindow.h"
#include "CommandConstants.h"
#include "GUIStrings.h"
#include "Settings.h"

#define WIN_LEFT        200
#define WIN_TOP         200
#define WIN_RIGHT       810
#define WIN_BOTTOM      600
#define WIN_MIN_WIDTH   610
#define WIN_MAX_WIDTH  2000
#define WIN_MIN_HEIGHT  400
#define WIN_MAX_HEIGHT 2000

AppWindow::AppWindow() : BWindow(BRect(WIN_LEFT,WIN_TOP,WIN_RIGHT,WIN_BOTTOM),
	APPLICATION,B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS)
{
	PRINT(("AppWindow::AppWindow()\n"));

	InitWindow();
}

AppWindow::AppWindow(BMessage* archive) : BWindow(archive)
{
	PRINT(("AppWindow::AppWindow(BMessage*)\n"));

	InitWindow();
}

AppWindow::~AppWindow()
{
	PRINT(("AppWindow::~AppWindow()\n"));

	stop_watching(this);
	delete volumes;


	SaveWindow();
}

void
AppWindow::InitWindow()
{
	PRINT(("AppWindow::InitWindow()\n"));

	Settings::OpenSettings();

	PRINT_OBJECT(*settings);

	encoderAddon = 0;

	SetTitle(APPLICATION);
	SetType(B_TITLED_WINDOW);
	SetFlags(B_ASYNCHRONOUS_CONTROLS);
	SetSizeLimits(WIN_MIN_WIDTH,WIN_MAX_WIDTH,WIN_MIN_HEIGHT,WIN_MAX_HEIGHT);
	SetWorkspaces(B_CURRENT_WORKSPACE);

	node_ref addon_ref;
	BDirectory addonDir(settings->AddOnsDirectory());
	addonDir.GetNodeRef(&addon_ref);
	watch_node(&addon_ref,B_WATCH_DIRECTORY,this);

	windowMessenger = new BMessenger(this);
	volumes = new BVolumeRoster();
	BVolume vol;
	volumes->Rewind();
	while(volumes->GetNextVolume(&vol) == B_NO_ERROR)
	{
		fs_info info;
		fs_stat_dev(vol.Device(),&info);
		if(strcmp(info.fsh_name,"cdda") == 0)
		{
			BDirectory root;
			vol.GetRootDirectory(&root);
			node_ref nref;
			root.GetNodeRef(&nref);
			watch_node(&nref,B_WATCH_NAME,this);
		}
	}

	volumes->StartWatching(*windowMessenger);

	InitMenus();

	BRect frame = Bounds();
	frame.top += (menuBar->Bounds()).Height() + 1;

	appView = new AppView(frame);
	AddChild(appView);
	frame = appView->Frame();
	if(Bounds().right < frame.right)
	{
		ResizeBy((frame.right - Bounds().right),0);
	}
	viewMessenger = new BMessenger(appView);

	BMessage* viewShortcut = 0;
	viewShortcut = new BMessage(VIEW_SHORTCUT);
	viewShortcut->AddInt32("view",0);
	AddShortcut('F',0,viewShortcut,appView);

	viewShortcut = new BMessage(VIEW_SHORTCUT);
	viewShortcut->AddInt32("view",1);
	AddShortcut('E',0,viewShortcut,appView);

	viewShortcut = new BMessage(VIEW_SHORTCUT);
	viewShortcut->AddInt32("view",2);
	AddShortcut('L',0,viewShortcut,appView);
}

BMenuBar*
AppWindow::MenuBar()
{
	PRINT(("AppWindow::MenuBar()\n"));

	return menuBar;
}

void
AppWindow::InitMenus()
{
	PRINT(("AppWindow::InitMenus()\n"));

	menuBar = new BMenuBar(BRect(),"menuBar");

	BMenu* fileMenu = new BMenu(FILE_MENU);
	BMenu* editMenu = new BMenu(EDIT_MENU);
	BMenu* columnMenu = new BMenu(COLUMN_MENU);

	loadCdMenu = new BMenu(LOAD_CD_SUBMENU);
	LoadCDMenu();
	fileMenu->AddItem(loadCdMenu);
	fileMenu->AddSeparatorItem();
	fileMenu->AddItem(new BMenuItem(ABOUT_MENU_ITEM,
				new BMessage(B_ABOUT_REQUESTED),'A',B_OPTION_KEY));
	fileMenu->AddSeparatorItem();
	fileMenu->AddItem(new BMenuItem(QUIT_MENU_ITEM,
				new BMessage(B_QUIT_REQUESTED),'Q'));

	editMenu->AddItem(new BMenuItem(SELECT_ALL,
				new BMessage(MENU_ITEM_SELECTED),'A'));
	editMenu->AddItem(new BMenuItem(DESELECT_ALL,
				new BMessage(MENU_ITEM_SELECTED),'D'));
	editMenu->AddItem(new BMenuItem(REMOVE,
				new BMessage(MENU_ITEM_SELECTED),'R'));

	columnMenu->AddItem(new BMenuItem(FILE_COLUMN,
				new BMessage(MENU_ITEM_SELECTED),'1'));
	columnMenu->AddItem(new BMenuItem(SAVE_AS_COLUMN,
				new BMessage(MENU_ITEM_SELECTED),'2'));
	columnMenu->AddItem(new BMenuItem(ARTIST_COLUMN,
				new BMessage(MENU_ITEM_SELECTED),'3'));
	columnMenu->AddItem(new BMenuItem(ALBUM_COLUMN,
				new BMessage(MENU_ITEM_SELECTED),'4'));
	columnMenu->AddItem(new BMenuItem(TITLE_COLUMN,
				new BMessage(MENU_ITEM_SELECTED),'5'));
	columnMenu->AddItem(new BMenuItem(YEAR_COLUMN,
				new BMessage(MENU_ITEM_SELECTED),'6'));
	columnMenu->AddItem(new BMenuItem(COMMENT_COLUMN,
				new BMessage(MENU_ITEM_SELECTED),'7'));
	columnMenu->AddItem(new BMenuItem(TRACK_COLUMN,
				new BMessage(MENU_ITEM_SELECTED),'8'));
	columnMenu->AddItem(new BMenuItem(GENRE_COLUMN,
				new BMessage(MENU_ITEM_SELECTED),'9'));

	bool* columnsShown = settings->ColumnsShown();
	for(int i=1;i<NUM_OF_COLUMNS;i++)
	{
		BMenuItem* item = columnMenu->ItemAt(i-1);
		item->SetMarked(columnsShown[i]);
	}

	encoderMenu = new BMenu(ENCODER_MENU);
	encoderMenu->SetRadioMode(true);

	menuBar->AddItem(fileMenu);
	menuBar->AddItem(editMenu);
	menuBar->AddItem(columnMenu);
	menuBar->AddItem(encoderMenu);
	LoadEncoderMenu();

	AddChild(menuBar);
	SetKeyMenuBar(menuBar);
}

void
AppWindow::LoadCDMenu()
{
	PRINT(("AppWindow::LoadCDMenu()\n"));

	int32 numItems = loadCdMenu->CountItems();
	for(int32 i=0;i<numItems;i++)
	{
		BMenuItem* item = loadCdMenu->RemoveItem((int32)0);
		delete item;
	}

	BVolume volume;
	volumes->Rewind();
	char shortcut = '1';
	while(volumes->GetNextVolume(&volume) == B_NO_ERROR)
	{
		fs_info info;
		fs_stat_dev(volume.Device(),&info);
		if(strcmp(info.fsh_name,"cdda") == 0)
		{
			BMessage* menuMessage = new BMessage(MENU_ITEM_SELECTED);
			menuMessage->AddInt32("dev",volume.Device());
			loadCdMenu->AddItem(new BMenuItem(info.volume_name,menuMessage,
						shortcut,B_CONTROL_KEY));
			shortcut++;
		}
	}

	numItems = loadCdMenu->CountItems();
	if(numItems == 0)
	{
		loadCdMenu->AddItem(new BMenuItem(NO_CD_MOUNTED,NULL));
	}
}

void
AppWindow::LoadEncoderMenu()
{
	PRINT(("AppWindow::LoadEncoderMenu()\n"));

	if(!settings->IsEncoding())
	{
		if(encoderAddon > 0)
		{
			AEEncoder* encoder = settings->Encoder();
			if(encoder)
			{
				if(menuBar->RemoveItem(encoder->GetMenu()))
				{
					PRINT(("Error removing encoder menu.\n"));
				}
				settings->SetEncoder(NULL);
				unload_add_on(encoderAddon);
				encoderAddon = 0;
			}
		}

		int32 numItems = encoderMenu->CountItems();
		for(int32 i=0;i<numItems;i++)
		{
			BMenuItem* item = encoderMenu->RemoveItem((int32)0);
			delete item;
		}

		BDirectory addonDir(settings->AddOnsDirectory());
		if(addonDir.InitCheck() == B_OK)
		{
			BEntry entry;
			addonDir.Rewind();
			char shortcut = '1';
			image_id addon = 0;
			while(addonDir.GetNextEntry(&entry) == B_OK)
			{
				if(entry.IsFile())
				{
					BMessage* itemMsg = new BMessage(MENU_ITEM_SELECTED);
					entry_ref ref;
					entry.GetRef(&ref);
					itemMsg->AddRef("addon",&ref);
					BPath path(&ref);
					addon = load_add_on(path.Path());
					if(addon > 0)
					{
						AEEncoder* (*load_encoder)();
						if(get_image_symbol(addon,"load_encoder",
									B_SYMBOL_TYPE_TEXT,(void**)&load_encoder) == B_OK)
						{
							AEEncoder* encoder = (*load_encoder)();
							encoderMenu->AddItem(new BMenuItem(encoder->GetName(),
										itemMsg,shortcut,B_OPTION_KEY));
							shortcut++;
							delete encoder;
						}
						unload_add_on(addon);
					}
				}
			}
		}

		BMenuItem* item = encoderMenu->FindItem(settings->EncoderName());
		if(item)
		{
			item->SetMarked(true);
		}
		else
		{
			item = encoderMenu->ItemAt(0);
			if(item)
			{
				item->SetMarked(true);
			}
		}

		item = encoderMenu->FindMarked();
		if(item)
		{
			BMessage* refMsg = item->Message();
			entry_ref ref;
			if(refMsg->FindRef("addon",&ref) == B_OK)
			{
				PRINT(("FOUND ADDON REF\n"));
				BPath path(&ref);
				if(path.InitCheck() == B_OK)
				{
					PRINT(("GOOD ADDON PATH\n"));
					encoderAddon = load_add_on(path.Path());
					if(encoderAddon > 0)
					{
						PRINT(("LOADED ADDON\n"));
						AEEncoder* (*load_encoder)();
						if(get_image_symbol(encoderAddon,"load_encoder",
									B_SYMBOL_TYPE_TEXT,(void**)&load_encoder) == B_OK)
						{
							PRINT(("FOUND load_encoder\n"));
							AEEncoder* encoder = (*load_encoder)();
							settings->SetEncoder(encoder);
							menuBar->AddItem(new BMenuItem(encoder->GetMenu()));
						}
					}
				}
			}
		}

		if(encoderMenu->CountItems() == 0)
		{
			encoderMenu->AddItem(new BMenuItem(NO_AVAILABLE_ENCODERS,NULL));
		}
	}
}

void
AppWindow::MenuItemSelected(BMessage* message)
{
	PRINT(("AppWindow::MenuItemSelected(BMessage*)\n"));

	BMenuItem* item;
	message->FindPointer("source",(void**)&item);
	BMenu* menu = item->Menu();

	if(menu)
	{
		if(strcmp(menu->Name(),FILE_MENU) == 0)
		{
		}
		else if(strcmp(menu->Name(),EDIT_MENU) == 0)
		{
			if(strcmp(item->Label(),SELECT_ALL) == 0)
			{
				BMessage msg(SELECT_ALL_MSG);
				viewMessenger->SendMessage(&msg);
			}
			else if(strcmp(item->Label(),DESELECT_ALL) == 0)
			{
				BMessage msg(DESELECT_ALL_MSG);
				viewMessenger->SendMessage(&msg);
			}
			else if(strcmp(item->Label(),REMOVE) == 0)
			{
				BMessage msg(REMOVE_MSG);
				viewMessenger->SendMessage(&msg);
			}
		}
		else if(strcmp(menu->Name(),LOAD_CD_SUBMENU) == 0)
		{
			PRINT(("CD SELECTED: %s\n",item->Label()));
			dev_t dev;
			if(message->FindInt32("dev",&dev) == B_OK)
			{
				AddVolumeToList(dev);
			}
			else
			{
				AddVolumeToList(item->Label());
			}
		}
		else if(strcmp(menu->Name(),COLUMN_MENU) == 0)
		{
			item->SetMarked(!item->IsMarked());
			BMessage msg(COLUMN_SET_SHOWN);
			msg.AddBool("show",item->IsMarked());
			int32 column = menu->IndexOf(item) + 1;
			msg.AddInt32("column",column);
			viewMessenger->SendMessage(&msg);
		}
		else if(strcmp(menu->Name(),ENCODER_MENU) == 0)
		{
			if(!settings->IsEncoding())
			{
				PRINT(("ENCODER SELECTED: %s\n",item->Label()));

				if(encoderAddon > 0)
				{
					AEEncoder* encoder = settings->Encoder();
					if(encoder)
					{
						if(menuBar->RemoveItem(encoder->GetMenu()))
						{
							PRINT(("Error removing encoder menu.\n"));
						}
						settings->SetEncoder(NULL);
						unload_add_on(encoderAddon);
						encoderAddon = 0;
					}
				}
				entry_ref ref;
				if(message->FindRef("addon",&ref) == B_OK)
				{
					BPath path(&ref);
					if(path.InitCheck() == B_OK)
					{
						encoderAddon = load_add_on(path.Path());
						if(encoderAddon > 0)
						{
							AEEncoder* (*load_encoder)();
							if(get_image_symbol(encoderAddon,"load_encoder",
										B_SYMBOL_TYPE_TEXT,(void**)&load_encoder) == B_OK)
							{
								AEEncoder* encoder = (*load_encoder)();
								settings->SetEncoder(encoder);
								menuBar->AddItem(encoder->GetMenu());
							}
						}
					}
				}
				BMessage msg(ENCODER_CHANGED);
				viewMessenger->SendMessage(&msg);
			}
		}
		else
		{
			if(!settings->IsEncoding())
			{
				item->SetMarked(!item->IsMarked());
			}
		}
	}
}

void
AppWindow::AddVolumeToList(const char* name)
{
	PRINT(("AppWindow::AddVolumeToList(const char*)\n"));

	volumes->Rewind();
	BVolume volume;
	while(volumes->GetNextVolume(&volume) == B_OK)
	{
		char volume_name[B_FILE_NAME_LENGTH];
		volume.GetName(volume_name);
		if(strcmp(volume_name,name) == 0)
		{
			AddVolumeToList(volume.Device());
			break;
		}
	}
}

void
AppWindow::AddVolumeToList(dev_t device)
{
	PRINT(("AppWindow::AddVolumeToList(dev_t)\n"));

	BVolume volume(device);
	BDirectory dir;
	volume.GetRootDirectory(&dir);
	BEntry entry;
	dir.FindEntry("wav",&entry);
	if((entry.InitCheck() == B_OK) && (dir.SetTo(&entry) == B_OK))
	{
		BMessage refMsg(B_REFS_RECEIVED);
		entry_ref ref;
		dir.Rewind();
		while(dir.GetNextRef(&ref) == B_OK)
		{
			refMsg.AddRef("refs",&ref);
		}
		viewMessenger->SendMessage(&refMsg);
	}
}

void
AppWindow::AboutRequested()
{
	PRINT(("AppWindow::AboutRequested()\n"));

	BString msg;
	msg = APPLICATION;
	msg << " ";
	msg << VERSION;
	msg << "\n";
	msg << COMPANY;
	msg << "\n";
	msg << COMPANY_WWW;
	msg << "\n";
	msg << COMPANY_EMAIL;
	BAlert* alert = new BAlert("AboutBox",msg.String(),ABOUT_BTN);
	alert->Go(NULL);
}

void
AppWindow::FrameResized(float width, float height)
{
#ifdef DEBUG
	BString title = APPLICATION;
	title << " WIDTH=";
	title << width;
	title << " HEIGHT=";
	title << height;
	SetTitle(title.String());
#endif
}

void
AppWindow::MessageReceived(BMessage* message)
{
	//PRINT(("AppWindow::MessageReceived(BMessage*)\n"));

	switch(message->what)
	{
		case B_NODE_MONITOR:
			{
				int32 opcode;
				message->FindInt32("opcode",&opcode);
				switch(opcode)
				{
					case B_ENTRY_CREATED:
					case B_ENTRY_REMOVED:
						LoadEncoderMenu();
						break;
					case B_ENTRY_MOVED:
						{
							BDirectory addonDir(settings->AddOnsDirectory());
							node_ref addon_ref;
							addonDir.GetNodeRef(&addon_ref);

							dev_t device;
							message->FindInt32("device",&device);
							PRINT(("DEVICE FROM MSG = %d\n",device));

							ino_t from_node_num;
							message->FindInt64("from directory",&from_node_num);

							ino_t to_node_num;
							message->FindInt64("to directory",&to_node_num);

							if((addon_ref.device == device)
									&&((addon_ref.node == from_node_num)
									|| (addon_ref.node == to_node_num)))
							{
								LoadEncoderMenu();
							}
							else
							{
								LoadCDMenu();
								viewMessenger->SendMessage(message);
							}
						}
						break;
					case B_DEVICE_MOUNTED:
						{
							dev_t dev;
							message->FindInt32("new device",&dev);
							BVolume vol(dev);
							BDirectory root;
							vol.GetRootDirectory(&root);
							node_ref nref;
							root.GetNodeRef(&nref);
							watch_node(&nref,B_WATCH_NAME,this);
						}
						LoadCDMenu();
						break;
					case B_DEVICE_UNMOUNTED:
						{
							dev_t dev;
							message->FindInt32("device",&dev);
							BMessage msg(REMOVE_MSG);
							msg.AddInt32("device",dev);
							viewMessenger->SendMessage(&msg);
						}
						LoadCDMenu();
						break;
				}
			}
			break;
		case MENU_ITEM_SELECTED:
			MenuItemSelected(message);
			break;
		case B_ABOUT_REQUESTED:
			AboutRequested();
			break;
		case B_REFS_RECEIVED:
			RefsReceived(message);
			break;
		default:
			BWindow::MessageReceived(message);
	}
}

void
AppWindow::RefsReceived(BMessage* message)
{
	PRINT(("AppWindow::RefsReceived(BMessage*)\n"));

	viewMessenger->SendMessage(message);
}

bool
AppWindow::QuitRequested()
{
	PRINT(("AppWindow::QuitRequested()\n"));

	if(settings->IsEncoding())
	{
		return false;
	}

	if(encoderAddon > 0)
	{
		AEEncoder* encoder = settings->Encoder();
		if(encoder)
		{
			if(menuBar->RemoveItem(encoder->GetMenu()))
			{
				PRINT(("Error removing encoder menu.\n"));
			}
			settings->SetEncoder(NULL);
			unload_add_on(encoderAddon);
			encoderAddon = 0;
		}
	}

	be_app->PostMessage(B_QUIT_REQUESTED);

	return true;
}

AppWindow*
AppWindow::GetInstance()
{
	PRINT(("AppWindow::GetInstance()\n"));

	BPath path;
	BFile settings;
	BMessage archive;
	AppWindow* win = 0;

	find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	path.Append(COMPANY);
	path.Append(APPLICATION);
	path.Append(WINDOW_FILE);

	if((settings.SetTo(path.Path(), B_READ_ONLY) == B_OK)
			&& (archive.Unflatten(&settings) == B_OK))
	{
		win = new AppWindow(&archive);
	}
	else
	{
		win = new AppWindow();
	}
	return(win);
}

void
AppWindow::SaveWindow()
{
	PRINT(("AppWindow::SaveWindow()\n"));

	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	path.Append(COMPANY);
	path.Append(APPLICATION);
	create_directory(path.Path(),0755);
	path.Append(WINDOW_FILE);

	BMessage archive;
	status_t status = Archive(&archive,false);
	BFile settings(path.Path(),B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);
	if((settings.InitCheck() == B_OK) && (status == B_OK))
	{
		archive.Flatten(&settings);
	}
}
