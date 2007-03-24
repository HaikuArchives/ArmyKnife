#include <string.h>
#include <be/kernel/OS.h>
#include <be/storage/Directory.h>
#include <be/storage/Entry.h>
#include <be/storage/File.h>
#include <be/storage/FindDirectory.h>
#include <be/storage/Path.h>
#include <be/storage/Query.h>
#include <be/storage/Volume.h>
#include <be/storage/VolumeRoster.h>
#include <be/support/Debug.h>
#include <be/support/String.h>
#include "AEEncoder.h"

#define COMPANY "FlipSide Software"
#define APPLICATION "FlipSide A.E."

AEEncoder::AEEncoder()
{
	PRINT(("AEEncoder::AEEncoder()\n"));
	// Private constructor
}

AEEncoder::AEEncoder(AEEncoder& enc)
{
	PRINT(("AEEncoder::AEEncoder(AEEncoder&)\n"));
	// Private constructor
}

AEEncoder::AEEncoder(const char* encoderName)
{
	PRINT(("AEEncoder::AEEncoder(const char*)\n"));

	error = B_OK;
	menu = 0;

	if(!encoderName)
	{
		error = B_ERROR;
		return;
	}

	name = encoderName;

	LoadSettings();
}

AEEncoder::~AEEncoder()
{
	PRINT(("AEEncoder::~AEEncoder()\n"));

	if(SaveSettings() != B_OK)
	{
		PRINT(("SaveSettings() Failed.\n"));
	}

	delete menu;
}

int32
AEEncoder::InitCheck()
{
	PRINT(("AEEncoder::InitCheck()\n"));

	return error;
}

bool
AEEncoder::CheckForCancel()
{
	//PRINT(("AEEncoder::CheckForCancel()\n"));

	thread_id thread = find_thread("_Encoder_");
	if(has_data(thread))
	{
		thread_id sender;
		int32 code = receive_data(&sender,0,0);
		if(code == FSS_CANCEL_ENCODING)
		{
			canceled = true;
		}
	}
	return canceled;
}

const char*
AEEncoder::GetName()
{
	PRINT(("AEEncoder::GetName()\n"));

	return name.String();
}

const char*
AEEncoder::GetPattern()
{
	PRINT(("AEEncoder::GetPattern()\n"));

	if(pattern == "")
	{
		LoadDefaultPattern();
	}

	return pattern.String();
}

void
AEEncoder::SetPattern(const char* value)
{
	PRINT(("AEEncoder::SetPattern(const char*)\n"));

	if(value)
	{
		pattern = value;
	}
	else
	{
		pattern = "";
	}
}

BMenu*
AEEncoder::GetMenu()
{
	PRINT(("AEEncoder::GetMenu()\n"));

	if(!menu)
	{
		LoadDefaultMenu();
	}

	return menu;
}

int32
AEEncoder::InitEncoder()
{
	PRINT(("AEEncoder::InitEncoder()\n"));

	canceled = false;

	return B_OK;
}

int32
AEEncoder::Encode(BMessage* encodeMessage)
{
	PRINT(("AEEncoder::Encode(BMessage*)\n"));

	return B_OK;
}

int32
AEEncoder::UninitEncoder()
{
	PRINT(("AEEncoder::UninitEncoder()\n"));

	return B_OK;
}

int32
AEEncoder::LoadDefaultPattern()
{
	PRINT(("AEEncoder::LoadDefaultPattern()\n"));
	SetPattern(0);
	return B_OK;
}

int32
AEEncoder::LoadDefaultMenu()
{
	PRINT(("AEEncoder::LoadDefaultMenu()\n"));

	menu = new BMenu("");
	return B_OK;
}

