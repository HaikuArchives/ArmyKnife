/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifdef _TTE_
#include <stdlib.h>
#include <string.h>
#include <Alert.h>
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <Entry.h>
#include <File.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Message.h>
#include <Messenger.h>
#include <NodeInfo.h>
#include <OptionPopUp.h>
#include <Path.h>
#include <RadioButton.h>
#include <Rect.h>
#include <ScrollBar.h>
#include <ScrollView.h>
#include <StringView.h>
#include <TextControl.h>

#define DEBUG 0
#include <be/support/Debug.h>
#include <be/support/List.h>
#include "audioattributes.h"
#include "genrelist.h"
#include "id3tags.h"
#include "appdefs.h"
#include "appview.h"
#include "appwindow.h"
#include "commandconstants.h"
#include "entryrefitem.h"
#include "guistrings.h"
#include "picklistview.h"
#include "ttinfoview.h"

TTInfoView::TTInfoView(Preferences * preferences)
 :	AddOnView		(TT_INFO_MODE_NAME),
	m_preferences	(preferences)
{
	PRINT(("TTInfoView::TTInfoView(BRect)\n"));

	InitView();
}

TTInfoView::~TTInfoView()
{
	PRINT(("TTInfoView::~TTInfoView()\n"));
}

void
TTInfoView::InitView()
{
	PRINT(("TTInfoView::InitView()\n"));
}

void
TTInfoView::AttachedToWindow()
{
	AddOnView::AttachedToWindow();

	PRINT(("TTInfoView::AttachedToWindow()\n"));

	BRect parentBounds = Bounds();

	// string view
	m_string_view = new BStringView(BRect(0,0,0,0),"Info About This Track",
		"Info About This Track", B_FOLLOW_TOP|B_FOLLOW_LEFT_RIGHT);
	m_string_view->SetAlignment(B_ALIGN_CENTER);
	m_string_view->SetFont(be_bold_font);
	float stringWidth, stringHeight;
	m_string_view->GetPreferredSize(&stringWidth, &stringHeight);
	m_string_view->ResizeTo(parentBounds.Width(), stringHeight);
	m_string_view->MoveBy(0,5);
	AddChild(m_string_view);

	// text view
	BRect textViewBounds = parentBounds;
	textViewBounds.InsetBy(2,2);
	textViewBounds.right -= B_V_SCROLL_BAR_WIDTH;
	textViewBounds.top += (stringHeight * 1.5);

	BRect textAreaBounds = textViewBounds;
	textAreaBounds.OffsetTo(0,0);
	textAreaBounds.InsetBy(3,3);

	m_comment_view = new BTextView(textViewBounds, NULL, textAreaBounds,
		B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_PULSE_NEEDED|B_NAVIGABLE);
	m_comment_view->ResizeToPreferred();


	BScrollView *scrollView = new BScrollView("scroller", m_comment_view, B_FOLLOW_ALL_SIDES,
		B_WILL_DRAW|B_PULSE_NEEDED|B_NAVIGABLE, false, true);

	AddChild(scrollView);
}

void
TTInfoView::GetPreferredSize(float* width, float* height)
{
	PRINT(("TTInfoView::GetPreferredSize(float*,float*)\n"));
/*
	int space = 6;

	*width = m_artist_textcontrol->Frame().right;
	if((m_edit_box->Frame().right) > *width)
	{
		*width = m_edit_box->Frame().right;
	}

	*height = m_genre_box->Frame().bottom;
*/
	*width = 50;
	*height = 50;
}

void
TTInfoView::Hide()
{
	AddOnView::Hide();

	PRINT(("TTInfoView::Hide()\n"));
}

void
TTInfoView::Show()
{
	AddOnView::Show();

	PRINT(("TTInfoView::Show()\n"));

	Window()->PostMessage(MSG_MAKE_APPLY_BUTTON_NOT_DEFAULT);
}

