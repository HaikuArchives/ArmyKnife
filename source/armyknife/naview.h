#ifndef __NA_VIEW_H__
#define __NA_VIEW_H__

#include "addonview.h"
#include "preferences.h"
#include "nawindow.h"

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

class NAView : public AddOnView
{
	public:
		NAView(BRect frame, Preferences * preferences);
		~NAView();
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
		void MakePatternMenu();
		
		BRadioButton*	m_a2n_radiobutton;
		BRadioButton*	m_n2a_radiobutton;
		BStringView*	m_artist_stringview;
		BStringView*	m_album_stringview;
		BStringView*	m_title_stringview;
		BStringView*	m_year_stringview;
		BStringView*	m_comment_stringview;
		BStringView*	m_track_stringview;
		BStringView*	m_genre_stringview;
		BStringView*	m_wildcard_stringview;
		BBox*			m_direction_box;
		
		BMessage		m_filename_patterns;
		BMenuField*		m_pattern_menufield;
		BMenu*			m_pattern_menu;
		PatternWindow*	m_pattern_window;
		
		Preferences*	m_preferences;

};

class NameValueMatcher
{
	public:
		NameValueMatcher(const char* nameStr, const char* valueStr, char delim);
		~NameValueMatcher();

		int NextMatch(char* name, char* value);

	private:
		char _delim;
		char* _name;
		char* _value;
		char* name_ptr;
		char* value_ptr;
};

#endif
