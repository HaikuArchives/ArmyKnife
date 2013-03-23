#ifndef __ALBUM_PICTURE_VIEW_H__
#define __ALBUM_PICTURE_VIEW_H__

#include <TranslationUtils.h>
#include <be/interface/View.h>

#include <id3v2tag.h>
#include <mpegfile.h>
#include <id3v2frame.h>
#include <id3v2header.h>
#include <attachedpictureframe.h>

class BBitmap;
class BRect;
class BFont;
class BPath;

class AlbumPictureView : public BView
{
	public:
		AlbumPictureView(BRect frame, const char *name);
		~AlbumPictureView();
		void UpdatePicture(const char* path);
		void NoImage();
		void Draw(BRect rect);
		void SetPicture(const char* path);
		virtual void MessageReceived(BMessage *message);
	private:
		void Clear();
		bool ProcessRefs(BMessage *message);
		BBitmap *m_bitmap;
		bool m_attached_to_track;
};

#endif
