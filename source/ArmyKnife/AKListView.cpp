#include "AKListView.h"
#include "EntryRefItem.h"

// AKListView is meant to hold EntryRefItems.
// EntryRefItem is a subclass of BListItem.
// 
// AKListView's primary reason to exist is as a subclass of
// BListView which allows deletion of list items by pressing
// Delete or Backspace.

AKListView::AKListView (BRect frame, const char* name, list_view_type type,
	uint32 resizingMode, uint32 flags)
:	BListView (frame, name, type, resizingMode, flags)
{

}


AKListView::~AKListView()
{

}


void 
AKListView::KeyDown(const char* bytes, int32 numBytes)
{
	uint8 byte = bytes[0];
	
	switch(byte)
	{
		case B_BACKSPACE:
			// fallthrough
		case B_DELETE:
			{
				while (CurrentSelection() >= 0) {
					BListItem* item = ItemAt(CurrentSelection(0));
					if (item) {
						RemoveItem(item);
					} else {
						break;
					}
					
					EntryRefItem* refItem = dynamic_cast<EntryRefItem*>(item);
					if (refItem) {
						delete refItem;
					} else {
						delete item;
					}
				}
			}
			break;
			
		default:
			BListView::KeyDown(bytes, numBytes);
	}
}

