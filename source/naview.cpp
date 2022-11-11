/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <Archivable.h>
#include <Box.h>
#include <Debug.h>
#include <Entry.h>
#include <File.h>
#include <LayoutBuilder.h>
#include <List.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Message.h>
#include <Messenger.h>
#include <RadioButton.h>
#include <Rect.h>
#include <StringView.h>
#include <TextControl.h>

#include "audioattributes.h"
#include "appdefs.h"
#include "appview.h"
#include "appwindow.h"
#include "commandconstants.h"
#include "entryrefitem.h"
#include "guistrings.h"
#include "naview.h"
#include "preferences.h"

NAView::NAView(Preferences * preferences)
 :	AddOnView		(NA_MODE_NAME),
	m_preferences	(preferences)
{
	PRINT(("NAView::NAView(BRect)\n"));

	InitView();
}

NAView::~NAView()
{
	PRINT(("NAView::~NAView()\n"));
}

void
NAView::InitView()
{
	PRINT(("NAView::InitView()\n"));

	m_a2n_radiobutton = new BRadioButton("m_a2n_radiobutton",A2N_LABEL,
			new BMessage(RADIO_BUTTON_EVENT));
	m_a2n_radiobutton->SetValue(B_CONTROL_ON);

	m_n2a_radiobutton = new BRadioButton("m_n2a_radiobutton",N2A_LABEL,
			new BMessage(RADIO_BUTTON_EVENT));

	m_direction_box = new BBox("m_direction_box");

	BGroupLayout *directionBoxLayout = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_SMALL_INSETS)
		.Add(m_a2n_radiobutton)
		.Add(m_n2a_radiobutton);

	m_direction_box->AddChild(directionBoxLayout->View());

	m_artist_stringview = new BStringView("m_artist_stv",ARTIST_PATTERN_INFO);
	m_album_stringview = new BStringView("m_album_strv",ALBUM_PATTERN_INFO);
	m_title_stringview = new BStringView("m_title_strv",TITLE_PATTERN_INFO);
	m_year_stringview = new BStringView("m_year_strv",YEAR_PATTERN_INFO);
	m_comment_stringview = new BStringView("m_comment_strv",COMMENT_PATTERN_INFO);
	m_track_stringview = new BStringView("m_track_strv",TRACK_PATTERN_INFO);
	m_genre_stringview = new BStringView("m_genre_strv",GENRE_PATTERN_INFO);
	m_wildcard_stringview = new BStringView("m_wildcard_strv",WILDCARD_PATTERN_INFO);

	m_pattern_menu = new BMenu("Please Add Pattern!");
	m_pattern_menufield = new BMenuField(PATTERN_LABEL, m_pattern_menu);
	m_pattern_menufield->SetDivider(StringWidth(PATTERN_LABEL) + 3);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(m_direction_box)
		.AddGroup(B_VERTICAL, B_USE_SMALL_SPACING)
			.Add(m_artist_stringview)
			.Add(m_album_stringview)
			.Add(m_title_stringview)
			.Add(m_year_stringview)
			.Add(m_comment_stringview)
			.Add(m_track_stringview)
			.Add(m_genre_stringview)
			.Add(m_wildcard_stringview)
		.End()
		.Add(m_pattern_menufield)
		.AddGlue();

	ResizeToPreferred();

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
NAView::AttachedToWindow()
{
	AddOnView::AttachedToWindow();

	PRINT(("NAView::AttachedToWindow()\n"));

	m_a2n_radiobutton->SetTarget(this);
	m_n2a_radiobutton->SetTarget(this);
	m_pattern_menu->SetTargetForItems(this);
}

void
NAView::Hide()
{
	AddOnView::Hide();

	PRINT(("NAView::Hide()\n"));
}

void
NAView::Show()
{
	AddOnView::Show();

	PRINT(("NAView::Show()\n"));
}

void
NAView::SelectionChanged(BList* list)
{
	AddOnView::SelectionChanged(list);

	PRINT(("NAView::SelectionChanged(BList* list)\n"));

	WidgetsSetEnabled();
}

void
NAView::Apply()
{
	AddOnView::Apply();

	PRINT(("NAView::Apply()\n"));

	thread_id thread = spawn_thread(NAView::ApplyFunction,"Names & Attributes",
			B_NORMAL_PRIORITY,(void*)this);
	resume_thread(thread);
}

