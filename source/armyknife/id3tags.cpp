#include <stdlib.h>
#include <string.h>

#include <be/support/Debug.h>
#include <Node.h>
#include <NodeInfo.h>
#include <Path.h>
#include <String.h>

#include <mpegfile.h>
#include <oggfile.h>
#include <vorbisfile.h>
#include <flacfile.h>

#include "genrelist.h"
#include "id3tags.h"

//using namespace TagLib;

ID3Tags::ID3Tags(EntryRefItem * ref_item)
	:	AudioInfo	(),
		m_file		(NULL),
		m_tag		(NULL),
		m_artist	(NULL),
		m_album		(NULL),
		m_title		(NULL),
		m_year		(NULL),
		m_comment	(NULL),
		m_track		(NULL),
		m_genre		(NULL),
		m_write_on_exit	(false)
{
	PRINT(("ID3Tags::ID3Tags()\n"));

	status_t status = B_OK;
	bool fileref_created_ok = false;

	BPath filename(ref_item->EntryRef());
	
	if (ref_item->IsMP3())
	{
		PRINT(("Filetype == MPEG\n"));
		m_file = new TagLib::FileRef(new TagLib::MPEG::File(filename.Path()));
		fileref_created_ok = true;
	}
	else if (ref_item->IsOGG())
	{
		PRINT(("Filetype == VORBIS\n"));
		m_file = new TagLib::FileRef(new TagLib::Vorbis::File(filename.Path()));
		fileref_created_ok = true;
	}
	else if (ref_item->IsFLAC())
	{
		PRINT(("Filetype == FLAC\n"));
		m_file = new TagLib::FileRef(new TagLib::FLAC::File(filename.Path()));
		fileref_created_ok = true;
	}
	
	// XXX this needs more work
	
	if (! fileref_created_ok)
	{
		this->m_file = new TagLib::FileRef (filename.Path());
		// XXX error checking
	}
	
	if (m_file == NULL)
	{
		PRINT(("ID3Tags::ID3Tags() :: file == NULL\n"));
		return;
	}

	if (m_file->isNull())
	{
		PRINT(("ID3Tags::ID3Tags() :: file->isNull()\n"));
		return;
	}
	
	
	if (! m_file->tag())
	{
		PRINT(("ID3Tags::ID3Tags() :: !file->tag()\n"));
		return;
	}
	
	if (this->m_file != NULL)
		this->m_tag	= this->m_file->tag();
	else
		this->m_tag = NULL;


	m_artist = new ID3Tag(m_file, m_tag, ARTIST_TAG);
	m_album = new ID3Tag(m_file, m_tag, ALBUM_TAG);
	m_title = new ID3Tag(m_file, m_tag, TITLE_TAG);
	m_year = new ID3Tag(m_file, m_tag, YEAR_TAG);
	m_comment = new ID3Tag(m_file, m_tag, COMMENT_TAG);
	m_track = new ID3Tag(m_file, m_tag, TRACK_TAG);
	m_genre = new ID3Tag(m_file, m_tag, GENRE_TAG);

	Read();

}

ID3Tags::~ID3Tags()
{
	PRINT(("ID3Tags::~ID3Tags()\n"));
	
	if (m_write_on_exit)
		m_file->save();		

	delete m_file;
	
	delete m_artist;
	delete m_album;
	delete m_title;
	delete m_year;
	delete m_comment;
	delete m_track;
	delete m_genre;
}

const char*
ID3Tags::Artist()
{
	PRINT(("ID3Tags::Artist()\n"));

	return m_artist->Value();
}

const char*
ID3Tags::Album()
{
	PRINT(("ID3Tags::Album()\n"));

	return m_album->Value();
}

const char*
ID3Tags::Title()
{
	PRINT(("ID3Tags::Title()\n"));

	return m_title->Value();
}

const char*
ID3Tags::Year()
{
	PRINT(("ID3Tags::Year()\n"));

	return m_year->Value();
}

const char*
ID3Tags::Comment()
{
	PRINT(("ID3Tags::Comment()\n"));

	return m_comment->Value();
}

