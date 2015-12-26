#include <stdlib.h>
#include <string.h>
#include <be/storage/File.h>
#include <be/support/Debug.h>
#include <be/support/TypeConstants.h>
#include "audioattributes.h"
#include "audioattribute.h"

#define ARTIST_NAME    "Artist"
#define ALBUM_NAME     "Album"
#define TITLE_NAME     "Title"
#define YEAR_NAME      "Year"
#define COMMENT_NAME   "Comment"
#define TRACK_NAME     "Track"
#define GENRE_NAME     "Genre"

#ifdef _TTE_
#define RATING_NAME    "Rating"
#define TEMPO_NAME     "Tempo"
#define COMPOSER_NAME  "Composer"
#define TT_INFO_NAME   "Info"
#define GENDER_NAME    "Gender"
#endif

#ifdef __BEOS__
#define ARTIST_ATTR    "Audio:Artist"
#define ALBUM_ATTR     "Audio:Album"
#define TITLE_ATTR     "Audio:Title"
#define YEAR_ATTR      "Audio:Year"
#define COMMENT_ATTR   "Audio:Comment"
#define TRACK_ATTR     "Audio:Track"
#define GENRE_ATTR     "Audio:Genre"
#elif __HAIKU__
#define ARTIST_ATTR    "Audio:Artist"
#define ALBUM_ATTR     "Audio:Album"
#define TITLE_ATTR     "Media:Title"
#define YEAR_ATTR      "Media:Year"
#define COMMENT_ATTR   "Media:Comment"
#define TRACK_ATTR     "Audio:Track"
#define GENRE_ATTR     "Media:Genre"
#endif

#ifdef _TTE_
#ifdef __BEOS__
#define RATING_ATTR    "Audio:Rating"
#elif __HAIKU__
#define RATING_ATTR    "Media:Rating"
#endif
#define TEMPO_ATTR     "Audio:Tempo"
#define COMPOSER_ATTR  "Audio:Composer"
#define TT_INFO_ATTR   "Audio:Info"
#define GENDER_ATTR    "Audio:Gender"
#endif

AudioAttributes::AudioAttributes(BFile* file) : AudioInfo()
{
	PRINT(("AudioAttributes::AudioAttributes(BFile*)\n"));

	this->m_file = file;
	m_artist = new AudioAttribute(m_file, ARTIST_NAME, ARTIST_ATTR, B_STRING_TYPE);
	m_album = new AudioAttribute(m_file, ALBUM_NAME, ALBUM_ATTR, B_STRING_TYPE);
	m_title = new AudioAttribute(m_file, TITLE_NAME, TITLE_ATTR, B_STRING_TYPE);
	m_year = new AudioAttribute(m_file, YEAR_NAME, YEAR_ATTR, B_INT32_TYPE);
	m_comment = new AudioAttribute(m_file, COMMENT_NAME, COMMENT_ATTR, B_STRING_TYPE);
	m_track = new AudioAttribute(m_file, TRACK_NAME, TRACK_ATTR, B_INT32_TYPE);
#ifdef _TTE_
	m_rating = new AudioAttribute(m_file, RATING_NAME, RATING_ATTR, B_INT32_TYPE);
	m_tempo = new AudioAttribute(m_file, TEMPO_NAME, TEMPO_ATTR, B_STRING_TYPE);
	m_composer = new AudioAttribute(m_file, COMPOSER_NAME, COMPOSER_ATTR, B_STRING_TYPE);
	m_tt_info = new AudioAttribute(m_file, TT_INFO_NAME, TT_INFO_ATTR, B_STRING_TYPE);
	m_gender = new AudioAttribute(m_file, GENDER_NAME, GENDER_ATTR, B_STRING_TYPE);
#endif
	m_genre = new AudioAttribute(m_file, GENRE_NAME, GENRE_ATTR, B_STRING_TYPE);

	InitAttribute(m_artist);
	InitAttribute(m_album);
	InitAttribute(m_title);
	InitAttribute(m_year);
	InitAttribute(m_comment);
	InitAttribute(m_track);
#ifdef _TTE_
	InitAttribute(m_rating);
	InitAttribute(m_tempo);
	InitAttribute(m_composer);
	InitAttribute(m_tt_info);
	InitAttribute(m_gender);
#endif
	InitAttribute(m_genre);

	Read();
}

AudioAttributes::~AudioAttributes()
{
	PRINT(("AudioAttributes::~AudioAttributes()\n"));

	delete m_artist;
	delete m_album;
	delete m_title;
	delete m_year;
	delete m_comment;
	delete m_track;
#ifdef _TTE_
	delete m_rating;
	delete m_tempo;
	delete m_composer;
	delete m_tt_info;
	delete m_gender;
#endif
	delete m_genre;
}


