#ifndef __ID3_TAG_H__
#define __ID3_TAG_H__

#include <stdio.h>
#include <String.h>
#include <fileref.h>
#include <tag.h>

enum
{
	TITLE_TAG	=	200,
	ARTIST_TAG	=	201,
	ALBUM_TAG	=	202,
	YEAR_TAG	=	203,
	COMMENT_TAG	=	204,
	BLANK_TAG	=	205,
	TRACK_TAG	=	206,
	GENRE_TAG	=	207
};

class ID3Tag
{
	public:
		ID3Tag(TagLib::FileRef * file, TagLib::Tag * tag, int tag_item);
		virtual ~ID3Tag();

		virtual const char * Value();
		virtual void SetValue(const char * string);

		virtual status_t Read();
		virtual status_t Write();

	protected:
		TagLib::FileRef	*	file;
		TagLib::Tag		*	tag;

		int	tag_item;
		BString value;
		
};

#endif

