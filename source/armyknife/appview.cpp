#include <signal.h>
#include <be/app/Message.h>
#include <be/interface/Button.h>
#include <be/interface/ListView.h>
#include <be/interface/Rect.h>
#include <be/interface/StatusBar.h>
#include <be/interface/StringView.h>
#include <be/storage/Directory.h>
#include <be/storage/Entry.h>
#include <be/storage/Node.h>
#include <be/storage/NodeInfo.h>
#include <be/support/Debug.h>
#include <be/support/Beep.h>
#include <be/support/List.h>
#include <be/support/String.h>
#include <betterscrollview.h>
#include "addonview.h"
#include "appdefs.h"
#include "appview.h"
#include "appwindow.h"
#include "commandconstants.h"
#include "editorview.h"
#include "guistrings.h"
#include "naview.h"
#include "mpegview.h"
#include "picklistview.h"
#include "preferences.h"
#include "taview.h"

#ifdef _TTE_
#include "ttinfoview.h"
#endif

AppView::AppView(BRect frame) : BView(frame, "ArmyKnifeAppView", B_FOLLOW_ALL,
		B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE_JUMP)
{
	PRINT(("AppView::AppView(BRect)\n"));

	m_adding = 0;
	m_applying = false;

	BRect r = Bounds();
	r.OffsetTo(B_ORIGIN);
	r.top = r.bottom - 7;
	r.left = r.right - 7;

//	BDragger *dragger = new BDragger(r, this, B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
//	AddChild(dragger);
}

AppView::AppView(BMessage *data) : BView(data)
{
	PRINT(("AppView::AppView(BMessage)\n"));

	m_adding = 0;
	m_applying = false;

	BMessage msg;
	BRect r = Bounds();
	r.OffsetTo(B_ORIGIN);
	r.top = r.bottom - 7;
	r.left = r.right - 7;
/*
	BDragger *dragger = new BDragger(r, this, B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	dragger->Archive(&msg); // tricky: force setting the popup menu
	delete dragger;
	dragger = new BDragger(&msg);
	AddChild(dragger);
*/
}

AppView::~AppView()
{
	PRINT(("AppView::~AppView()\n"));
	
	m_preferences->SetMode(m_pick_list_view->SelectedIndex());
	
	delete m_preferences;
}

void
AppView::SaveWindowFrame()
{
	PRINT(("AppView::SaveWindowFrame()\n"));

	BRect rect = Window()->Frame();
	m_preferences->SetWindowFrame(rect);
}

