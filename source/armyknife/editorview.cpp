#include <stdlib.h>
#include <string.h>
#include <be/app/Message.h>
#include <be/app/Messenger.h>
#include <be/interface/Box.h>
#include <be/interface/CheckBox.h>
#include <be/interface/Menu.h>
#include <be/interface/MenuField.h>
#include <be/interface/MenuItem.h>
#include <be/interface/OptionPopUp.h>
#include <be/interface/RadioButton.h>
#include <be/interface/Rect.h>
#include <be/interface/TextControl.h>
#include <be/storage/Entry.h>
#include <be/storage/File.h>
#include <be/storage/NodeInfo.h>
#include <be/storage/Path.h>
#include <be/support/Debug.h>
#include <be/support/List.h>
#include "AudioAttributes.h"
#include "GenreList.h"
#include "ID3Tags.h"
#include "AppDefs.h"
#include "AppView.h"
#include "AppWindow.h"
#include "CommandConstants.h"
#include "EditorView.h"
#include "EntryRefItem.h"
#include "GUIStrings.h"

EditorView::EditorView(BRect frame, Preferences * preferences)
 :	AddOnView		(frame, EDITOR_MODE_NAME),
 	m_preferences	(preferences)
{
	PRINT(("EditorView::EditorView(BRect)\n"));

	InitView();
}

EditorView::~EditorView()
{
	PRINT(("EditorView::~EditorView()\n"));
}

