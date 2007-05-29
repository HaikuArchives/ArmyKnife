#include <string.h>
#include <be/support/Debug.h>
#include <String.h>
#include "ID3Tag.h"

ID3Tag::ID3Tag(TagLib::FileRef * file, TagLib::Tag * tag, int tag_item)
	:	file		(file),
		tag			(tag),
		tag_item	(tag_item),
		value		()
{
	PRINT(("ID3Tag::ID3Tag(TagLib::FileRef*, TagLib::Tag*, int)\n"));

}

ID3Tag::~ID3Tag()
{
	PRINT(("ID3Tag::~ID3Tag()\n"));

}

const char *
ID3Tag::Value()
{
	PRINT(("ID3Tag::Value()\n"));

	if(value.Length() == 0)
		return NULL;

	return this->value.String();
}

void
ID3Tag::SetValue(const char * string)
{
	PRINT(("ID3Tag::SetValue(const char*)\n"));

	if(string)
		value = string;
	else
		value.Truncate(0);
}

status_t
ID3Tag::Read()
{
	PRINT(("ID3Tag::Read()\n"));

	switch (tag_item)
	{
		case TITLE_TAG:
			value = tag->title().toCString(true);
			break;
		case ARTIST_TAG:
			value = tag->artist().toCString(true);;
			break;
		case ALBUM_TAG:
			value = tag->album().toCString(true);;
			break;
		case COMMENT_TAG:
			value = tag->comment().toCString(true);;
			break;
		case GENRE_TAG:
			value = tag->genre().toCString(true);;
			break;
		case YEAR_TAG:
			value.Truncate(0);
			value << tag->year(); // t->setYear(value.toInt());
			printf("ID3Tag::Read() value << tag->year(); value.String():%s\n", value.String());
			break;
		case TRACK_TAG:
			value.Truncate(0);
			value << tag->track(); // t->setTrack(value.toInt());
			printf("ID3Tag::Read() value << tag->track(); value.String():%s\n", value.String());
			break;
		default:
			break;
	}

	PRINT(("ID3Tag::Read() *** \n"));

	fflush(NULL);

	return B_OK;
}

status_t
ID3Tag::Write()
{
	PRINT(("ID3Tag::Write()\n"));
	
    enum 
    {
      Latin1 = 0,
      UTF16 = 1,
      UTF16BE = 2,
      UTF8 = 3
    };
	
//	TagLib::String::Type type = UTF8;
	
	TagLib::String string (value.String(), (TagLib::String::Type) 3);  // 3 = UTF8

	switch (tag_item)
	{
		case TITLE_TAG:
			tag->setTitle(string);
			break;
		case ARTIST_TAG:
			tag->setArtist(string);
			break;
		case ALBUM_TAG:
			tag->setAlbum(string);
			break;
		case COMMENT_TAG:
			tag->setComment(string);
			break;
		case GENRE_TAG:
			tag->setGenre(string);
			break;
		case YEAR_TAG:
			PRINT(("ID3Tag::Write() *** value.String(): %s\n", value.String()));
			PRINT(("ID3Tag::Write() *** tag->year(): %u\n", tag->year()));	
			PRINT(("ID3Tag::Write() *** tag->year(): %u\n", tag->year()));	
			tag->setYear(atoi(value.String()));
			PRINT(("ID3Tag::Write() atoi(value.String(): %ld\n", atoi(value.String())));
			PRINT(("ID3Tag::Write() *** tag->year(): %u\n", tag->year()));
			break;
		case TRACK_TAG:
			tag->setTrack(atoi(value.String()));
			break;
		default:
			break;
	}

	return B_OK;
}

