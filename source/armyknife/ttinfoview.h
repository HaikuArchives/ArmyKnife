#ifndef __TT_COMMENT_VIEW_H__
#define __TT_COMMENT_VIEW_H__
#ifdef _TTE_

#include "addonview.h"
#include "preferences.h"

class BList;
class BMessage;
class BRect;
class BTextView;
class BStringView;

class TTInfoView : public AddOnView
{
	public:
		TTInfoView(BRect frame, Preferences * preferences);
		~TTInfoView();
		virtual void AttachedToWindow();
		virtual void GetPreferredSize(float* width, float* height);
		virtual void Hide();
		virtual void Show();
		virtual void SelectionChanged(BList* list);
		virtual void Apply();
		virtual void Reset();
		virtual void MessageReceived(BMessage* message);
		virtual bool AcceptListItem(EntryRefItem* listItem);
		
	private:
		void InitView();
		static int32 ApplyFunction(void* args);

		void WidgetsSetValues();
		void WidgetsSetEnabled();
		void WidgetsRBValues();
		
		void GetInfoFromSelection(int32 numberOfSelectedItems, BString *output, bool doListFiles = false);
		bool IsNewInfoSane(const char * text);

		BStringView *m_string_view;
		BTextView *m_comment_view;

		Preferences *m_preferences;
};

#endif // _TTE_
#endif // __TT_COMMENT_VIEW_H__
