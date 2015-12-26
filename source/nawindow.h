#ifndef __NA_WINDOW_H__
#define __NA_WINDOW_H__

#include <be/app/Messenger.h>
#include <be/interface/Button.h>
#include <be/interface/TextControl.h>
#include <be/interface/View.h>
#include <be/interface/Window.h>

class PatternWindow : public BWindow
{
	public:
		PatternWindow(BMessenger * messenger);
		~PatternWindow();
		virtual void MessageReceived(BMessage* message);

	private:
		
		BTextControl *	m_text_control;
		BButton *		m_add_button;
		BButton *		m_cancel_button;
		
		BMessenger *	m_parent_messenger;
	
};

#endif
