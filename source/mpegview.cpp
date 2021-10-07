/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <Archivable.h>
#include <Box.h>
#include <CheckBox.h>
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
#include <Path.h>
#include <RadioButton.h>
#include <Rect.h>
#include <StringView.h>
#include <TextControl.h>

#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/apetag.h>
#include <taglib/tfile.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>

#include "audioattributes.h"
#include "appdefs.h"
#include "appview.h"
#include "appwindow.h"
#include "commandconstants.h"
#include "entryrefitem.h"
#include "guistrings.h"
#include "mpegview.h"
#include "preferences.h"

MPEGView::MPEGView(Preferences * preferences)
 :	AddOnView			(MPEG_MODE_NAME),
	m_preferences		(preferences),
	m_info_box			(NULL),
	m_files_string		(NULL),
	m_id3v1_string		(NULL),
	m_id3v2_string		(NULL),
	m_ape_string		(NULL),
	m_add_remove_box	(NULL),
	m_add_radiobutton	(NULL),
	m_remove_radiobutton	(NULL),
	m_id3v1_checkbox	(NULL),
	m_id3v2_checkbox	(NULL),
	m_ape_checkbox		(NULL),
	m_files_count		(0),
	m_id3v1_count		(0),
	m_id3v2_count		(0),
	m_ape_count			(0)
{
	PRINT(("MPEGView::MPEGView(BRect)\n"));

	InitView();
}

MPEGView::~MPEGView()
{
	PRINT(("MPEGView::~MPEGView()\n"));
}

void
MPEGView::InitView()
{
	PRINT(("MPEGView::InitView()\n"));

	m_files_string = new BStringView("m_files_string", MPEG_FILES_STRING_DEFAULT);
	m_id3v1_string = new BStringView("m_id3v1_string", MPEG_ID3V1_STRING_DEFAULT);
	m_id3v2_string = new BStringView("m_id3v2_string", MPEG_ID3V2_STRING_DEFAULT);
	m_ape_string = new BStringView("m_ape_string", MPEG_APE_STRING_DEFAULT);

	m_info_box = new BBox("m_info_box");
	m_info_box->SetLabel(MPEG_INFO_BOX_LABEL);

	BGroupLayout *infoBoxLayout = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_SMALL_INSETS)
		.Add(m_files_string)
		.Add(m_id3v1_string)
		.Add(m_id3v2_string)
		.Add(m_ape_string);

	m_info_box->AddChild(infoBoxLayout->View());

	m_add_radiobutton = new BRadioButton("m_add_radiobutton",
		MPEG_ADD_RADIOBUTTON, new BMessage(RADIO_BUTTON_EVENT));
	m_add_radiobutton->SetValue(B_CONTROL_ON);

	m_remove_radiobutton = new BRadioButton("m_remove_radiobutton",
		MPEG_REMOVE_RADIOBUTTON, new BMessage(RADIO_BUTTON_EVENT));

	m_id3v1_checkbox = new BCheckBox("m_id3v1_checkbox", "ID3v1", NULL);
	m_id3v2_checkbox = new BCheckBox("m_id3v2_checkbox", "ID3v2", NULL);
	m_ape_checkbox = new BCheckBox("m_ape_checkbox", "APE", NULL);

	m_add_remove_box = new BBox("m_add_remove_box");
	m_add_remove_box->SetLabel(MPEG_ADD_REMOVE_BOX_LABEL);

	BGroupLayout *addRemoveBoxLayout = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_SMALL_INSETS)
		.Add(m_add_radiobutton)
		.Add(m_remove_radiobutton)
		.Add(m_id3v1_checkbox)
		.Add(m_id3v2_checkbox)
		.Add(m_ape_checkbox);

	m_add_remove_box->AddChild(addRemoveBoxLayout->View());

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(m_info_box)
		.Add(m_add_remove_box)
		.AddGlue();

	ResizeToPreferred();

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
MPEGView::AttachedToWindow()
{
	AddOnView::AttachedToWindow();

	PRINT(("MPEGView::AttachedToWindow()\n"));

	m_add_radiobutton->SetTarget(this);
	m_remove_radiobutton->SetTarget(this);
}

void
MPEGView::Hide()
{
	PRINT(("MPEGView::Hide()\n"));

	AddOnView::Hide();
}

void
MPEGView::Show()
{
	PRINT(("MPEGView::Show()\n"));

	AddOnView::Show();
}

