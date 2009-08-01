#include <signal.h>
#include <be/app/Application.h>
#include <be/app/Message.h>
#include <be/interface/Alert.h>
#include <be/interface/Button.h>
#include <be/interface/Menu.h>
#include <be/interface/MenuBar.h>
#include <be/interface/MenuItem.h>
#include <be/storage/Directory.h>
#include <be/storage/Entry.h>
#include <be/storage/File.h>
#include <be/storage/FindDirectory.h>
#include <be/storage/Path.h>
#include <be/support/ClassInfo.h>
#include <be/support/Debug.h>
#include "AppDefs.h"
#include "Application.h"
#include "AppView.h"
#include "AppWindow.h"
#include "CommandConstants.h"
#include "GUIStrings.h"
#include "Preferences.h"

#define WIN_LEFT 200
#define WIN_TOP 200
#define WIN_RIGHT 700
#define WIN_BOTTOM 600
#define WIN_MIN_WIDTH 500
#define WIN_MAX_WIDTH 2000
#define WIN_MIN_HEIGHT 400
#define WIN_MAX_HEIGHT 2000

AppWindow::AppWindow() : BWindow(BRect(WIN_LEFT,WIN_TOP,WIN_RIGHT,WIN_BOTTOM),
	WIN_TITLE,B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS)
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
}

void
AppWindow::InitWindow()
{
	PRINT(("AppWindow::InitWindow()\n"));

	SetTitle(WIN_TITLE);
	SetType(B_TITLED_WINDOW);
	SetFlags(B_ASYNCHRONOUS_CONTROLS);
	SetSizeLimits(WIN_MIN_WIDTH,WIN_MAX_WIDTH,WIN_MIN_HEIGHT,WIN_MAX_HEIGHT);
	SetWorkspaces(B_CURRENT_WORKSPACE);

	m_menu_bar = new BMenuBar(BRect(),"menuBar");

	//create file menu
	m_file_menu = new BMenu(FILE_MENU);
	m_about_menu_item = new BMenuItem(ABOUT_ITEM, new BMessage(B_ABOUT_REQUESTED));
	m_quit_menu_item = new BMenuItem(QUIT_ITEM, new BMessage(B_QUIT_REQUESTED), QUIT_SHORTCUT);

	m_file_menu->AddItem(m_about_menu_item);
	m_file_menu->AddSeparatorItem();
	m_file_menu->AddItem(m_quit_menu_item);
	
	m_menu_bar->AddItem(m_file_menu);

	//create edit menu
	m_edit_menu = new BMenu(EDIT_MENU);
	
	m_cut_menu_item = new BMenuItem(CUT_ITEM, new BMessage(MSG_EDIT_CUT), CUT_ITEM_SHORTCUT);
	m_copy_menu_item = new BMenuItem(COPY_ITEM, new BMessage(MSG_EDIT_COPY), COPY_ITEM_SHORTCUT);
	m_paste_menu_item = new BMenuItem(PASTE_ITEM, new BMessage(MSG_EDIT_PASTE), PASTE_ITEM_SHORTCUT);
	m_select_all_menu_item= new BMenuItem(SELECT_ALL_ITEM, new BMessage(MSG_SELECT_ALL), SELECT_ALL_SHORTCUT); //, B_SHIFT_KEY);
	
	m_first_file_menu_item = new BMenuItem(FIRST_FILE_ITEM, new BMessage(MSG_FIRST_FILE), FIRST_FILE_SHORTCUT);
	m_last_file_menu_item = new BMenuItem(LAST_FILE_ITEM, new BMessage(MSG_LAST_FILE), LAST_FILE_SHORTCUT);
	m_previous_file_menu_item = new BMenuItem(PREVIOUS_FILE_ITEM, new BMessage(MSG_PREVIOUS_FILE), PREVIOUS_FILE_SHORTCUT);
	m_next_file_menu_item = new BMenuItem(NEXT_FILE_ITEM, new BMessage(MSG_NEXT_FILE), NEXT_FILE_SHORTCUT);
	m_reset_menu_item = new BMenuItem(RESET_ITEM, new BMessage(MSG_RESET), RESET_SHORTCUT);
	m_clear_list_menu_item = new BMenuItem(CLEAR_LIST_ITEM, new BMessage(MSG_CLEAR_LIST), CLEAR_LIST_SHORTCUT);
	
	m_edit_menu->AddItem(m_cut_menu_item);
	m_edit_menu->AddItem(m_copy_menu_item);
	m_edit_menu->AddItem(m_paste_menu_item);
	m_edit_menu->AddSeparatorItem();
	m_edit_menu->AddItem(m_select_all_menu_item);
	m_edit_menu->AddSeparatorItem();
	m_edit_menu->AddItem(m_first_file_menu_item);
	m_edit_menu->AddItem(m_last_file_menu_item);
	m_edit_menu->AddSeparatorItem();
	m_edit_menu->AddItem(m_previous_file_menu_item);
	m_edit_menu->AddItem(m_next_file_menu_item);
	m_edit_menu->AddSeparatorItem();
	m_edit_menu->AddItem(m_reset_menu_item);
	m_edit_menu->AddItem(m_clear_list_menu_item);
	

	m_menu_bar->AddItem(m_edit_menu);

	//create mode menu
	m_mode_menu = new BMenu(MODE_MENU);
	m_mode_menu->AddItem(new BMenuItem(PREVIOUS_MODE_NAME, new BMessage(MSG_PREVIOUS_MODE), PREVIOUS_MODE_SHORTCUT));
	m_mode_menu->AddItem(new BMenuItem(NEXT_MODE_NAME, new BMessage(MSG_NEXT_MODE), NEXT_MODE_SHORTCUT));
	m_mode_menu->AddSeparatorItem();
	m_mode_menu->AddItem(new BMenuItem(EDITOR_MODE_NAME, new BMessage(MSG_EDITOR_MODE), '1'));
	m_mode_menu->AddItem(new BMenuItem(TA_MODE_NAME, new BMessage(MSG_TA_MODE), '2'));
	m_mode_menu->AddItem(new BMenuItem(NA_MODE_NAME, new BMessage(MSG_NA_MODE), '3'));
	m_mode_menu->AddSeparatorItem();
	m_mode_menu->AddItem(new BMenuItem(MPEG_MODE_NAME, new BMessage(MSG_MPEG_MODE), '4'));
#ifdef _TTE_
	m_mode_menu->AddSeparatorItem();
	m_mode_menu->AddItem(new BMenuItem(TT_INFO_MODE_NAME, new BMessage(MSG_TT_INFO_MODE), '5'));
#endif
	m_menu_bar->AddItem(m_mode_menu);
		
	// create options menu
	/*
	m_options_menu = new BMenu(OPTIONS_MENU);
	m_beep_menu_item = new BMenuItem(BEEP_ON_UNSUPPORTED, new BMessage(MSG_BEEP_ON_UNSUPPORTED));

	m_options_menu->AddItem(m_beep_menu_item);
	m_menu_bar->AddItem(m_options_menu);
	*/
	
	// add m_app_view below menu bar
	AddChild(m_menu_bar);
	SetKeyMenuBar(m_menu_bar);

	BRect frame = Bounds();
	frame.top += (m_menu_bar->Bounds()).Height() + 1;

	m_app_view = new AppView(frame);
	AddChild(m_app_view);
	
	// final touches
	float wHt = Bounds().Height() - (m_menu_bar->Bounds().Height() + 1);
	float vHt,vWt;
	m_app_view->GetPreferredSize(&vWt,&vHt);

	if(wHt < vHt)
	{
		ResizeBy(0,vHt-wHt);
	}
	
	SetSizeLimits(WIN_MIN_WIDTH,WIN_MAX_WIDTH,vHt,WIN_MAX_HEIGHT);
}

