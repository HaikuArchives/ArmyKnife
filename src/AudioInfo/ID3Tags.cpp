#include <stdlib.h>
#include <string.h>

#include <be/support/Debug.h>
#include <Node.h>
#include <NodeInfo.h>
#include <String.h>

#include <mpegfile.h>
#include <oggfile.h>
#include <vorbisfile.h>

#include "GenreList.h"
#include "ID3Tags.h"

//using namespace TagLib;

ID3Tags::ID3Tags(const char * filename)
	:	AudioInfo	(),
		file		(NULL),
		tag			(NULL),
		artist		(NULL),
		album		(NULL),
		title		(NULL),
		year		(NULL),
		comment		(NULL),
		track		(NULL),
		genre		(NULL),
		write_on_exit	(false)
{
	PRINT(("ID3Tags::ID3Tags(const char*): %s\n", filename));

	status_t status = B_OK;
	BNode node;
	BNodeInfo node_info;
	char * mimetype = new char [B_MIME_TYPE_LENGTH];
	bool fileref_created_ok = false;
	
	if (node.SetTo(filename) == B_OK)
		if (node_info.SetTo(& node) == B_OK)
			if (node_info.GetType(mimetype) == B_OK)
			{
				BString string (mimetype);
				
				if (string == "audio/mp3"
					||	string == "audio/x-mp3"
					||	string == "audio/mpeg"
					||	string == "audio/x-mpeg"
					||	string == "audio/mpg"
					||	string == "audio/x-mpg")
				{
					PRINT(("Filetype == MPEG\n"));
					file = new TagLib::FileRef(new TagLib::MPEG::File(filename));
					fileref_created_ok = true;
				}
				else if (string == "audio/ogg"
					||	string == "audio/x-ogg"
					||	string == "audio/vorbis"
					||	string == "audio/x-vorbis"
					||	string == "application/ogg"
					||	string == "application/x-ogg"
					||	string == "application/vorbis"
					||	string == "application/x-vorbis")
				{
					PRINT(("Filetype == VORBIS\n"));
					file = new TagLib::FileRef(new TagLib::Vorbis::File(filename));
					fileref_created_ok = true;
				}
			}
	
	// XXX this needs more work
	
	if (! fileref_created_ok)
	{
		this->file = new TagLib::FileRef (filename);
		// XXX error checking
	}
	
	if (file == NULL)
	{
		PRINT(("ID3Tags::ID3Tags() :: file == NULL\n"));
		return;
	}

	if (file->isNull())
	{
		PRINT(("ID3Tags::ID3Tags() :: file->isNull()\n"));
		return;
	}
	
	
	if (! file->tag())
	{
		PRINT(("ID3Tags::ID3Tags() :: !file->tag()\n"));
		return;
	}
	
	if (this->file != NULL)
		this->tag	= this->file->tag();
	else
		this->tag = NULL;


	artist = new ID3Tag(file, tag, ARTIST_TAG);
	album = new ID3Tag(file, tag, ALBUM_TAG);
	title = new ID3Tag(file, tag, TITLE_TAG);
	year = new ID3Tag(file, tag, YEAR_TAG);
	comment = new ID3Tag(file, tag, COMMENT_TAG);
	track = new ID3Tag(file, tag, TRACK_TAG);
	genre = new ID3Tag(file, tag, GENRE_TAG);

	Read();

}

ID3Tags::~ID3Tags()
{
	PRINT(("ID3Tags::~ID3Tags()\n"));
	
	if (write_on_exit)
		file->save();		

	delete file;
	
	delete artist;
	delete album;
	delete title;
	delete year;
	delete comment;
	delete track;
	delete genre;
}

bool
ID3Tags::HasID3Tags()
{
	PRINT(("ID3Tags::HasID3Tags()\n"));

	return true;
}

int
ID3Tags::CreateID3Tags()
{
	PRINT(("ID3Tags::CreateID3Tags()\n"));

}

const char*
ID3Tags::Artist()
{
	PRINT(("ID3Tags::Artist()\n"));

	return artist->Value();
}

const char*
ID3Tags::Album()
{
	PRINT(("ID3Tags::Album()\n"));

	return album->Value();
}

const char*
ID3Tags::Title()
{
	PRINT(("ID3Tags::Title()\n"));

	return title->Value();
}

const char*
ID3Tags::Year()
{
	PRINT(("ID3Tags::Year()\n"));

	return year->Value();
}

const char*
ID3Tags::Comment()
{
	PRINT(("ID3Tags::Comment()\n"));

	return comment->Value();
}

