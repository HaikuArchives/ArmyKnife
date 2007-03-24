#include <be/app/Message.h>
#include <be/interface/Box.h>
#include <be/interface/Menu.h>
#include <be/interface/MenuField.h>
#include <be/interface/MenuItem.h>
#include <be/interface/Rect.h>
#include <be/support/Archivable.h>
#include <be/support/Debug.h>
#include "CommandConstants.h"
#include "PickListView.h"

#include "AppView.h"

PickListView::PickListView(BRect frame, const char* name = NULL,
	uint32 resizingMode = B_FOLLOW_ALL,
	uint32 flags = B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE_JUMP,
	border_style border = B_FANCY_BORDER) :
		BView(frame,name,resizingMode,flags)
{
	PRINT(("PickListView::PickListView(BRect,const char*,uint32,uint32,border_style)\n"));

	m_selected_index = NO_VIEW_SELECTED;

	frame = Bounds();
	frame.InsetBy(10,10);
	m_view_menu = new BMenu("m_view_menu");
	m_view_menu->SetLabelFromMarked(true);
	m_view_box = new BBox(frame,"pickListViewBox",B_FOLLOW_ALL,flags,border);
	m_view_box->SetLabel(new BMenuField(BRect(0,0,0,0), "label", NULL, m_view_menu));
	AddChild(m_view_box);
}

PickListView::PickListView(BMessage* archive) : BView(archive)
{
	PRINT(("PickListView::PickListView(BMessage*)\n"));

	if(archive->FindInt32("m_selected_index",&m_selected_index) != B_OK)
	{
		m_selected_index = NO_VIEW_SELECTED;
	}
	m_view_box = (BBox*)FindView("pickListViewBox");
	m_view_menu = ((BMenuField*)FindView("label"))->Menu();
}

PickListView::~PickListView()
{
	PRINT(("PickListView::~PickListView()\n"));
}

BArchivable*
PickListView::Instantiate(BMessage* archive)
{
	PRINT(("PickListView::Instantiate(BMessage*)\n"));

	BArchivable* result = NULL;
	if(validate_instantiation(archive,"PickListView"))
	{
		result = new PickListView(archive);
	}
	return result;
}

status_t
PickListView::Archive(BMessage* archive, bool deep=true) const
{
	PRINT(("PickListView::Archive(BMessage*,bool)\n"));

	status_t result = BView::Archive(archive, deep);
	if(result == B_OK)
	{
		result = archive->AddInt32("m_selected_index", m_selected_index);
	}
	return result;
}

void
PickListView::AttachedToWindow()
{
	PRINT(("PickListView::AttachedToWindow()\n"));

	BView::AttachedToWindow();

	BView* parent = Parent();
	if(parent)
	{
		SetViewColor(parent->ViewColor());
	}
	else
	{
		SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	}
	m_view_menu->SetTargetForItems(this);
	SelectView(m_selected_index);
}

void
PickListView::MessageReceived(BMessage* message)
{
	//PRINT(("PickListView::MessageReceived(BMessage*)\n"));

	switch(message->what)
	{
		case MENU_ITEM:
			MenuSelectionChanged(message);
			break;
			
		case START_APPLY:
			m_view_menu->Supermenu()->SetEnabled(false);
			break;
			
		case END_APPLY:
			m_view_menu->Supermenu()->SetEnabled(true);
			break;			
			
		default:
			BView::MessageReceived(message);
	}
}

void
PickListView::MenuSelectionChanged(BMessage* message)
{
	PRINT(("PickListView::MenuSelectionChanged(BMessage*)\n"));

	int32 index;
	message->FindInt32("index",&index);
	SelectView(index);
}

BRect
PickListView::BoxBounds()
{
	PRINT(("PickListView::BoxBounds()\n"));

	BRect bounds = m_view_box->Bounds();
	bounds.OffsetBy(5,10);
	bounds.InsetBy(10,15);
	return bounds;
}