const char*
ID3Tags::Track()
{
	PRINT(("ID3Tags::Track()\n"));

	return m_track->Value();
}

const char*
ID3Tags::Genre()
{
	PRINT(("ID3Tags::Genre()\n"));
	
	return m_genre->Value();
}

void
ID3Tags::SetArtist(const char* value)
{
	PRINT(("ID3Tags::SetArtist(const char*)\n"));

	m_artist->SetValue(value);
}

void
ID3Tags::SetAlbum(const char* value)
{
	PRINT(("ID3Tags::SetAlbum(const char*)\n"));

	m_album->SetValue(value);
}

void
ID3Tags::SetTitle(const char* value)
{
	PRINT(("ID3Tags::SetTitle(const char*)\n"));

	m_title->SetValue(value);
}

void
ID3Tags::SetYear(const char* value)
{
	PRINT(("ID3Tags::SetYear(const char*)\n"));

	m_year->SetValue(value);
}

void
ID3Tags::SetComment(const char* value)
{
	PRINT(("ID3Tags::SetComment(const char*)\n"));

	m_comment->SetValue(value);
}

void
ID3Tags::SetTrack(const char* value)
{
	PRINT(("ID3Tags::SetTrack(const char*)\n"));

	int trk = 0;
	if(value)
	{
		trk = atoi(value);
		BString	string;
		string.Truncate(0);
		string << trk;
		m_track->SetValue(string.String());
	}
}

void
ID3Tags::SetGenre(const char* value)
{
	PRINT(("ID3Tags::SetGenre(const char*)\n"));

	m_genre->SetValue(value);
}

status_t
ID3Tags::ReadArtist()
{
	PRINT(("ID3Tags::ReadArtist()\n"));

	return m_artist->Read();
}

status_t
ID3Tags::ReadAlbum()
{
	PRINT(("ID3Tags::ReadAlbum()\n"));

	return m_album->Read();
}

status_t
ID3Tags::ReadTitle()
{
	PRINT(("ID3Tags::ReadTitle()\n"));

	return m_title->Read();
}

status_t
ID3Tags::ReadYear()
{
	PRINT(("ID3Tags::ReadYear()\n"));

	return m_year->Read();
}

status_t
ID3Tags::ReadComment()
{
	PRINT(("ID3Tags::ReadComment()\n"));

	return m_comment->Read();
}

status_t
ID3Tags::ReadTrack()
{
	PRINT(("ID3Tags::ReadTrack()\n"));

	return m_track->Read();
}

status_t
ID3Tags::ReadGenre()
{
	PRINT(("ID3Tags::ReadGenre()\n"));

	return m_genre->Read();
}

status_t
ID3Tags::Write()
{
	PRINT(("ID3Tags::Write()\n"));
	
	m_write_on_exit = true;
	
	return AudioInfo::Write();
}

status_t
ID3Tags::WriteArtist()
{
	PRINT(("ID3Tags::WriteArtist()\n"));
	
	m_write_on_exit = true;
	
	return m_artist->Write();
}

status_t
ID3Tags::WriteAlbum()
{
	PRINT(("ID3Tags::WriteAlbum()\n"));

	m_write_on_exit = true;

	return m_album->Write();
}

status_t
ID3Tags::WriteTitle()
{
	PRINT(("ID3Tags::WriteTitle()\n"));

	m_write_on_exit = true;

	return m_title->Write();
}

status_t
ID3Tags::WriteYear()
{
	PRINT(("ID3Tags::WriteYear()\n"));

	m_write_on_exit = true;

	return m_year->Write();
}

status_t
ID3Tags::WriteComment()
{
	PRINT(("ID3Tags::WriteComment()\n"));

	m_write_on_exit = true;

	return m_comment->Write();
}

status_t
ID3Tags::WriteTrack()
{
	PRINT(("ID3Tags::WriteTrack()\n"));

	m_write_on_exit = true;

	return m_track->Write();
}

status_t
ID3Tags::WriteGenre()
{
	PRINT(("ID3Tags::WriteGenre()\n"));

	m_write_on_exit = true;
	
	return m_genre->Write();
}
