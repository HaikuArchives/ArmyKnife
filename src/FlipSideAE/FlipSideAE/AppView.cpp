#include <string.h>
#include <be/app/Message.h>
#include <be/interface/Alert.h>
#include <be/interface/Bitmap.h>
#include <be/interface/Button.h>
#include <be/interface/Font.h>
#include <be/interface/InterfaceDefs.h>
#include <be/interface/MenuBar.h>
#include <be/interface/Rect.h>
#include <be/interface/StatusBar.h>
#include <be/interface/StringView.h>
#include <be/interface/TextControl.h>
#include <be/kernel/fs_attr.h>
#include <be/kernel/fs_info.h>
#include <be/kernel/image.h>
#include <be/kernel/OS.h>
#include <be/storage/Directory.h>
#include <be/storage/Entry.h>
#include <be/storage/File.h>
#include <be/storage/Node.h>
#include <be/storage/NodeInfo.h>
#include <be/storage/NodeMonitor.h>
#include <be/storage/Path.h>
#include <be/storage/Volume.h>
#include <be/storage/VolumeRoster.h>
#include <be/support/Beep.h>
#include <be/support/Debug.h>
#include <be/support/String.h>
#include <String/String.h>
#include <AEEncoder/AEEncoder.h>
#include <AudioInfo/AudioAttributes.h>
#include <AudioInfo/GenreList.h>
#include <Santa/CLVRefListItem.h>
#include "AppDefs.h"
#include "AppView.h"
#include "AppWindow.h"
#include "CommandConstants.h"
#include "CheckMark.h"
#include "GUIStrings.h"
#include "EditorView.h"
#include "EncoderListView.h"
#include "FileNamePatternView.h"
#include "Settings.h"
#include "StatusBarFilter.h"

AppView::AppView(BRect frame) : BView(frame, "AppView", B_FOLLOW_ALL,
		B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE_JUMP)
{
	PRINT(("AppView::AppView(BRect)\n"));

}

AppView::~AppView()
{
	PRINT(("AppView::~AppView()\n"));

	stop_watching(this);
}

void
AppView::InitView()
{
	PRINT(("AppView::InitView()\n"));

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	int32 space = 6;

	BRect fileNameFrame = Bounds();
	fileNameFrame.InsetBy(space,space);
	fileNamePatternView = new FileNamePatternView(fileNameFrame);
	AddChild(fileNamePatternView);
	fileNameFrame = fileNamePatternView->Frame(); //it resizes itself

	BRect editorFrame = fileNameFrame;
	editorFrame.left = fileNameFrame.right + space;
	editorFrame.right = editorFrame.left + 365;
	editorView = new EditorView(editorFrame);
	AddChild(editorView);
	editorFrame = editorView->Frame(); //it resizes itself

	if(editorFrame.bottom > fileNameFrame.bottom)
	{
		float diff = editorFrame.bottom - fileNameFrame.bottom;
		fileNamePatternView->ResizeBy(0,diff);
		fileNameFrame = fileNamePatternView->Frame();
	}
	else if(editorFrame.bottom < fileNameFrame.bottom)
	{
		float diff = fileNameFrame.bottom - editorFrame.bottom;
		editorView->ResizeBy(0,diff);
		editorFrame = editorView->Frame();
	}

	BRect buttonFrame = Bounds();
	buttonFrame.InsetBy(space,space);
	buttonFrame.top = buttonFrame.bottom - 30;
	buttonFrame.left = buttonFrame.right - 70;
	encodeButton = new BButton(buttonFrame,"encodeButton",ENCODE_BTN,
			new BMessage(ENCODE_MSG),B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);

	buttonFrame.OffsetBy(-(buttonFrame.Width()+space),0);
	cancelButton = new BButton(buttonFrame,"cancelButton",CANCEL_BTN,
			new BMessage(CANCEL_MSG),B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);

	BRect listViewFrame = Bounds();
	listViewFrame.left += space;
	listViewFrame.top = fileNameFrame.bottom + (space);
	listViewFrame.right -= (space + B_V_SCROLL_BAR_WIDTH);
	listViewFrame.bottom = buttonFrame.top - (space + B_H_SCROLL_BAR_HEIGHT);
	listView = new EncoderListView(listViewFrame,&scrollView);
	listView->SetSelectionMessage(new BMessage(LIST_SELECTION_MSG));

	BRect statusBarFrame = buttonFrame;
	statusBarFrame.left = Bounds().left + space;
	statusBarFrame.right = buttonFrame.left - space;
	BString remaining(STATUS_TRAILING_LABEL);
	remaining << 0;
	statusBar = new BStatusBar(statusBarFrame,"statusBar",STATUS_LABEL,
			remaining.String());
	statusBar->SetResizingMode(B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM);
	statusBar->AddFilter(new StatusBarFilter());

	BRect viewFrame = Bounds();
	if(viewFrame.right < (editorFrame.right + space))
	{
		float diff = (editorFrame.right + space) - viewFrame.right;
		ResizeBy(diff,0);
	}

	AddChild(scrollView);
	AddChild(encodeButton);
	AddChild(cancelButton);
	AddChild(statusBar);
}

