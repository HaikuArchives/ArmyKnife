#ifndef __ENCODER_LIST_VIEW_H__
#define __ENCODER_LIST_VIEW_H__

#include <Santa/ColumnListView.h>

class BBitmap;
class BMessage;
class BRect;
class BetterScrollView;

class EncoderListView : public ColumnListView
{
	public:
		EncoderListView(BRect frame, BetterScrollView** container);
		~EncoderListView();
		virtual void AttachedToWindow();
		virtual void MessageReceived(BMessage* message);
		const BBitmap* GetCheckMark();
	private:
		void InitView();
		BBitmap* checkMark;
};

int sort_function(const CLVListItem* item1, const CLVListItem* item2,
		int32 sort_key);

#endif