void
NAView::Reset()
{
	AddOnView::Reset();

	PRINT(("NAView::Reset()\n"));

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
NAView::WidgetsSetValues()
{
	PRINT(("NAView::WidgetsSetValues()\n"));

	m_a2n_radiobutton->SetValue(B_CONTROL_ON);

	MakePatternMenu();

	WidgetsRBValues();
}

void
NAView::WidgetsSetEnabled()
{
	PRINT(("NAView::WidgetsSetEnabled()\n"));

	if(m_selected_items->CountItems() > 0)
	{
		m_a2n_radiobutton->SetEnabled(true);
		m_n2a_radiobutton->SetEnabled(true);
		m_pattern_menufield->SetEnabled(true);
	}
	else
	{
		m_a2n_radiobutton->SetEnabled(false);
		m_n2a_radiobutton->SetEnabled(false);
		m_pattern_menufield->SetEnabled(false);
	}
}

void
NAView::WidgetsRBValues()
{
	PRINT(("NAView::WidgetsRBValues()\n"));

	if(m_a2n_radiobutton->Value() == B_CONTROL_ON)
	{
		if(!m_wildcard_stringview->IsHidden())
		{
			m_wildcard_stringview->Hide();
		}
	}
	else
	{
		if(m_wildcard_stringview->IsHidden())
		{
			m_wildcard_stringview->Show();
		}
	}
}

void
NAView::MakePatternMenu()
{
	status_t status = B_OK;
	BString pattern;
	int32 count = 0;

	while(m_pattern_menu->RemoveItem((int32) 0) != NULL)
	{
		// loop
	}


	while(m_preferences->GetPatternAt(count++, & pattern) == B_OK)
	{
		m_pattern_menu->AddItem(new BMenuItem(pattern.String(), new BMessage(MSG_PATTERN_CHANGED)));
	}

	m_pattern_menu->AddSeparatorItem();
	m_pattern_menu->AddItem(new BMenuItem(PATTERN_MENU_NEW, new BMessage(MSG_NEW_PATTERN)));
	m_pattern_menu->AddItem(new BMenuItem(PATTERN_MENU_DELETE, new BMessage(MSG_DELETE_PATTERN)));

	int32 index = m_preferences->GetPattern();

	m_pattern_menu->SetRadioMode(true);
	m_pattern_menu->SetLabelFromMarked(true);
	m_pattern_menu->ItemAt(index)->SetMarked(true);
	m_pattern_menu->SetLabelFromMarked(false);
	m_pattern_menu->SetRadioMode(false);

	m_pattern_menu->SetTargetForItems(this);
}

int32 NAView::ApplyFunction(void* args) {
	NAView* view = (NAView*)args;

	char pattern[256];
	strcpy(pattern, view->m_pattern_menufield->Menu()->FindMarked()->Label());

	bool a2n = false;
	if(view->m_a2n_radiobutton->Value() == B_CONTROL_ON) {
		a2n = true;
	}

	BView* appView = view->Window()->FindView("ArmyKnifeAppView");
	BMessenger messenger(appView);
	BMessage startMsg(START_APPLY);
	BMessage endMsg(END_APPLY);
	BMessage maxMsg(STATUS_BAR_SET_MAX_VALUE);
	BMessage fileMsg(STATUS_BAR_FILE);
	BMessage updateMsg(STATUS_BAR_UPDATE);
	BMessage resetMsg(STATUS_BAR_RESET);

	messenger.SendMessage(&startMsg);

	int numSelected = view->m_selected_items->CountItems();
	maxMsg.AddInt32("maxValue",numSelected);
	messenger.SendMessage(&maxMsg);
	for(int i=0;i<numSelected;i++) {
		EntryRefItem* refItem = (EntryRefItem*)view->m_selected_items->ItemAt(i);
		entry_ref* ref = refItem->EntryRef();
		BFile id3File(ref,B_READ_WRITE);
		AudioAttributes attributes(&id3File);

		fileMsg.MakeEmpty();
		fileMsg.AddString("file", refItem->EntryRef()->name);
		messenger.SendMessage(&fileMsg);

		if(a2n)	{
			const char* artist = attributes.Artist();
			if(!artist) {
				artist = "";
			}
			const char* album = attributes.Album();
			if(!album) {
				album = "";
			}
			const char* title = attributes.Title();
			if(!title) {
				title = "";
			}
			const char* year = attributes.Year();
			if(!year) {
				year = "";
			}
			const char* comment = attributes.Comment();
			if(!comment) {
				comment = "";
			}
			const char* track = attributes.Track();
			if(!track) {
				track = "";
			}
			const char* genre = attributes.Genre();
			if(!genre) {
				genre = "";
			}

			BString name(pattern);
			name.ReplaceAll(ARTIST_PATTERN, artist);
			name.ReplaceAll(ALBUM_PATTERN, album);
			name.ReplaceAll(TITLE_PATTERN, title);
			name.ReplaceAll(YEAR_PATTERN, year);
			name.ReplaceAll(COMMENT_PATTERN, comment);
			name.ReplaceAll(TRACK_PATTERN, track);
			name.ReplaceAll(GENRE_PATTERN, genre);
			BEntry entry(ref);
			entry.Rename(name.String());
			entry.GetRef(ref);
			refItem->UpdateText();
		} else {
			char name[2];
			char value[B_PATH_NAME_LENGTH+1];
			memset(name,0,2);
			memset(value,0,B_PATH_NAME_LENGTH+1);
			NameValueMatcher matcher(pattern,ref->name,'/');
			while(matcher.NextMatch(name,value) == 0) {
				switch(name[0]) {
					case 'a':
						attributes.SetArtist(value);
						attributes.WriteArtist();
						break;
					case 'n':
						attributes.SetAlbum(value);
						attributes.WriteAlbum();
						break;
					case 't':
						attributes.SetTitle(value);
						attributes.WriteTitle();
						break;
					case 'y':
						attributes.SetYear(value);
						attributes.WriteYear();
						break;
					case 'c':
						attributes.SetComment(value);
						attributes.WriteComment();
						break;
					case 'k':
						attributes.SetTrack(value);
						attributes.WriteTrack();
						break;
					case 'g':
						attributes.SetGenre(value);
						attributes.WriteGenre();
						break;
					default:
						break;
				}
			}
		}
		messenger.SendMessage(&updateMsg);
	}
	if(a2n)	{
		BMessage invalidateListMsg(INVALIDATE_LIST);
		messenger.SendMessage(&invalidateListMsg);
	}
	messenger.SendMessage(&resetMsg);
	messenger.SendMessage(&endMsg);

	return B_OK;
}

void NAView::MessageReceived(BMessage* message) {
	switch(message->what) {
		case RADIO_BUTTON_EVENT:
			WidgetsRBValues();
			break;
		case MSG_PATTERN_CHANGED:
			{
				int32 index;
				message->FindInt32("index", &index);
				m_pattern_menu->SetRadioMode(true);
				m_pattern_menu->SetLabelFromMarked(true);
				m_pattern_menu->ItemAt(index)->SetMarked(true);
				m_pattern_menu->SetLabelFromMarked(false);
				m_pattern_menu->SetRadioMode(false);

				m_preferences->SetPattern(index);
			}
			break;

		case MSG_NEW_PATTERN:
			{
				m_pattern_window = new PatternWindow(new BMessenger(this));
				m_pattern_window->Show();
			}
			break;

		case MSG_PATTERN_CREATED:
			{
				BString pattern;

				if (message->FindString("pattern", & pattern) == B_OK)
					m_preferences->AddPattern(& pattern);

				MakePatternMenu();
			}
			break;

		case MSG_DELETE_PATTERN:
			{
				if (m_pattern_menu->CountItems() > 4)
				{
					BMenuItem * item =	m_pattern_menu->FindMarked();
					int32 index	=		m_pattern_menu->IndexOf(item);

					m_preferences->DeletePattern(index);
					m_preferences->SetPattern((int32) 0);

					item = m_pattern_menu->RemoveItem(index);
					delete item;

					m_pattern_menu->SetRadioMode(true);
					m_pattern_menu->SetLabelFromMarked(true);
					m_pattern_menu->ItemAt(0)->SetMarked(true);
					m_pattern_menu->SetLabelFromMarked(false);
					m_pattern_menu->SetRadioMode(false);
				}
			}
			break;

		default:
			AddOnView::MessageReceived(message);
	}
}

bool
NAView::AcceptListItem(EntryRefItem* listItem)
{
	PRINT(("NAView::AcceptListItem()\n"));

	if (!listItem->IsFSWritable())
		return false;

	if (!listItem->IsFSAttributable())
		return false;

	return true;
}

NameValueMatcher::NameValueMatcher(const char* nameStr, const char* valueStr, char delim)
{
	_delim = delim;
	_name = 0;
	_value = 0;

	if(nameStr)
	{
		int len = strlen(nameStr);
		_name = new char[len+1];
		strcpy(_name,nameStr);
	}

	if(valueStr)
	{
		int len = strlen(valueStr);
		_value = new char[len+1];
		strcpy(_value,valueStr);
	}

	name_ptr = _name;
	value_ptr = _value;

	if(name_ptr && value_ptr)
	{
		while(*name_ptr == *value_ptr)
		{
			name_ptr++;
			value_ptr++;
		}
	}
}

NameValueMatcher::~NameValueMatcher()
{
	delete [] _name;
	delete [] _value;
}

int
NameValueMatcher::NextMatch(char* name, char* value)
{
	int iVal;
	int iTmp;
	int len;
	char* tmp = 0;

	if(!name_ptr || !value_ptr || (*name_ptr != _delim))
	{
		return -1;
	}

	name[0] = name_ptr[1];
	name[1] = 0;

	name_ptr += 2;

	iVal = 0;
	while((*name_ptr != *value_ptr) && (*value_ptr != 0))
	{
		value[iVal] = *value_ptr;
		value_ptr++;
		iVal++;
	}

	if(*name_ptr == *value_ptr == 0)
	{
		name_ptr = 0;
		value_ptr = 0;
	}

	if(name_ptr)
	{
		tmp = new char[strlen(name_ptr)+1];
		memset(tmp,0,strlen(name_ptr)+1);

		iTmp = 0;
		while((*name_ptr != _delim) && (*name_ptr != 0))
		{
			tmp[iTmp] = *name_ptr;
			name_ptr++;
			iTmp++;
		}
		tmp[iTmp] = 0;
	}

	if(tmp)
	{
		len = strlen(tmp);
		while((strncmp(value_ptr,tmp,len) != 0) && (*value_ptr != 0))
		{
			value[iVal] = *value_ptr;
			value_ptr++;
			iVal++;
		}
		value_ptr += len;
	}

	value[iVal] = 0;

	delete [] tmp;

	return 0;
}