void
AppView::AttachedToWindow()
{
	PRINT(("AppView::AttachedToWindow()\n"));

	BView::AttachedToWindow();

	InitView();

	listView->SetTarget(editorView);
	encodeButton->SetTarget(this);
	cancelButton->SetTarget(this);
}

void
AppView::MessageReceived(BMessage* message)
{
	//PRINT(("AppView::MessageReceived(BMessage*)\n"));
	switch(message->what)
	{
		case VIEW_SHORTCUT:
			{
				int32 view;
				message->FindInt32("view",&view);
				PRINT(("View Shortcut: %d\n",view));
				switch(view)
				{
					case 0:
						fileNamePatternView->MakeFocus(true);
						break;
					case 1:
						editorView->MakeFocus(true);
						break;
					case 2:
						listView->MakeFocus(true);
						break;
				}
			}
			break;
		case ENCODE_MSG:
			Encode();
			break;
		case CANCEL_MSG:
			Cancel();
			break;
		case SELECT_ALL_MSG:
			if(listView->CountItems() > 0)
			{
				listView->Select(0,listView->CountItems()-1);
			}
			break;
		case DESELECT_ALL_MSG:
			listView->DeselectAll();
			break;
		case REMOVE_MSG:
			{
				int32 device;
				if((message->FindInt32("device",&device) == B_OK))
				{
					RemoveDeviceItemsFromList(device);
				}
				else
				{ 
					thread_id thread = spawn_thread(AppView::RemoveItemsFromList,
							"RemoveItems", B_NORMAL_PRIORITY,(void*)this);
					resume_thread(thread);
				}
			}
			break;
		case APPLY_ATTRIBUTE_CHANGES:
			ApplyAttributeChanges(message);
			break;
		case FILE_NAME_PATTERN_CHANGED:
			{
				int32 numItems = listView->CountItems();
				for(int i=0;i<numItems;i++)
				{
					CLVRefListItem* item =
						(CLVRefListItem*)listView->ItemAt(i);
					SetSaveAsColumn(item);
					listView->InvalidateItem(i);
				}
			}
			break;
		case ENCODER_CHANGED:
			{
				BMessenger msgr(fileNamePatternView);
				msgr.SendMessage(message);
			}
			break;
		case B_REFS_RECEIVED:
		case B_SIMPLE_DATA:
			{ 
				BList* args = new BList();
				args->AddItem(this);
				args->AddItem(new BMessage(*message));
				thread_id thread = spawn_thread(AppView::RefsRecievedWrapper,
						"RefsReceived", B_NORMAL_PRIORITY,
						(void*)args);
				resume_thread(thread);
			}
			break;
		case COLUMN_SET_SHOWN:
			{
				int32 column;
				bool show;
				message->FindInt32("column",&column);
				message->FindBool("show",&show);
				listView->ColumnAt(column)->SetShown(show);
			}
			break;
		case B_NODE_MONITOR:
			{
				int32 opcode;
				if(message->FindInt32("opcode",&opcode) != B_OK)
				{
					return;
				}

				switch(opcode)
				{
					case B_ENTRY_MOVED:
						UpdateItem(message);
						break;
					case B_ENTRY_REMOVED:
						{
							node_ref nref;

							if(message->FindInt64("node",&nref.node) != B_OK)
							{
								break;
							}
							if(message->FindInt32("device",&nref.device) != B_OK)
							{
								break;
							}
							RemoveNodeFromList(&nref);
						}
						break;
				}
			}
			break;
		default:
			BView::MessageReceived(message);
	}
}

int32
AppView::RefsRecievedWrapper(void* args)
{
	PRINT(("AppView::RefsReceivedWrapper(void*)\n"));

	BList* params = (BList*)args;
	AppView* view = (AppView*)params->ItemAt(0);
	BMessage* message = (BMessage*)params->ItemAt(1);
	view->RefsReceived(message);
	delete message;
	delete params;
	return B_OK;
}

