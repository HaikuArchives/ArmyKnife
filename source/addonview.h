#ifndef __ADDON_VIEW_H__
#define __ADDON_VIEW_H__

#include <be/interface/View.h>

#include "aklistview.h"
#include "entryrefitem.h"

class BList;
class BMessage;
class BRect;

class AddOnView : public BView
{
	public:
		AddOnView(const char* name);
		~AddOnView();
		virtual void AttachedToWindow();
		virtual void Hide();
		virtual void Show();
		virtual void SelectionChanged(BList* list);
		virtual void Apply();
		virtual void Reset();
		virtual status_t InitCheck();

		virtual void SetListView(AKListView* listView);
		virtual void ListContentAdded();
		virtual void FlagUnacceptedListItems();
		virtual bool AcceptListItem(EntryRefItem* listItem);
	protected:
		AKListView*	m_list_view;
		BList*		m_selected_items;
		status_t	m_init_status;
};

#endif
