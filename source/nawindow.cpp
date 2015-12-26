#include <be/support/Beep.h>
#include <be/support/Debug.h>
#include <be/support/String.h>
#include <LayoutBuilder.h>

#include "nawindow.h"
#include "commandconstants.h"
#include "guistrings.h"

PatternWindow::PatternWindow(BMessenger * messenger)
 :	BWindow(BRect(300,300,600,385), "New filename pattern", B_TITLED_WINDOW, B_NOT_V_RESIZABLE|B_NOT_ZOOMABLE|B_NOT_MINIMIZABLE),
	m_text_control		(NULL),
	m_add_button		(NULL),
	m_cancel_button		(NULL),
	m_parent_messenger	(messenger)
{
	m_text_control = new BTextControl("m_text_control","","",NULL);

	m_add_button = new BButton("m_add_button", ADD_BUTTON, new BMessage(MSG_PATTERN_CREATED));
	m_add_button->MakeDefault(true);

	m_cancel_button = new BButton("m_cancel_button", CANCEL_BUTTON, new BMessage(B_QUIT_REQUESTED));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(m_text_control)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(m_add_button)
			.Add(m_cancel_button);

	m_text_control->MakeFocus();
}

PatternWindow::~PatternWindow()
{
	delete m_parent_messenger;
}

void
PatternWindow::MessageReceived(BMessage* message)
{
//	PRINT(("PatternWindow::MessageReceived()\n"));

	switch(message->what)
	{
		case MSG_PATTERN_CREATED:
		{
			BString string = m_text_control->Text();

			if (string.FindFirst("/") < 0)
			{
				// XXX -- show BAlert and explain some
				beep();
				break;
			}

			message->AddString("pattern", m_text_control->Text());
			m_parent_messenger->SendMessage(message);
			PostMessage(B_QUIT_REQUESTED);
		}
			break;

		default:
			BWindow::MessageReceived(message);
	}
}

