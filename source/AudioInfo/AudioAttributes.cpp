#include <stdlib.h>
#include <string.h>
#include <be/storage/File.h>
#include <be/support/Debug.h>
#include <be/support/TypeConstants.h>
#include <String/String.h>
#include "AudioAttributes.h"
#include "AudioAttribute.h"

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
#endif

#define ARTIST_ATTR    "Audio:Artist"
#define ALBUM_ATTR     "Audio:Album"
#define TITLE_ATTR     "Audio:Title"
#define YEAR_ATTR      "Audio:Year"
#define COMMENT_ATTR   "Audio:Comment"
#define TRACK_ATTR     "Audio:Track"
#define GENRE_ATTR     "Audio:Genre"

#ifdef _TTE_
#define RATING_ATTR    "Audio:Rating"
#define TEMPO_ATTR     "Audio:Tempo"
#define COMPOSER_ATTR  "Audio:Composer"
#endif

AudioAttributes::AudioAttributes(BFile* file) : AudioInfo()
{
	PRINT(("AudioAttributes::AudioAttributes(BFile*)\n"));

	this->file = file;
	artist = new AudioAttribute(file,ARTIST_NAME,ARTIST_ATTR,B_STRING_TYPE);
	album = new AudioAttribute(file,ALBUM_NAME,ALBUM_ATTR,B_STRING_TYPE);
	title = new AudioAttribute(file,TITLE_NAME,TITLE_ATTR,B_STRING_TYPE);
	year = new AudioAttribute(file,YEAR_NAME,YEAR_ATTR,B_INT32_TYPE);
	comment = new AudioAttribute(file,COMMENT_NAME,COMMENT_ATTR,B_STRING_TYPE);
	track = new AudioAttribute(file,TRACK_NAME,TRACK_ATTR,B_INT32_TYPE);
#ifdef _TTE_
	rating = new AudioAttribute(file,RATING_NAME,RATING_ATTR,B_INT32_TYPE);
	tempo = new AudioAttribute(file,TEMPO_NAME,TEMPO_ATTR,B_STRING_TYPE);
	composer = new AudioAttribute(file,COMPOSER_NAME,COMPOSER_ATTR,
			B_STRING_TYPE);
#endif
	genre = new AudioAttribute(file,GENRE_NAME,GENRE_ATTR,B_STRING_TYPE);

	InitAttribute(artist);
	InitAttribute(album);
	InitAttribute(title);
	InitAttribute(year);
	InitAttribute(comment);
	InitAttribute(track);
#ifdef _TTE_
	InitAttribute(rating);
	InitAttribute(tempo);
	InitAttribute(composer);
#endif
	InitAttribute(genre);

	Read();
}

AudioAttributes::~AudioAttributes()
{
	PRINT(("AudioAttributes::~AudioAttributes()\n"));

	delete artist;
	delete album;
	delete title;
	delete year;
	delete comment;
	delete track;
#ifdef _TTE_
	delete rating;
	delete tempo;
	delete composer;
#endif
	delete genre;
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
		return attrib->Create();
	}

	return B_OK;
}

const char*
AudioAttributes::Artist()
{
	PRINT(("AudioAttributes::Artist()\n"));

	return artist->Value();
}

const char*
AudioAttributes::Album()
{
	PRINT(("AudioAttributes::Album()\n"));

	return album->Value();
}

const char*
AudioAttributes::Title()
{
	PRINT(("AudioAttributes::Title()\n"));

	return title->Value();
}

const char*
AudioAttributes::Year()
{
	PRINT(("AudioAttributes::Year()\n"));

	return year->Value();
}

const char*
AudioAttributes::Comment()
{
	PRINT(("AudioAttributes::Comment()\n"));

	return comment->Value();
}

const char*
AudioAttributes::Track()
{
	PRINT(("AudioAttributes::Track()\n"));

	return track->Value();
}

#ifdef _TTE_
const char*
AudioAttributes::Rating()
{
	PRINT(("AudioAttributes::Rating()\n"));

	return rating->Value();
}

const char*
AudioAttributes::Tempo()
{
	PRINT(("AudioAttributes::Tempo()\n"));

	return tempo->Value();
}
	
const char*
AudioAttributes::Composer()
{
	PRINT(("AudioAttributes::Composer()\n"));

	return composer->Value();
}
#endif

const char*
AudioAttributes::Genre()
{
	PRINT(("AudioAttributes::Genre()\n"));

	return genre->Value();
}

void
AudioAttributes::SetArtist(const char* value)
{
	PRINT(("AudioAttributes::SetArtist(const char*)\n"));

	artist->SetValue(value);
}

void
AudioAttributes::SetAlbum(const char* value)
{
	PRINT(("AudioAttributes::SetAlbum(const char*)\n"));

	album->SetValue(value);
}

