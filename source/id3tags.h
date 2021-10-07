/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __ID3_TAGS_H__
#define __ID3_TAGS_H__

#include <stdio.h>

#include "audioinfo.h"
#include "entryrefitem.h"
#include "id3tag.h"

class ID3Tags : public AudioInfo
{
	public:
		ID3Tags(EntryRefItem * ref_item);
		virtual ~ID3Tags();

		virtual const char* Artist();
		virtual const char* Album();
		virtual const char* Title();
		virtual const char* Year();
		virtual const char* Comment();
		virtual const char* Track();
		virtual const char* Genre();

		virtual void SetArtist(const char* value);
		virtual void SetAlbum(const char* value);
		virtual void SetTitle(const char* value);
		virtual void SetYear(const char* value);
		virtual void SetComment(const char* value);
		virtual void SetTrack(const char* value);
		virtual void SetGenre(const char* value);

		virtual status_t ReadArtist();
		virtual status_t ReadAlbum();
		virtual status_t ReadTitle();
		virtual status_t ReadYear();
		virtual status_t ReadComment();
		virtual status_t ReadTrack();
		virtual status_t ReadGenre();

		virtual status_t Write();
		virtual status_t WriteArtist();
		virtual status_t WriteAlbum();
		virtual status_t WriteTitle();
		virtual status_t WriteYear();
		virtual status_t WriteComment();
		virtual status_t WriteTrack();
		virtual status_t WriteGenre();

	protected:
		TagLib::FileRef*	m_file;
		TagLib::Tag*		m_tag;

		ID3Tag*		m_artist;
		ID3Tag*		m_album;
		ID3Tag*		m_title;
		ID3Tag*		m_year;
		ID3Tag*		m_comment;
		ID3Tag*		m_track;
		ID3Tag*		m_genre;

	private:
		bool	m_write_on_exit;
};

#endif

