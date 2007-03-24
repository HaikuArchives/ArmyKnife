#ifndef __ADDON_VIEW_H__
#define __ADDON_VIEW_H__

#include <be/interface/View.h>

class BList;
class BMessage;
class BRect;

class AddOnView : public BView
{
	public:
		AddOnView(BRect frame, const char* name);
		~AddOnView();
		virtual void AttachedToWindow();
		virtual void Hide();
		virtual void Show();
		virtual void SelectionChanged(BList* list);
		virtual void Apply();
		virtual void Reset();
		virtual status_t InitCheck();
	protected:
		BList*		m_selected_items;
		status_t	m_init_status;
};

#endif