void
AppView::InitView()
{
	PRINT(("AppView::InitView()\n"));
	
	m_preferences = new Preferences();
	
	BRect winframe;
	m_preferences->GetWindowFrame(& winframe);
	
	Window()->MoveTo(winframe.LeftTop());
	Window()->ResizeTo(winframe.Width(), winframe.Height());

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	float space = 6;

	BRect frame(0,0,100,100);
	m_pick_list_view = new PickListView(frame,"m_pick_list_view",
			B_FOLLOW_LEFT | B_FOLLOW_TOP);
	AddChild(m_pick_list_view);

	BRect inframe = m_pick_list_view->BoxBounds();
	AddOnView * aView;

	// Edit view
	aView = new EditorView(inframe, m_preferences);
	if (aView->InitCheck() == B_OK)
		m_pick_list_view->AddView(aView);
	else
		delete aView;

	// Copy view
	aView = new TAView(inframe, m_preferences);
	if (aView->InitCheck() == B_OK)
		m_pick_list_view->AddView(aView);
	else
		delete aView;

	// Name view
	aView = new NAView(inframe, m_preferences);
	if (aView->InitCheck() == B_OK)
		m_pick_list_view->AddView(aView);
	else
		delete aView;

	// MPEG view
	aView = new MPEGView(inframe, m_preferences);
	if (aView->InitCheck() == B_OK)
		m_pick_list_view->AddView(aView);
	else
		delete aView;

#ifdef _TTE_
	aView = new TTInfoView(inframe, m_preferences);
	if (aView->InitCheck() == B_OK)
		m_pick_list_view->AddView(aView);
	else
		delete aView;
#endif

	frame = m_pick_list_view->Frame();
	frame.bottom = Bounds().bottom - 50;
	frame.OffsetBy(frame.Width() + space,0);
	frame.InsetBy(0,12);
	frame.right = Bounds().right - space - B_V_SCROLL_BAR_WIDTH;
	frame.bottom -=  B_H_SCROLL_BAR_HEIGHT;
	m_list_view = new AKListView(frame,"m_list_view",B_MULTIPLE_SELECTION_LIST, B_FOLLOW_ALL);
	m_list_view->SetSelectionMessage(new BMessage(SELECTION_CHANGED));

	m_scroll_view = new BetterScrollView("m_scroll_view",m_list_view,B_FOLLOW_ALL);
	m_scroll_view->SetDataRect(BRect(0,0,0,0));
	AddChild(m_scroll_view);


	BRect textFrame = m_scroll_view->Frame();
	textFrame.bottom += 15;
	textFrame.top = textFrame.bottom - 10;
	textFrame.left = textFrame.right - 150;
	m_selected_string_view = new BStringView(textFrame,"m_selected_string_view","",
			B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	m_selected_string_view->SetAlignment(B_ALIGN_RIGHT);
	m_selected_string_view->SetFontSize(10);
	AddChild(m_selected_string_view);

	frame = Bounds();
	frame.InsetBy(space,space);
	frame.top = frame.bottom - 30;
	frame.left = frame.right - 70;
	m_apply_button = new BButton(frame,"m_apply_button", APPLY_BUTTON, new BMessage(MSG_APPLY),
			B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	m_apply_button->SetEnabled(false);
	AddChild(m_apply_button);

	frame.OffsetBy(-(frame.Width() + space),0);
	m_reset_button = new BButton(frame,"m_reset_button", RESET_BUTTON, new BMessage(MSG_RESET),
			B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	m_reset_button->SetEnabled(true);
	AddChild(m_reset_button);

	frame = Bounds();
	frame.InsetBy(space,space);
	frame.right = (m_reset_button->Frame()).left - space;
	frame.top = frame.bottom - 35;
	m_status_bar = new BStatusBar(frame,"m_status_bar","0%","100%");
	m_status_bar->SetResizingMode(B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM);
	m_status_bar->Hide();
	AddChild(m_status_bar);

	frame.InsetBy(0,5);
	frame.top += 10;

	m_barberpole = new Barberpole (frame, "barberpole", B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM, B_WILL_DRAW|B_FRAME_EVENTS);
	AddChild(m_barberpole);
/*	
	BView *dragger = FindView("_dragger_");
	if (dragger) {
		RemoveChild(dragger); // tricky: avoid redraw bugs
		AddChild(dragger);
		dragger->SetViewColor(ViewColor());
		dragger->SetLowColor(LowColor());
	}
*/
	// The following is a kludge to give each addon view a 
	// pointer to the list view. It's done this way, for now,
	// as the m_list_view is created after the add-ons
	// due to how the layout is done.
	
	AddOnView* addOnView;
	int numViews = m_pick_list_view->CountViews();
	for (int i = 0; i < numViews; i++) {
		addOnView = dynamic_cast<AddOnView*>(m_pick_list_view->ViewAt(i));
		addOnView->SetListView(m_list_view);
	}

	SelectView(m_preferences->GetMode());
}

void // *******
AppView::DisableInterface()
{
	m_apply_button->SetEnabled(false);
	m_reset_button->SetEnabled(false);
	((AppWindow*)Window())->DisableInterface();
}

void
AppView::EnableInterface()
{
	m_apply_button->SetEnabled(true);
	m_reset_button->SetEnabled(true);
	((AppWindow*)Window())->EnableInterface();
} // *******

status_t
AppView::Archive(BMessage *data, bool deep) const
{
	deep = false; /* force */
	BView::Archive(data, deep);
	data->AddString("add_on", SIGNATURE);
	return 0;
}

BArchivable
*AppView::Instantiate(BMessage *data)
{
	if (!validate_instantiation(data, "AppView"))
		return NULL;
	return new AppView(data);
}

void
AppView::SelectView(int32 index)
{
	PRINT(("AppView::SelectView(int32)\n"));

	m_pick_list_view->SelectView(index);
}

bool AppView::HasAddonView(const char *name)
{
	int32 count = m_pick_list_view->CountViews();
	for (; count > 0;) {
		BView *v = m_pick_list_view->ViewAt(--count);
		if (!strcmp(v->Name(), name))
			return true;
	}
	return false;
}

void
AppView::SelectionChanged()
{
	PRINT(("AppView::SelectionChanged()\n"));

	if(! m_applying)
	{
		int numViews = m_pick_list_view->CountViews();

		if(numViews > 0)
		{
			BList list;
			int i=0;
			int selected;
			while((selected = m_list_view->CurrentSelection(i)) >= 0)
			{
				list.AddItem(m_list_view->ItemAt(selected));
				i++;
			}

			BString buf;
			buf << list.CountItems();
			buf << " / ";
			buf << m_list_view->CountItems();
			buf << SELECTED_TEXT;
			m_selected_string_view->SetText(buf.String());

			if(m_list_view->HasSelectionOfOnlyAcceptedItems())
			{
				m_apply_button->SetEnabled(true);
			}
			else
			{
				m_apply_button->SetEnabled(false);
			}

			for(int i = 0; i < numViews; i++)
			{
				AddOnView* view = (AddOnView*)m_pick_list_view->ViewAt(i);
				if(view)
				{
					view->SelectionChanged(&list);
				}
			}
		}
	}
}

void
AppView::Apply()
{
	PRINT(("AppView::Apply()\n"));
	
	if (! Busy())
	{
		DisableInterface();
	
		AddOnView* view = (AddOnView*)m_pick_list_view->SelectedView();
		if(view)
		{
			view->Apply();
		}
	}
}

void
AppView::Reset()
{
	PRINT(("AppView::Reset()\n"));

	if (! Busy())
	{
		AddOnView* view = (AddOnView*)m_pick_list_view->SelectedView();
		if(view)
		{
			view->Reset();
		}
	}
}

void
AppView::ClearList()
{
	PRINT(("AppView::ClearList()\n"));

	if (! Busy())
	{
		m_list_view->DeselectAll();
		int numItems = m_list_view->CountItems();
		for(int i=0;i<numItems;i++)
		{
			BListItem* item = m_list_view->RemoveItem((int32)0);
			delete item;
		}
		m_list_view->Invalidate();
		m_scroll_view->SetDataRect(BRect(0,0,0,0));
	}
}

void
AppView::SelectAll()
{
	PRINT(("AppView::SelectAll()\n"));

	if (! Busy())
	{
		int numItems = m_list_view->CountItems();
		m_list_view->Select(0,(int32)numItems-1);
	}
}

void
AppView::SelectAllUnsupported()
{
	PRINT(("AppView::SelectAllUnsupported()\n"));

	if (!Busy())
		m_list_view->SelectAllUnsupported();
}

void
AppView::SelectPreviousFile()
{
	PRINT(("AppView::SelectPreviousFile()\n"));
	
	if (! Busy())
	{
		int32 index = m_list_view->CurrentSelection(0);
	
		if (index < 0)
			m_list_view->Select(m_list_view->CountItems()-1);
	
		if (index > 0)
			m_list_view->Select(--index);
			
			m_list_view->ScrollToSelection();
	}
}

void
AppView::SelectNextFile()
{
	PRINT(("AppView::SelectNextFile()\n"));

	if (! Busy())
	{
		if (m_list_view->CurrentSelection(0) < 0)
			m_list_view->Select(0);
		else
		{
			int32 index = 0;
					
			while (m_list_view->CurrentSelection(index) >= 0)
				index++;
			
			int32 last_selected = m_list_view->CurrentSelection(--index);
			m_list_view->Select(++last_selected);
			
			m_list_view->ScrollToSelection();
		}
	}
}

void
AppView::SelectFirstFile()
{
	PRINT(("AppView::SelectFirstFile()\n"));

	if (! Busy())
	{
		if (m_list_view->CountItems() > 0)
		{
			m_list_view->DeselectAll();
			m_list_view->Select(0);
			m_list_view->ScrollToSelection();
		}
	}
}

void
AppView::SelectLastFile()
{
	PRINT(("AppView::SelectLastFile()\n"));

	if (! Busy())
	{
		if (m_list_view->CountItems() > 0)
		{
			m_list_view->DeselectAll();
			m_list_view->Select(m_list_view->CountItems() - 1);
			m_list_view->ScrollToSelection();
		}
	}
}

bool
AppView::Busy()
{
	PRINT(("AppView::WeAreBusy()\n"));

	if (m_applying || m_adding > 0)
		return true;
	else
		return false;
}

void
AppView::AddRefs(BMessage* refsMessage)
{
	PRINT(("AppView::AddRefs(BMessage*)\n"));
	
	if (! m_applying)
	{
		m_adding++;
		m_barberpole->Start();
		
	
		BList* args = new BList();
		BMessage* msg = new BMessage(*refsMessage);
		args->AddItem(this);
		args->AddItem(msg);
	
		thread_id my_thread = spawn_thread(&AppView::AddRefsThreadFunc,"AddRefs",
				B_NORMAL_PRIORITY,(void*)args);
		resume_thread(my_thread);
	}
}

int32
AppView::AddRefsThreadFunc(void* data)
{
	PRINT(("AppView::AddRefsThreadFunc(void*)\n"));

	signal(SIGQUIT,quit_requested);

	BList* dataList = (BList*)data;
	AppView* view = (AppView*)dataList->ItemAt(0);
	BMessage* refsMessage = (BMessage*)dataList->ItemAt(1);

	if(view->LockLooper())
	{
		view->m_list_view->DeselectAll();
		view->UnlockLooper();
	}

	// recurse on refs message
	entry_ref tmp_ref;
	int numRefs = 0;
	EntryRefItem * ref_item = NULL;
	
	while(refsMessage->FindRef("refs", numRefs++, & tmp_ref) == B_NO_ERROR)
	{
		ref_item = new EntryRefItem (& tmp_ref);
		view->AddRefsHelper(view, ref_item);
	}

	if(view->LockLooper())
	{
		int numItems = view->m_list_view->CountItems();
		float frameWidth = 0.0;
		float frameHeight= 0.0;
		for(int i=0;i<numItems;i++)
		{
			BRect frame = view->m_list_view->ItemFrame(i);
			frameHeight += (frame.Height() + 1.00);

			EntryRefItem* item = (EntryRefItem*)view->m_list_view->ItemAt(i);
			float width = view->m_list_view->StringWidth(item->Text());
			if(width > frameWidth)
			{
				frameWidth = width;
			}
		}
		BRect dataRect(0,0,frameWidth,frameHeight);
		view->m_scroll_view->SetDataRect(dataRect);
		
		view->m_list_view->SortItems(&AppView::SortFunc);

		view->m_list_view->SetSelectionMessage(NULL);
		AddOnView* addOnView = dynamic_cast<AddOnView*>(view->m_pick_list_view->SelectedView());
		addOnView->ListContentAdded();
		view->m_list_view->SetSelectionMessage(new BMessage(SELECTION_CHANGED));
		view->SelectionChanged();
		
		view->m_barberpole->Stop();

		view->m_adding--;

		view->UnlockLooper();
	}

	return(B_OK);
}

void
AppView::AddRefsHelper(AppView * view, EntryRefItem * ref_item)
{
	PRINT(("AppView::AddRefsHelper()\n"));

	if(ref_item->IsFile())
	{
		bool found = false;
		for(int i=0; i<m_list_view->CountItems(); i++)
		{
			EntryRefItem* item = (EntryRefItem*)m_list_view->ItemAt(i);
			entry_ref* ref = item->EntryRef();
			if(*ref == *(ref_item->EntryRef()))
			{
				found = true;
				delete ref_item;
				return;
			}
		}
		if(!found)
		{
			if(view->LockLooper())
			{
				view->m_list_view->AddItem(ref_item);
				view->UnlockLooper();
			}
		}
	}
	else if(ref_item->IsDirectory())
	{
		BDirectory dir(ref_item->EntryRef());
		if(dir.InitCheck() == B_OK)
		{
			entry_ref ref;
			EntryRefItem * ref_item;
			while(dir.GetNextRef(& ref) == B_OK)
			{
				ref_item = new EntryRefItem(& ref);
				AddRefsHelper(view, ref_item);
			}
		}
		delete ref_item;
	}
	else if(ref_item->IsSymLink())
	{
		if (ref_item->TraverseSymlink() == B_OK)
		{
			AddRefsHelper(view, ref_item);
		}
		else
		{
			delete ref_item;
		}
	}
}

int
AppView::SortFunc(const void* first, const void* second)
{
	//PRINT(("AppWindow::SortFunc(const void*,const void*)\n"));
	EntryRefItem* firstItem = *((EntryRefItem**)first);
	EntryRefItem* secondItem = *((EntryRefItem**)second);
	return strcmp(firstItem->Text(),secondItem->Text());
}

void
AppView::AttachedToWindow()
{
	PRINT(("AppView::AttachedToWindow()\n"));

	BView::AttachedToWindow();

	InitView();

	m_list_view->SetTarget(this);
	m_apply_button->SetTarget(this);
	m_reset_button->SetTarget(this);
	
	m_apply_button->MakeDefault(true);
}

void
AppView::GetPreferredSize(float* width, float* height)
{
	int space = 10;

	*width = Bounds().Width();
	*height = m_pick_list_view->Frame().Height() + m_status_bar->Frame().Height() +
		3*space;
}

void
AppView::MessageReceived(BMessage* message)
{
	//PRINT(("AppView::MessageReceived(BMessage*)\n"));

	switch(message->what)
	{
		case SELECTION_CHANGED:
			SelectionChanged();
			break;
		case MSG_APPLY:
			Apply();
			break;
		case MSG_RESET:
			Reset();
			break;
		case START_APPLY:
			m_applying = true;
			m_barberpole->Hide();
			m_status_bar->Show();
			m_apply_button->SetEnabled(false);
			m_reset_button->SetEnabled(false);
			m_pick_list_view->MessageReceived(message);
			break;
		case END_APPLY:
			m_applying = false;
			m_status_bar->Hide();
			m_barberpole->Show();
			m_apply_button->SetEnabled(true);
			m_reset_button->SetEnabled(true);
			SelectionChanged();
			EnableInterface();
			m_pick_list_view->MessageReceived(message);
			break;
		case STATUS_BAR_SET_MAX_VALUE:
			int32 maxValue;
			message->FindInt32("maxValue",&maxValue);
			m_status_bar->SetMaxValue(maxValue);
			break;
		case STATUS_BAR_FILE:
		{
			BString file;
			if (message->FindString("file", &file) == B_OK)
			{
				file.Prepend("  ");
				m_status_bar->SetText(file.String());
			}	
			break;
		}
		case STATUS_BAR_UPDATE:
			m_status_bar->Update(1);
			break;
		case STATUS_BAR_RESET:
			m_status_bar->Reset("0%","100%");
			break;
		case INVALIDATE_LIST:
			m_list_view->Invalidate();
			m_list_view->SortItems(&AppView::SortFunc);
			break;
		case B_REFS_RECEIVED:
			AddRefs(message);
			break;
		case B_SIMPLE_DATA:
			AddRefs(message);
			break;
			
		default:
			BView::MessageReceived(message);
	}
}

void
AppView::SetPreviousMode()
{
	if (! Busy())
	{
		int32 mode = m_pick_list_view->SelectedIndex();
	
		if (mode > 0)
		{
			mode--;
			m_pick_list_view->SelectView(mode);
		}
	}
}

void
AppView::SetNextMode()
{
	if (! Busy())
	{
		int32 mode = m_pick_list_view->SelectedIndex();
	
		if ((mode+1) < m_pick_list_view->CountViews())
		{
			mode++;
			m_pick_list_view->SelectView(mode);
		}
	}
}

void quit_requested(int sig)
{
	exit_thread(0);
}
