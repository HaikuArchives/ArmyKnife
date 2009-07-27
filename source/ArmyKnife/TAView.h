#ifndef __TA_VIEW_H__
#define __TA_VIEW_H__

#include "AddOnView.h"
#include "Preferences.h"

class BCheckBox;
class BList;
class BRadioButton;
class BRect;

class TAView : public AddOnView
{
	public:
		TAView(BRect frame, Preferences * preferences);
		~TAView();
		virtual void AttachedToWindow();
		virtual void GetPreferredSize(float* width, float* height);
		virtual void Hide();
		virtual void Show();
		virtual void SelectionChanged(BList* list);
		virtual void Apply();
		virtual void Reset();
		virtual bool AcceptListItem(EntryRefItem* listItem);
	private:
		void InitView();
		static int32 ApplyFunction(void* args);
		void WidgetsSetValues();
		void WidgetsSetEnabled();
		
		BRadioButton*	m_a2t_radiobutton;
		BRadioButton*	m_t2a_radiobutton;
		BCheckBox*		m_artist_checkbox;
		BCheckBox*		m_album_checkbox;
		BCheckBox*		m_title_checkbox;
		BCheckBox*		m_year_checkbox;
		BCheckBox*		m_comment_checkbox;
		BCheckBox*		m_track_checkbox;
		BCheckBox*		m_genre_checkbox;
		BBox*			m_direction_box;
		
		Preferences*	m_preferences;
};

#endif
