/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "musicbrainzquery.h"

#include <exception>

#include <Debug.h>

#include <musicbrainz5/Artist.h>
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/Medium.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/NameCreditList.h>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/Recording.h>
#include <musicbrainz5/Release.h>
#include <musicbrainz5/Track.h>

#include "appdefs.h"

MusicBrainzQuery::MusicBrainzQuery(Queryable original)
	:
	Query(),
	m_total_count(-1),
	m_next_count(0)
{
	PRINT(("MusicBrainzQuery::MusicBrainzQuery(Queryable)\n"));

	if (original.Artist() != NULL)
		m_query_string << "artist:\"" << original.Artist() << "\" AND ";

	if (original.Album() != NULL)
		m_query_string << "release:\"" << original.Album() << "\" AND ";

	if (original.Title() != NULL)
		m_query_string << "recording:\"" << original.Title() << "\" AND ";

	if (original.Year() != NULL)
		m_query_string << "date:\"" << original.Year() << "\" AND ";

	if (original.Comment() != NULL)
		m_query_string << "comment:\"" << original.Comment() << "\" AND ";

	if (original.Track() != NULL)
		m_query_string << "tnum:\"" << original.Track() << "\" AND ";

	// Get rid of the final " AND "
	if (m_query_string.Length() > 5);
		m_query_string.Truncate(m_query_string.Length() - 5);
}

MusicBrainzQuery::~MusicBrainzQuery()
{
	PRINT(("MusicBrainzQuery::~MusicBrainzQuery()\n"));
}

BString
MakeArtist(MusicBrainz5::CArtistCredit* credit)
{
	MusicBrainz5::CNameCreditList* list = credit->NameCreditList();

	BString res;
	for (int32 i = 0; i < list->NumItems(); i++) {
		MusicBrainz5::CNameCredit* single = list->Item(i);
		res << single->Artist()->Name().c_str() << single->JoinPhrase().c_str();
	}
	return res;
}

status_t
MusicBrainzQuery::RefillBuffer()
{
	PRINT(("MusicBrainzQuery::RefillBuffer()\n"));

	m_buffer.erase(m_buffer.begin(), m_buffer.end());

	if (m_total_count == m_next_count)
		return B_ERROR;

	MusicBrainz5::CQuery query("ArmyKnife " VERSION " for Haiku (https://github.com/HaikuArchives/ArmyKnife)");

	try {
		MusicBrainz5::CQuery::tParamMap params;

		params["query"] = m_query_string.String();
		BString c1;
		c1 << k_fetch_size;
		params["limit"] = c1.String();

		BString c2;
		c2 << m_next_count;
		params["offset"] = c2.String();

		MusicBrainz5::CMetadata result = query.Query("recording", "", "", params);
		MusicBrainz5::CRecordingList* results = result.RecordingList();

		if (results == NULL)
			return B_ERROR;

		m_total_count = results->Count();
		m_next_count += results->NumItems();
		for (int32 i = 0; i < results->NumItems(); i++) {
			MusicBrainz5::CRecording* recording = results->Item(i);
			Queryable templ;

			templ.SetTitle(recording->Title().c_str());
			templ.SetComment(recording->Disambiguation().c_str());
			templ.SetArtist(MakeArtist(recording->ArtistCredit()).String());

			MusicBrainz5::CReleaseList* releases = recording->ReleaseList();

			if (releases == NULL)
				continue;

			for (int32 j = 0; j < releases->NumItems(); j++) {
				MusicBrainz5::CRelease* release = releases->Item(j);
				Queryable toAdd = templ; // Copy

				toAdd.SetAlbum(release->Title().c_str());
				toAdd.SetYear(release->Date().c_str());
				toAdd.SetTrack(release
					->MediumList()->Item(0)
					->TrackList()->Item(0)
					->Number().c_str());

				m_buffer.push_back(toAdd);
			}
		}
		return m_buffer.size() > 0 ? B_OK : B_ERROR;
	}
	catch (std::exception& ex) {
		std::cout << "An exception occurred: " << ex.what() << std::endl;
		// Check whether there was an actual error or simply throttling
		return query.LastHTTPCode() == 503 ? B_BUSY : B_ERROR;
	}
}
