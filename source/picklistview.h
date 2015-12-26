#ifndef __PICK_LIST_VIEW_H__
#define __PICK_LIST_VIEW_H__

#include <be/interface/View.h>
#include <Box.h>

#define MENU_ITEM 'item'
#define VIEW_NOT_FOUND -888
#define NO_VIEW_SELECTED -999

class BArchivable;
class BBox;
class BMessage;
class BMenu;
class BRect;
class BCardLayout;

class PickListView : public BBox
{
	public:
		PickListView(const char* name = NULL,
				uint32 flags = B_WILL_DRAW | B_NAVIGABLE_JUMP,
				border_style border = B_FANCY_BORDER);
		PickListView(BMessage* archive);
		~PickListView();

		static BArchivable* Instantiate(BMessage* archive);

		virtual status_t Archive(BMessage* archive, bool deep=true)const;
		virtual void AttachedToWindow();
		virtual void MessageReceived(BMessage* message);

		virtual void AddView(BView* view);

		void RemoveView(BView* view);
		void RemoveView(int32 index);

		void SelectView(BView* view);
		void SelectView(int32 index);

		BView* SelectedView();
		int32 SelectedIndex();

		bool IsSelected(BView* view);
		bool IsSelected(int32 index);
		
		int32 CountViews();
		BView* ViewAt(int32 index);
		int32 IndexOf(BView* view);
	private:
		void MenuSelectionChanged(BMessage* message);

		BCardLayout*		m_view_layout;
		BView*				m_card_view;
		BBox*				m_view_box;
		BMenu*				m_view_menu;
		int32				m_selected_index;
};

#endif
