#ifndef __GENRE_LIST_H__
#define __GENRE_LIST_H__

class GenreList
{
	public:
		GenreList();
		~GenreList();

		static char* Genre(int value);
		static int Genre(const char* value);
		static int NumGenres();
		static int GenreSort(const void* g1, const void* g2);
};

#endif

