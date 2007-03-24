#include <be/interface/Rect.h>
#include <be/support/Debug.h>
#include <be/support/List.h>
#include "AddOnView.h"

AddOnView::AddOnView(BRect frame, const char* name) :
	BView(frame, name, B_FOLLOW_ALL, B_NAVIGABLE_JUMP | B_FULL_UPDATE_ON_RESIZE)
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
