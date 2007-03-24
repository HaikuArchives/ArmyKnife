#ifndef __CLV_REF_LIST_ITEM_H__
#define __CLV_REF_LIST_ITEM_H__

#include "CLVEasyItem.h"
#include <be/storage/Entry.h>
#include <be/storage/Node.h>

class CLVRefListItem : public CLVEasyItem
{
	public:
		CLVRefListItem(entry_ref* entryRef, node_ref* nodeRef = 0,
				uint32 level = 0, bool superitem = false,
				bool expanded = false, float minheight = 0.0);
		~CLVRefListItem();
		entry_ref* EntryRef();
		node_ref* NodeRef();
		void SetEntryRef(entry_ref* entryRef);
		void SetNodeRef(node_ref* nodeRef);
	private:
		entry_ref* entry;
		node_ref* node;
};

#endif

