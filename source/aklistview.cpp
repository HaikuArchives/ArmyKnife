#include "aklistview.h"
#include "entryrefitem.h"

// AKListView is meant to hold EntryRefItems.
// EntryRefItem is a subclass of BListItem.
//
// AKListView's primary reason to exist is as a subclass of
// BListView which allows deletion of list items by pressing
// Delete or Backspace.

AKListView::AKListView(const char* name, list_view_type type, uint32 flags)
:	BListView (name, type, flags)
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
				Invoke();
			}
			break;

		default:
			BListView::KeyDown(bytes, numBytes);
	}
}

void
AKListView::SelectAll()
{
	Select(0,CountItems()-1);
}

void
AKListView::DeselectAll()
{
	BListView::DeselectAll();
}

void
AKListView::ScrollToFirstUnaccepted()
{
	int numItems = CountItems();

	for (int i = 0; i < numItems; i++) {

		BListItem* item = ItemAt(i);
		if (!item)
			return;

		EntryRefItem* refItem = dynamic_cast<EntryRefItem*>(item);
		if (!refItem)
			return;

		if (!refItem->IsAccepted()) {
			BPoint point = ItemFrame(i).LeftTop();
			if (i > 0);
			point.y -= item->Height();
			ScrollTo(point);
			break;
		}
	}
}

bool
AKListView::HasSelectionOfOnlyAcceptedItems()
{
	int32 i = 0;
	int32 selection_index = 0;
	int32 selection_count = 0;
	BListItem* item;
	EntryRefItem* refItem;

	while ((selection_index = CurrentSelection(i)) >= 0) {

		BListItem* item = ItemAt(selection_index);
		if (!item)
			return false;

		EntryRefItem* refItem = dynamic_cast<EntryRefItem*>(item);
		if (!refItem)
			return false;
		if (!refItem->IsAccepted()) {
			return false;
		}

		selection_count++;
		i++;
	}

	if (selection_count > 0)
		return true;
	else
		return false;
}

void
AKListView::SelectAllUnsupported()
{
	DeselectAll();

	BListItem* item;
	EntryRefItem* refItem;

	for (int32 i = 0; item = ItemAt(i); i++) {

		EntryRefItem* refItem = dynamic_cast<EntryRefItem*>(item);
		if (refItem == NULL)
			continue;

		if(!refItem->IsAccepted())
			Select(i, true);
	}
}