void
AudioAttributes::SetTitle(const char* value)
{
	PRINT(("AudioAttributes::SetTitle(const char*)\n"));

	title->SetValue(value);
}

void
AudioAttributes::SetYear(const char* value)
{
	PRINT(("AudioAttributes::SetYear(const char*)\n"));

	year->SetValue(value);
}

void
AudioAttributes::SetComment(const char* value)
{
	PRINT(("AudioAttributes::SetComment(const char*)\n"));

	comment->SetValue(value);
}

void
AudioAttributes::SetTrack(const char* value)
{
	PRINT(("AudioAttributes::SetTrack(const char*)\n"));

	track->SetValue(value);
}

#ifdef _TTE_
void
AudioAttributes::SetRating(const char* value)
{
	PRINT(("AudioAttributes::SetRating(const char*)\n"));

	rating->SetValue(value);
}

void
AudioAttributes::SetTempo(const char* value)
{
	PRINT(("AudioAttributes::SetTempo(const char*)\n"));

	tempo->SetValue(value);
}

void
AudioAttributes::SetComposer(const char* value)
{
	PRINT(("AudioAttributes::SetComposer(const char*)\n"));

	composer->SetValue(value);
}
#endif

void
AudioAttributes::SetGenre(const char* value)
{
	PRINT(("AudioAttributes::SetGenre(const char*)\n"));

	genre->SetValue(value);
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
#endif

	return result;
}

status_t
AudioAttributes::ReadArtist()
{
	PRINT(("AudioAttributes::ReadArtist()\n"));

	return artist->Read();
}

status_t
AudioAttributes::ReadAlbum()
{
	PRINT(("AudioAttributes::ReadAlbum()\n"));

	return album->Read();
}

status_t
AudioAttributes::ReadTitle()
{
	PRINT(("AudioAttributes::ReadTitle()\n"));

	return title->Read();
}

status_t
AudioAttributes::ReadYear()
{
	PRINT(("AudioAttributes::ReadYear()\n"));

	return year->Read();
}

status_t
AudioAttributes::ReadComment()
{
	PRINT(("AudioAttributes::ReadComment()\n"));

	return comment->Read();
}

status_t
AudioAttributes::ReadTrack()
{
	PRINT(("AudioAttributes::ReadTrack()\n"));

	int retval = track->Read();
	if(track->Value() && strlen(track->Value()) == 1)
	{
		String tmp = "0";
		tmp += track->Value();
		track->SetValue(tmp.Value());
	}
	return retval;
}

#ifdef _TTE_
status_t
AudioAttributes::ReadRating()
{
	PRINT(("AudioAttributes::ReadRating()\n"));

	return rating->Read();
}

status_t
AudioAttributes::ReadTempo()
{
	PRINT(("AudioAttributes::ReadTempo()\n"));

	return tempo->Read();
}

status_t
AudioAttributes::ReadComposer()
{
	PRINT(("AudioAttributes::ReadComposer()\n"));

	return composer->Read();
}
#endif

status_t
AudioAttributes::ReadGenre()
{
	PRINT(("AudioAttributes::ReadGenre()\n"));

	return genre->Read();
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
#endif

	return result;
}

status_t
AudioAttributes::WriteArtist()
{
	PRINT(("AudioAttributes::WriteArtist()\n"));

	return artist->Write();
}

status_t
AudioAttributes::WriteAlbum()
{
	PRINT(("AudioAttributes::WriteAlbum()\n"));

	return album->Write();
}

status_t
AudioAttributes::WriteTitle()
{
	PRINT(("AudioAttributes::WriteTitle()\n"));

	return title->Write();
}

status_t
AudioAttributes::WriteYear()
{
	PRINT(("AudioAttributes::WriteYear()\n"));

	return year->Write();
}

status_t
AudioAttributes::WriteComment()
{
	PRINT(("AudioAttributes::WriteComment()\n"));

	return comment->Write();
}

status_t
AudioAttributes::WriteTrack()
{
	PRINT(("AudioAttributes::WriteTrack()\n"));

	return track->Write();
}

#ifdef _TTE_
status_t
AudioAttributes::WriteRating()
{
	PRINT(("AudioAttributes::WriteRating()\n"));

	return rating->Write();
}

status_t
AudioAttributes::WriteTempo()
{
	PRINT(("AudioAttributes::WriteTempo()\n"));

	return tempo->Write();
}

status_t
AudioAttributes::WriteComposer()
{
	PRINT(("AudioAttributes::WriteComposer()\n"));

	return composer->Write();
}
#endif

status_t
AudioAttributes::WriteGenre()
{
	PRINT(("AudioAttributes::WriteGenre()\n"));

	return genre->Write();
}

