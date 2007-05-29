#include <be/app/Message.h>
#include <be/app/Messenger.h>
#include <be/interface/Button.h>
#include <be/interface/Rect.h>
#include <be/interface/StringView.h>
#include <be/interface/TextControl.h>
#include <be/support/Debug.h>
#include <be/support/String.h>
#include <AEEncoder/AEEncoder.h>
#include "AppDefs.h"
#include "CommandConstants.h"
#include "FileNamePatternView.h"
#include "GUIStrings.h"
#include "Settings.h"

FileNamePatternView::FileNamePatternView(BRect frame) : BBox(frame)
{
	PRINT(("FileNamePatternView::FileNamePatternView(BRect)\n"));
}

FileNamePatternView::~FileNamePatternView()
{
	PRINT(("FileNamePatternView::~FileNamePatternView()\n"));
}

void
FileNamePatternView::InitView()
{
	PRINT(("FileNamePatternView::InitView()\n"));

	SetLabel(FILE_NAME_PATTERN_BOX_LABEL);

	int space = 6;

	font_height fh;
	GetFontHeight(&fh);
	float height = fh.ascent + fh.descent + fh.leading;

	char* left[] = {ARTIST_LABEL,YEAR_LABEL,0};
	char* center[] = {ALBUM_LABEL,TRACK_LABEL,0};
	char* right[] = {TITLE_LABEL,GENRE_LABEL,0};
	float leftWidth = 0;
	float centerWidth = 0;
	float rightWidth = 0;

	int i=0;
	while(left[i])
	{
		if(StringWidth(left[i]) > leftWidth)
		{
			leftWidth = StringWidth(left[i]);
		}
		i++;
	}

	i=0;
	while(center[i])
	{
		if(StringWidth(center[i]) > centerWidth)
		{
			centerWidth = StringWidth(center[i]);
		}
		i++;
	}

	i=0;
	while(right[i])
	{
		if(StringWidth(right[i]) > rightWidth)
		{
			rightWidth = StringWidth(right[i]);
		}
		i++;
	}

	BRect leftFrame = Bounds();
	leftFrame.InsetBy(space,2*space);
	leftFrame.right = leftFrame.left + leftWidth;
	leftFrame.bottom = leftFrame.top + height;
	BRect centerFrame = leftFrame;
	centerFrame.left = leftFrame.right + space;
	centerFrame.right = centerFrame.left + centerWidth;
	BRect rightFrame = leftFrame;
	rightFrame.left = centerFrame.right + space;
	rightFrame.right = rightFrame.left + rightWidth;

	artistStringView = new BStringView(leftFrame,"artist",ARTIST_LABEL);

	leftFrame.OffsetBy(0,height+space);
	yearStringView = new BStringView(leftFrame,"year",YEAR_LABEL);

	leftFrame.OffsetBy(0,height+space);
	leftFrame.right = leftFrame.left + StringWidth(COMMENT_LABEL);
	commentStringView = new BStringView(leftFrame,"comment",COMMENT_LABEL);

	albumStringView = new BStringView(centerFrame,"album",ALBUM_LABEL);

	centerFrame.OffsetBy(0,height+space);
	trackStringView = new BStringView(centerFrame,"track",TRACK_LABEL);

	titleStringView = new BStringView(rightFrame,"title",TITLE_LABEL);

	rightFrame.OffsetBy(0,height+space);
	genreStringView = new BStringView(rightFrame,"genre",GENRE_LABEL);

	BRect tcFrame = Bounds();
	tcFrame.InsetBy(space,space);
	tcFrame.top = leftFrame.bottom + space;
	tcFrame.bottom = tcFrame.top + height;
	AEEncoder* encoder = settings->Encoder();
	BString str;
	if(encoder)
	{
		str = encoder->GetPattern();
	}
	fileNamePatternTextControl = new BTextControl(tcFrame,
			"fileNamePatternTextControl",0,str.String(),0,B_FOLLOW_LEFT_RIGHT);

	BRect buttonFrame = Bounds();
	buttonFrame.InsetBy(space,space);
	buttonFrame.left = buttonFrame.right - 60;
	buttonFrame.top = buttonFrame.bottom - 25;

	applyButton = new BButton(buttonFrame,"applyButton",APPLY_BTN,
			new BMessage(FILE_NAME_PATTERN_CHANGED),
			B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);

	AddChild(artistStringView);
	AddChild(albumStringView);
	AddChild(titleStringView);
	AddChild(yearStringView);
	AddChild(trackStringView);
	AddChild(genreStringView);
	AddChild(commentStringView);
	AddChild(fileNamePatternTextControl);
	AddChild(applyButton);

	ResizeToPreferred();
}

void
FileNamePatternView::MakeFocus(bool focused=true)
{
	PRINT(("FileNamePatternView::MakeFocused(bool)\n"));

	fileNamePatternTextControl->MakeFocus(focused);
}

void
FileNamePatternView::SetEnabled(bool value)
{
	PRINT(("FileNamePatternView::SetEnabled(bool)\n"));

	fileNamePatternTextControl->SetEnabled(value);
	applyButton->SetEnabled(value);
}

void
FileNamePatternView::AttachedToWindow()
{
	PRINT(("FileNamePatternView::AttachedToWindow()\n"));

	InitView();

	fileNamePatternTextControl->SetTarget(this);
	applyButton->SetTarget(this);
}

void
FileNamePatternView::GetPreferredSize(float* width, float* height)
{
	PRINT(("FileNamePatternView::GetPreferredSize(float*,float*)\n"));

	int space = 6;

	*width = genreStringView->Frame().right + space;
	*height = fileNamePatternTextControl->Frame().bottom + 2*space +
		applyButton->Frame().Height();
}

void
FileNamePatternView::MessageReceived(BMessage* message)
{
	//PRINT(("FileNamePatternView::MessageReceived(BMessage*)\n"));

	BMessenger parentMessenger(Parent());

	switch(message->what)
	{
		case FILE_NAME_PATTERN_CHANGED:
			{
				AEEncoder* encoder = settings->Encoder();
				if(encoder)
				{
					encoder->SetPattern(fileNamePatternTextControl->Text());
					parentMessenger.SendMessage(message);
				}
			}
			break;
		case ENCODER_CHANGED:
			{
				AEEncoder* encoder = settings->Encoder();
				if(encoder)
				{
					fileNamePatternTextControl->SetText(encoder->GetPattern());
					BMessage msg(FILE_NAME_PATTERN_CHANGED);
					parentMessenger.SendMessage(&msg);
				}
			}
			break;
		default:
			BBox::MessageReceived(message);
	}
}
