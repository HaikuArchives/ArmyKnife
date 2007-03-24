#ifndef __APP_VIEW_H__
#define __APP_VIEW_H__

#include <be/interface/View.h>

class BetterScrollView;
class BButton;
class BMessage;
class BRect;
class BStatusBar;
class BStringView;
class BTextControl;
class CLVRefListItem;
class EditorView;
class EncoderListView;
class FileNamePatternView;

class AppView : public BView
{
	public:
		AppView(BRect frame);
		~AppView();
		virtual void AttachedToWindow();
		virtual void MessageReceived(BMessage* message);
		virtual void RefsReceived(BMessage* message);
	private:
		void InitView();
		void InitializeColumn(CLVRefListItem* item);
		void SetSaveAsColumn(CLVRefListItem* item);
		void ApplyAttributeChanges(BMessage* message);
		void RemoveNodeFromList(node_ref* ref);
		void RemoveDeviceItemsFromList(int32 device);
		int32 UpdateItem(BMessage* message);
		static int32 RefsRecievedWrapper(void* args);
		static int32 RemoveItemsFromList(void* args);
		void Encode();
		void Cancel();
		void AlertUser(const char* message);
		static int32 EncodeThread(void* args);
		BetterScrollView* scrollView;
		EncoderListView* listView;
		FileNamePatternView* fileNamePatternView;
		EditorView* editorView;
		BButton* encodeButton;
		BButton* cancelButton;
		BStatusBar* statusBar;
		bool cancel;
};

#endif
