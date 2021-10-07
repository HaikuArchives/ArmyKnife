/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "querywindow.h"

#include <Button.h>
#include <ColumnListView.h>
#include <ColumnTypes.h>
#include <Debug.h>
#include <LayoutBuilder.h>
#include <Message.h>

#include "commandconstants.h"
#include "guistrings.h"
#include "query.h"

QueryWindow::QueryWindow(Query* query, BMessenger replyTo)
	:
	BWindow(BRect(0, 0, 1000, 400), LOOKUP_TITLE, B_TITLED_WINDOW,
		B_AUTO_UPDATE_SIZE_LIMITS),
	m_reply_to(replyTo)
{
	PRINT(("QueryWindow::QueryWindow(Query*, BMessenger)\n"));

	m_worker = new QueryWindow::Worker(query, BMessenger(this));
	m_worker->Run();

	m_result_view = new BColumnListView("m_result_view", 0);

	m_result_view->AddColumn(new BStringColumn(ARTIST_LABEL, 200, 50, 1000,
		B_TRUNCATE_END), 0);

	m_result_view->AddColumn(new BStringColumn(TITLE_LABEL, 200, 50, 1000,
		B_TRUNCATE_END), 1);

	m_result_view->AddColumn(new BStringColumn(ALBUM_LABEL, 200, 50, 1000,
		B_TRUNCATE_END), 2);

	m_result_view->AddColumn(new BStringColumn(TRACK_LABEL, 60, 50, 1000,
		B_TRUNCATE_END), 3);

	m_result_view->AddColumn(new BStringColumn(YEAR_LABEL, 60, 50, 1000,
		B_TRUNCATE_END), 4);

	m_result_view->AddColumn(new BStringColumn(COMMENT_LABEL, 400, 50, 1000,
		B_TRUNCATE_END), 5);

	m_result_view->SetInvocationMessage(new BMessage(MSG_APPLY));
	m_result_view->SetSelectionMessage(new BMessage(SELECTION_CHANGED));

	m_more = new BButton("more", QUERY_MORE, new BMessage(MSG_SHOW_MORE));
	m_more->SetTarget(m_worker);
	BButton* cancel = new BButton("cancel", CANCEL_BUTTON, new BMessage(B_QUIT_REQUESTED));
	m_apply = new BButton("apply", APPLY_BUTTON, new BMessage(MSG_APPLY));
	m_apply->SetEnabled(false);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(m_result_view)
		.AddGroup(B_HORIZONTAL)
			.Add(m_more)
			.AddGlue()
			.Add(cancel)
			.Add(m_apply);

	BMessenger(m_worker).SendMessage(MSG_SHOW_MORE);

	CenterOnScreen();
}

QueryWindow::~QueryWindow()
{
	PRINT(("QueryWindow::~QueryWindow()\n"));

	BMessenger(m_worker).SendMessage(B_QUIT_REQUESTED);
}

void
QueryWindow::MessageReceived(BMessage* message)
{
	PRINT(("QueryWindow::MessageReceived(BMessage*)\n"));

	switch (message->what) {
		case MSG_SHOW_MORE:
		{
			void* ptr;
			if (message->FindPointer("row", &ptr) == B_OK)
				m_result_view->AddRow(static_cast<BRow*>(ptr));
			break;
		}

		case MSG_APPLY:
		{
			BMessage adopt(MSG_ADOPT_TAGS);
			m_worker->m_records.ItemAt(m_result_view->IndexOf(
					m_result_view->CurrentSelection()))->Archive(&adopt);
			m_reply_to.SendMessage(&adopt);
			BMessenger(this).SendMessage(B_QUIT_REQUESTED);
			break;
		}

		case MSG_UPDATE_BUTTON:
		{
			const char* label;
			if (message->FindString("label", &label) == B_OK)
				m_more->SetLabel(label);

			bool enabled;
			if (message->FindBool("enabled", &enabled) == B_OK)
				m_more->SetEnabled(enabled);

			break;
		}

		case SELECTION_CHANGED:
			m_apply->SetEnabled(m_result_view->CurrentSelection() != NULL);
			break;

		default:
			BWindow::MessageReceived(message);
			break;
	}
}

QueryWindow::Worker::Worker(Query* query, BMessenger reply_to)
	:
	BLooper(),
	m_query(query),
	m_reply_to(reply_to),
	m_records(20, true)
{
	PRINT(("QueryWindow::Worker::Worker(Query*, BMessenger)\n"));
}

QueryWindow::Worker::~Worker()
{
	PRINT(("QueryWindow::Worker::~Worker()"));

	delete m_query;
}

void
QueryWindow::Worker::MessageReceived(BMessage* message)
{
	PRINT(("QueryWindow::Worker::MessageReceived(BMessage*)\n"));

	switch (message->what)
	{
		case MSG_SHOW_MORE:
		{
			BMessage update(MSG_UPDATE_BUTTON);
			update.AddBool("enabled", false);
			update.AddString("label", QUERY_WORKING);
			m_reply_to.SendMessage(&update);

			int32 received = 0;
			status_t result;
			while (received < k_update_size && (result = m_query->HasNextResult()) == B_OK) {
				Queryable* result = new Queryable(m_query->GetNextResult());
				m_records.AddItem(result);

				BRow* row = new BRow();
				row->SetField(new BStringField(result->Artist()), 0);
				row->SetField(new BStringField(result->Title()), 1);
				row->SetField(new BStringField(result->Album()), 2);
				row->SetField(new BStringField(result->Track()), 3);
				row->SetField(new BStringField(result->Year()), 4);
				row->SetField(new BStringField(result->Comment()), 5);

				BMessage row_msg(MSG_SHOW_MORE);
				row_msg.AddPointer("row", row);
				m_reply_to.SendMessage(&row_msg);

				received++;
			}

			BMessage post(MSG_UPDATE_BUTTON);
			if (result != B_ERROR) {
				post.AddString("label", QUERY_MORE);
				post.AddBool("enabled", true);
			} else {
				post.AddString("label", QUERY_NO_MORE_RESULTS);
			}
			m_reply_to.SendMessage(&post);
			break;
		}

		case SELECTION_CHANGED:
			m_reply_to.SendMessage(message);
			break;

		default:
			BLooper::MessageReceived(message);
			break;
	}
}