void
EditorView::InitView()
{
	PRINT(("EditorView::InitView()\n"));

	int space = 6;

	BRect frame;

	BCheckBox cb(BRect(0,0,0,0),0,0,0);
	cb.ResizeToPreferred();

	m_attribute_radiobutton = new BRadioButton(BRect(0,0,0,0),0,ATTRIBUTES_LABEL,
			new BMessage(RADIO_BUTTON_EVENT));
	m_attribute_radiobutton->ResizeToPreferred();
	m_attribute_radiobutton->SetValue(B_CONTROL_ON);

	m_tag_radiobutton = new BRadioButton(BRect(0,0,0,0),0,TAGS_LABEL,
			new BMessage(RADIO_BUTTON_EVENT));
	m_tag_radiobutton->ResizeToPreferred();

	m_apply_checkbox = new BCheckBox(BRect(0,0,0,0),0,APPLY_TO_ATTRIBUTES,new BMessage(MSG_APPLY_TO_BOTH));
	m_apply_checkbox->ResizeToPreferred();
	m_apply_checkbox->SetLabel(APPLY_TO_TAGS);

	frame.left = space;
	frame.top = space;
	frame.right = m_apply_checkbox->Frame().Width() + (3 * space);

	if(m_attribute_radiobutton->Frame().Height() >=
			m_tag_radiobutton->Frame().Height())
	{
		frame.bottom = m_attribute_radiobutton->Frame().Height() +
			m_apply_checkbox->Frame().Height() + (4 * space);
	}
	else
	{
		frame.bottom = m_tag_radiobutton->Frame().Height() +
			m_apply_checkbox->Frame().Height() + (4 * space);
	}

	m_edit_box = new BBox(frame,0);
	AddChild(m_edit_box);
	m_edit_box->AddChild(m_attribute_radiobutton);
	m_edit_box->AddChild(m_tag_radiobutton);
	m_edit_box->AddChild(m_apply_checkbox);
	m_attribute_radiobutton->MoveBy(space,space);
	m_tag_radiobutton->MoveBy(2*space+m_attribute_radiobutton->Frame().Width(),space);
	m_apply_checkbox->MoveBy(space,2*space+m_attribute_radiobutton->Frame().Height());
	
	m_artist_checkbox = new BCheckBox(BRect(0,0,0,0),0,ARTIST_LABEL,
			new BMessage(MSG_ARTIST_CHECKBOX));
	m_artist_checkbox->ResizeToPreferred();
	frame = m_artist_checkbox->Frame();

	m_album_checkbox = new BCheckBox(BRect(0,0,0,0),0,ALBUM_LABEL,
			new BMessage(MSG_ALBUM_CHECKBOX));
	m_album_checkbox->ResizeToPreferred();
	if(m_album_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_album_checkbox->Frame();
	}

	m_title_checkbox = new BCheckBox(BRect(0,0,0,0),0,TITLE_LABEL,
			new BMessage(MSG_TITLE_CHECKBOX));
	m_title_checkbox->ResizeToPreferred();
	if(m_title_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_title_checkbox->Frame();
	}

	m_year_checkbox = new BCheckBox(BRect(0,0,0,0),0,YEAR_LABEL,
			new BMessage(MSG_YEAR_CHECKBOX));
	m_year_checkbox->ResizeToPreferred();
	if(m_year_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_year_checkbox->Frame();
	}

	m_comment_checkbox = new BCheckBox(BRect(0,0,0,0),0,COMMENT_LABEL,
			new BMessage(MSG_COMMENT_CHECKBOX));
	m_comment_checkbox->ResizeToPreferred();
	if(m_comment_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_comment_checkbox->Frame();
	}

	m_track_checkbox = new BCheckBox(BRect(0,0,0,0),0,TRACK_LABEL,
			new BMessage(MSG_TRACK_CHECKBOX));
	m_track_checkbox->ResizeToPreferred();
	if(m_track_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_track_checkbox->Frame();
	}

#ifdef _TTE_
	m_rating_checkbox = new BCheckBox(BRect(0,0,0,0),0,RATING_LABEL,
			new BMessage(MSG_RATING_CHECKBOX));
	m_rating_checkbox->ResizeToPreferred();
	if(m_rating_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_rating_checkbox->Frame();
	}

	m_tempo_checkbox = new BCheckBox(BRect(0,0,0,0),0,TEMPO_LABEL,
			new BMessage(MSG_TEMPO_CHECKBOX));
	m_tempo_checkbox->ResizeToPreferred();
	if(m_tempo_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_tempo_checkbox->Frame();
	}

	m_composer_checkbox = new BCheckBox(BRect(0,0,0,0),0,COMPOSER_LABEL,
			new BMessage(MSG_COMPOSER_CHECKBOX));
	m_composer_checkbox->ResizeToPreferred();
	if(m_composer_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_composer_checkbox->Frame();
	}
	
	m_gender_checkbox = new BCheckBox(BRect(0,0,0,0),0,GENDER_LABEL,
			new BMessage(MSG_GENDER_CHECKBOX));
	m_gender_checkbox->ResizeToPreferred();
	if(m_gender_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_gender_checkbox->Frame();
	}
#endif

	m_genre_checkbox = new BCheckBox(BRect(0,0,0,0),0,GENRE_LABEL,
			new BMessage(MSG_GENRE_CHECKBOX));
	m_genre_checkbox->ResizeToPreferred();
	if(m_genre_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_genre_checkbox->Frame();
	}
	
	m_clear_all_checkbox = new BCheckBox(BRect(0,0,0,0),0,B_UTF8_ELLIPSIS, new BMessage(MSG_CLEAR_CHECKBOX));
	m_clear_all_checkbox->ResizeToPreferred();
	if(m_clear_all_checkbox->Frame().Width() > frame.Width())
	{
		frame = m_genre_checkbox->Frame();
	}

	float x,y;
	x = space;
	y = m_edit_box->Frame().bottom + 2*space;
	m_artist_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
	m_album_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
	m_title_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
	m_year_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
	m_comment_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
	m_track_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
#ifdef _TTE_
	m_rating_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
	m_tempo_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
	m_composer_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
	m_gender_checkbox->MoveTo(x,y);
	y += frame.Height() + space;
#endif

	m_genre_checkbox->MoveTo(x,y);
	y += frame.Height() + space * 2;
	m_clear_all_checkbox->MoveTo(x,y);

	BRect tcFrame(0,0,110,frame.Height());

	m_artist_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_artist_textcontrol->MoveTo(frame.right+5,m_artist_checkbox->Frame().top);

	m_album_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_album_textcontrol->MoveTo(frame.right+5,m_album_checkbox->Frame().top);
	
	m_title_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_title_textcontrol->MoveTo(frame.right+5,m_title_checkbox->Frame().top);
	
	m_year_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_year_textcontrol->MoveTo(frame.right+5,m_year_checkbox->Frame().top);
	
	m_comment_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_comment_textcontrol->MoveTo(frame.right+5,m_comment_checkbox->Frame().top);
	
	m_track_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_track_textcontrol->MoveTo(frame.right+5,m_track_checkbox->Frame().top);

#ifdef _TTE_
	m_rating_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_rating_textcontrol->MoveTo(frame.right+5,m_rating_checkbox->Frame().top);
	m_tempo_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_tempo_textcontrol->MoveTo(frame.right+5,m_tempo_checkbox->Frame().top);
	m_composer_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_composer_textcontrol->MoveTo(frame.right+5,m_composer_checkbox->Frame().top);
	m_gender_textcontrol = new BTextControl(tcFrame,0,0,0,0);
	m_gender_textcontrol->MoveTo(frame.right+5,m_gender_checkbox->Frame().top);
#endif

	//INSIDE BOX
	BMenu* menu = new BMenu(GENRE_LABEL);
	menu->SetLabelFromMarked(true);
	int genres = GenreList::NumGenres();
	BList genreList;
	for(int i=0;i<genres;i++)
	{
		char* genre = GenreList::Genre(i);
		genreList.AddItem(genre);

	}
	genreList.SortItems(GenreList::GenreSort);
	char last = 'A';
	char current;
	for(int i=0;i<genres;i++)
	{
		char* genre = (char*)genreList.ItemAt(i);
		current = genre[0];
		if(current != last)
		{
			menu->AddSeparatorItem();
		}
		menu->AddItem(new BMenuItem(genre,new BMessage(MSG_GENRE_CHANGED)));
		last = current;
	}

	m_genre_menufield = new BMenuField(BRect(space, space, 0, 0), 0, 0, menu);

	//since a BMenuField doesn't report it's adjusted size correctly
	//we use the height of a BTextControl to approximate the height
	//of the BMenuField.  Not perfect, but close.
	m_genre_textcontrol = new BTextControl(BRect(0,0,110-(2*space),0),0,0,0,0);
	m_genre_textcontrol->MoveTo(space,2*space+m_genre_textcontrol->Frame().bottom);
	//DONE INSIDE BOX

	tcFrame.bottom = m_genre_textcontrol->Frame().bottom + 2*space;
	m_genre_box = new BBox(tcFrame,0);
	m_genre_box->MoveTo(frame.right+5, m_genre_checkbox->Frame().top);

	ResizeToPreferred();

	PRINT_OBJECT(Frame());

	AddChild(m_artist_checkbox);
	AddChild(m_artist_textcontrol);

	AddChild(m_album_checkbox);
	AddChild(m_album_textcontrol);

	AddChild(m_title_checkbox);
	AddChild(m_title_textcontrol);

	AddChild(m_year_checkbox);
	AddChild(m_year_textcontrol);

	AddChild(m_comment_checkbox);
	AddChild(m_comment_textcontrol);

	AddChild(m_track_checkbox);
	AddChild(m_track_textcontrol);

#ifdef _TTE_
	AddChild(m_rating_checkbox);
	AddChild(m_rating_textcontrol);

	AddChild(m_tempo_checkbox);
	AddChild(m_tempo_textcontrol);

	AddChild(m_composer_checkbox);
	AddChild(m_composer_textcontrol);
	
	AddChild(m_gender_checkbox);
	AddChild(m_gender_textcontrol);
#endif

	AddChild(m_genre_checkbox);
	AddChild(m_genre_box);
	m_genre_box->AddChild(m_genre_menufield);
	m_genre_box->AddChild(m_genre_textcontrol);
	
	AddChild(m_clear_all_checkbox);

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
EditorView::AttachedToWindow()
{
	AddOnView::AttachedToWindow();

	PRINT(("EditorView::AttachedToWindow()\n"));

	m_tag_radiobutton->SetTarget(this);
	m_attribute_radiobutton->SetTarget(this);
	m_apply_checkbox->SetTarget(this);
	m_artist_checkbox->SetTarget(this);
	m_album_checkbox->SetTarget(this);
	m_title_checkbox->SetTarget(this);
	m_year_checkbox->SetTarget(this);
	m_comment_checkbox->SetTarget(this);
	m_track_checkbox->SetTarget(this);
#ifdef _TTE_
	m_rating_checkbox->SetTarget(this);
	m_tempo_checkbox->SetTarget(this);
	m_composer_checkbox->SetTarget(this);
	m_gender_checkbox->SetTarget(this);
#endif
	m_genre_checkbox->SetTarget(this);
	m_genre_menufield->Menu()->SetTargetForItems(this);
	
	m_clear_all_checkbox->SetTarget(this);
}

void
EditorView::GetPreferredSize(float* width, float* height)
{
	PRINT(("EditorView::GetPreferredSize(float*,float*)\n"));

	int space = 6;

	*width = m_artist_textcontrol->Frame().right;
	if((m_edit_box->Frame().right) > *width)
	{
		*width = m_edit_box->Frame().right;
	}

	*height = m_genre_box->Frame().bottom;
}

void
EditorView::Hide()
{
	AddOnView::Hide();

	PRINT(("EditorView::Hide()\n"));
}

void
EditorView::Show()
{
	AddOnView::Show();

	PRINT(("EditorView::Show()\n"));
}

void
EditorView::SelectionChanged(BList* list)
{
	AddOnView::SelectionChanged(list);

	PRINT(("EditorView::SelectionChanged(BList* list)\n"));

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
EditorView::GenreSelectionAction()
{
	m_genre_textcontrol->SetText("");
}

void
EditorView::Apply()
{
	AddOnView::Apply();

	PRINT(("EditorView::Apply()\n"));

	thread_id thread = spawn_thread(EditorView::ApplyFunction,"TheEditor",
			B_NORMAL_PRIORITY,(void*)this);
	resume_thread(thread);
}

void
EditorView::Reset()
{
	AddOnView::Reset();

	PRINT(("EditorView::Reset()\n"));

	WidgetsSetValues();
	WidgetsSetEnabled();
	m_attribute_radiobutton->SetValue(B_CONTROL_ON);
}

void
EditorView::SetEnabled(BCheckBox* checkbox, BControl* control)
{
	PRINT(("EditorView::SetEnabled(BCheckBox*,BControl*)\n"));

	if ((checkbox->Value() == B_CONTROL_ON) && checkbox->IsEnabled())
	{
		control->SetEnabled(true);
	}
	else
	{
		control->SetEnabled(false);
	}
}

void
EditorView::SetEnabled(BCheckBox* checkbox, BMenuField* menufield)
{
	PRINT(("EditorView::SetEnabled(BCheckBox*,BMenuField*)\n"));

	if ((checkbox->Value() == B_CONTROL_ON) && checkbox->IsEnabled())
	{
		menufield->SetEnabled(true);
	}
	else
	{
		menufield->SetEnabled(false);
	}
}

void
EditorView::WidgetsSetValues()
{
	PRINT(("EditorView::WidgetsSetValues()\n"));

	int numSelected = m_selected_items->CountItems();
	if(numSelected == 0)
	{
		m_artist_checkbox->SetValue(B_CONTROL_OFF);
		m_album_checkbox->SetValue(B_CONTROL_OFF);
		m_title_checkbox->SetValue(B_CONTROL_OFF);
		m_comment_checkbox->SetValue(B_CONTROL_OFF);
		m_track_checkbox->SetValue(B_CONTROL_OFF);
#ifdef _TTE_
		m_rating_checkbox->SetValue(B_CONTROL_OFF);
		m_tempo_checkbox->SetValue(B_CONTROL_OFF);
		m_composer_checkbox->SetValue(B_CONTROL_OFF);
		m_gender_checkbox->SetValue(B_CONTROL_OFF);
#endif
		m_year_checkbox->SetValue(B_CONTROL_OFF);
		m_genre_checkbox->SetValue(B_CONTROL_OFF);
	}
	else if(numSelected == 1)
	{
		m_artist_checkbox->SetValue(B_CONTROL_ON);
		m_album_checkbox->SetValue(B_CONTROL_ON);
		m_title_checkbox->SetValue(B_CONTROL_ON);
		m_comment_checkbox->SetValue(B_CONTROL_ON);
		m_track_checkbox->SetValue(B_CONTROL_ON);
#ifdef _TTE_
		m_rating_checkbox->SetValue(B_CONTROL_ON);
		m_tempo_checkbox->SetValue(B_CONTROL_ON);
		m_composer_checkbox->SetValue(B_CONTROL_ON);
		m_gender_checkbox->SetValue(B_CONTROL_ON);
#endif
		m_year_checkbox->SetValue(B_CONTROL_ON);
		m_genre_checkbox->SetValue(B_CONTROL_ON);
	}
	else if(numSelected > 1)
	{
		m_artist_checkbox->SetValue(B_CONTROL_ON);
		m_album_checkbox->SetValue(B_CONTROL_ON);
		m_title_checkbox->SetValue(B_CONTROL_OFF);
		m_comment_checkbox->SetValue(B_CONTROL_ON);
		m_track_checkbox->SetValue(B_CONTROL_OFF);
#ifdef _TTE_
		m_rating_checkbox->SetValue(B_CONTROL_ON);
		m_tempo_checkbox->SetValue(B_CONTROL_ON);
		m_composer_checkbox->SetValue(B_CONTROL_ON);
		m_gender_checkbox->SetValue(B_CONTROL_ON);
#endif
		m_year_checkbox->SetValue(B_CONTROL_ON);
		m_genre_checkbox->SetValue(B_CONTROL_ON);
	}

	if(numSelected > 0)
	{
		EntryRefItem* item = (EntryRefItem*)m_selected_items->ItemAt(0);

		const char* artist;
		const char* album;
		const char* title;
		const char* year;
		const char* comment;
		const char* track;
#ifdef _TTE_
		const char* rating;
		const char* tempo;
		const char* composer;
		const char* gender;
#endif
		const char* genre;

		BFile audioFile(item->EntryRef(), B_READ_WRITE);

		if(m_attribute_radiobutton->Value() == B_CONTROL_ON)
		{
			AudioAttributes attributes(& audioFile);
			artist = attributes.Artist();
			if(!artist)
			{
				artist = "";
			}
			album = attributes.Album();
			if(!album)
			{
				album = "";
			}
			title = attributes.Title();
			if(!title)
			{
				title = "";
			}
			year = attributes.Year();
			if(!year)
			{
				year = "";
			}
			comment = attributes.Comment();
			if(!comment)
			{
				comment = "";
			}
			track = attributes.Track();
			if(!track)
			{
				track = "";
			}
#ifdef _TTE_
			rating = attributes.Rating();
			if(!rating)
			{
				rating = "";
			}

			tempo = attributes.Tempo();
			if(!tempo)
			{
				tempo = "";
			}

			composer = attributes.Composer();
			if(!composer)
			{
				composer = "";
			}
			
			gender = attributes.Gender();
			if(!gender)
			{
				gender = "";
			}
#endif
			genre = attributes.Genre();
			if(!genre)
			{
				genre = "";
			}

			m_artist_textcontrol->SetText(artist);
			m_album_textcontrol->SetText(album);
			m_title_textcontrol->SetText(title);
			m_year_textcontrol->SetText(year);
			m_comment_textcontrol->SetText(comment);
			m_track_textcontrol->SetText(track);
#ifdef _TTE_
			m_rating_textcontrol->SetText(rating);
			m_tempo_textcontrol->SetText(tempo);
			m_composer_textcontrol->SetText(composer);
			m_gender_textcontrol->SetText(gender);
#endif
			BMenu* menu = m_genre_menufield->Menu();
			BMenuItem* item = menu->FindItem(genre);
			if(item)
			{
				item->SetMarked(true);
				m_genre_textcontrol->SetText("");
			}
			else
			{
				item = menu->FindItem("Other");
				item->SetMarked(true);
				m_genre_textcontrol->SetText(genre);
			}

			return;
		}
		else if(item->IsSupportedByTaglib())
		{
			ID3Tags tags(item);

			artist = tags.Artist();
			if(!artist)
			{
				artist = "";
			}
			album = tags.Album();
			if(!album)
			{
				album = "";
			}
			title = tags.Title();
			if(!title)
			{
				title = "";
			}
			year = tags.Year();
			if(!year)
			{
				year = "";
			}
			comment = tags.Comment();
			if(!comment)
			{
				comment = "";
			}
			track = tags.Track();
			if(!track)
			{
				track = "";
			}
			genre = tags.Genre();
			if(!genre)
			{
				genre = "";
			}

			m_artist_textcontrol->SetText(artist);
			m_album_textcontrol->SetText(album);
			m_title_textcontrol->SetText(title);
			m_year_textcontrol->SetText(year);
			m_comment_textcontrol->SetText(comment);
			m_track_textcontrol->SetText(track);
#ifdef _TTE_
			m_rating_textcontrol->SetText("");
			m_tempo_textcontrol->SetText("");
			m_composer_textcontrol->SetText("");
			m_gender_textcontrol->SetText("");
#endif
			BMenu* menu = m_genre_menufield->Menu();
			BMenuItem* item = menu->FindItem(genre);
			if(item)
			{
				item->SetMarked(true);
				m_genre_textcontrol->SetText("");
			}
			else
			{
				item = menu->FindItem("Other");
				item->SetMarked(true);
				m_genre_textcontrol->SetText(genre);
			}
			

			return;
		}
	}

	m_artist_textcontrol->SetText("");
	m_album_textcontrol->SetText("");
	m_title_textcontrol->SetText("");
	m_year_textcontrol->SetText("");
	m_comment_textcontrol->SetText("");
	m_track_textcontrol->SetText("");
#ifdef _TTE_
	m_rating_textcontrol->SetText("");
	m_tempo_textcontrol->SetText("");
	m_composer_textcontrol->SetText("");
	m_gender_textcontrol->SetText("");
#endif
	m_genre_textcontrol->SetText("");
}

void
EditorView::WidgetsSetEnabled()
{
	PRINT(("EditorView::WidgetsSetEnabled()\n"));

	int numSelected = m_selected_items->CountItems();
	if(numSelected == 0)
	{
		m_attribute_radiobutton->SetEnabled(false);
		m_tag_radiobutton->SetEnabled(false);
		m_apply_checkbox->SetEnabled(false);
		m_artist_checkbox->SetEnabled(false);
		m_album_checkbox->SetEnabled(false);
		m_title_checkbox->SetEnabled(false);
		m_year_checkbox->SetEnabled(false);
		m_comment_checkbox->SetEnabled(false);
		m_track_checkbox->SetEnabled(false);
#ifdef _TTE_
		m_rating_checkbox->SetEnabled(false);
		m_tempo_checkbox->SetEnabled(false);
		m_composer_checkbox->SetEnabled(false);
		m_gender_checkbox->SetEnabled(false);
#endif
		m_genre_checkbox->SetEnabled(false);
//		m_clear_all_checkbox->SetEnabled(true);
		m_artist_textcontrol->SetEnabled(false);
		m_album_textcontrol->SetEnabled(false);
		m_title_textcontrol->SetEnabled(false);
		m_year_textcontrol->SetEnabled(false);
		m_comment_textcontrol->SetEnabled(false);
		m_track_textcontrol->SetEnabled(false);
#ifdef _TTE_
		m_rating_textcontrol->SetEnabled(false);
		m_tempo_textcontrol->SetEnabled(false);
		m_composer_textcontrol->SetEnabled(false);
		m_gender_textcontrol->SetEnabled(false);
#endif
		m_genre_menufield->SetEnabled(false);
		m_genre_textcontrol->SetEnabled(false);
	}
	else if(numSelected > 0)
	{
		m_attribute_radiobutton->SetEnabled(true);
		m_tag_radiobutton->SetEnabled(true);
		m_apply_checkbox->SetEnabled(true);
		m_artist_checkbox->SetEnabled(true);
		m_album_checkbox->SetEnabled(true);
		m_title_checkbox->SetEnabled(true);
		m_year_checkbox->SetEnabled(true);
		m_comment_checkbox->SetEnabled(true);
		m_track_checkbox->SetEnabled(true);
		m_genre_checkbox->SetEnabled(true);
//		m_clear_all_checkbox->SetEnabled(true);
		m_artist_textcontrol->SetEnabled(true);
		m_album_textcontrol->SetEnabled(true);
		m_title_textcontrol->SetEnabled(true);
		m_year_textcontrol->SetEnabled(true);
		m_comment_textcontrol->SetEnabled(true);
		m_track_textcontrol->SetEnabled(true);
		m_genre_menufield->SetEnabled(true);
		m_genre_textcontrol->SetEnabled(true);

#ifdef _TTE_
		if(m_attribute_radiobutton->Value() == B_CONTROL_ON)
		{
			m_rating_checkbox->SetEnabled(true);
			m_rating_textcontrol->SetEnabled(true);

			m_tempo_checkbox->SetEnabled(true);
			m_tempo_textcontrol->SetEnabled(true);

			m_composer_checkbox->SetEnabled(true);
			m_composer_textcontrol->SetEnabled(true);
			
			m_gender_checkbox->SetEnabled(true);
			m_gender_textcontrol->SetEnabled(true);
		}
		else
		{
			m_rating_checkbox->SetValue(B_CONTROL_OFF);
			m_rating_checkbox->SetEnabled(false);
			m_rating_textcontrol->SetEnabled(false);

			m_tempo_checkbox->SetValue(B_CONTROL_OFF);
			m_tempo_checkbox->SetEnabled(false);
			m_tempo_textcontrol->SetEnabled(false);

			m_composer_checkbox->SetValue(B_CONTROL_OFF);
			m_composer_checkbox->SetEnabled(false);
			m_composer_textcontrol->SetEnabled(false);
		}
#endif

		SetEnabled(m_artist_checkbox,m_artist_textcontrol);
		SetEnabled(m_album_checkbox,m_album_textcontrol);
		SetEnabled(m_title_checkbox,m_title_textcontrol);
		SetEnabled(m_year_checkbox,m_year_textcontrol);
		SetEnabled(m_comment_checkbox,m_comment_textcontrol);
		SetEnabled(m_track_checkbox,m_track_textcontrol);
#ifdef _TTE_
		SetEnabled(m_rating_checkbox,m_rating_textcontrol);
		SetEnabled(m_tempo_checkbox,m_tempo_textcontrol);
		SetEnabled(m_composer_checkbox,m_composer_textcontrol);
		SetEnabled(m_gender_checkbox,m_gender_textcontrol);
#endif
		SetEnabled(m_genre_checkbox,m_genre_menufield);
		SetEnabled(m_genre_checkbox,m_genre_textcontrol);
	}
}

void
EditorView::WidgetsRBValues()
{
	if(m_attribute_radiobutton->Value() == B_CONTROL_ON)
	{
		m_apply_checkbox->SetLabel(APPLY_TO_TAGS);
		m_apply_checkbox->SetValue(B_CONTROL_OFF);
	}
	else
	{
		m_apply_checkbox->SetLabel(APPLY_TO_ATTRIBUTES);
		m_apply_checkbox->SetValue(B_CONTROL_OFF);
	}
}

int32
EditorView::ApplyFunction(void* args)
{
	PRINT(("EditorView::ApplyFunction(void*)\n"));

	EditorView* view = (EditorView*)args;

	BView* appView = view->Window()->FindView("ArmyKnifeAppView");
	BMessenger messenger(appView);
	BMessage startMsg(START_APPLY);
	BMessage endMsg(END_APPLY);
	BMessage maxMsg(STATUS_BAR_SET_MAX_VALUE);
	BMessage fileMsg(STATUS_BAR_FILE);
	BMessage updateMsg(STATUS_BAR_UPDATE);
	BMessage resetMsg(STATUS_BAR_RESET);

	messenger.SendMessage(&startMsg);

	char artist[256];
	char album[256];
	char title[256];
	char year[256];
	char comment[256];
	char track[256];
#ifdef _TTE_
	char rating[256];
	char tempo[256];
	char composer[256];
	char gender[256];
#endif
	char genre[256];
	bool doAttributes = false;
	bool doTags = false;

	/* If the string is longer than 255 bytes, strncpy won't add a \0 at the end,
	 * so we use memset to make sure that the buffer always ends with \0
	 */
	memset(artist,0,256);
	memset(album,0,256);
	memset(title,0,256);
	memset(year,0,256);
	memset(comment,0,256);
	memset(track,0,256);
#ifdef _TTE_
	memset(rating,0,256);
	memset(tempo,0,256);
	memset(composer,0,256);
	memset(gender,0,256);
#endif
	memset(genre,0,256);

	strncpy(artist,view->m_artist_textcontrol->Text(),255);
	strncpy(album,view->m_album_textcontrol->Text(),255);
	strncpy(title,view->m_title_textcontrol->Text(),255);
	strncpy(year,view->m_year_textcontrol->Text(),255);
	strncpy(comment,view->m_comment_textcontrol->Text(),255);
	strncpy(track,view->m_track_textcontrol->Text(),255);
#ifdef _TTE_
	strncpy(rating,view->m_rating_textcontrol->Text(),255);
	strncpy(tempo,view->m_tempo_textcontrol->Text(),255);
	strncpy(composer,view->m_composer_textcontrol->Text(),255);
	strncpy(gender,view->m_gender_textcontrol->Text(),255);
#endif
	strncpy(genre,view->m_genre_textcontrol->Text(),255);
	if(strcmp(genre,"") == 0)
	{
		BMenu* menu = view->m_genre_menufield->Menu();
		BMenuItem* item = menu->FindMarked();
		strncpy(genre,item->Label(),255);
	}

	if((view->m_attribute_radiobutton->Value() == B_CONTROL_ON) ||
			(view->m_apply_checkbox->Value() == B_CONTROL_ON))
	{
		doAttributes = true;
	}

	if((view->m_tag_radiobutton->Value() == B_CONTROL_ON) ||
			(view->m_apply_checkbox->Value() == B_CONTROL_ON))
	{
		doTags = true;
	}

	int numSelected = view->m_selected_items->CountItems();
	maxMsg.AddInt32("maxValue",numSelected);
	messenger.SendMessage(&maxMsg);
	
	for(int i=0; i<numSelected; i++)
	{
		EntryRefItem * refItem = (EntryRefItem*)view->m_selected_items->ItemAt(i);
		BFile audioFile(refItem->EntryRef(), B_READ_WRITE);
		
		fileMsg.MakeEmpty();
		fileMsg.AddString("file", refItem->EntryRef()->name);
		messenger.SendMessage(&fileMsg);

		if(doAttributes)
		{
			AudioAttributes attributes(& audioFile);
			if(view->m_artist_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetArtist(artist);
				attributes.WriteArtist();
			}
			if(view->m_album_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetAlbum(album);
				attributes.WriteAlbum();
			}
			if(view->m_title_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetTitle(title);
				attributes.WriteTitle();
			}
			if(view->m_year_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetYear(year);
				attributes.WriteYear();
			}
			if(view->m_comment_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetComment(comment);
				attributes.WriteComment();
			}
			if(view->m_track_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetTrack(track);
				attributes.WriteTrack();
			}
#ifdef _TTE_
			if(view->m_rating_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetRating(rating);
				attributes.WriteRating();
			}
			if(view->m_tempo_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetTempo(tempo);
				attributes.WriteTempo();
			}
			if(view->m_composer_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetComposer(composer);
				attributes.WriteComposer();
			}
			if(view->m_gender_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetGender(gender);
				attributes.WriteGender();
			}
#endif
			if(view->m_genre_checkbox->Value() == B_CONTROL_ON)
			{
				attributes.SetGenre(genre);
				attributes.WriteGenre();
			}
		}

		if(doTags && refItem->IsSupportedByTaglib())
		{
			ID3Tags tags(refItem);
			if(view->m_artist_checkbox->Value() == B_CONTROL_ON)
			{
				tags.SetArtist(artist);
				tags.WriteArtist();
			}
			if(view->m_album_checkbox->Value() == B_CONTROL_ON)
			{
				tags.SetAlbum(album);
				tags.WriteAlbum();
			}
			if(view->m_title_checkbox->Value() == B_CONTROL_ON)
			{
				tags.SetTitle(title);
				tags.WriteTitle();
			}
			if(view->m_year_checkbox->Value() == B_CONTROL_ON)
			{
				tags.SetYear(year);
				tags.WriteYear();
			}
			if(view->m_comment_checkbox->Value() == B_CONTROL_ON)
			{
				tags.SetComment(comment);
				tags.WriteComment();
			}
			if(view->m_track_checkbox->Value() == B_CONTROL_ON)
			{
				tags.SetTrack(track);
				tags.WriteTrack();
			}
			if(view->m_genre_checkbox->Value() == B_CONTROL_ON)
			{
				tags.SetGenre(genre);
				tags.WriteGenre();
			}
		}
		
		refItem->UpdateTaglibMetadata();
		
		messenger.SendMessage(&updateMsg);
	}
	messenger.SendMessage(&resetMsg);
	messenger.SendMessage(&endMsg);
}

void
EditorView::MessageReceived(BMessage* message)
{
	//PRINT(("EditorView::MessageReceived(BMessage*)\n"));

	switch(message->what)
	{
		case MSG_ARTIST_CHECKBOX:
			SetEnabled(m_artist_checkbox,m_artist_textcontrol);
			break;
		case MSG_ALBUM_CHECKBOX:
			SetEnabled(m_album_checkbox,m_album_textcontrol);
			break;
		case MSG_TITLE_CHECKBOX:
			SetEnabled(m_title_checkbox,m_title_textcontrol);
			break;
		case MSG_YEAR_CHECKBOX:
			SetEnabled(m_year_checkbox,m_year_textcontrol);
			break;
		case MSG_COMMENT_CHECKBOX:
			SetEnabled(m_comment_checkbox,m_comment_textcontrol);
			break;
		case MSG_TRACK_CHECKBOX:
			SetEnabled(m_track_checkbox,m_track_textcontrol);
			break;
#ifdef _TTE_
		case MSG_RATING_CHECKBOX:
			SetEnabled(m_rating_checkbox,m_rating_textcontrol);
			break;
		case MSG_TEMPO_CHECKBOX:
			SetEnabled(m_tempo_checkbox,m_tempo_textcontrol);
			break;
		case MSG_COMPOSER_CHECKBOX:
			SetEnabled(m_composer_checkbox,m_composer_textcontrol);
			break;
		case MSG_GENDER_CHECKBOX:
			SetEnabled(m_gender_checkbox,m_gender_textcontrol);
			break;
#endif
		case MSG_GENRE_CHECKBOX:
			SetEnabled(m_genre_checkbox,m_genre_menufield);
			SetEnabled(m_genre_checkbox,m_genre_textcontrol);
			break;
			
		case MSG_CLEAR_CHECKBOX:
			{
				if (m_clear_all_checkbox->Value() == B_CONTROL_ON)
				{
					CheckAllBoxes(B_CONTROL_ON);
					SetAllEnabled();
				}
				else
				{
					CheckAllBoxes(B_CONTROL_OFF);
					SetAllEnabled();
				}
			}
			break;
		case MSG_APPLY_TO_BOTH:
			FlagUnacceptedListItems();
			break;
		case RADIO_BUTTON_EVENT:
			FlagUnacceptedListItems();
			WidgetsSetValues();
			WidgetsSetEnabled();
			WidgetsRBValues();
			break;
		case MSG_GENRE_CHANGED:
			GenreSelectionAction();
			break;
		default:
			AddOnView::MessageReceived(message);
	}
}

void
EditorView::SetAllEnabled()
{
	SetEnabled(m_artist_checkbox, m_artist_textcontrol);
	SetEnabled(m_album_checkbox, m_album_textcontrol);
	SetEnabled(m_title_checkbox, m_title_textcontrol);
	SetEnabled(m_year_checkbox, m_year_textcontrol);
	SetEnabled(m_comment_checkbox, m_comment_textcontrol);
	SetEnabled(m_track_checkbox, m_track_textcontrol);
#ifdef _TTE_
	SetEnabled(m_rating_checkbox, m_rating_textcontrol);
	SetEnabled(m_tempo_checkbox, m_tempo_textcontrol);
	SetEnabled(m_composer_checkbox, m_composer_textcontrol);
	SetEnabled(m_gender_checkbox, m_gender_textcontrol);
#endif
	SetEnabled(m_genre_checkbox, m_genre_menufield);
	SetEnabled(m_genre_checkbox, m_genre_textcontrol);
	// exception: m_clear_all_checkbox
}

void
EditorView::CheckAllBoxes(int32 value)
{
	m_artist_checkbox->SetValue(value);
	m_album_checkbox->SetValue(value);
	m_title_checkbox->SetValue(value);
	m_year_checkbox->SetValue(value);
	m_comment_checkbox->SetValue(value);
	m_track_checkbox->SetValue(value);
#ifdef _TTE_
	m_rating_checkbox->SetValue(value);
	m_tempo_checkbox->SetValue(value);
	m_composer_checkbox->SetValue(value);
	m_gender_checkbox->SetValue(value);
#endif
	m_genre_checkbox->SetValue(value);
}

bool
EditorView::AcceptListItem(EntryRefItem* listItem)
{
	PRINT(("EditorView::AcceptListItem()\n"));
	
	if (m_apply_checkbox->Value() == B_CONTROL_ON)
	{
		if (!listItem->IsFSWritable())
			return false;
		
		if (!listItem->IsFSAttributable())
			return false;
	
		if(!listItem->IsSupportedByTaglib())
			return false;
	} 
	else 
	{
		if (m_attribute_radiobutton->Value() == B_CONTROL_ON)
		{
			if (!listItem->IsFSWritable())
				return false;
			
			if (!listItem->IsFSAttributable())
				return false;
		}
		else
		{
			if (!listItem->IsFSWritable())
				return false;
		
			if(!listItem->IsSupportedByTaglib())
				return false;
		}
	}

	return true;
}
