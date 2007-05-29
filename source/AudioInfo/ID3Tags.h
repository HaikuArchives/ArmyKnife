#ifndef __ID3_TAGS_H__
#define __ID3_TAGS_H__

#include <stdio.h>

#include "AudioInfo.h"
#include "ID3Tag.h"

class ID3Tags : public AudioInfo
{
	public:
		ID3Tags(const char* filename);
		virtual ~ID3Tags();

		virtual bool HasID3Tags();
		virtual int CreateID3Tags();

		virtual const char* Artist();
		virtual const char* Album();
		virtual const char* Title();
		virtual const char* Year();
		virtual const char* Comment();
		virtual const char* Track();
		virtual const char* Genre();

		virtual void SetArtist(const char* value);
		virtual void SetAlbum(const char* value);
		virtual void SetTitle(const char* value);
		virtual void SetYear(const char* value);
		virtual void SetComment(const char* value);
		virtual void SetTrack(const char* value);
		virtual void SetGenre(const char* value);

		virtual status_t ReadArtist();
		virtual status_t ReadAlbum();
		virtual status_t ReadTitle();
		virtual status_t ReadYear();
		virtual status_t ReadComment();
		virtual status_t ReadTrack();
		virtual status_t ReadGenre();

		virtual status_t Write();
		virtual status_t WriteArtist();
		virtual status_t WriteAlbum();
		virtual status_t WriteTitle();
		virtual status_t WriteYear();
		virtual status_t WriteComment();
		virtual status_t WriteTrack();
		virtual status_t WriteGenre();

	protected:
		TagLib::FileRef	*	file;
		TagLib::Tag		*	tag;

		ID3Tag* artist;
		ID3Tag* album;
		ID3Tag* title;
		ID3Tag* year;
		ID3Tag* comment;
		ID3Tag* track;
		ID3Tag* genre;

	private:
		bool	write_on_exit;
};

#endif

