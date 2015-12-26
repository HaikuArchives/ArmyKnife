#ifndef __EDITOR_VIEW_H__
#define __EDITOR_VIEW_H__

#include "addonview.h"
#include "preferences.h"
#include "albumpictureview.h"

class BCheckBox;
class BList;
class BMenuField;
class BMessage;
class BRadioButton;
class BRect;
class BTextControl;

class EditorView : public AddOnView
{
	public:
		EditorView(Preferences * preferences);
		~EditorView();
		virtual void AttachedToWindow();
		virtual void Hide();
		virtual void Show();
		virtual void SelectionChanged(BList* list);
		virtual void Apply();
		virtual void Reset();
		virtual void MessageReceived(BMessage* message);
		virtual bool AcceptListItem(EntryRefItem* listItem);
	private:
		void InitView();
		void GenreSelectionAction();
		static int32 ApplyFunction(void* args);
		void SetAllEnabled();
		void SetEnabled(BCheckBox* checkbox, BControl* control);
		void SetEnabled(BCheckBox* checkbox, BMenuField* menufield);
		void CheckAllBoxes(int32 value);
		void WidgetsSetValues();
		void WidgetsSetEnabled();
		void WidgetsRBValues();

		AlbumPictureView* m_album_picture;

		BRadioButton*	m_tag_radiobutton;
		BRadioButton*	m_attribute_radiobutton;
		BCheckBox*		m_apply_checkbox;
		BCheckBox*		m_picture_checkbox;
		BCheckBox*		m_artist_checkbox;
		BCheckBox*		m_album_checkbox;
		BCheckBox*		m_title_checkbox;
		BCheckBox*		m_year_checkbox;
		BCheckBox*		m_comment_checkbox;
		BCheckBox*		m_track_checkbox;
		BCheckBox*		m_genre_checkbox;
#ifdef _TTE_
		BCheckBox*		m_rating_checkbox;
		BCheckBox*		m_tempo_checkbox;
		BCheckBox*		m_composer_checkbox;
		BCheckBox*		m_gender_checkbox;
#endif
		BCheckBox*		m_clear_all_checkbox;

		BTextControl*	m_artist_textcontrol;
		BTextControl*	m_album_textcontrol;
		BTextControl*	m_title_textcontrol;
		BTextControl*	m_year_textcontrol;
		BTextControl*	m_comment_textcontrol;
		BTextControl*	m_track_textcontrol;
		BTextControl*	m_genre_textcontrol;
		BMenuField*		m_genre_menufield;
#ifdef _TTE_
		BTextControl*	m_rating_textcontrol;
		BTextControl*	m_tempo_textcontrol;
		BTextControl*	m_composer_textcontrol;
		BTextControl*	m_gender_textcontrol;
#endif
		BBox*			m_edit_box;
		BBox*			m_genre_box;

		bool			m_album_picture_changed;

		Preferences *	m_preferences;
};

#endif
