#include <string.h>
#include <be/app/Application.h>
#include <be/app/Message.h>
#include <be/app/Roster.h>
#include <be/storage/Directory.h>
#include <be/storage/File.h>
#include <be/storage/FindDirectory.h>
#include <be/storage/Path.h>
#include <be/support/Debug.h>
#include <be/support/String.h>
#include <AEEncoder/AEEncoder.h>
#include <Santa/ColumnListView.h>
#include "AppDefs.h"
#include "CommandConstants.h"
#include "Settings.h"

Settings* settings = 0;

#define DEFAULT_ENCODER "MP3 GoGo"
#define DEFAULT_NUMBER_OF_SORT_KEYS 0

Settings::Settings() : BArchivable()
{
	PRINT(("Settings::Settings()\n"));

	encoding = false;
	encoder = NULL;
	strcpy(encoderName,DEFAULT_ENCODER);

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		columnDisplayOrder[i] = i;
	}

	numberOfSortKeys = DEFAULT_NUMBER_OF_SORT_KEYS;

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		sortKeys[i] = i;
	}

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		sortModes[i] = NoSort;
	}

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		columnsShown[i] = true;
	}

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		columnWidths[i] = 0.0;
	}

	app_info info;
	be_app->GetAppInfo(&info);

	BPath path(&(info.ref));
	path.GetParent(&path);
	path.Append(ADD_ON_DIR);

	strcpy(addonDirectory,path.Path());

	settings = this;
}

Settings::Settings(BMessage* archive) : BArchivable(archive)
{
	PRINT(("Settings::Settings(BMessage*)\n"));

	status_t status;
	BString tmp;

	encoding = false;
	encoder = NULL;
	status = archive->FindString("encoderName",&tmp);
	if(status != B_OK)
	{
		PRINT(("Error loading ENCODER\n"));
		strcpy(encoderName,DEFAULT_ENCODER);
	}
	else
	{
		strcpy(encoderName,tmp.String());
	}

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		status = archive->FindInt32("columnDisplayOrder",i,
				&columnDisplayOrder[i]);
		if(status != B_OK)
		{
			PRINT(("Error loading COLUMN_DISPLAY_ORDER\n"));
			break;
		}
	}
	if(status != B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			columnDisplayOrder[i] = i;
		}
	}

	status = archive->FindInt32("numberOfSortKeys",&numberOfSortKeys);
	if(status != B_OK)
	{
		PRINT(("Error loading NUMBER_OF_SORT_KEYS\n"));
		numberOfSortKeys = DEFAULT_NUMBER_OF_SORT_KEYS;
	}

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		status = archive->FindInt32("sortKeys",i,&sortKeys[i]);
		if(status != B_OK)
		{
			PRINT(("Error loading SORT_KEYS\n"));
			break;
		}
	}
	if(status != B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			sortKeys[i] = i;
		}
	}

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		status = archive->FindInt32("sortModes",i,(int32*)&sortModes[i]);
		if(status != B_OK)
		{
			PRINT(("Error loading SORT_MODES\n"));
			break;
		}
	}
	if(status != B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			sortModes[i] = NoSort;
		}
	}

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		status = archive->FindBool("columnsShown",i,
				&columnsShown[i]);
		if(status != B_OK)
		{
			PRINT(("Error loading COLUMNS_SHOWN\n"));
			break;
		}
	}
	if(status != B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			columnsShown[i] = true;
		}
	}

	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		status = archive->FindFloat("columnWidths",i,&columnWidths[i]);
		if(status != B_OK)
		{
			PRINT(("Error loading COLUMN_WIDTHS\n"));
			break;
		}
	}
	if(status != B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			columnWidths[i] = 0.0;
		}
	}

	app_info info;
	be_app->GetAppInfo(&info);

	BPath path(&(info.ref));
	path.GetParent(&path);
	path.Append(ADD_ON_DIR);

	strcpy(addonDirectory,path.Path());

	settings = this;
}

Settings::~Settings()
{
	PRINT(("Settings::~Settings()\n"));

	SaveSettings();
}

BArchivable*
Settings::Instantiate(BMessage* archive)
{
	PRINT(("Settings::Instantiate(BMessage*)\n"));

	BArchivable* result = NULL;

	if(validate_instantiation(archive,"Settings"))
	{
		result = new Settings(archive);
	}

	return result;
}

status_t
Settings::Archive(BMessage* archive, bool deep=true) const
{
	PRINT(("Settings::Archive(BMessage*,bool)\n"));

	status_t status = B_OK;

	status = BArchivable::Archive(archive,deep);

	if(status == B_OK)
	{
		status = archive->AddString("encoderName",encoderName);
	}

	if(status == B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			status = archive->AddInt32("columnDisplayOrder",columnDisplayOrder[i]);
			if(status != B_OK)
			{
				break;
			}
		}
	}

	if(status == B_OK)
	{
		status = archive->AddInt32("numberOfSortKeys",numberOfSortKeys);
	}

	if(status == B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			status = archive->AddInt32("sortKeys",sortKeys[i]);
			if(status != B_OK)
			{
				break;
			}
		}
	}

	if(status == B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			status = archive->AddInt32("sortModes",sortModes[i]);
			if(status != B_OK)
			{
				break;
			}
		}
	}

	if(status == B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			status = archive->AddBool("columnsShown",columnsShown[i]);
			if(status != B_OK)
			{
				break;
			}
		}
	}

	if(status == B_OK)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			status = archive->AddFloat("columnWidths",columnWidths[i]);
			if(status != B_OK)
			{
				break;
			}
		}
	}

	return status;
}

