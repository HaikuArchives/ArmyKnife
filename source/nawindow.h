/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __NA_WINDOW_H__
#define __NA_WINDOW_H__

#include <Button.h>
#include <Messenger.h>
#include <TextControl.h>
#include <View.h>
#include <Window.h>

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
