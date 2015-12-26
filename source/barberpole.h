#ifndef _BARBERPOLE_H_
#define _BARBERPOLE_H_

#include <stdlib.h>

#include <Box.h>
#include <Bitmap.h>
#include <View.h>
#include <Window.h>


class Barberpole : public BBox
{
public:
							Barberpole(const char* name,
								uint32 flags);
							~Barberpole();

			void			Start();
			void			Pause();
			void			Stop();
			bool			IsRunning();
	virtual	void			Pulse();
	virtual	void			Draw(BRect draw);
	virtual void			FrameMoved(BPoint point);
	virtual	void			FrameResized(float width, float height);

private:
			void			_CreateBitmap();
			void			_LightenBitmapHighColor(rgb_color* color);
			void			_DrawOnBitmap();

			bool			fIsRunning;
			pattern			fPattern;
			BBitmap*		fBitmap;
			BView*			fBitmapView;
};

#endif	// _BARBERPOLE_H_