void
AppView::RefsReceived(BMessage* message)
{
	PRINT(("AppView::RefsReceived(BMessage*)\n"));

	CLVRefListItem* item;

	entry_ref tmp_ref;
	int numRefs = 0;
	while(message->FindRef("refs",numRefs,&tmp_ref) == B_NO_ERROR)
	{
		BNode node(&tmp_ref);
		if(node.IsFile())
		{
			BNodeInfo info(&node);
			char type[B_MIME_TYPE_LENGTH+1];
			info.GetType(type);
			if((strncmp(type,"audio/",6) == 0)
				&& (strcmp(type,CDDA_MIME_TYPE) != 0))
			{
				bool found = false;
				for(int i=0;i<listView->CountItems();i++)
				{
					CLVRefListItem* item =
						(CLVRefListItem*)listView->ItemAt(i);
					entry_ref* ref = item->EntryRef();
					if(*ref == tmp_ref)
					{
						found = true;
						break;
					}
				}
				if(!found)
				{
					if(LockLooper())
					{
						node_ref nref;
						if(node.GetNodeRef(&nref) == B_OK)
						{
							item = new CLVRefListItem(&tmp_ref,&nref);
							watch_node(&nref,B_WATCH_NAME,this);
						}
						else
						{
							item = new CLVRefListItem(&tmp_ref);
						}
						InitializeColumn(item);
						listView->AddItem(item);
						listView->SortItems();
						UnlockLooper();
					}
				}
			}
		}
		else if(node.IsDirectory())
		{
			BDirectory dir(&tmp_ref);
			if(dir.InitCheck() == B_OK)
			{
				BMessage refsMessage(B_REFS_RECEIVED);
				entry_ref ref;
				while(dir.GetNextRef(&ref) != B_ENTRY_NOT_FOUND)
				{
					refsMessage.AddRef("refs",&ref);
				}
				RefsReceived(&refsMessage);
			}
		}
		else if(node.IsSymLink())
		{
			BEntry entry(&tmp_ref,true);
			if((entry.InitCheck() == B_OK) && (entry.GetRef(&tmp_ref) == B_OK))
			{
				BMessage refsMessage(B_REFS_RECEIVED);
				refsMessage.AddRef("refs",&tmp_ref);
				RefsReceived(&refsMessage);
			}
		}
		numRefs++;
	}
}

void
AppView::InitializeColumn(CLVRefListItem* item)
{
	PRINT(("AppView::InitializeColumn(CLVRefListItem*)\n"));

	entry_ref* ref = item->EntryRef();

	item->SetColumnContent(FILE_COLUMN_INDEX,ref->name);

	BVolume volume(ref->device);
	fs_info fsinfo;
	fs_stat_dev(volume.Device(),&fsinfo);
	if(strcmp(fsinfo.fsh_name,"cdda") == 0)
	{
		char volume_name[B_FILE_NAME_LENGTH];
		volume.GetName(volume_name);
		if(strcmp(volume_name,"Audio CD") != 0)
		{
			String artist(volume_name);
			int index = artist.FindFirst(" - ");
			artist.Substring(0,index);
			String album(volume_name);
			index += 3;
			album.Substring(index);
			artist.Trim();
			album.Trim();
			item->SetColumnContent(ARTIST_COLUMN_INDEX,artist.Value());
			item->SetColumnContent(ALBUM_COLUMN_INDEX,album.Value());
			item->SetColumnContent(TITLE_COLUMN_INDEX,ref->name);
		}
		else
		{
			item->SetColumnContent(ARTIST_COLUMN_INDEX,(const char*)0);
			item->SetColumnContent(ALBUM_COLUMN_INDEX,(const char*)0);
			item->SetColumnContent(TITLE_COLUMN_INDEX,(const char*)0);
		}
	}

	if(volume.KnowsAttr())
	{
		PRINT(("Volume knows attributes.\n"));

		BFile file(ref,B_READ_ONLY);
		AudioAttributes attributes(&file);
		PRINT_OBJECT(attributes);

		const char* artist = attributes.Artist();
		if(artist)
		{
			item->SetColumnContent(ARTIST_COLUMN_INDEX,artist);
		}

		const char* album = attributes.Album();
		if(album)
		{
			item->SetColumnContent(ALBUM_COLUMN_INDEX,album);
		}

		const char* title = attributes.Title();
		if(title)
		{
			item->SetColumnContent(TITLE_COLUMN_INDEX,title);
		}

		const char* year = attributes.Year();
		if(year)
		{
			item->SetColumnContent(YEAR_COLUMN_INDEX,year);
		}

		const char* comment = attributes.Comment();
		if(comment)
		{
			item->SetColumnContent(COMMENT_COLUMN_INDEX,comment);
		}

		const char* track = attributes.Track();
		if(track)
		{
			item->SetColumnContent(TRACK_COLUMN_INDEX,track);
		}

		const char* genre = attributes.Genre();
		if(genre)
		{
			item->SetColumnContent(GENRE_COLUMN_INDEX,genre);
		}
	}

	SetSaveAsColumn(item);
}