void
AppWindow::MessageReceived(BMessage* message)
{
	//PRINT(("AppWindow::MessageReceived(BMessage*)\n"));

	switch(message->what)
	{
		case B_ABOUT_REQUESTED:
			AboutRequested();
			break;

		case MSG_EDITOR_MODE:
			m_app_view->SelectView(0);
			break;
		case MSG_TA_MODE:
			m_app_view->SelectView(1);
			break;
		case MSG_NA_MODE:
			m_app_view->SelectView(2);
			break;
		case MSG_MPEG_MODE:
			m_app_view->SelectView(3);
			break;
#ifdef _TTE_
		case MSG_TT_INFO_MODE:
			m_app_view->SelectView(4);
			break;
#endif			
		case MSG_PREVIOUS_MODE:
			m_app_view->SetPreviousMode();
			break;
		case MSG_NEXT_MODE:
			m_app_view->SetNextMode();
			break;
			
		case MSG_RESET:
			m_app_view->Reset();
			break;
		case MSG_CLEAR_LIST:
			m_app_view->ClearList();
			break;
			
		case MSG_PREVIOUS_FILE:
			m_app_view->SelectPreviousFile();
			break;
		case MSG_NEXT_FILE:
			m_app_view->SelectNextFile();
			break;
			
		case MSG_FIRST_FILE:
			m_app_view->SelectFirstFile();
			break;
		case MSG_LAST_FILE:
			m_app_view->SelectLastFile();
			break;
	
		case B_REFS_RECEIVED:
			m_app_view->MessageReceived(message);
			break;
			
		case MSG_MAKE_APPLY_BUTTON_DEFAULT:
			m_app_view->m_apply_button->MakeDefault(true);
			break;
			
		case MSG_MAKE_APPLY_BUTTON_NOT_DEFAULT:
			m_app_view->m_apply_button->MakeDefault(false);
			break;
			
		case MSG_EDIT_CUT:
			{
				message->what = B_CUT;
				BView *view = CurrentFocus();
				if (view)
					view->MessageReceived(message);
			}	
			break;
			
		case MSG_EDIT_COPY:
			{
				message->what = B_COPY;
				BView *view = CurrentFocus();
				if (view)
					view->MessageReceived(message);
			}	
			break;
			
		case MSG_EDIT_PASTE:
			{
				message->what = B_PASTE;
				BView *view = CurrentFocus();
				if (view)
					view->MessageReceived(message);
			}	
			break;

		case MSG_SELECT_ALL:
			{
				message->what = B_SELECT_ALL;
				BView *view = CurrentFocus();
				if (view)
					view->MessageReceived(message);
				else
					m_app_view->SelectAll();
			}
			break;
		
		default:
			BWindow::MessageReceived(message);
	}
}

