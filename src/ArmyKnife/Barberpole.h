#ifndef __BARBERPOLE_H__
#define __BARBERPOLE_H__

#include <stdlib.h>

#include <Window.h>
#include <View.h>
#include <Box.h>
#include <Bitmap.h>

class Barberpole : public BBox 
{
	public:
		Barberpole  (BRect a_rect, const char * a_name, uint32 a_resizing_mode, 
					uint32 a_flags);
		~Barberpole	();

					void	Start		();
					void	Pause		();
					void	Stop		();
					bool	IsRunning	();
			virtual void 	Pulse		();
			virtual void 	Draw		(BRect a_draw);
			virtual void	FrameMoved 	(BPoint a_point);
			virtual void	FrameResized (float a_width, float a_height);
				
	protected:
					void	CreateBitmap			(void);
					void	LightenBitmapHighColor	(rgb_color * a_color);
					void	DrawIntoBitmap			(void);

		bool		m_is_running;
		pattern		m_pattern;
		
		BBitmap	*	m_barberpole_bitmap;
		BView	*	m_barberpole_bitmap_view;

	private:

};

#endif