void
AppView::SetSaveAsColumn(CLVRefListItem* item)
{
	PRINT(("AppView::SetSaveAsColumn(CLVRefListItem*)\n"));

	const char* artist = item->GetColumnContentText(ARTIST_COLUMN_INDEX);
	if(!artist) artist = "";
	const char* album = item->GetColumnContentText(ALBUM_COLUMN_INDEX);
	if(!album) album = "";
	const char* title = item->GetColumnContentText(TITLE_COLUMN_INDEX);
	if(!title) title = "";
	const char* year = item->GetColumnContentText(YEAR_COLUMN_INDEX);
	if(!year) year = "";
	const char* comment = item->GetColumnContentText(COMMENT_COLUMN_INDEX);
	if(!comment) comment = "";
	const char* track = item->GetColumnContentText(TRACK_COLUMN_INDEX);
	if(!track) track = "";
	const char* genre = item->GetColumnContentText(GENRE_COLUMN_INDEX);
	if(!genre) genre = "";

	AEEncoder* encoder = settings->Encoder();
	if(encoder)
	{
		BString fileNamePattern = encoder->GetPattern();
		fileNamePattern.ReplaceAll("%a",artist);
		fileNamePattern.ReplaceAll("%n",album);
		fileNamePattern.ReplaceAll("%t",title);
		fileNamePattern.ReplaceAll("%y",year);
		fileNamePattern.ReplaceAll("%c",comment);
		fileNamePattern.ReplaceAll("%k",track);
		fileNamePattern.ReplaceAll("%g",genre);
		item->SetColumnContent(SAVE_AS_COLUMN_INDEX,fileNamePattern.String());
	}
}

void
AppView::ApplyAttributeChanges(BMessage* message)
{
	PRINT(("AppView::ApplyAttributeChanges(BMessage*)\n"));

	type_code index_type;
	int32 numSelected;
	message->GetInfo("index",&index_type,&numSelected);

	int32 index;
	CLVRefListItem* item;
	for(int i=0;i<numSelected;i++)
	{
		if(message->FindInt32("index",i,&index) != B_OK)
		{
			return;
		}

		item = (CLVRefListItem*)listView->ItemAt(index);
		if(!item)
		{
			return;
		}

		BString tmp;

		if(message->FindString("artist",&tmp) == B_OK)
		{
			item->SetColumnContent(ARTIST_COLUMN_INDEX,tmp.String());
		}

		if(message->FindString("album",&tmp) == B_OK)
		{
			item->SetColumnContent(ALBUM_COLUMN_INDEX,tmp.String());
		}

		if(message->FindString("title",&tmp) == B_OK)
		{
			item->SetColumnContent(TITLE_COLUMN_INDEX,tmp.String());
		}

		if(message->FindString("year",&tmp) == B_OK)
		{
			item->SetColumnContent(YEAR_COLUMN_INDEX,tmp.String());
		}

		if(message->FindString("comment",&tmp) == B_OK)
		{
			item->SetColumnContent(COMMENT_COLUMN_INDEX,tmp.String());
		}

		if(message->FindString("track",&tmp) == B_OK)
		{
			if(tmp.CountChars() == 1)
			{
				tmp.Prepend("0");
			}
			item->SetColumnContent(TRACK_COLUMN_INDEX,tmp.String());
		}

		if(message->FindString("genre",&tmp) == B_OK)
		{
			item->SetColumnContent(GENRE_COLUMN_INDEX,tmp.String());
		}

		SetSaveAsColumn(item);
		listView->InvalidateItem(index);
	}
}

void
AppView::RemoveNodeFromList(node_ref* ref)
{
	CLVRefListItem* item;
	node_ref* itemRef;
	int32 numItems = listView->CountItems();
	for(int i=numItems-1;i>=0;i--)
	{
		item = (CLVRefListItem*)listView->ItemAt(i);
		if(item)
		{
			itemRef = item->NodeRef();
			if(*ref == *itemRef)
			{
				if(settings->IsEncoding())
				{
					(new BAlert(0,REMOVED_TXT,OK))->Go(0);
					Cancel();
				}
				watch_node(itemRef,B_STOP_WATCHING,this);
				listView->Deselect(i);
				listView->RemoveItem(i);
				listView->Invalidate();
				delete item;
			}
		}
	}

}