status_t
AudioAttributes::InitAttribute(AudioAttribute* attrib)
{
	PRINT(("AudioAttributes::InitAttribute(AudioAttribute*)\n"));

	if(!attrib)
	{
		return B_ERROR;
	}

	if(!attrib->Exists())
	{
#ifndef __HAIKU__
		return attrib->Create();
#endif
	}

	return B_OK;
}

const char*
AudioAttributes::Artist()
{
	PRINT(("AudioAttributes::Artist()\n"));

	return m_artist->Value();
}

const char*
AudioAttributes::Album()
{
	PRINT(("AudioAttributes::Album()\n"));

	return m_album->Value();
}

const char*
AudioAttributes::Title()
{
	PRINT(("AudioAttributes::Title()\n"));

	return m_title->Value();
}

const char*
AudioAttributes::Year()
{
	PRINT(("AudioAttributes::Year()\n"));

	return m_year->Value();
}

const char*
AudioAttributes::Comment()
{
	PRINT(("AudioAttributes::Comment()\n"));

	return m_comment->Value();
}

const char*
AudioAttributes::Track()
{
	PRINT(("AudioAttributes::Track()\n"));

	return m_track->Value();
}

#ifdef _TTE_
const char*
AudioAttributes::Rating()
{
	PRINT(("AudioAttributes::Rating()\n"));

	return m_rating->Value();
}

const char*
AudioAttributes::Tempo()
{
	PRINT(("AudioAttributes::Tempo()\n"));

	return m_tempo->Value();
}

const char*
AudioAttributes::Composer()
{
	PRINT(("AudioAttributes::Composer()\n"));

	return m_composer->Value();
}

const char*
AudioAttributes::TTInfo()
{
	PRINT(("AudioAttributes::TTInfo()\n"));

	return m_tt_info->Value();
}

const char*
AudioAttributes::Gender()
{
	PRINT(("AudioAttributes::Gender()\n"));

	return m_gender->Value();
}
#endif

const char*
AudioAttributes::Genre()
{
	PRINT(("AudioAttributes::Genre()\n"));

	return m_genre->Value();
}

void
AudioAttributes::SetArtist(const char* value)
{
	PRINT(("AudioAttributes::SetArtist(const char*)\n"));

	m_artist->SetValue(value);
}

void
AudioAttributes::SetAlbum(const char* value)
{
	PRINT(("AudioAttributes::SetAlbum(const char*)\n"));

	m_album->SetValue(value);
}

void
AudioAttributes::SetTitle(const char* value)
{
	PRINT(("AudioAttributes::SetTitle(const char*)\n"));

	m_title->SetValue(value);
}

void
AudioAttributes::SetYear(const char* value)
{
	PRINT(("AudioAttributes::SetYear(const char*)\n"));

	m_year->SetValue(value);
}

void
AudioAttributes::SetComment(const char* value)
{
	PRINT(("AudioAttributes::SetComment(const char*)\n"));

	m_comment->SetValue(value);
}

void
AudioAttributes::SetTrack(const char* value)
{
	PRINT(("AudioAttributes::SetTrack(const char*)\n"));

	m_track->SetValue(value);
}

#ifdef _TTE_
void
AudioAttributes::SetRating(const char* value)
{
	PRINT(("AudioAttributes::SetRating(const char*)\n"));

	m_rating->SetValue(value);
}

void
AudioAttributes::SetTempo(const char* value)
{
	PRINT(("AudioAttributes::SetTempo(const char*)\n"));

	m_tempo->SetValue(value);
}

void
AudioAttributes::SetComposer(const char* value)
{
	PRINT(("AudioAttributes::SetComposer(const char*)\n"));

	m_composer->SetValue(value);
}

void
AudioAttributes::SetTTInfo(const char* value)
{
	PRINT(("AudioAttributes::SetTTInfo(const char*)\n"));

	m_tt_info->SetValue(value);
}

void
AudioAttributes::SetGender(const char* value)
{
	PRINT(("AudioAttributes::SetGender(const char*)\n"));

	m_gender->SetValue(value);
}
#endif

void
AudioAttributes::SetGenre(const char* value)
{
	PRINT(("AudioAttributes::SetGenre(const char*)\n"));

	m_genre->SetValue(value);
}

status_t
AudioAttributes::Read()
{
	PRINT(("AudioAttributes::Read()\n"));

	int status = B_OK;
	int result = AudioInfo::Read();

#ifdef _TTE_
	status = ReadRating();
	if(status != B_OK)
	{
		result = status;
	}

	status = ReadTempo();
	if(status != B_OK)
	{
		result = status;
	}

	status = ReadComposer();
	if(status != B_OK)
	{
		result = status;
	}

	status = ReadTTInfo();
	if(status != B_OK)
	{
		result = status;
	}

	status = ReadGender();
	if(status != B_OK)
	{
		result = status;
	}
#endif

	return result;
}

