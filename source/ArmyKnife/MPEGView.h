#ifndef __MPEG_VIEW_H__
#define __MPEG_VIEW_H__

#include "AddOnView.h"
#include "Preferences.h"

class BBox;
class BList;
class BMenu;
class BMenuField;
class BMenuItem;
class BMessage;
class BRadioButton;
class BRect;
class BStringView;
class BTextControl;

class MPEGView : public AddOnView
{
	public:
		MPEGView(BRect frame, Preferences * preferences);
		~MPEGView();
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
		
		Preferences*	m_preferences;
		
		BBox*			m_info_box;
		BStringView*	m_files_string;
		BStringView*	m_id3v1_string;
		BStringView*	m_id3v2_string;
		BStringView*	m_ape_string;
		
		BBox*			m_add_remove_box;
		BRadioButton*	m_add_radiobutton;
		BRadioButton*	m_remove_radiobutton;
		BCheckBox*		m_id3v1_checkbox;
		BCheckBox*		m_id3v2_checkbox;
		BCheckBox*		m_ape_checkbox;
		
		int32	m_files_count;
		int32	m_id3v1_count;
		int32	m_id3v2_count;
		int32	m_ape_count;
};

#endif
