/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include <Message.h>
#include <String.h>

class BMessage;
class BString;

class Preferences
{
	public:
		Preferences();
		~Preferences();

		void	GetPreferences();
		void	SavePreferences();

		int32	GetMode();
		void	SetMode(int32 mode);

		int32	GetPattern();
		void	SetPattern(int32 index);
		void	GetPattern(BString * string);

		void	AddPattern(BString * string);

		status_t	GetPatternAt(int32 index, BString * string);

		void	DeletePattern(int32 index);
		void	NewPattern(BString * string);

		void	GetWindowFrame(BRect * rect);
		void	SetWindowFrame(BRect rect);

	private:

		void		MakeDefaults();

		int32		m_mode;
		int32		m_pattern;
		BMessage	m_filename_patterns;
		BRect		m_window_frame;


};

#endif
