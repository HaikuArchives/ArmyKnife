/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Alert.h>
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <Debug.h>
#include <Entry.h>
#include <File.h>
#include <GroupLayout.h>
#include <Layout.h>
#include <LayoutBuilder.h>
#include <Message.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Messenger.h>
#include <NodeInfo.h>
#include <ObjectList.h>
#include <OptionPopUp.h>
#include <Path.h>
#include <RadioButton.h>
#include <Rect.h>
#include <String.h>
#include <TextControl.h>

#include "audioattributes.h"
#include "genrelist.h"
#include "id3tags.h"
#include "appdefs.h"
#include "appview.h"
#include "appwindow.h"
#include "commandconstants.h"
#include "editorview.h"
#include "entryrefitem.h"
#include "guistrings.h"
#include "albumpictureview.h"
#include "querywindow.h"
#include "musicbrainzquery.h"

EditorView::EditorView(Preferences * preferences)
 :	AddOnView		(EDITOR_MODE_NAME),
	m_preferences	(preferences)
{
	m_album_picture_changed = false;
	InitView();
}

EditorView::~EditorView(){
}

void EditorView::InitView()
{
	int space = 6;
	m_attribute_radiobutton = new BRadioButton("m_attribute_radiobutton",ATTRIBUTES_LABEL,
			new BMessage(RADIO_BUTTON_EVENT));
	m_attribute_radiobutton->SetValue(B_CONTROL_ON);

	m_tag_radiobutton = new BRadioButton("m_tag_radiobutton",TAGS_LABEL,
			new BMessage(RADIO_BUTTON_EVENT));

	m_apply_checkbox = new BCheckBox("m_apply_checkbox",APPLY_TO_ATTRIBUTES,
			new BMessage(MSG_APPLY_TO_BOTH));
	m_apply_checkbox->SetValue(B_CONTROL_OFF);
	m_apply_checkbox->SetLabel(APPLY_TO_TAGS);

	m_edit_box = new BBox("m_edit_box");
	BGroupLayout *editBoxLayout = BLayoutBuilder::Group<>(B_VERTICAL, 0)
		.SetInsets(B_USE_SMALL_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(m_attribute_radiobutton)
			.Add(m_tag_radiobutton)
		.End()
		.Add(m_apply_checkbox);

	m_edit_box->AddChild(editBoxLayout->View());
	m_picture_checkbox = new BCheckBox("m_picutre_checkbox","",
			new BMessage(MSG_PICTURE_CHECKBOX));
	m_album_picture = new AlbumPictureView("bitmap_view");
	m_artist_checkbox = new BCheckBox("m_artist_checkbox",ARTIST_LABEL,
			new BMessage(MSG_ARTIST_CHECKBOX));
	m_album_checkbox = new BCheckBox("m_album_checkbox",ALBUM_LABEL,
			new BMessage(MSG_ALBUM_CHECKBOX));
	m_title_checkbox = new BCheckBox("m_title_checkbox",TITLE_LABEL,
			new BMessage(MSG_TITLE_CHECKBOX));
	m_year_checkbox = new BCheckBox("m_year_checkbox",YEAR_LABEL,
			new BMessage(MSG_YEAR_CHECKBOX));
	m_comment_checkbox = new BCheckBox("m_comment_checkbox",COMMENT_LABEL,
			new BMessage(MSG_COMMENT_CHECKBOX));
	m_track_checkbox = new BCheckBox("m_track_checkbox",TRACK_LABEL,
			new BMessage(MSG_TRACK_CHECKBOX));
#ifdef _TTE_
	m_rating_checkbox = new BCheckBox("m_rating_checkbox",RATING_LABEL,
			new BMessage(MSG_RATING_CHECKBOX));
	m_tempo_checkbox = new BCheckBox("m_tempo_checkbox",TEMPO_LABEL,
			new BMessage(MSG_TEMPO_CHECKBOX));
	m_composer_checkbox = new BCheckBox("m_composer_checkbox",COMPOSER_LABEL,
			new BMessage(MSG_COMPOSER_CHECKBOX));
	m_gender_checkbox = new BCheckBox("m_gender_checkbox",GENDER_LABEL,
			new BMessage(MSG_GENDER_CHECKBOX));
#endif
	m_genre_checkbox = new BCheckBox("m_genre_checkbox",GENRE_LABEL,
			new BMessage(MSG_GENRE_CHECKBOX));
	m_clear_all_checkbox = new BCheckBox("m_clear_all_checkbox",B_UTF8_ELLIPSIS,
			new BMessage(MSG_CLEAR_CHECKBOX));
	m_artist_textcontrol = new BTextControl("m_artist_textcontrol","","",NULL);
	m_album_textcontrol = new BTextControl("m_album_textcontrol","","",NULL);
	m_title_textcontrol = new BTextControl("m_title_textcontrol","","",NULL);
	m_year_textcontrol = new BTextControl("m_year_textcontrol","","",NULL);
	m_comment_textcontrol = new BTextControl("m_comment_textcontrol","","",NULL);
	m_track_textcontrol = new BTextControl("m_track_textcontrol","","",NULL);

#ifdef _TTE_
	m_rating_textcontrol = new BTextControl("m_rating_textcontrol","","",NULL);
	m_tempo_textcontrol = new BTextControl("m_tempo_textcontrol","","",NULL);
	m_composer_textcontrol = new BTextControl("m_composer_textcontrol","","",NULL);
	m_gender_textcontrol = new BTextControl("m_gender_textcontrol","","",NULL);
#endif

	//INSIDE BOX
	BMenu* menu = new BMenu(GENRE_LABEL);
	menu->SetLabelFromMarked(true);
	int genres = GenreList::NumGenres();
	BObjectList<const char> genreList;
	for(int i=0;i<genres;i++){
		BString genre = GenreList::Genre(i);
		genreList.AddItem(genre);
	}
	genreList.SortItems(&GenreList::GenreSort);
	char last = 'A';
	char current;
	for(int i=0;i<genres;i++){
		BString genre = genreList.ItemAt(i);
		current = genre[0];
		if(current != last)		{
			menu->AddSeparatorItem();
		}
		menu->AddItem(new BMenuItem(genre,new BMessage(MSG_GENRE_CHANGED)));
		last = current;
	}
	menu->SetMaxContentWidth(StringWidth("contemporary classical WWWWW "));
	m_genre_menufield = new BMenuField("m_genre_menufield", "", menu);
	m_genre_textcontrol = new BTextControl("m_genre_textcontrol","","",NULL);
	//DONE INSIDE BOX
	m_genre_box = new BBox("m_genre_box");
	BGroupLayout *genreBoxLayout = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_SMALL_INSETS)
		.Add(m_genre_menufield)
		.Add(m_genre_textcontrol);

	m_genre_box->AddChild(genreBoxLayout->View());
	m_tag_lookup = new BButton("m_tag_lookup", QUERY_START_QUERY, new BMessage(MSG_TAG_LOOKUP));
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(m_edit_box)
		.AddGroup(B_HORIZONTAL)
			.Add(m_picture_checkbox)
			.Add(m_album_picture)
		.End()
		.AddGrid()
			.Add(m_artist_checkbox, 0, 0)
			.Add(m_artist_textcontrol, 1, 0)
			.Add(m_album_checkbox, 0, 1)
			.Add(m_album_textcontrol, 1, 1)
			.Add(m_title_checkbox, 0, 2)
			.Add(m_title_textcontrol, 1, 2)
			.Add(m_year_checkbox, 0, 3)
			.Add(m_year_textcontrol, 1, 3)
			.Add(m_comment_checkbox, 0, 4)
			.Add(m_comment_textcontrol, 1, 4)
			.Add(m_track_checkbox, 0, 5)
			.Add(m_track_textcontrol, 1, 5)
			.Add(m_genre_checkbox, 0, 6)
			.Add(m_genre_box, 1, 6, 1, 3)
			.Add(m_clear_all_checkbox, 0, 7, 1, 2)
		.End()
		.AddGlue()
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(m_tag_lookup);

	ResizeToPreferred();

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void EditorView::AttachedToWindow(){
	AddOnView::AttachedToWindow();

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
	m_tag_lookup->SetTarget(this);
}

void EditorView::Hide(){
	AddOnView::Hide();
}

void EditorView::Show(){
	AddOnView::Show();
}

void EditorView::SelectionChanged(BList* list)
{
	AddOnView::SelectionChanged(list);
	m_album_picture_changed = false;

	PRINT(("EditorView::SelectionChanged(BList* list)\n"));

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void EditorView::GenreSelectionAction() {
	m_genre_textcontrol->SetText("");
}

void EditorView::Apply() {
	AddOnView::Apply();
	thread_id thread = spawn_thread(EditorView::ApplyFunction,"TheEditor",
			B_NORMAL_PRIORITY,(void*)this);
	resume_thread(thread);
}

void EditorView::Reset() {
	AddOnView::Reset();
	WidgetsSetValues();
	WidgetsSetEnabled();
	m_attribute_radiobutton->SetValue(B_CONTROL_ON);
}

void EditorView::SetEnabled(BCheckBox* checkbox, BControl* control) {
	if ((checkbox->Value() == B_CONTROL_ON) && checkbox->IsEnabled()) {
		control->SetEnabled(true);
	} else{
		control->SetEnabled(false);
	}
}

void EditorView::SetEnabled(BCheckBox* checkbox, BMenuField* menufield) {
	if ((checkbox->Value() == B_CONTROL_ON) && checkbox->IsEnabled()) {
		menufield->SetEnabled(true);
	} else {
		menufield->SetEnabled(false);
	}
}

void EditorView::WidgetsSetValues() {
	int numSelected = m_selected_items->CountItems();
	if (numSelected == 0 || m_list_view->HasSelectionOfOnlyAcceptedItems() == false) {
		m_album_picture->NoImage();
		m_picture_checkbox->SetValue(B_CONTROL_OFF);
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
	} else if(numSelected == 1) {
		m_picture_checkbox->SetValue(B_CONTROL_ON);
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
	} else if(numSelected > 1) {
		m_picture_checkbox->SetValue(B_CONTROL_ON);
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

	if(numSelected > 0) {
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

		if(m_attribute_radiobutton->Value() == B_CONTROL_ON) {
			m_picture_checkbox->SetValue(B_CONTROL_OFF);
			m_album_picture->NoImage();
			AudioAttributes attributes(& audioFile);
			artist = attributes.Artist();
			if(!artist) {
				artist = "";
			}
			album = attributes.Album();
			if(!album) {
				album = "";
			}
			title = attributes.Title();
			if(!title) {
				title = "";
			}
			year = attributes.Year();
			if(!year) {
				year = "";
			}
			comment = attributes.Comment();
			if(!comment) {
				comment = "";
			}
			track = attributes.Track();
			if(!track) {
				track = "";
			}
#ifdef _TTE_
			rating = attributes.Rating();
			if(!rating) {
				rating = "";
			}
			tempo = attributes.Tempo();
			if(!tempo) {
				tempo = "";
			}
			composer = attributes.Composer();
			if(!composer) {
				composer = "";
			}
			gender = attributes.Gender();
			if(!gender) {
				gender = "";
			}
#endif
			genre = attributes.Genre();
			if(!genre) {
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
			if(item) {
				item->SetMarked(true);
				m_genre_textcontrol->SetText("");
			} else {
				item = menu->FindItem("Other");
				item->SetMarked(true);
				m_genre_textcontrol->SetText(genre);
			}

			return;
		} else if(item->IsSupportedByTaglib()) {
			// Add album art
			BPath filePath;
			BEntry *entry = new BEntry(item->EntryRef());
			if (item->IsMP3() && entry->GetPath(&filePath) == B_OK) {
				m_album_picture->UpdatePicture(filePath.Path());
			}
			delete entry;
			ID3Tags tags(item);
			artist = tags.Artist();
			if(!artist) {
				artist = "";
			}
			album = tags.Album();
			if(!album) {
				album = "";
			}
			title = tags.Title();
			if(!title) {
				title = "";
			}
			year = tags.Year();
			if(!year) {
				year = "";
			}
			comment = tags.Comment();
			if(!comment) {
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
			menu->SetMaxContentWidth(StringWidth("contemporary classical WWWWW "));
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
	if (numSelected == 0 || m_list_view->HasSelectionOfOnlyAcceptedItems() == false)
	{
		m_album_picture->NoImage();
		m_picture_checkbox->SetEnabled(false);
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
		m_tag_lookup->SetEnabled(false);
	}
	else if(numSelected > 0)
	{
		m_tag_lookup->SetEnabled(true);
		m_picture_checkbox->SetEnabled(true);
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

		if(m_attribute_radiobutton->Value() == B_CONTROL_ON)
			m_picture_checkbox->SetEnabled(false);
		else
			m_picture_checkbox->SetEnabled(true);

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
		m_apply_checkbox->SetLabel(APPLY_TO_TAGS);
	else
		m_apply_checkbox->SetLabel(APPLY_TO_ATTRIBUTES);
}

int32 EditorView::ApplyFunction(void* args) {
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
			(view->m_apply_checkbox->Value() == B_CONTROL_ON)) {
		doAttributes = true;
	}
	if((view->m_tag_radiobutton->Value() == B_CONTROL_ON) ||
			(view->m_apply_checkbox->Value() == B_CONTROL_ON)) {
		doTags = true;
	}
	int numSelected = view->m_selected_items->CountItems();
	maxMsg.AddInt32("maxValue",numSelected);
	messenger.SendMessage(&maxMsg);

	for(int i=0; i<numSelected; i++) {
		EntryRefItem * refItem = (EntryRefItem*)view->m_selected_items->ItemAt(i);
		BFile audioFile(refItem->EntryRef(), B_READ_WRITE);
		fileMsg.MakeEmpty();
		fileMsg.AddString("file", refItem->EntryRef()->name);
		messenger.SendMessage(&fileMsg);
		if(doAttributes) {
			AudioAttributes attributes(& audioFile);
			if(view->m_artist_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetArtist(artist);
				attributes.WriteArtist();
			}
			if(view->m_album_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetAlbum(album);
				attributes.WriteAlbum();
			}
			if(view->m_title_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetTitle(title);
				attributes.WriteTitle();
			}
			if(view->m_year_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetYear(year);
				attributes.WriteYear();
			}
			if(view->m_comment_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetComment(comment);
				attributes.WriteComment();
			}
			if(view->m_track_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetTrack(track);
				attributes.WriteTrack();
			}
#ifdef _TTE_
			if(view->m_rating_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetRating(rating);
				attributes.WriteRating();
			}
			if(view->m_tempo_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetTempo(tempo);
				attributes.WriteTempo();
			}
			if(view->m_composer_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetComposer(composer);
				attributes.WriteComposer();
			}
			if(view->m_gender_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetGender(gender);
				attributes.WriteGender();
			}
#endif
			if(view->m_genre_checkbox->Value() == B_CONTROL_ON) {
				attributes.SetGenre(genre);
				attributes.WriteGenre();
			}
		}

		if(doTags && refItem->IsSupportedByTaglib()) {
			if (view->m_picture_checkbox->Value() == B_CONTROL_ON) {
				BPath filePath;
				BEntry *entry = new BEntry(refItem->EntryRef());
				if (refItem->IsMP3() && entry->GetPath(&filePath) == B_OK) {
					view->m_album_picture->SetPicture(filePath.Path());
				}
				delete entry;
			}
			ID3Tags tags(refItem);
			if(view->m_artist_checkbox->Value() == B_CONTROL_ON) {
				tags.SetArtist(artist);
				tags.WriteArtist();
			}
			if(view->m_album_checkbox->Value() == B_CONTROL_ON) {
				tags.SetAlbum(album);
				tags.WriteAlbum();
			}
			if(view->m_title_checkbox->Value() == B_CONTROL_ON) {
				tags.SetTitle(title);
				tags.WriteTitle();
			}
			if(view->m_year_checkbox->Value() == B_CONTROL_ON) {
				tags.SetYear(year);
				tags.WriteYear();
			}
			if(view->m_comment_checkbox->Value() == B_CONTROL_ON) {
				tags.SetComment(comment);
				tags.WriteComment();
			}
			if(view->m_track_checkbox->Value() == B_CONTROL_ON) {
				tags.SetTrack(track);
				tags.WriteTrack();
			}
			if(view->m_genre_checkbox->Value() == B_CONTROL_ON) {
				tags.SetGenre(genre);
				tags.WriteGenre();
			}
		}

		refItem->UpdateTaglibMetadata();
		messenger.SendMessage(&updateMsg);
	}
	messenger.SendMessage(&resetMsg);
	messenger.SendMessage(&endMsg);

	return B_OK;
}

void EditorView::MessageReceived(BMessage* message) {
	switch(message->what) {
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
		case MSG_TAG_LOOKUP:
		{
			Queryable a;

			if (m_artist_checkbox->Value() == B_CONTROL_ON
				&& m_artist_textcontrol->Text()[0] != '\0')
				a.SetArtist(m_artist_textcontrol->Text());

			if (m_album_checkbox->Value() == B_CONTROL_ON
				&& m_album_textcontrol->Text()[0] != '\0')
				a.SetAlbum(m_album_textcontrol->Text());

			if (m_title_checkbox->Value() == B_CONTROL_ON
				&& m_title_textcontrol->Text()[0] != '\0')
				a.SetTitle(m_title_textcontrol->Text());

			if (m_year_checkbox->Value() == B_CONTROL_ON
				&& m_year_textcontrol->Text()[0] != '\0')
				a.SetYear(m_year_textcontrol->Text());

			if (m_comment_checkbox->Value() == B_CONTROL_ON
				&& m_comment_textcontrol->Text()[0] != '\0')
				a.SetComment(m_comment_textcontrol->Text());

			if (m_track_checkbox->Value() == B_CONTROL_ON
				&& m_track_textcontrol->Text()[0] != '\0')
				a.SetTrack(m_track_textcontrol->Text());

			QueryWindow* win = new QueryWindow(new MusicBrainzQuery(a),
				BMessenger(this));
			win->Show();
			break;
		}
		case MSG_ADOPT_TAGS:
		{
			Queryable* dest = static_cast<Queryable*>(
				Queryable::Instantiate(message));
			if (dest == NULL)
				break;

			m_artist_textcontrol->SetText(dest->Artist());
			m_album_textcontrol->SetText(dest->Album());
			m_title_textcontrol->SetText(dest->Title());
			m_year_textcontrol->SetText(dest->Year());
			m_comment_textcontrol->SetText(dest->Comment());
			m_track_textcontrol->SetText(dest->Track());

			delete dest;
			break;
		}
		default:
			AddOnView::MessageReceived(message);
	}
}

void EditorView::SetAllEnabled() {
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
	m_picture_checkbox->SetValue(value);
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

	if (!listItem->IsFSWritable())
		return false;

	if (m_apply_checkbox->Value() == B_CONTROL_ON)
	{
		if (!listItem->IsFSAttributable())
			return false;

		if(!listItem->IsSupportedByTaglib())
			return false;
	}

	if (m_attribute_radiobutton->Value() == B_CONTROL_ON
		&& !listItem->IsFSAttributable())
		return false;


	if (m_tag_radiobutton->Value() == B_CONTROL_ON
		&& !listItem->IsSupportedByTaglib())
		return false;

	return true;
}