status_t
AudioAttributes::ReadArtist()
{
	PRINT(("AudioAttributes::ReadArtist()\n"));

	return m_artist->Read();
}

status_t
AudioAttributes::ReadAlbum()
{
	PRINT(("AudioAttributes::ReadAlbum()\n"));

	return m_album->Read();
}

status_t
AudioAttributes::ReadTitle()
{
	PRINT(("AudioAttributes::ReadTitle()\n"));

	return m_title->Read();
}

status_t
AudioAttributes::ReadYear()
{
	PRINT(("AudioAttributes::ReadYear()\n"));

	return m_year->Read();
}

status_t
AudioAttributes::ReadComment()
{
	PRINT(("AudioAttributes::ReadComment()\n"));

	return m_comment->Read();
}

status_t
AudioAttributes::ReadTrack()
{
	PRINT(("AudioAttributes::ReadTrack()\n"));

	int retval = m_track->Read();
	if(m_track->Value() && strlen(m_track->Value()) == 1)
	{
		BString tmp = "0";
		tmp += m_track->Value();
		m_track->SetValue(tmp.String());
	}
	return retval;
}

#ifdef _TTE_
status_t
AudioAttributes::ReadRating()
{
	PRINT(("AudioAttributes::ReadRating()\n"));

	return m_rating->Read();
}

status_t
AudioAttributes::ReadTempo()
{
	PRINT(("AudioAttributes::ReadTempo()\n"));

	return m_tempo->Read();
}

status_t
AudioAttributes::ReadComposer()
{
	PRINT(("AudioAttributes::ReadComposer()\n"));

	return m_composer->Read();
}

status_t
AudioAttributes::ReadTTInfo()
{
	PRINT(("AudioAttributes::ReadTTInfo()\n"));

	return m_tt_info->Read();
}

status_t
AudioAttributes::ReadGender()
{
	PRINT(("AudioAttributes::ReadGender()\n"));

	return m_gender->Read();
}
#endif

status_t
AudioAttributes::ReadGenre()
{
	PRINT(("AudioAttributes::ReadGenre()\n"));

	return m_genre->Read();
}

status_t
AudioAttributes::Write()
{
	PRINT(("AudioAttributes::Write()\n"));

	int status = B_OK;
	int result = AudioInfo::Write();

#ifdef _TTE_
	status = WriteRating();
	if(status != B_OK)
	{
		result = status;
	}

	status = WriteTempo();
	if(status != B_OK)
	{
		result = status;
	}

	status = WriteComposer();
	if(status != B_OK)
	{
		result = status;
	}

	status = WriteTTInfo();
	if(status != B_OK)
	{
		result = status;
	}

	status = WriteGender();
	if(status != B_OK)
	{
		result = status;
	}
#endif

	return result;
}

status_t
AudioAttributes::WriteArtist()
{
	PRINT(("AudioAttributes::WriteArtist()\n"));

	return m_artist->Write();
}

status_t
AudioAttributes::WriteAlbum()
{
	PRINT(("AudioAttributes::WriteAlbum()\n"));

	return m_album->Write();
}

status_t
AudioAttributes::WriteTitle()
{
	PRINT(("AudioAttributes::WriteTitle()\n"));

	return m_title->Write();
}

status_t
AudioAttributes::WriteYear()
{
	PRINT(("AudioAttributes::WriteYear()\n"));

	return m_year->Write();
}

status_t
AudioAttributes::WriteComment()
{
	PRINT(("AudioAttributes::WriteComment()\n"));

	return m_comment->Write();
}

status_t
AudioAttributes::WriteTrack()
{
	PRINT(("AudioAttributes::WriteTrack()\n"));

	return m_track->Write();
}

#ifdef _TTE_
status_t
AudioAttributes::WriteRating()
{
	PRINT(("AudioAttributes::WriteRating()\n"));

	return m_rating->Write();
}

status_t
AudioAttributes::WriteTempo()
{
	PRINT(("AudioAttributes::WriteTempo()\n"));

	return m_tempo->Write();
}

status_t
AudioAttributes::WriteComposer()
{
	PRINT(("AudioAttributes::WriteComposer()\n"));

	return m_composer->Write();
}

status_t
AudioAttributes::WriteTTInfo()
{
	PRINT(("AudioAttributes::WriteTTInfo()\n"));

	return m_tt_info->Write();
}

status_t
AudioAttributes::WriteGender()
{
	PRINT(("AudioAttributes::WriteGender()\n"));

	return m_gender->Write();
}
#endif

status_t
AudioAttributes::WriteGenre()
{
	PRINT(("AudioAttributes::WriteGenre()\n"));

	return m_genre->Write();
}

