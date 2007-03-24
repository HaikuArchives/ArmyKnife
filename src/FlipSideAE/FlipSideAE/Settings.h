#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include <be/support/Archivable.h>
#include <Santa/ColumnListView.h>
#include "CommandConstants.h"

class AEEncoder;
class BMessage;

class Settings : public BArchivable
{
	public:
		Settings();
		Settings(BMessage* archive);
		~Settings();

		virtual status_t Archive(BMessage* archive,bool deep=true) const;
		virtual BArchivable* Instantiate(BMessage* archive);

		static void OpenSettings();
		void SaveSettings();

		bool IsEncoding();
		void SetEncoding(bool value);

		AEEncoder* Encoder();
		void SetEncoder(AEEncoder* value);

		const char* EncoderName();
		void SetEncoderName(const char* value);

		int32* ColumnDisplayOrder();
		void SetColumnDisplayOrder(const int32* value);

		int32 NumberOfSortKeys();
		void SetNumberOfSortKeys(int32 value);

		int32* SortKeys();
		void SetSortKeys(const int32* value);

		CLVSortMode* SortModes();
		void SetSortModes(const CLVSortMode* value);

		bool* ColumnsShown();
		void SetColumnsShown(const bool* value);

		float* ColumnWidths();
		void SetColumnWidths(const float* value);

		const char* AddOnsDirectory();

		void PrintToStream();
	private:
		bool encoding;
		AEEncoder* encoder;
		char encoderName[B_FILE_NAME_LENGTH];
		char addonDirectory[B_PATH_NAME_LENGTH];
		int32 columnDisplayOrder[NUM_OF_COLUMNS];
		int32 numberOfSortKeys;
		int32 sortKeys[NUM_OF_COLUMNS];
		CLVSortMode sortModes[NUM_OF_COLUMNS];
		bool columnsShown[NUM_OF_COLUMNS];
		float columnWidths[NUM_OF_COLUMNS];

};

extern Settings* settings;

#endif
