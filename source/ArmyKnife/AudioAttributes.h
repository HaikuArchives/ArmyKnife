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
		virtual const char* TTInfo();
		virtual const char* Gender();
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
		virtual void SetTTInfo(const char* value);
		virtual void SetGender(const char* value);
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
		virtual status_t ReadTTInfo();
		virtual status_t ReadGender();
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
		virtual status_t WriteTTInfo();
		virtual status_t WriteGender();
#endif
		virtual status_t WriteGenre();

	protected:
		status_t InitAttribute(AudioAttribute* attrib);

		BFile*				m_file;
		AudioAttribute*		m_tag;
		AudioAttribute*		m_artist;
		AudioAttribute*		m_album;
		AudioAttribute*		m_title;
		AudioAttribute*		m_year;
		AudioAttribute*		m_comment;
		AudioAttribute*		m_blank;
		AudioAttribute*		m_track;
#ifdef _TTE_
		AudioAttribute*		m_rating;
		AudioAttribute*		m_tempo;
		AudioAttribute*		m_composer;
		AudioAttribute*		m_tt_info;
		AudioAttribute*		m_gender;
#endif
		AudioAttribute*		m_genre;
};

#endif

