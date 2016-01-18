#include "queryable.h"

#include <iostream>
#include <string.h>

#include <Debug.h>
#include <SupportDefs.h>

Queryable::Queryable()
	:
	BArchivable(),
	m_artist(NULL),
	m_album(NULL),
	m_title(NULL),
	m_year(NULL),
	m_comment(NULL),
	m_track(NULL)
{
	PRINT(("Queryable::Queryable()\n"));
}

Queryable::Queryable(const Queryable& other)
	:
	BArchivable(),
	m_artist(strdup(other.Artist())),
	m_album(strdup(other.Album())),
	m_title(strdup(other.Title())),
	m_year(strdup(other.Year())),
	m_comment(strdup(other.Comment())),
	m_track(strdup(other.Track()))
{
	PRINT(("Queryable::Queryable(const Queryable&)\n"));
}

Queryable::Queryable(BMessage* from)
	:
	BArchivable(from),
	m_artist(NULL),
	m_album(NULL),
	m_title(NULL),
	m_year(NULL),
	m_comment(NULL),
	m_track(NULL)
{
	PRINT(("Queryable::Quertable(BMessage*)\n"));

	if (from == NULL)
		return;

	const char* artist;
	const char* album;
	const char* title;
	const char* year;
	const char* comment;
	const char* track;

	// If FindString does not find the field, it sets the pointer to NULL
	from->FindString("m_artist", &artist);
	from->FindString("m_album", &album);
	from->FindString("m_title", &title);
	from->FindString("m_year", &year);
	from->FindString("m_comment", &comment);
	from->FindString("m_track", &track);

	m_artist = strdup(artist);
	m_album = strdup(album);
	m_title = strdup(title);
	m_year = strdup(year);
	m_comment = strdup(comment);
	m_track = strdup(track);
}

Queryable::~Queryable()
{
	PRINT(("Queryable::~Queryable()\n"));

	delete m_artist;
	delete m_album;
	delete m_title;
	delete m_year;
	delete m_comment;
	delete m_track;
}

status_t
Queryable::Archive(BMessage* into, bool deep) const
{
	PRINT(("Queryable::Archive(BMessage*, bool)\n"));

	status_t status = BArchivable::Archive(into, deep);

	if (status == B_OK && m_artist != NULL)
		status = into->AddString("m_artist", m_artist);

	if (status == B_OK && m_album != NULL)
		status = into->AddString("m_album", m_album);

	if (status == B_OK && m_title != NULL)
		status = into->AddString("m_title", m_title);

	if (status == B_OK && m_year != NULL)
		status = into->AddString("m_year", m_year);

	if (status == B_OK && m_comment != NULL)
		status = into->AddString("m_comment", m_comment);

	if (status == B_OK && m_track != NULL)
		status = into->AddString("m_track", m_track);

	if (status == B_OK)
		status = into->AddString("class", "Queryable");

	return status;
}

BArchivable*
Queryable::Instantiate(BMessage* archive)
{
	if (validate_instantiation(archive, "Queryable"))
		return new Queryable(archive);

	return NULL;
}

const char*
Queryable::Artist() const
{
	PRINT(("Queryable::Artist()\n"));
	return m_artist;
}

const char*
Queryable::Album() const
{
	PRINT(("Queryable::Album()\n"));
	return m_album;
}

const char*
Queryable::Title() const
{
	PRINT(("Queryable::Title()\n"));
	return m_title;
}

const char*
Queryable::Year() const
{
	PRINT(("Queryable::Year()\n"));
	return m_year;
}

const char*
Queryable::Comment() const
{
	PRINT(("Queryable::Comment()\n"));
	return m_comment;
}

const char*
Queryable::Track() const
{
	PRINT(("Queryable::Track()\n"));
	return m_track;
}

void
Queryable::SetArtist(const char* value)
{
	PRINT(("Queryable::SetArtist(const char*)\n"));
	delete m_artist;
	m_artist = strdup(value);
}

void
Queryable::SetAlbum(const char* value)
{
	PRINT(("Queryable::SetAlbum(const char*)\n"));
	delete m_album;
	m_album = strdup(value);
}

void
Queryable::SetTitle(const char* value)
{
	PRINT(("Queryable::SetTitle(const char*)\n"));
	delete m_title;
	m_title = strdup(value);
}

void
Queryable::SetYear(const char* value)
{
	PRINT(("Queryable::SetYear(const char*)\n"));
	delete m_year;
	m_year = strdup(value);
}

void
Queryable::SetComment(const char* value)
{
	PRINT(("Queryable::SetComment(const char*)\n"));
	delete m_comment;
	m_comment = strdup(value);
}

void
Queryable::SetTrack(const char* value)
{
	PRINT(("Queryable::SetTrack(const char*)\n"));
	delete m_track;
	m_track = strdup(value);
}

void
Queryable::PrintToStream()
{
	std::cout << std::endl
		<< "Artist  = " << Artist() << std::endl
		<< "Album   = " << Album() << std::endl
		<< "Title   = " << Title() << std::endl
		<< "Year    = " << Year() << std::endl
		<< "Comment = " << Comment() << std::endl
		<< "Track   = " << Track() << std::endl;
}
