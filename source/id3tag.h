/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __ID3_TAG_H__
#define __ID3_TAG_H__

#include <stdio.h>
#include <String.h>
#include "taglib/fileref.h"
#include "taglib/tag.h"

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

		virtual const char *	Value();
		virtual void			SetValue(const char * string);

		virtual status_t		Read();
		virtual status_t		Write();

	protected:
		TagLib::FileRef*	m_file;
		TagLib::Tag*		m_tag;

		int					m_tag_item;
		BString				m_value;

};

#endif