void
AppView::RemoveDeviceItemsFromList(int32 device)
{
	PRINT(("AppView::RemoveDeviceItemsFromList(int32)\n"));

	bool deleted = false;
	CLVRefListItem* item;
	entry_ref* ref;
	int32 numItems = listView->CountItems();
	for(int i=numItems-1;i>=0;i--)
	{
		item = (CLVRefListItem*)listView->ItemAt(i);
		if(item)
		{
			ref = item->EntryRef();
			if(device == ref->device)
			{
				deleted = true;
				watch_node(item->NodeRef(),B_STOP_WATCHING,this);
				listView->Deselect(i);
				listView->RemoveItem(i);
				listView->Invalidate();
				delete item;
			}
		}
	}

	if(deleted && settings->IsEncoding())
	{
		(new BAlert(0,REMOVED_TXT,OK))->Go(0);
		Cancel();
	}
}

int32
AppView::RemoveItemsFromList(void* args)
{
	PRINT(("AppView::RemoveItemsFromList(void*)\n"));

	AppView* view = (AppView*)args;
	CLVRefListItem* item;
	int32 max = view->listView->CountItems();
	for(int i=max-1;i>=0;i--)
	{
		if(view->listView->IsItemSelected(i))
		{
			PRINT(("%03d\n",i));
			if(view->LockLooper())
			{
				view->listView->Deselect(i);
				item = (CLVRefListItem*)view->listView->RemoveItem(i);
				watch_node(item->NodeRef(),B_STOP_WATCHING,view);
				delete item;
				view->listView->Invalidate();
				view->UnlockLooper();
			}
		}
	}

	return B_OK;
}

int32
AppView::UpdateItem(BMessage* message)
{
	PRINT(("AppView::UpdateItem(BMessage*)\n"));

	ino_t node;
	ino_t dir;
	dev_t dev;
	const char* name;

	if(message->FindInt64("node",&node) != B_OK)
	{
		return B_ERROR;
	}

	if(message->FindInt64("to directory",&dir) != B_OK)
	{
		return B_ERROR;
	}

	if(message->FindInt32("device",&dev) != B_OK)
	{
		return B_ERROR;
	}

	if(message->FindString("name",&name) != B_OK)
	{
		return B_ERROR;
	}

	PRINT(("device = %d\n",dev));

	node_ref nref;
	entry_ref eref;

	nref.device = dev;
	nref.node = node;

	eref.device = dev;
	eref.directory = dir;
	eref.set_name(name);

	bool doVolume = false;
	if(dev == 1)
	{
		BVolumeRoster roster;
		BVolume volume;
		char volName[B_PATH_NAME_LENGTH];
		roster.Rewind();
		while(roster.GetNextVolume(&volume) == B_OK)
		{
			if(volume.InitCheck() != B_OK)
			{
				return B_ERROR;
			}
			volume.GetName(volName);
			if(strcmp(volName,name) == 0)
			{
				doVolume = true;
				dev = volume.Device();
				break;
			}
		}
	}

	CLVRefListItem* item;
	int32 max = listView->CountItems();
	for(int i=0;i<max;i++)
	{
		item = (CLVRefListItem*)listView->ItemAt(i);
		if(item)
		{
			PRINT(("item's device = %d\n",item->NodeRef()->device));
			if(item->NodeRef()->node == node)
			{
				item->SetNodeRef(&nref);
				item->SetEntryRef(&eref);
				if(LockLooper())
				{
					InitializeColumn(item);
					listView->SortItems();
					listView->Invalidate();
					UnlockLooper();
				}
			}
			else if(doVolume && (item->NodeRef()->device == dev))
			{
				if(LockLooper())
				{
					InitializeColumn(item);
					listView->SortItems();
					listView->Invalidate();
					UnlockLooper();
				}
			}
		}
	}

	return B_OK;
}

void
AppView::Encode()
{
	PRINT(("AppView::Encode()\n"));

	thread_id thread = spawn_thread(AppView::EncodeThread,"_Encoder_",
			B_NORMAL_PRIORITY,(void*)this);
	resume_thread(thread);
}
	
