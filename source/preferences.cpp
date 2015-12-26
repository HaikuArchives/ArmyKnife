#include <errno.h>
#include <string.h>

#include <Debug.h>
#include <Directory.h>
#include <File.h>
#include <FindDirectory.h>
#include <Message.h>
#include <Path.h>

#include "appdefs.h"
#include "commandconstants.h"
#include "guistrings.h"
#include "preferences.h"

Preferences::Preferences()
:	m_mode				(0),
	m_pattern			(0),
	m_filename_patterns	(),
	m_window_frame		(200,200,700,600)
{
	PRINT(("Preferences::Preferences()\n"));

	MakeDefaults();
	GetPreferences();
}

Preferences::~Preferences()
{
	PRINT(("Preferences::~Preferences()\n"));

	SavePreferences();
}

void
Preferences::MakeDefaults()
{
	PRINT(("Preferences::MakeDefaults()\n"));

	m_filename_patterns.AddString("nameattr", "/a - /n - /k - /t.mp3");
	m_filename_patterns.AddString("nameattr", "/a - /n - /k - /t.ogg");
	m_filename_patterns.AddString("nameattr", "/a - /n - /k - /t.flac");
	m_filename_patterns.AddString("nameattr", "/a - /n - /k - /t");
}

int32
Preferences::GetMode()
{
	PRINT(("Preferences::GetMode()\n"));

	return m_mode;
}

void
Preferences::SetMode(int32 mode)
{
	PRINT(("Preferences::SetMode()\n"));

	m_mode = mode;
}

int32
Preferences::GetPattern()
{
	return m_pattern;
}

void
Preferences::GetPattern(BString * string)
{
	PRINT(("Preferences::GetPattern(BString * string)\n"));

	BString pattern;

	if(m_filename_patterns.FindString("nameattr", m_pattern, & pattern) == B_OK)
	{
		*string = pattern;
	}
}

void
Preferences::SetPattern(int32 index)
{
	PRINT(("Preferences::SetPattern(int32 index: %ld)\n", index));

	BString pattern;

	if(m_filename_patterns.FindString("nameattr", index, & pattern) == B_OK)
	{
		m_pattern = index;
	}

}

status_t
Preferences::GetPatternAt(int32 index, BString * string)
{
	PRINT(("Preferences::GetPatternAt(int32 index, BString * string)\n"));

	BString pattern;

	if(m_filename_patterns.FindString("nameattr", index, & pattern) == B_OK)
	{
		*string = pattern;
		return B_OK;
	}
	else
		return B_ERROR;
}

void
Preferences::AddPattern(BString * string)
{
	PRINT(("Preferences::AddPattern(BString * string)\n"));

	if (string != NULL)
	{
		BMessage new_pattern_message;

		new_pattern_message.AddString("nameattr", string->String());

		BString temp;
		int32 index = 0;

		while (m_filename_patterns.FindString("nameattr", index++, & temp) == B_OK)
		{
			new_pattern_message.AddString("nameattr", temp.String());
		}

		m_filename_patterns.MakeEmpty();
		m_filename_patterns = new_pattern_message;

		SetPattern(0);
	}
}

void
Preferences::DeletePattern(int32 index)
{
	PRINT(("Preferences::DeletePattern(int32 index)\n"));

	m_filename_patterns.RemoveData("nameattr", index);
}


void
Preferences::GetWindowFrame(BRect * rect)
{
	PRINT(("Preferences::GetWindowFrame()\n"));

	*rect = m_window_frame;
}

void
Preferences::SetWindowFrame(BRect rect)
{
	PRINT(("Preferences::SetWindowFrame()\n"));

	m_window_frame = rect;
}

void
Preferences::GetPreferences()
{
	PRINT(("Preferences::GetPreferences()\n"));

	BPath path;
	BFile file;
	BMessage message;
	status_t status = B_OK;

	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.Append(PROJECT_DIR);
	path.Append(APPLICATION_DIR);
	path.Append(PREFS_FILE);

	status = file.SetTo(path.Path(), B_READ_ONLY);
	if((status == B_OK) && (message.Unflatten(&file) == B_OK))
	{
		status_t	status	=	B_OK;
		int32		mode	=	0;
		int32		pattern	=	0;
		BMessage	patterns;
		BRect		rect	(200,200,700,600);

		status = message.FindInt32("mode", &mode);
		if(status == B_OK)
			m_mode = mode;

		status = message.FindInt32("pattern", &pattern);
		if(status == B_OK)
			m_pattern = pattern;

		status = message.FindMessage("patterns", &patterns);
		if(status == B_OK)
			m_filename_patterns = patterns;

		status = message.FindRect("winframe", &rect);
		if(status == B_OK)
			m_window_frame = rect;
	}
	else
		PRINT(("Error: %s\n", strerror(status)));
}

void
Preferences::SavePreferences()
{
	PRINT(("Preferences::SavePreferences()\n"));

	BMessage preferences;
	preferences.AddInt32("mode", m_mode);
	preferences.AddInt32("pattern", m_pattern);
	preferences.AddMessage("patterns", & m_filename_patterns);
	preferences.AddRect("winframe", m_window_frame);

	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	path.Append(PROJECT_DIR);
	path.Append(APPLICATION_DIR);
	create_directory(path.Path(), 0755);
	path.Append(PREFS_FILE);

	BFile file(path.Path(), B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);
	if(file.InitCheck() != B_OK)
	{
		PRINT(("file.InitCheck(): %s\n", strerror(errno)));
		return;
	}

	if(preferences.Flatten(&file) != B_OK)
	{
		PRINT(("preferences.Flatten(&file): %s\n", strerror(errno)));
		return;
	}
}