void
PickListView::AddView(BView* view)
{
	PRINT(("PickListView::AddView(BView*)\n"));

	// if view doesn't belong to anyone
	if((view->Window() == NULL) && (view->Parent() == NULL))
	{
		while(view->IsHidden())
		{
			view->Show();
		}
		while(!view->IsHidden())
		{
			view->Hide();
		}

		if(view->Bounds().Width() > BoxBounds().Width())
		{
			float diff = view->Bounds().Width() - BoxBounds().Width();
			ResizeBy(diff+15,0);
		}
		else
		{
			float diff = BoxBounds().Width() - view->Bounds().Width();
			view->ResizeBy(diff-15,0);
		}

		if(view->Bounds().Height() > BoxBounds().Height())
		{
			float diff = view->Bounds().Height() - BoxBounds().Height();
			ResizeBy(0,diff+25);
		}
		else
		{
			float diff = BoxBounds().Height() - view->Bounds().Height();
			view->ResizeBy(0,diff-25);
		}

		m_view_menu->AddItem(new BMenuItem(view->Name(),new BMessage(MENU_ITEM)));
		m_view_menu->SetTargetForItems(this);
		m_view_box->AddChild(view);
	}
}

void
PickListView::RemoveView(BView* view)
{
	PRINT(("PickListView::RemoveView(BView*)\n"));

	RemoveView(IndexOf(view));
}

void
PickListView::RemoveView(int32 index)
{
	PRINT(("PickListView::RemoveView(int32)\n"));

	if((index >= 0) && (index < CountViews()))
	{
		BMenuItem* item = m_view_menu->RemoveItem(index);
		delete item;
		m_view_box->RemoveChild(ViewAt(index));
	}
	if(CountViews() > 0)
	{
		SelectView((int32)0);
	}
	else
	{
		m_selected_index = NO_VIEW_SELECTED;
	}
}

void
PickListView::SelectView(BView* view)
{
	PRINT(("PickListView::SelectView(BView*)\n"));

	SelectView(IndexOf(view));
}

void
PickListView::SelectView(int32 index)
{
	PRINT(("PickListView::SelectView(int32)\n"));

	if(index != m_selected_index)
	{
		if(Window())
		{
			if(LockLooper())
			{
				if((index >= 0) && (index < CountViews()))
				{
					if(m_selected_index != NO_VIEW_SELECTED)
					{
						BView* view = ViewAt(m_selected_index);
						view->Hide();
					}

					BMenuItem* item = m_view_menu->ItemAt(index);
					item->SetMarked(true);
					BView* view = ViewAt(index);
					view->Show();
					m_selected_index = index;
				}
				UnlockLooper();
			}
		}
	}
}

BView*
PickListView::SelectedView()
{
	PRINT(("PickListView::SelectdView()\n"));

	return ViewAt(SelectedIndex());
}

int32
PickListView::SelectedIndex()
{
	PRINT(("PickListView::SelectdIndex()\n"));

	return m_selected_index;
}

bool
PickListView::IsSelected(BView* view)
{
	PRINT(("PickListView::IsSelectd(BView*)\n"));

	return IsSelected(IndexOf(view));
}

bool
PickListView::IsSelected(int32 index)
{
	PRINT(("PickListView::IsSelectd(int32)\n"));

	return (index == SelectedIndex());
}
		
int32
PickListView::CountViews()
{
	PRINT(("PickListView::CountViews()\n"));

	return m_view_box->CountChildren() - 1;
}

BView*
PickListView::ViewAt(int32 index)
{
	PRINT(("PickListView::ViewAt(int32)\n"));

	BView* result = NULL;
	if((index >= 0) && (index < CountViews()))
	{
		result = m_view_box->ChildAt(index+1);
	}
	return result;
}

int32
PickListView::IndexOf(BView* view)
{
	PRINT(("PickListView::IndexOf(BView*)\n"));

	int32 numChildren = CountViews();
	int32 result = VIEW_NOT_FOUND;
	for(int i=0;i<numChildren;i++)
	{
		if(view == ViewAt(i))
		{
			result = i;
			break;
		}
	}
	return result;
}