const char*
ID3Tags::Track()	// XXX
{
	PRINT(("ID3Tags::Track()\n"));

	return track->Value();
/*
	const char* trk = track->Value();

	if(trk)
	{
		sprintf(trackValue,"%02d",trk[0]);
		return trackValue;
	}
	return 0;
*/
}

const char*
ID3Tags::Genre()	// XXX
{
	PRINT(("ID3Tags::Genre()\n"));
	
	return genre->Value();

/*
	const char* value = genre->Value();
	if(value)
	{
		return GenreList::Genre((unsigned char)value[0]);
	}
	return GenreList::Genre(0);
*/
}

void
ID3Tags::SetArtist(const char* value)
{
	PRINT(("ID3Tags::SetArtist(const char*)\n"));

	artist->SetValue(value);
}

void
ID3Tags::SetAlbum(const char* value)
{
	PRINT(("ID3Tags::SetAlbum(const char*)\n"));

	album->SetValue(value);
}

void
ID3Tags::SetTitle(const char* value)
{
	PRINT(("ID3Tags::SetTitle(const char*)\n"));

	title->SetValue(value);
}

void
ID3Tags::SetYear(const char* value)
{
	PRINT(("ID3Tags::SetYear(const char*)\n"));

	year->SetValue(value);
}

void
ID3Tags::SetComment(const char* value)
{
	PRINT(("ID3Tags::SetComment(const char*)\n"));

	comment->SetValue(value);
}

void
ID3Tags::SetTrack(const char* value)
{
	PRINT(("ID3Tags::SetTrack(const char*)\n"));

	int trk = 0;
	if(value)
	{
		trk = atoi(value);
	}
	if(trk >= 0 && trk <= 255)
	{
//		char val = trk;
//		track->SetValue(&val);

		BString	string;
		string.Truncate(0);
		string << trk;
		track->SetValue(string.String());
	}
	else
	{
		track->SetValue(0);
	}
}

void
ID3Tags::SetGenre(const char* value)
{
	PRINT(("ID3Tags::SetGenre(const char*)\n"));

//	char val = GenreList::Genre(value);
//	genre->SetValue(&val);
	genre->SetValue(value);
}

status_t
ID3Tags::ReadArtist()
{
	PRINT(("ID3Tags::ReadArtist()\n"));

	return artist->Read();
}

status_t
ID3Tags::ReadAlbum()
{
	PRINT(("ID3Tags::ReadAlbum()\n"));

	return album->Read();
}

status_t
ID3Tags::ReadTitle()
{
	PRINT(("ID3Tags::ReadTitle()\n"));

	return title->Read();
}

status_t
ID3Tags::ReadYear()
{
	PRINT(("ID3Tags::ReadYear()\n"));

	return year->Read();
}

status_t
ID3Tags::ReadComment()
{
	PRINT(("ID3Tags::ReadComment()\n"));

	return comment->Read();
}

status_t
ID3Tags::ReadTrack()
{
	PRINT(("ID3Tags::ReadTrack()\n"));

	return track->Read();
}

status_t
ID3Tags::ReadGenre()
{
	PRINT(("ID3Tags::ReadGenre()\n"));

	return genre->Read();
}

status_t
ID3Tags::Write()
{
	PRINT(("ID3Tags::Write()\n"));
	
	write_on_exit = true;
	
	return AudioInfo::Write();
}

status_t
ID3Tags::WriteArtist()
{
	PRINT(("ID3Tags::WriteArtist()\n"));
	
	write_on_exit = true;
	
	return artist->Write();
}

status_t
ID3Tags::WriteAlbum()
{
	PRINT(("ID3Tags::WriteAlbum()\n"));

	write_on_exit = true;

	return album->Write();
}

status_t
ID3Tags::WriteTitle()
{
	PRINT(("ID3Tags::WriteTitle()\n"));

	write_on_exit = true;

	return title->Write();
}

status_t
ID3Tags::WriteYear()
{
	PRINT(("ID3Tags::WriteYear()\n"));

	write_on_exit = true;

	return year->Write();
}

status_t
ID3Tags::WriteComment()
{
	PRINT(("ID3Tags::WriteComment()\n"));

	write_on_exit = true;

	return comment->Write();
}

status_t
ID3Tags::WriteTrack()
{
	PRINT(("ID3Tags::WriteTrack()\n"));

	write_on_exit = true;

	return track->Write();
}

status_t
ID3Tags::WriteGenre()
{
	PRINT(("ID3Tags::WriteGenre()\n"));

	write_on_exit = true;
	
	return genre->Write();
}