void
AppWindow::FrameMoved(BPoint point)
{
	m_app_view->SaveWindowFrame();
}

void
AppWindow::FrameResized(float width, float height)
{
	m_app_view->SaveWindowFrame();
}

bool
AppWindow::QuitRequested()
{
	PRINT(("AppWindow::QuitRequested()\n"));
	
	thread_id addrefs = find_thread("AddRefs");
	if(addrefs != B_NAME_NOT_FOUND)
	{
		send_signal(addrefs,SIGQUIT);
	}

	if(is_instance_of(be_app,Application))
	{
		be_app->PostMessage(B_QUIT_REQUESTED);
	}
	else
	{
		if(Lock())
		{
			Quit();
		}
	}
	return true;
}

void
AppWindow::AboutRequested()
{
	PRINT(("AppWindow::AboutRequested()\n"));
	BString msg;
	msg = APPLICATION " " VERSION "\n\n"
		"Created by Jason Burgess of now defunct FlipSide Software. "
		"His former website has since been taken over by "
		"another company by the same name.\n\n"
		
		"The FlipSide Software applications are hosted by OsDrawer at www.osdrawer.net.\n\n"
		
		MAINTAINER " is the current maintainer.\n"
		"You can reach him at " MAINTAINER_EMAIL "\n\n"

		"To request new features, or to report bugs, file a new issue at:\n"
		"http://dev.osdrawer.net/projects/armyknife/issues/new";
	
	BAlert* alert = new BAlert("AboutBox", msg.String(), ABOUT_BTN);
	alert->Go(NULL);
}

AppWindow*
AppWindow::CreateWindow()
{
	PRINT(("AppWindow::CreateWindow()\n"));

	BPath path;
	BFile settings;
	BMessage archive;
	AppWindow* win = 0;

	find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	path.Append(PROJECT_DIR);
	path.Append(APPLICATION_DIR);
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
AppWindow::DisableInterface()
{
	m_menu_bar->SetEnabled(false);
	m_file_menu->SetEnabled(false);
	m_edit_menu->SetEnabled(false);
	m_mode_menu->SetEnabled(false);
//	m_options_menu->SetEnabled(false);
	m_about_menu_item->SetEnabled(false);
	m_quit_menu_item->SetEnabled(false);
	m_first_file_menu_item->SetEnabled(false);
	m_last_file_menu_item->SetEnabled(false);
	m_previous_file_menu_item->SetEnabled(false);
	m_next_file_menu_item->SetEnabled(false);
	m_reset_menu_item->SetEnabled(false);
	m_clear_list_menu_item->SetEnabled(false);
	m_select_all_menu_item->SetEnabled(false);
//	m_beep_menu_item->SetEnabled(false);
}

void
AppWindow::EnableInterface()
{
	m_menu_bar->SetEnabled(true);
	m_file_menu->SetEnabled(true);
	m_edit_menu->SetEnabled(true);
	m_mode_menu->SetEnabled(true);
//	m_options_menu->SetEnabled(true);
	m_about_menu_item->SetEnabled(true);
	m_quit_menu_item->SetEnabled(true);
	m_first_file_menu_item->SetEnabled(true);
	m_last_file_menu_item->SetEnabled(true);
	m_previous_file_menu_item->SetEnabled(true);
	m_next_file_menu_item->SetEnabled(true);
	m_reset_menu_item->SetEnabled(true);
	m_clear_list_menu_item->SetEnabled(true);
	m_select_all_menu_item->SetEnabled(true);
//	m_beep_menu_item->SetEnabled(true);
}
