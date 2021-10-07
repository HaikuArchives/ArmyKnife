/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _MUSIC_BRAINZ_QUERY_H__
#define _MUSIC_BRAINZ_QUERY_H__

#include <String.h>

#include "query.h"


class MusicBrainzQuery : public Query
{
	public:
		MusicBrainzQuery(Queryable original);
		virtual ~MusicBrainzQuery();

	protected:
		virtual status_t RefillBuffer();

	private:
		static const int k_fetch_size = 10;

		BString m_query_string;;
		int32 m_total_count;
		int32 m_next_count;
};

#endif