int32
AEEncoder::LoadSettings()
{
	PRINT(("AEEncoder::LoadSettings()\n"));

	status_t status;
	BPath settingsPath;
	BFile settingsFile;
	BMessage settingsArchive;
	BString settingsName;

	find_directory(B_USER_SETTINGS_DIRECTORY,&settingsPath);
	status = settingsPath.InitCheck();
	if(status != B_OK)
	{
		PRINT(("ERROR in settingsPath\n"));
		return status;
	}

	settingsPath.Append(COMPANY);
	settingsPath.Append(APPLICATION);
	settingsName = name;
	settingsName << " Settings";
	settingsPath.Append(settingsName.String());

	status = settingsFile.SetTo(settingsPath.Path(),B_READ_ONLY);
	if(status != B_OK)
	{
		PRINT(("ERROR opening settingsFile\n"));
		return status;
	}

	status = settingsArchive.Unflatten(&settingsFile);
	if(status != B_OK)
	{
		PRINT(("ERROR unflattening settingsArchive\n"));
		return status;
	}

	menu = new BMenu(&settingsArchive);

	status = settingsArchive.FindString("pattern",&pattern);
	if(status != B_OK)
	{
		pattern = "";
	}

	return B_OK;
}

int32
AEEncoder::SaveSettings()
{
	PRINT(("AEEncoder::SaveSettings()\n"));

	if(!menu)
	{
		return B_ERROR;
	}

	status_t status;
	BPath settingsPath;
	BFile settingsFile;
	BMessage settingsArchive;
	BString settingsName;

	find_directory(B_USER_SETTINGS_DIRECTORY,&settingsPath);
	status = settingsPath.InitCheck();
	if(status != B_OK)
	{
		return status;
	}

	settingsPath.Append(COMPANY);
	settingsPath.Append(APPLICATION);
	create_directory(settingsPath.Path(),0755);

	settingsName = name;
	settingsName << " Settings";
	settingsPath.Append(settingsName.String());

	status = menu->Archive(&settingsArchive,true);
	if(status != B_OK)
	{
		return status;
	}

	status = settingsArchive.AddString("pattern",pattern);
	if(status != B_OK)
	{
		return status;
	}

	status = settingsFile.SetTo(settingsPath.Path(),
			B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);
	if(status != B_OK)
	{
		return status;
	}

	status = settingsArchive.Flatten(&settingsFile);
	if(status != B_OK)
	{
		return status;
	}

	return B_OK;
}


int32
AEEncoder::FindExecutable(const char* executable, char* path)
{
	BVolumeRoster volumeRoster;
	BVolume volume;

	if(volumeRoster.GetBootVolume(&volume) != B_OK)
	{
		// that sucks
		return B_ERROR;
	}

	if(QueryForExecutable(executable,&volume,path) == B_OK)
	{
		// we found it!!!
		return B_OK;
	}

	volumeRoster.Rewind();
	while(volumeRoster.GetNextVolume(&volume) == B_OK)
	{
		if(QueryForExecutable(executable,&volume,path) == B_OK)
		{
			// we found it!!!
			return B_OK;
		}
	}

	// can't find it
	return FSS_EXE_NOT_FOUND;
}

int32
AEEncoder::QueryForExecutable(const char* executable, BVolume* volume,
		char* path)
{
	PRINT(("AEEncoder::QueryForExecutable(const char*,BVolume*,char*)\n"));

	BQuery query;
	if(query.SetVolume(volume) != B_OK)
	{
		return B_ERROR;
	}

	query.PushAttr("name");
	query.PushString(executable);
	query.PushOp(B_EQ);
	query.PushAttr("BEOS:TYPE");
	query.PushString("application/x-vnd.Be-elfexecutable");
	query.PushOp(B_EQ);
	query.PushOp(B_AND);

	if(query.Fetch() != B_OK)
	{
		return B_ERROR;
	}

	BEntry entry;
	if(query.GetNextEntry(&entry) != B_OK)
	{
		return B_ERROR;
	}
	if(entry.InitCheck() != B_OK)
	{
		return B_ERROR;
	}

	BPath entryPath;
	entry.GetPath(&entryPath);
	if(entryPath.InitCheck() != B_OK)
	{
		return B_ERROR;
	}

	strcpy(path,entryPath.Path());
	if(strcmp(path,entryPath.Path()) != 0)
	{
		return B_ERROR;
	}

	PRINT(("Path for %s is %s\n",executable,path));
	return B_OK;
}