void
TTInfoView::SelectionChanged(BList* list)
{
	AddOnView::SelectionChanged(list);

	PRINT(("TTInfoView::SelectionChanged(BList* list)\n"));

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
TTInfoView::Apply()
{
	AddOnView::Apply();

	PRINT(("TTInfoView::Apply()\n"));

	thread_id thread = spawn_thread(TTInfoView::ApplyFunction,"TheEditor",
			B_NORMAL_PRIORITY,(void*)this);
	resume_thread(thread);
}

void
TTInfoView::Reset()
{
	AddOnView::Reset();

	PRINT(("TTInfoView::Reset()\n"));

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
TTInfoView::WidgetsSetValues()
{
	PRINT(("TTInfoView::WidgetsSetValues()\n"));

	int numSelected = m_selected_items->CountItems();

	if(numSelected == 0)
	{
		m_string_view->SetText("Info About This Track");
		m_comment_view->SetText("");
	}
	else if(numSelected == 1)
	{
		m_string_view->SetText("Info About This Track");

		BString info;
		GetInfoFromSelection(numSelected, &info);
		m_comment_view->SetText(info.String());
	}
	else if(numSelected > 1)
	{
		m_string_view->SetText("Info About These Tracks");

		BString info;
		GetInfoFromSelection(numSelected, &info);
		m_comment_view->SetText(info.String());
	}
}

void
TTInfoView::WidgetsSetEnabled()
{
	PRINT(("TTInfoView::WidgetsSetEnabled()\n"));

	int numSelected = m_selected_items->CountItems();

	if(numSelected == 0)
	{
		m_comment_view->MakeEditable(false);
		m_comment_view->MakeSelectable(false);
	}
	else if(numSelected > 0)
	{
		m_comment_view->MakeEditable(true);
		m_comment_view->MakeSelectable(true);
	}
}

int32
TTInfoView::ApplyFunction(void* args)
{
	PRINT(("TTInfoView::ApplyFunction(void*)\n"));

	TTInfoView* view = (TTInfoView*)args;

	BView* appView = view->Window()->FindView("ArmyKnifeAppView");
	if (appView == NULL)
		return B_ERROR;

	BMessenger messenger(appView);
	BMessage startMsg(START_APPLY);
	BMessage endMsg(END_APPLY);
	BMessage maxMsg(STATUS_BAR_SET_MAX_VALUE);
	BMessage fileMsg(STATUS_BAR_FILE);
	BMessage updateMsg(STATUS_BAR_UPDATE);
	BMessage resetMsg(STATUS_BAR_RESET);

	messenger.SendMessage(&startMsg);

	if (! view->IsNewInfoSane(view->m_comment_view->Text())) {
		PRINT(("Info not sane!\n"));

		BAlert* alert = new BAlert("ArmyKnifeError", "Info not applied.\n\nThe current info text includes at least one "
			"of the names of the selected files.\n\nPlease look it over and try again.", "Okay", NULL, NULL,
			B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();

		messenger.SendMessage(&resetMsg);
		messenger.SendMessage(&endMsg);
		return B_ERROR;
	}

	int numSelected = view->m_selected_items->CountItems();
	maxMsg.AddInt32("maxValue",numSelected);
	messenger.SendMessage(&maxMsg);

	for(int i=0; i<numSelected; i++)
	{
		EntryRefItem * refItem = NULL;
		refItem = (EntryRefItem*)view->m_selected_items->ItemAt(i);

		if (refItem == NULL) {
			messenger.SendMessage(&resetMsg);
			messenger.SendMessage(&endMsg);
			return B_ERROR;
		}

		BFile audioFile(refItem->EntryRef(), B_READ_WRITE);

		fileMsg.MakeEmpty();
		fileMsg.AddString("file", refItem->EntryRef()->name);
		messenger.SendMessage(&fileMsg);

		AudioAttributes attributes(& audioFile);
		attributes.SetTTInfo(view->m_comment_view->Text());
		attributes.WriteTTInfo();

		messenger.SendMessage(&updateMsg);
	}
	messenger.SendMessage(&resetMsg);
	messenger.SendMessage(&endMsg);
}

void
TTInfoView::MessageReceived(BMessage* message)
{
	//PRINT(("TTInfoView::MessageReceived(BMessage*)\n"));

	AddOnView::MessageReceived(message);
}

void
TTInfoView::GetInfoFromSelection(int32 numberOfSelectedItems, BString *output, bool doListFiles = false)
{
	PRINT(("TTInfoView::GetInfoFromSelection(%ld, %s, ", numberOfSelectedItems, output->String()));
	if (doListFiles)
		PRINT(("true)\n"));
	else
		PRINT(("false)\n"));

	if (numberOfSelectedItems < 1)
		return;

	if (output == NULL)
		return;

	output->Truncate(0);

	BString info;
	BString last_info;

	// check for differing info
	if (!doListFiles) {
		for (int32 count = 0; count < numberOfSelectedItems; count++) {

			EntryRefItem* item = (EntryRefItem*)m_selected_items->ItemAt(count);
			BFile audioFile(item->EntryRef(), B_READ_WRITE);
			AudioAttributes attributes(& audioFile);
			info = attributes.TTInfo();

			if ((count > 0) && (info != last_info)) {

				output->Truncate(0);
				GetInfoFromSelection(numberOfSelectedItems, output, true);
				return;
			}

			last_info = info;
		}
	}

	// present info
	for (int32 count = 0; count < numberOfSelectedItems; count++) {

		EntryRefItem* item = (EntryRefItem*)m_selected_items->ItemAt(count);
		BFile audioFile(item->EntryRef(), B_READ_WRITE);
		AudioAttributes attributes(& audioFile);
		info = attributes.TTInfo();

		if (doListFiles) {
			*output << item->EntryRef()->name;
			*output << ":\n";
			if (info.Length() > 0)
				*output << info;
			if (count != (numberOfSelectedItems - 1))
				*output << "\n\n";
		}
		else {
			if (info.Length() > 0)
				*output << info;
			return;
		}
	}
}

bool
TTInfoView::IsNewInfoSane(const char * text)
{
	BString newInfo = text;
	BString fileName;

	int32 count = 0;
	EntryRefItem *item = NULL;


	while ((item = (EntryRefItem*)m_selected_items->ItemAt(count++)) != NULL) {

		fileName =  item->EntryRef()->name;
		fileName += ":\n";

		PRINT(("%ld: ref: %s\n", count, fileName.String()));

		if (newInfo.FindFirst(fileName) >= 0)
			return false;
	}

	return true;
}

bool
TTInfoView::AcceptListItem(EntryRefItem* listItem)
{
	PRINT(("TTInfoView::AcceptListItem()\n"));

	if (!listItem->IsFSWritable())
		return false;

	if (!listItem->IsFSAttributable())
		return false;

	return true;
}

#endif // _TTE_

