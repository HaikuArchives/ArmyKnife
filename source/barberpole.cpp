// license: public domain
// authors: jonas.sundstrom@kirilla.com


#include "barberpole.h"


Barberpole::Barberpole(const char* name, uint32 flags)
	:
	BBox(name, flags),
	fIsRunning(false),
	fBitmap(NULL)
{
	fPattern.data[0] = 0x0f;
	fPattern.data[1] = 0x1e;
	fPattern.data[2] = 0x3c;
	fPattern.data[3] = 0x78;
	fPattern.data[4] = 0xf0;
	fPattern.data[5] = 0xe1;
	fPattern.data[6] = 0xc3;
	fPattern.data[7] = 0x87;
	
	_CreateBitmap();
	
	SetFont(be_plain_font);
};


Barberpole::~Barberpole()
{
	delete fBitmap;
}


void 
Barberpole::Start()
{
	fIsRunning = true;
	Window()->SetPulseRate(100000);
	SetFlags(Flags() | B_PULSE_NEEDED);
	SetViewColor(B_TRANSPARENT_COLOR);
	Invalidate();
}


void 
Barberpole::Pause()
{
	Window()->SetPulseRate(500000);
	SetFlags(Flags() & (~B_PULSE_NEEDED));
	Invalidate();
}


void 
Barberpole::Stop()
{
	fIsRunning = false;
	Window()->SetPulseRate(500000);
	SetFlags(Flags() & (~B_PULSE_NEEDED));
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	Invalidate();
}


bool 
Barberpole::IsRunning()	
{
	return fIsRunning;
}


void
Barberpole::Pulse()
{
	uchar tmp = fPattern.data[7];
	
	for (int j = 7;  j > 0;  --j)
		fPattern.data[j] = fPattern.data[j-1];
	
	fPattern.data[0] = tmp;
	
	Invalidate();
}


void
Barberpole::Draw(BRect rect)
{
	if (IsRunning())
	{
		_DrawOnBitmap();
		SetDrawingMode(B_OP_COPY);
		DrawBitmap(fBitmap);
	}
	else
	{
		BBox::Draw(rect);

		BFont font;
		GetFont(&font);

		float string_width = StringWidth("idle");
		float string_height = font.Size();

		BRect bounds = Bounds();
		float view_width = bounds.Width();
		float view_height = bounds.Height();
		float view_left = bounds.left;
		float view_bottom = bounds.bottom;

		DrawString("idle",
			BPoint(view_left + view_width/2 - string_width/2,
				view_bottom - view_height/2 + string_height/2));
	}
}


void
Barberpole::_DrawOnBitmap()
{
	if (fBitmap->Lock())
	{
		BRect rect = fBitmap->Bounds();

		fBitmapView->SetDrawingMode(B_OP_COPY);
	
		rgb_color  color;
		color.red = 0;
		color.green = 0;
		color.blue = 200;
		color.alpha = 255;
		fBitmapView->SetHighColor(color);

		// draw the pole
		rect.InsetBy(2,2);
		fBitmapView->FillRect(rect, fPattern);	
		
		// draw frame

		// left
		color.red = 150;
		color.green = 150;
		color.blue = 150;
		fBitmapView->SetHighColor(color);
		fBitmapView->SetDrawingMode(B_OP_OVER);
		BPoint point_a = fBitmap->Bounds().LeftTop();
		BPoint point_b = fBitmap->Bounds().LeftBottom();
		point_b.y -= 1;
		fBitmapView->StrokeLine(point_a, point_b);
		point_a.x += 1;
		point_b.x += 1;
		point_b.y -= 1;
		fBitmapView->StrokeLine(point_a, point_b);

		// top
		point_a = fBitmap->Bounds().LeftTop();
		point_b = fBitmap->Bounds().RightTop();
		point_b.x -= 1;
		fBitmapView->StrokeLine(point_a, point_b);
		point_a.y += 1;
		point_b.y += 1;
		point_b.x -= 1;
		fBitmapView->StrokeLine(point_a, point_b);

		// right
		color.red = 255;
		color.green = 255;
		color.blue = 255;
		fBitmapView->SetHighColor(color);
		point_a = fBitmap->Bounds().RightTop();
		point_b = fBitmap->Bounds().RightBottom();
		fBitmapView->StrokeLine(point_a, point_b);
		point_a.y += 1;
		point_a.x -= 1;
		point_b.x -= 1;
		fBitmapView->StrokeLine(point_a, point_b);

		// bottom
		point_a = fBitmap->Bounds().LeftBottom();
		point_b = fBitmap->Bounds().RightBottom();
		fBitmapView->StrokeLine(point_a, point_b);
		point_a.x += 1;
		point_a.y -= 1;
		point_b.y -= 1;
		fBitmapView->StrokeLine(point_a, point_b);		
		
		// some blending
		color.red = 150;
		color.green = 150;
		color.blue = 150;
		fBitmapView->SetHighColor(color);
		fBitmapView->SetDrawingMode(B_OP_SUBTRACT);
		fBitmapView->StrokeRect(rect);
	
		rect.InsetBy(1,1);
		_LightenBitmapHighColor(&color);
		fBitmapView->StrokeRect(rect);
		
		rect.InsetBy(1,1);
		_LightenBitmapHighColor(&color);
		fBitmapView->StrokeRect(rect);
		
		rect.InsetBy(1,1);
		_LightenBitmapHighColor(&color);
		fBitmapView->StrokeRect(rect);
		
		rect.InsetBy(1,1);
		_LightenBitmapHighColor(&color);
		fBitmapView->StrokeRect(rect);
		
		fBitmapView->Sync();
		fBitmap->Unlock();
	}
}


void
Barberpole::_LightenBitmapHighColor(rgb_color* color)
{
	color->red -= 30;
	color->green -= 30;
	color->blue -= 30;
	
	fBitmapView->SetHighColor(*color);
}


void
Barberpole::_CreateBitmap()
{
	BRect rect = Bounds();
	fBitmap = new BBitmap(rect, B_CMAP8, true);
	fBitmapView = new BView(rect, "buffer", B_FOLLOW_NONE, 0);
	fBitmap->AddChild(fBitmapView);
}


void
Barberpole::FrameMoved(BPoint point)
{
	Invalidate();
}


void
Barberpole::FrameResized(float width, float height)
{
	delete fBitmap;
	_CreateBitmap();
	Invalidate();
}
