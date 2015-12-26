#include <be/interface/Rect.h>
#include <be/interface/Window.h>
#include <be/support/Debug.h>
#include <be/support/List.h>

#include "commandconstants.h"
#include "addonview.h"

AddOnView::AddOnView(const char* name) :
	BView(name, B_NAVIGABLE_JUMP | B_FULL_UPDATE_ON_RESIZE)
{
	PRINT(("AddOnView::AddOnView(BRect,const char*)\n"));

	m_selected_items = new BList();
	m_init_status = B_OK;
}

AddOnView::~AddOnView()
{
	PRINT(("AddOnView::~AddOnView()\n"));

	delete m_selected_items;
}

void
AddOnView::AttachedToWindow()
{
	BView::AttachedToWindow();

	PRINT(("AddOnView::AttachedToWindow()\n"));

	BView* parent = Parent();
	if(parent)
	{
		SetViewColor(parent->ViewColor());
	}
	else
	{
		SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	}
}

void
AddOnView::Hide()
{
	PRINT(("AddOnView::Hide()\n"));

	BView::Hide();
}

void
AddOnView::Show()
{
	PRINT(("AddOnView::Show()\n"));

	BView::Show();
	
	FlagUnacceptedListItems();
	
	if (Window())
		Window()->PostMessage(MSG_MAKE_APPLY_BUTTON_DEFAULT);
}

void
AddOnView::SelectionChanged(BList* list)
{
	PRINT(("AddOnView::SelectionChanged(BList* list)\n"));
	
	m_selected_items->MakeEmpty();
	m_selected_items->AddList(list);
}

void
AddOnView::Apply()
{
	PRINT(("AddOnView::Apply()\n"));
}

void
AddOnView::Reset()
{
	PRINT(("AddOnView::Reset()\n"));
}

status_t
AddOnView::InitCheck()
{
	PRINT(("AddOnView::Initcheck(), returning 0x%08lx\n", m_init_status));
	return m_init_status;
}

void
AddOnView::SetListView(AKListView* listView)
{
	PRINT(("AddOnView::SetListView()\n"));
	m_list_view = listView;
}

void
AddOnView::FlagUnacceptedListItems()
{
	PRINT(("AddOnView::FlagUnacceptedListItems()\n"));
	
	bool foundUnaccepted = false;
	
	EntryRefItem* listItem;
	int numItems = m_list_view->CountItems();
	for (int i = 0; i < numItems; i++) {
		listItem = dynamic_cast<EntryRefItem*>(m_list_view->ItemAt(i));
		if (listItem) {
			if (!AcceptListItem(listItem)) {
				listItem->SetAccepted(false);
				m_list_view->InvalidateItem(i);
				foundUnaccepted = true;
			} else {
				listItem->SetAccepted(true);
				m_list_view->InvalidateItem(i);
			}
		}
	}
	
	if (foundUnaccepted) {
		m_list_view->DeselectAll();
		m_list_view->ScrollToFirstUnaccepted();
	} else {
		m_list_view->SelectAll();
	}
}

void
AddOnView::ListContentAdded()
{
	PRINT(("AddOnView::ListContentAdded()\n"));
	
	FlagUnacceptedListItems();
}

bool
AddOnView::AcceptListItem(EntryRefItem* listItem)
{
	PRINT(("AddOnView::AcceptListItem()\n"));
	
	if (listItem->IsSupportedByTaglib())
		return true;
	else
		return false;
}
