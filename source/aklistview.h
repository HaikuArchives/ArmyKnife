/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __AKLISTVIEW_H__
#define __AKLISTVIEW_H__

#include <ListView.h>

class AKListView : public BListView
{
	public:
		AKListView  (const char* name,
			list_view_type type = B_SINGLE_SELECTION_LIST,
			uint32 flags = B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE);
		~AKListView	();

			virtual void	KeyDown(const char* bytes, int32 numBytes);
			virtual void	Draw(BRect rect);

			virtual void	SelectAll();
			virtual void	DeselectAll();
			virtual void	SelectAllUnsupported();
			virtual void	ScrollToFirstUnaccepted();
					bool	HasSelectionOfOnlyAcceptedItems();
					void	NoItem();
};

#endif
