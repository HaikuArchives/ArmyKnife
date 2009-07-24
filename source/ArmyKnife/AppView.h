#ifndef __APP_VIEW_H__
#define __APP_VIEW_H__

#include <be/interface/View.h>
#include "AKListView.h"
#include "Barberpole.h"
#include "EntryRefItem.h"
#include "Preferences.h"

class BButton;
class BetterScrollView;
class BListView;
class BMessage;
class BRect;
class BStatusBar;
class BStringView;
class PickListView;
class BDragger;

class AppView : public BView
{
	public:
		AppView(BRect frame);
		AppView(BMessage *data);
		~AppView();
		static	BArchivable	*Instantiate(BMessage *data);
		virtual	status_t Archive(BMessage *data, bool deep = true) const;
		virtual void AttachedToWindow();
		virtual void GetPreferredSize(float* width, float* height);
		virtual void MessageReceived(BMessage* message);
		
		void Apply();
		void Reset();
		void ClearList();
		void SelectAll();
		void SelectPreviousFile();
		void SelectNextFile();
		void SelectFirstFile();
		void SelectLastFile();

		void SelectView(int32 index);
		bool HasAddonView(const char *name);
		void SetPreviousMode();
		void SetNextMode();
		
		void SaveWindowFrame();
		
		BButton*			m_apply_button;
		
	private:
	
		bool Busy();
		void InitView();
		void DisableInterface();
		void EnableInterface();
		void SelectionChanged();
		void AddRefs(BMessage* refsMessage);
		static int32 AddRefsThreadFunc(void* data);
		void AddRefsHelper(AppView * view, EntryRefItem * ref_item);
		static int SortFunc(const void* first, const void* second);

		PickListView*		m_pick_list_view;
		AKListView*			m_list_view;
		BetterScrollView*	m_scroll_view;
		BStringView*		m_selected_string_view;
		
		BButton*			m_reset_button;
		BStatusBar*			m_status_bar;

		int32				m_adding;
		bool				m_applying;

		Preferences*		m_preferences;
		Barberpole*			m_barberpole;
};

void quit_requested(int sig);

#endif