int32
AppView::EncodeThread(void* args)
{
	PRINT(("AppView::EncodeThread(void*)\n"));

	settings->SetEncoding(true);
	AppView* view = (AppView*)args;
	BMenuBar* menuBar = ((AppWindow*)view->Window())->MenuBar();

	AEEncoder* encoder = settings->Encoder();
	if(!encoder || (encoder->InitCheck() != B_OK))
	{
		if(encoder->InitCheck() == FSS_EXE_NOT_FOUND)
		{
			PRINT(("ERROR: exe not found\n"));
			BString msg("The addon could not find the required executable.");
			msg << "\n";
			msg << "Please check the addon's dccumentation.\n";
			view->AlertUser(msg.String());
		}
		system_beep(SYSTEM_BEEP_ENCODING_DONE);
		return B_ERROR;
	}

	encoder->InitEncoder();
	BMessenger statusBarMessenger(view->statusBar);

	if(view->LockLooper())
	{
		view->editorView->SetEnabled(false);
		view->fileNamePatternView->SetEnabled(false);
		view->encodeButton->SetEnabled(false);
		menuBar->SetEnabled(false);
		view->UnlockLooper();
	}

	BList list;
	CLVRefListItem* item;
	int32 numItems = view->listView->CountItems();
	if(view->LockLooper())
	{
		for(int i=0;i<numItems;i++)
		{
			item = (CLVRefListItem*)view->listView->ItemAt(i);
			item->SetColumnContent(COMPLETE_COLUMN_INDEX,(const BBitmap*)NULL);
			list.AddItem(item);
			view->listView->InvalidateItem(i);
		}
		view->UnlockLooper();
	}

	for(int i=0;i<numItems;i++)
	{
		item = (CLVRefListItem*)list.ItemAt(i);

		const char* artist = item->GetColumnContentText(ARTIST_COLUMN_INDEX);
		const char* album = item->GetColumnContentText(ALBUM_COLUMN_INDEX);
		const char* title = item->GetColumnContentText(TITLE_COLUMN_INDEX);
		const char* year = item->GetColumnContentText(YEAR_COLUMN_INDEX);
		const char* comment = item->GetColumnContentText(COMMENT_COLUMN_INDEX);
		const char* track = item->GetColumnContentText(TRACK_COLUMN_INDEX);
		const char* genre = item->GetColumnContentText(GENRE_COLUMN_INDEX);
		int32 genreNum;
		if(genre)
		{
			genreNum = GenreList::Genre(genre);
		}
		const char* outputFile = item->GetColumnContentText(SAVE_AS_COLUMN_INDEX);
		entry_ref* ref = item->EntryRef();
		BEntry entry(ref);
		if(entry.InitCheck() != B_OK)
		{
			PRINT(("ERROR: entry failed InitCheck()\n"));
			BString msg("Error opening file: ");
			msg << ref->name;
			view->AlertUser(msg.String());
			if(view->LockLooper())
			{
				BString remaining(STATUS_TRAILING_LABEL);
				remaining << 0;
				view->statusBar->Reset(STATUS_LABEL,remaining.String());
				view->editorView->SetEnabled(true);
				view->fileNamePatternView->SetEnabled(true);
				view->encodeButton->SetEnabled(true);
				view->cancelButton->SetEnabled(true);
				menuBar->SetEnabled(true);
				view->UnlockLooper();
			}
			settings->SetEncoding(false);
			encoder->UninitEncoder();
			system_beep(SYSTEM_BEEP_ENCODING_DONE);
			return B_ERROR;
		}

		BPath path;
		entry.GetPath(&path);
		if(path.InitCheck() != B_OK)
		{
			PRINT(("ERROR: path failed InitCheck()\n"));
			BString msg("Error opening file: ");
			msg << ref->name;
			view->AlertUser(msg.String());
			if(view->LockLooper())
			{
				BString remaining(STATUS_TRAILING_LABEL);
				remaining << 0;
				view->statusBar->Reset(STATUS_LABEL,remaining.String());
				view->editorView->SetEnabled(true);
				view->fileNamePatternView->SetEnabled(true);
				view->encodeButton->SetEnabled(true);
				view->cancelButton->SetEnabled(true);
				menuBar->SetEnabled(true);
				view->UnlockLooper();
			}
			settings->SetEncoding(false);
			encoder->UninitEncoder();
			system_beep(SYSTEM_BEEP_ENCODING_DONE);
			return B_ERROR;
		}
		
		BPath outputPath(outputFile);
		if(outputPath.InitCheck() != B_OK)
		{
			PRINT(("ERROR: outputPath failed InitCheck()\n"));
			BString msg("Error creating path for: ");
			msg << outputFile;
			view->AlertUser(msg.String());
			if(view->LockLooper())
			{
				BString remaining(STATUS_TRAILING_LABEL);
				remaining << 0;
				view->statusBar->Reset(STATUS_LABEL,remaining.String());
				view->editorView->SetEnabled(true);
				view->fileNamePatternView->SetEnabled(true);
				view->encodeButton->SetEnabled(true);
				view->cancelButton->SetEnabled(true);
				menuBar->SetEnabled(true);
				view->UnlockLooper();
			}
			settings->SetEncoding(false);
			encoder->UninitEncoder();
			system_beep(SYSTEM_BEEP_ENCODING_DONE);
			return B_ERROR;
		}
		outputFile = outputPath.Path();
		BPath outputParent;
		outputPath.GetParent(&outputParent);
		if(outputParent.InitCheck() != B_OK)
		{
			PRINT(("ERROR: outputParent failed InitCheck()\n"));
			BString msg("Error creating path for: ");
			msg << outputFile;
			view->AlertUser(msg.String());
			if(view->LockLooper())
			{
				BString remaining(STATUS_TRAILING_LABEL);
				remaining << 0;
				view->statusBar->Reset(STATUS_LABEL,remaining.String());
				view->editorView->SetEnabled(true);
				view->fileNamePatternView->SetEnabled(true);
				view->encodeButton->SetEnabled(true);
				view->cancelButton->SetEnabled(true);
				menuBar->SetEnabled(true);
				view->UnlockLooper();
			}
			settings->SetEncoding(false);
			encoder->UninitEncoder();
			system_beep(SYSTEM_BEEP_ENCODING_DONE);
			return B_ERROR;
		}
		if(create_directory(outputParent.Path(),0777) != B_OK)
		{
			PRINT(("ERROR: failed to create directory\n"));
			BString msg("Error creating path for: ");
			msg << outputFile;
			view->AlertUser(msg.String());
			if(view->LockLooper())
			{
				BString remaining(STATUS_TRAILING_LABEL);
				remaining << 0;
				view->statusBar->Reset(STATUS_LABEL,remaining.String());
				view->editorView->SetEnabled(true);
				view->fileNamePatternView->SetEnabled(true);
				view->encodeButton->SetEnabled(true);
				view->cancelButton->SetEnabled(true);
				menuBar->SetEnabled(true);
				view->UnlockLooper();
			}
			settings->SetEncoding(false);
			encoder->UninitEncoder();
			system_beep(SYSTEM_BEEP_ENCODING_DONE);
			return B_ERROR;
		}

		const char* inputFile = path.Path();

		if(view->LockLooper())
		{
			BString encoding(STATUS_LABEL);
			encoding << path.Leaf();
			BString remaining(STATUS_TRAILING_LABEL);
			remaining << (numItems - i);
			view->statusBar->Reset(encoding.String(),remaining.String());
			view->UnlockLooper();
		}

		BMessage encodeMessage(FSS_ENCODE);
		encodeMessage.AddString("input file",inputFile);
		encodeMessage.AddString("output file",outputFile);
		encodeMessage.AddMessenger("statusBarMessenger",statusBarMessenger);
		if(artist)
		{
			encodeMessage.AddString("artist",artist);
		}
		if(album)
		{
			encodeMessage.AddString("album",album);
		}
		if(title)
		{
			encodeMessage.AddString("title",title);
		}
		if(year)
		{
			encodeMessage.AddString("year",year);
		}
		if(comment)
		{
			encodeMessage.AddString("comment",comment);
		}
		if(track)
		{
			encodeMessage.AddString("track",track);
		}
		if(genre)
		{
			encodeMessage.AddString("genre",genre);
			encodeMessage.AddInt32("genre number",genreNum);
		}

		PRINT(("Calling Addon: %s\n",encoder->GetName()));
		int32 retValue = encoder->Encode(&encodeMessage);
		switch(retValue)
		{
			case B_OK:
				PRINT(("Addon returned successfully.\n"));
				break;
			case FSS_EXE_NOT_FOUND:
				{
					PRINT(("ERROR: exe not found\n"));
					BString msg("The addon could not find the required executable.");
					msg << "\n";
					msg << "Please check the addon's dccumentation.\n";
					view->AlertUser(msg.String());
					if(view->LockLooper())
					{
						BString remaining(STATUS_TRAILING_LABEL);
						remaining << 0;
						view->statusBar->Reset(STATUS_LABEL,remaining.String());
						view->editorView->SetEnabled(true);
						view->fileNamePatternView->SetEnabled(true);
						view->encodeButton->SetEnabled(true);
						view->cancelButton->SetEnabled(true);
						menuBar->SetEnabled(true);
						view->UnlockLooper();
					}
					settings->SetEncoding(false);
					encoder->UninitEncoder();
					system_beep(SYSTEM_BEEP_ENCODING_DONE);
				}
				return B_ERROR;
			case FSS_INPUT_NOT_SUPPORTED:
				{
					PRINT(("ERROR: input not supported\n"));
					BString msg("Input File: ");
					msg << ref->name;
					msg << " cannot be encoded with this encoder. Continue?";
					BAlert* alert = new BAlert("alert",msg.String(),YES,NO);
					int32 button = alert->Go();
					if(button == 1)
					{
						if(view->LockLooper())
						{
							BString remaining(STATUS_TRAILING_LABEL);
							remaining << 0;
							view->statusBar->Reset(STATUS_LABEL,remaining.String());
							view->editorView->SetEnabled(true);
							view->fileNamePatternView->SetEnabled(true);
							view->encodeButton->SetEnabled(true);
							view->cancelButton->SetEnabled(true);
							menuBar->SetEnabled(true);
							view->UnlockLooper();
						}
						settings->SetEncoding(false);
						encoder->UninitEncoder();
						system_beep(SYSTEM_BEEP_ENCODING_DONE);
						return B_ERROR;
					}
					else
					{
						continue;
					}
				}
			case FSS_CANCEL_ENCODING:
				{
					PRINT(("User canceled encoding.\n"));
					if(view->LockLooper())
					{
						BString remaining(STATUS_TRAILING_LABEL);
						remaining << 0;
						view->statusBar->Reset(STATUS_LABEL,remaining.String());
						view->editorView->SetEnabled(true);
						view->fileNamePatternView->SetEnabled(true);
						view->encodeButton->SetEnabled(true);
						view->cancelButton->SetEnabled(true);
						menuBar->SetEnabled(true);
						view->UnlockLooper();
					}
					settings->SetEncoding(false);
					encoder->UninitEncoder();
					BEntry outputEntry(outputFile);
					outputEntry.Remove();
					while(1)
					{
						BDirectory directory(outputParent.Path());
						if(directory.CountEntries() > 0)
						{
							break;
						}
						directory.Unset();
						BEntry dirEntry(outputParent.Path());
						dirEntry.Remove();
						dirEntry.Unset();
						outputParent.GetParent(&outputParent);
					}
					system_beep(SYSTEM_BEEP_ENCODING_DONE);
				}
				return B_OK;
			case B_ERROR:
				{
					PRINT(("ERROR: encoding failed\n"));
					const char* errmsg;
					encodeMessage.FindString("error",&errmsg);
					BString msg("Error encoding file: ");
					msg << ref->name;
					msg << "\n";
					msg << errmsg;
					msg << "Cannot continue.";
					view->AlertUser(msg.String());
					if(view->LockLooper())
					{
						BString remaining(STATUS_TRAILING_LABEL);
						remaining << 0;
						view->statusBar->Reset(STATUS_LABEL,remaining.String());
						view->editorView->SetEnabled(true);
						view->fileNamePatternView->SetEnabled(true);
						view->encodeButton->SetEnabled(true);
						view->cancelButton->SetEnabled(true);
						menuBar->SetEnabled(true);
						view->UnlockLooper();
					}
					settings->SetEncoding(false);
					encoder->UninitEncoder();
					system_beep(SYSTEM_BEEP_ENCODING_DONE);
				}
				return B_ERROR;
		}

		if(view->LockLooper())
		{
			const BBitmap* checkMark = view->listView->GetCheckMark();
			item->SetColumnContent(COMPLETE_COLUMN_INDEX,checkMark);
			int index = view->listView->IndexOf(item);
			view->listView->InvalidateItem(index);
			view->UnlockLooper();
		}
	}

	encoder->UninitEncoder();
	settings->SetEncoding(false);

	if(view->LockLooper())
	{
		BString remaining(STATUS_TRAILING_LABEL);
		remaining << 0;
		view->statusBar->Reset(STATUS_LABEL,remaining.String());
		view->editorView->SetEnabled(true);
		view->fileNamePatternView->SetEnabled(true);
		view->encodeButton->SetEnabled(true);
		view->cancelButton->SetEnabled(true);
		menuBar->SetEnabled(true);
		view->UnlockLooper();
	}

	system_beep(SYSTEM_BEEP_ENCODING_DONE);
	return B_OK;
}

void
AppView::Cancel()
{
	PRINT(("AppView::Cancel()\n"));

	if(settings->IsEncoding())
	{
		thread_id encoder = find_thread("_Encoder_");
		if(!has_data(encoder))
		{
			send_data(encoder,FSS_CANCEL_ENCODING,0,0);
		}
		cancelButton->SetEnabled(false);
	}
}

void
AppView::AlertUser(const char* message)
{
	PRINT(("AppView::AlertUser(const char*)\n"));
	BAlert* alert = new BAlert("alert",message,OK,NULL,NULL,B_WIDTH_AS_USUAL,
			B_WARNING_ALERT);
	alert->Go();
}
