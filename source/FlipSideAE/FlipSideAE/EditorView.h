#ifndef __EDITOR_VIEW_H__
#define __EDITOR_VIEW_H__

#include <be/interface/Box.h>

class BButton;
class BCheckBox;
class BList;
class BMenuField;
class BMessage;
class BRadioButton;
class BRect;
class BTextControl;
class CLVRefListItem;

class EditorView : public BBox
{
	public:
		EditorView(BRect frame);
		~EditorView();
		virtual void AttachedToWindow();
		virtual void GetPreferredSize(float* width, float* height);
		virtual void MessageReceived(BMessage* message);
		virtual void MakeFocus(bool focused=true);
		void SetEnabled(bool value);
	private:
		void InitView();
		void Apply();
		void GenreSelectionAction();
		void SetEnabled(BCheckBox* checkbox, BControl* control);
		void SetEnabled(BCheckBox* checkbox, BMenuField* menufield);
		void ListSelectionChanged(BMessage* message);
		void EnableCheckBoxes(bool value);
		void SetControlValues(CLVRefListItem* item);
		BCheckBox* artistCheckBox;
		BCheckBox* albumCheckBox;
		BCheckBox* titleCheckBox;
		BCheckBox* yearCheckBox;
		BCheckBox* commentCheckBox;
		BCheckBox* trackCheckBox;
		BCheckBox* genreCheckBox;
		BTextControl* artistTextControl;
		BTextControl* albumTextControl;
		BTextControl* titleTextControl;
		BTextControl* yearTextControl;
		BTextControl* commentTextControl;
		BTextControl* trackTextControl;
		BTextControl* genreTextControl;
		BMenuField* genreMenuField;
		BBox* genreBox;
		BButton* applyButton;
		int32 numSelected;
		int32* selectedIndexes;
};

#endif
