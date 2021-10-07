/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __ALBUM_PICTURE_VIEW_H__
#define __ALBUM_PICTURE_VIEW_H__

#include <TranslationUtils.h>
#include <View.h>

#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/attachedpictureframe.h>

class BBitmap;
class BRect;
class BFont;
class BPath;

class AlbumPictureView : public BView
{
	public:
		AlbumPictureView(const char *name);
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
