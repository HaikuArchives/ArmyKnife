#include <be/app/Message.h>
#include <be/interface/ListView.h>
#include <be/interface/Rect.h>
#include <be/support/Debug.h>
#include <Santa/BetterScrollView.h>
#include <Santa/CLVRefListItem.h>
#include "AppDefs.h"
#include "CheckMark.h"
#include "CommandConstants.h"
#include "EncoderListView.h"
#include "GUIStrings.h"
#include "Settings.h"

EncoderListView::EncoderListView(BRect frame, BetterScrollView** container) :
	ColumnListView(frame,(CLVContainerView**)container,"listView",B_FOLLOW_ALL,
		B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE|B_NAVIGABLE_JUMP,
		B_MULTIPLE_SELECTION_LIST,false,true,true,true,B_FANCY_BORDER)
{
	PRINT(("EncoderListView::EncoderListView(BRect)\n"));

	InitView();
}

EncoderListView::~EncoderListView()
{
	PRINT(("EncoderListView::~EncoderListView()\n"));

	delete checkMark;

	int32 columnDisplayOrder[NUM_OF_COLUMNS];
	int32 numOfSortKeys;
	int32 sortKeys[NUM_OF_COLUMNS];
	CLVSortMode sortModes[NUM_OF_COLUMNS];

	GetDisplayOrder(columnDisplayOrder);
	numOfSortKeys = GetSorting(sortKeys,sortModes);

	settings->SetColumnDisplayOrder(columnDisplayOrder);
	settings->SetNumberOfSortKeys(numOfSortKeys);
	settings->SetSortKeys(sortKeys);
	settings->SetSortModes(sortModes);

	bool columnsShown[NUM_OF_COLUMNS];
	float columnWidths[NUM_OF_COLUMNS];
	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		CLVColumn* column = ColumnAt(i);
		columnsShown[i] = column->IsShown();
		columnWidths[i] = column->Width();
	}
	settings->SetColumnsShown(columnsShown);
	settings->SetColumnWidths(columnWidths);
}

void
EncoderListView::InitView()
{
	PRINT(("EncoderListView::InitView()\n"));

	checkMark = new CheckMark();

	float minWidth;
	uint32 flags = CLV_HEADER_TRUNCATE | CLV_SORT_KEYABLE | CLV_TELL_ITEMS_WIDTH;
	
	AddColumn(new CLVColumn("",16,CLV_NOT_MOVABLE | CLV_NOT_RESIZABLE |
				CLV_LOCK_AT_BEGINNING,16));

	minWidth = StringWidth(FILE_COLUMN) + 20;
	AddColumn(new CLVColumn(FILE_COLUMN,minWidth,flags,minWidth));

	minWidth = StringWidth(SAVE_AS_COLUMN) + 20;
	AddColumn(new CLVColumn(SAVE_AS_COLUMN,minWidth,flags,minWidth));

	minWidth = StringWidth(ARTIST_COLUMN) + 20;
	AddColumn(new CLVColumn(ARTIST_COLUMN,minWidth,flags,minWidth));

	minWidth = StringWidth(ALBUM_COLUMN) + 20;
	AddColumn(new CLVColumn(ALBUM_COLUMN,minWidth,flags,minWidth));

	minWidth = StringWidth(TITLE_COLUMN) + 20;
	AddColumn(new CLVColumn(TITLE_COLUMN,minWidth,flags,minWidth));

	minWidth = StringWidth(YEAR_COLUMN) + 20;
	AddColumn(new CLVColumn(YEAR_COLUMN,minWidth,flags,minWidth));

	minWidth = StringWidth(COMMENT_COLUMN) + 20;
	AddColumn(new CLVColumn(COMMENT_COLUMN,minWidth,flags,minWidth));

	minWidth = StringWidth(TRACK_COLUMN) + 20;
	AddColumn(new CLVColumn(TRACK_COLUMN,minWidth,flags,minWidth));

	minWidth = StringWidth(GENRE_COLUMN) + 20;
	AddColumn(new CLVColumn(GENRE_COLUMN,minWidth,flags,minWidth));

	SetDisplayOrder(settings->ColumnDisplayOrder());

	SetSorting(settings->NumberOfSortKeys(),settings->SortKeys(),
			settings->SortModes());

	bool* columnsShown = settings->ColumnsShown();
	float* columnWidths = settings->ColumnWidths();
	for(int i=0;i<NUM_OF_COLUMNS;i++)
	{
		CLVColumn* column = ColumnAt(i);
		column->SetShown(columnsShown[i]);
		if(columnWidths[i] > 0.0)
		{
			column->SetWidth(columnWidths[i]);
		}
	}

	SetSortFunction(sort_function);
}

void
EncoderListView::AttachedToWindow()
{
	PRINT(("EncoderListView::AttachedToWindow()\n"));
}

void
EncoderListView::MessageReceived(BMessage* message)
{
	//PRINT(("EncoderListView::MessageReceived(BMessage*)\n"));

	switch(message->what)
	{
		default:
			ColumnListView::MessageReceived(message);
	}
}

const BBitmap*
EncoderListView::GetCheckMark()
{
	return checkMark;
}

int
sort_function(const CLVListItem* item1, const CLVListItem* item2,
		int32 sort_key)
{
	int result = -1;

	const char* item1Text =
		((CLVRefListItem*)item1)->GetColumnContentText(sort_key);

	const char* item2Text =
		((CLVRefListItem*)item2)->GetColumnContentText(sort_key);

	if(item1Text && item2Text)
	{
		result = strcmp(item1Text,item2Text);
	}
	else if(!item1Text && !item2Text)
	{
		result = 0;
	}
	else if(!item1Text)
	{
		result = -1;
	}
	else if(!item2Text)
	{
		result = 1;
	}

	return(result);
}
