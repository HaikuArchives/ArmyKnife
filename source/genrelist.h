/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __GENRE_LIST_H__
#define __GENRE_LIST_H__

class GenreList
{
	public:
		GenreList();
		~GenreList();

		static BString Genre(int genreindex);
		static int Genre(BString genretofind);
		static int NumGenres();
		static int GenreSort(const char* g1, const char* g2);
};

#endif

