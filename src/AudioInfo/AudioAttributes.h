#ifndef __AUDIO_ATTRIBUTES_H__
#define __AUDIO_ATTRIBUTES_H__

#include "AudioInfo.h"

class BFile;
class AudioAttribute;

class AudioAttributes : public AudioInfo
{
	public:
		AudioAttributes(BFile* file);
		virtual ~AudioAttributes();

		virtual const char* Artist();
		virtual const char* Album();
		virtual const char* Title();
		virtual const char* Year();
		virtual const char* Comment();
		virtual const char* Track();
#ifdef _TTE_
		virtual const char* Rating();
		virtual const char* Tempo();
		virtual const char* Composer();
#endif
		virtual const char* Genre();

		virtual void SetArtist(const char* value);
		virtual void SetAlbum(const char* value);
		virtual void SetTitle(const char* value);
		virtual void SetYear(const char* value);
		virtual void SetComment(const char* value);
		virtual void SetTrack(const char* value);
#ifdef _TTE_
		virtual void SetRating(const char* value);
		virtual void SetTempo(const char* value);
		virtual void SetComposer(const char* value);
#endif
		virtual void SetGenre(const char* value);

		virtual status_t Read();
		virtual status_t ReadArtist();
		virtual status_t ReadAlbum();
		virtual status_t ReadTitle();
		virtual status_t ReadYear();
		virtual status_t ReadComment();
		virtual status_t ReadTrack();
#ifdef _TTE_
		virtual status_t ReadRating();
		virtual status_t ReadTempo();
		virtual status_t ReadComposer();
#endif
		virtual status_t ReadGenre();

		virtual status_t Write();
		virtual status_t WriteArtist();
		virtual status_t WriteAlbum();
		virtual status_t WriteTitle();
		virtual status_t WriteYear();
		virtual status_t WriteComment();
		virtual status_t WriteTrack();
#ifdef _TTE_
		virtual status_t WriteRating();
		virtual status_t WriteTempo();
		virtual status_t WriteComposer();
#endif
		virtual status_t WriteGenre();

	protected:
		status_t InitAttribute(AudioAttribute* attrib);

		BFile* file;
		AudioAttribute* tag;
		AudioAttribute* artist;
		AudioAttribute* album;
		AudioAttribute* title;
		AudioAttribute* year;
		AudioAttribute* comment;
		AudioAttribute* blank;
		AudioAttribute* track;
#ifdef _TTE_
		AudioAttribute* rating;
		AudioAttribute* tempo;
		AudioAttribute* composer;
#endif
		AudioAttribute* genre;
};

#endif