void
MPEGView::SelectionChanged(BList* list)
{
	AddOnView::SelectionChanged(list);

	PRINT(("MPEGView::SelectionChanged()\n"));

	EntryRefItem * ref_item = (NULL);
	int32 index = 0;
	int32 id3v1_tags = 0;
	int32 id3v2_tags = 0;
	int32 ape_tags = 0;

	while (ref_item = (EntryRefItem*) m_selected_items->ItemAt(index++))
	{
		if (ref_item->HasID3V1Tag())
			id3v1_tags++;

		if (ref_item->HasID3V2Tag())
			id3v2_tags++;

		if (ref_item->HasAPETag())
			ape_tags++;
	}

	index--;

	m_files_count = index;
	m_id3v1_count = id3v1_tags;
	m_id3v2_count = id3v2_tags;
	m_ape_count = ape_tags;

	WidgetsSetValues();
	WidgetsSetEnabled();
	WidgetsRBValues();
}

void
MPEGView::Apply()
{
	AddOnView::Apply();

	PRINT(("MPEGView::Apply()\n"));

	thread_id thread = spawn_thread(MPEGView::ApplyFunction, "MPEG", B_NORMAL_PRIORITY,(void*)this);
	resume_thread(thread);
}

void
MPEGView::Reset()
{
	AddOnView::Reset();

	PRINT(("MPEGView::Reset()\n"));

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
MPEGView::WidgetsSetValues()
{
	PRINT(("MPEGView::WidgetsSetValues()\n"));

	BString files_string;
	BString id3v1_string;
	BString id3v2_string;
	BString ape_string;

	if (m_files_count == 1)	files_string << m_files_count << " file in selection.";
	else					files_string << m_files_count << " files in selection.";

	if (m_id3v1_count == 1)	id3v1_string << m_id3v1_count << " has ID3v1 tags.";
	else					id3v1_string << m_id3v1_count << " have ID3v1 tags.";

	if (m_id3v2_count == 1)	id3v2_string << m_id3v2_count << " has ID3v2 tags.";
	else					id3v2_string << m_id3v2_count << " have ID3v2 tags.";

	if (m_ape_count == 1)	ape_string << m_ape_count << " has APE tags.";
	else					ape_string << m_ape_count << " have APE tags.";

	m_files_string->SetText(files_string.String());
	m_id3v1_string->SetText(id3v1_string.String());
	m_id3v2_string->SetText(id3v2_string.String());
	m_ape_string->SetText(ape_string.String());

	m_files_string->ResizeToPreferred();
	m_id3v1_string->ResizeToPreferred();
	m_id3v2_string->ResizeToPreferred();
	m_ape_string->ResizeToPreferred();

	m_id3v1_checkbox->SetValue(B_CONTROL_OFF);
	m_id3v2_checkbox->SetValue(B_CONTROL_OFF);
	m_ape_checkbox->SetValue(B_CONTROL_OFF);
}

void
MPEGView::WidgetsSetEnabled()
{
	PRINT(("MPEGView::WidgetsSetEnabled()\n"));

	if(m_selected_items->CountItems() > 0)
	{
		m_add_radiobutton->SetEnabled(true);
		m_remove_radiobutton->SetEnabled(true);
		m_id3v1_checkbox->SetEnabled(true);
		m_id3v2_checkbox->SetEnabled(true);
		m_ape_checkbox->SetEnabled(true);
	}
	else
	{
		m_add_radiobutton->SetEnabled(false);
		m_remove_radiobutton->SetEnabled(false);
		m_id3v1_checkbox->SetEnabled(false);
		m_id3v2_checkbox->SetEnabled(false);
		m_ape_checkbox->SetEnabled(false);
	}
}

void
MPEGView::WidgetsRBValues()
{
	PRINT(("MPEGView::WidgetsRBValues()\n"));

	m_id3v1_checkbox->SetEnabled(false);
	m_id3v2_checkbox->SetEnabled(false);
	m_ape_checkbox->SetEnabled(false);

	if(m_add_radiobutton->Value() == B_CONTROL_ON)
	{
		if (m_id3v1_count < m_files_count)
			m_id3v1_checkbox->SetEnabled(true);

		if (m_id3v2_count < m_files_count)
			m_id3v2_checkbox->SetEnabled(true);

		//if (m_ape_count < m_files_count)
		//	m_ape_checkbox->SetEnabled(true);
		m_ape_checkbox->SetEnabled(false);
	}
	else
	{
		if (m_id3v1_count > 0)
			m_id3v1_checkbox->SetEnabled(true);

		if (m_id3v2_count > 0)
			m_id3v2_checkbox->SetEnabled(true);

		if (m_ape_count > 0)
			m_ape_checkbox->SetEnabled(true);
	}
}

int32 MPEGView::ApplyFunction(void* args) {
	MPEGView* view = (MPEGView*)args;

	BView* appView = view->Window()->FindView("ArmyKnifeAppView");
	BMessenger messenger(appView);
	BMessage startMsg(START_APPLY);
	BMessage endMsg(END_APPLY);
	BMessage maxMsg(STATUS_BAR_SET_MAX_VALUE);
	BMessage updateMsg(STATUS_BAR_UPDATE);
	BMessage resetMsg(STATUS_BAR_RESET);

	messenger.SendMessage(&startMsg);

	int numSelected = view->m_selected_items->CountItems();
	maxMsg.AddInt32("maxValue",numSelected);
	messenger.SendMessage(&maxMsg);

	for(int i=0; i<numSelected; i++)
	{
		EntryRefItem* refItem = (EntryRefItem*)view->m_selected_items->ItemAt(i);

		BPath  filename(refItem->EntryRef());
		TagLib::MPEG::File * file;

		if (! refItem->IsMP3())
			continue;

		if (! refItem->IsSupportedByTaglib())
			continue;

		file = new TagLib::MPEG::File(filename.Path());

		if (! file->isValid())
		{
			delete file;
			continue;
		}

		bool add_id3v1 = false;
		bool add_id3v2 = false;
		bool add_ape = false;
		bool remove_id3v1 = false;
		bool remove_id3v2 = false;
		bool remove_ape = false;

		if(view->m_add_radiobutton->Value() == B_CONTROL_ON)
		{
			if (view->m_id3v1_checkbox->Value() == B_CONTROL_ON)
				add_id3v1 = true;

			if (view->m_id3v2_checkbox->Value() == B_CONTROL_ON)
				add_id3v2 = true;

			if (view->m_ape_checkbox->Value() == B_CONTROL_ON)
				add_ape = true;
		}
		else
		{
			if (view->m_id3v1_checkbox->Value() == B_CONTROL_ON)
				remove_id3v1 = true;

			if (view->m_id3v2_checkbox->Value() == B_CONTROL_ON)
				remove_id3v2 = true;

			if (view->m_ape_checkbox->Value() == B_CONTROL_ON)
				remove_ape = true;
		}

		TagLib::ID3v1::Tag * id3v1_tag;
		TagLib::ID3v2::Tag * id3v2_tag;
		TagLib::APE::Tag * ape_tag;

		if (add_id3v1 && ! refItem->HasID3V1Tag())
		{
			id3v1_tag = file->ID3v1Tag(true);
			if (id3v1_tag != NULL)
			{
				id3v1_tag->setGenre("Other");
				file->save();
			}
		}

		if (add_id3v2 && ! refItem->HasID3V2Tag())
		{
			id3v2_tag = file->ID3v2Tag(true);
			if (id3v2_tag != NULL)
			{
				id3v2_tag->setGenre("Other");
				file->save();
			}
		}

		if (add_ape && ! refItem->HasAPETag())
		{
			ape_tag = file->APETag(true);
			file->save();
		}

		if (remove_id3v1 && refItem->HasID3V1Tag())
			file->strip(TagLib::MPEG::File::ID3v1);

		if (remove_id3v2 && refItem->HasID3V2Tag())
			file->strip(TagLib::MPEG::File::ID3v2);

		if (remove_ape && refItem->HasAPETag())
			file->strip(TagLib::MPEG::File::APE);

		delete file;
		refItem->UpdateTaglibMetadata();

		messenger.SendMessage(&updateMsg);
	}

	messenger.SendMessage(&resetMsg);
	messenger.SendMessage(&endMsg);

	return B_OK;
}

void
MPEGView::MessageReceived(BMessage* message)
{
	//PRINT(("MPEGView::MessageReceived(BMessage*)\n"));

	switch(message->what)
	{

		case RADIO_BUTTON_EVENT:
			WidgetsRBValues();
			break;

		default:
			AddOnView::MessageReceived(message);
	}
}

bool
MPEGView::AcceptListItem(EntryRefItem* listItem)
{
	PRINT(("MPEGView::AcceptListItem()\n"));

	if (!listItem->IsFSWritable())
		return false;

	if (!listItem->IsSupportedByTaglib())
		return false;

	if (!listItem->IsMP3())
		return false;

	return true;
}