void
Settings::OpenSettings()
{
	PRINT(("Settings::GetInstance()\n"));

	BPath path;
	BFile settingsFile;
	BMessage archive;
	status_t status;

	find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	path.Append(COMPANY);
	path.Append(APPLICATION);
	path.Append(SETTINGS);

	status = settingsFile.SetTo(path.Path(), B_READ_ONLY);
	if((status == B_OK) && (archive.Unflatten(&settingsFile) == B_OK))
	{
		new Settings(&archive);
	}
	else
	{
		new Settings();
	}
}

void
Settings::SaveSettings()
{
	PRINT(("Settings::SaveWindow()\n"));

	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	path.Append(COMPANY);
	path.Append(APPLICATION);
	create_directory(path.Path(),0755);
	path.Append(SETTINGS);
	BMessage archive;

	Archive(&archive,false);
	BFile settings(path.Path(),B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);
	if(settings.InitCheck() == B_OK)
	{
		archive.Flatten(&settings);
	}
}

bool
Settings::IsEncoding()
{
	return encoding;
}

void
Settings::SetEncoding(bool value)
{
	encoding = value;
}

AEEncoder*
Settings::Encoder()
{
	PRINT(("Settings::Encoder()\n"));

	return encoder;
}

void
Settings::SetEncoder(AEEncoder* value)
{
	PRINT(("Settings::SetEncoder()\n"));

	if(!encoding)
	{
		delete encoder;
		encoder = value;

		if(encoder)
		{
			SetEncoderName(encoder->GetName());
		}
	}
}

const char*
Settings::EncoderName()
{
	PRINT(("Settings::EncoderName()\n"));

	return encoderName;
}

void
Settings::SetEncoderName(const char* value)
{
	PRINT(("Settings::SetEncoderName(const char*)\n"));

	if(value && !encoding)
	{
		strcpy(encoderName,value);
	}
}

int32*
Settings::ColumnDisplayOrder()
{
	PRINT(("Settings::ColumnDisplayOrder()\n"));

	return columnDisplayOrder;
}

void
Settings::SetColumnDisplayOrder(const int32* value)
{
	PRINT(("Settings::SetColumnDisplayOrder(const int32*)\n"));

	if(!encoding)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			columnDisplayOrder[i] = value[i];
		}
	}
}

int32
Settings::NumberOfSortKeys()
{
	PRINT(("Settings::NumberOfSortKeys()\n"));

	return numberOfSortKeys;
}

void
Settings::SetNumberOfSortKeys(int32 value)
{
	PRINT(("Settings::SetNumberOfSortKeys(int32)\n"));

	if(!encoding)
	{
		numberOfSortKeys = value;
	}
}

int32*
Settings::SortKeys()
{
	PRINT(("Settings::SortKeys()\n"));

	return sortKeys;
}

void
Settings::SetSortKeys(const int32* value)
{
	PRINT(("Settings::SetSortKeys(const int32*)\n"));

	if(!encoding)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			sortKeys[i] = value[i];
		}
	}
}

CLVSortMode*
Settings::SortModes()
{
	PRINT(("Settings::SortModes()\n"));

	return sortModes;
}

void
Settings::SetSortModes(const CLVSortMode* value)
{
	PRINT(("Settings::SetSortModes(const CLVSortMode*)\n"));

	if(!encoding)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			sortModes[i] = value[i];
		}
	}
}

bool*
Settings::ColumnsShown()
{
	PRINT(("Settings::ColumnsShown()\n"));

	return columnsShown;
}

void
Settings::SetColumnsShown(const bool* value)
{
	PRINT(("Settings::SetColumnsShown(const bool*)\n"));

	if(!encoding)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			columnsShown[i] = value[i];
		}
	}
}

float*
Settings::ColumnWidths()
{
	PRINT(("Settings::ColumnWidths()\n"));

	return columnWidths;
}

void
Settings::SetColumnWidths(const float* value)
{
	PRINT(("Settings::SetColumnWidths(const float*)\n"));

	if(!encoding)
	{
		for(int i=0;i<NUM_OF_COLUMNS;i++)
		{
			columnWidths[i] = value[i];
		}
	}
}

const char*
Settings::AddOnsDirectory()
{
	PRINT(("Settings::AddOnsDirectory()\n"));

	return addonDirectory;
}

void
Settings::PrintToStream()
{
	PRINT(("\n"));
	PRINT(("\tENCODER NAME = %s\n",encoderName));
	PRINT(("\tADD_ON_DIR = %s\n",addonDirectory));
	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		PRINT(("\tCOLUMN_DISPLAY_ORDER[%d] = %d\n",i,columnDisplayOrder[i]));
	}
	PRINT(("\tNUMBER_OF_SORT_KEYS = %d\n",numberOfSortKeys));
	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		PRINT(("\tSORT_KEYS[%d] = %d\n",i,sortKeys[i]));
	}
	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		PRINT(("\tSORT_MODES[%d] = %d\n",i,sortModes[i]));
	}
}
