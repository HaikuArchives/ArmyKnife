#include <be/support/Beep.h>
#include <be/support/Debug.h>
#include <be/support/String.h>

#include "nawindow.h"
#include "commandconstants.h"
#include "guistrings.h"

PatternWindow::PatternWindow(BMessenger * messenger)
 :	BWindow(BRect(300,300,600,385), "New filename pattern", B_TITLED_WINDOW, B_NOT_V_RESIZABLE|B_NOT_ZOOMABLE|B_NOT_MINIMIZABLE),
	m_view				(new BView(BRect(0,0,Bounds().Width(),Frame().Height()), "box view", B_FOLLOW_ALL, B_NAVIGABLE)),
	m_text_control		(NULL),
	m_add_button		(NULL),
	m_cancel_button		(NULL),
	m_parent_messenger	(messenger)
{
	rgb_color color = {216, 216, 216, 0};
	m_view->SetViewColor(color);
	AddChild(m_view);

	BRect text_rect = Bounds();
	text_rect.InsetBy(10, 10);
	text_rect.bottom = text_rect.top + 30;
	m_text_control = new BTextControl(text_rect, "pattern",0,0,0);
	m_view->AddChild(m_text_control);
	
	BRect button_rect = Bounds();
	button_rect.right -= 10;
	button_rect.left = button_rect.right - 80;
	button_rect.bottom -= 10;
	button_rect.top = button_rect.bottom - 30;

	m_add_button = new BButton(button_rect, "add", ADD_BUTTON, new BMessage(MSG_PATTERN_CREATED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	m_add_button->MakeDefault(true);
	
	button_rect.left -= 90;
	button_rect.right -= 90;

	m_cancel_button = new BButton(button_rect, "cancel", CANCEL_BUTTON, new BMessage(B_QUIT_REQUESTED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	
	m_view->AddChild(m_add_button);
	m_view->AddChild(m_cancel_button);
	
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

