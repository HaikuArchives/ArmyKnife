#include <be/support/Debug.h>
#include "ID3Tag.h"

ID3Tag::ID3Tag(TagLib::FileRef * file, TagLib::Tag * tag, int tag_item)
	:	m_file		(file),
		m_tag		(tag),
		m_tag_item	(tag_item),
		m_value		()
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

	if(m_value.Length() == 0)
		return NULL;

	return this->m_value.String();
}

void
ID3Tag::SetValue(const char * string)
{
	PRINT(("ID3Tag::SetValue(const char*)\n"));

	if(string)
		m_value = string;
	else
		m_value.Truncate(0);
}

status_t
ID3Tag::Read()
{
	PRINT(("ID3Tag::Read()\n"));

	switch (m_tag_item)
	{
		case TITLE_TAG:
			m_value = m_tag->title().toCString(true);
			break;
		case ARTIST_TAG:
			m_value = m_tag->artist().toCString(true);;
			break;
		case ALBUM_TAG:
			m_value = m_tag->album().toCString(true);;
			break;
		case COMMENT_TAG:
			m_value = m_tag->comment().toCString(true);;
			break;
		case GENRE_TAG:
			m_value = m_tag->genre().toCString(true);;
			break;
		case YEAR_TAG:
			m_value.Truncate(0);
			m_value << m_tag->year(); // t->setYear(value.toInt());
			printf("ID3Tag::Read() value << tag->year(); value.String():%s\n", m_value.String());
			break;
		case TRACK_TAG:
			m_value.Truncate(0);
			m_value << m_tag->track(); // t->setTrack(value.toInt());
			printf("ID3Tag::Read() value << tag->track(); value.String():%s\n", m_value.String());
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
	
	TagLib::String string (m_value.String(), (TagLib::String::Type) 3);  // 3 = UTF8

	switch (m_tag_item)
	{
		case TITLE_TAG:
			m_tag->setTitle(string);
			break;
		case ARTIST_TAG:
			m_tag->setArtist(string);
			break;
		case ALBUM_TAG:
			m_tag->setAlbum(string);
			break;
		case COMMENT_TAG:
			m_tag->setComment(string);
			break;
		case GENRE_TAG:
			m_tag->setGenre(string);
			break;
		case YEAR_TAG:
			PRINT(("ID3Tag::Write() *** value.String(): %s\n", m_value.String()));
			PRINT(("ID3Tag::Write() *** tag->year(): %u\n", m_tag->year()));	
			PRINT(("ID3Tag::Write() *** tag->year(): %u\n", m_tag->year()));	
			m_tag->setYear(atoi(m_value.String()));
			PRINT(("ID3Tag::Write() atoi(value.String(): %ld\n", atoi(m_value.String())));
			PRINT(("ID3Tag::Write() *** tag->year(): %u\n", m_tag->year()));
			break;
		case TRACK_TAG:
			m_tag->setTrack(atoi(m_value.String()));
			break;
		default:
			break;
	}

	return B_OK;
}

