#ifndef __GENRE_LIST_H__
#define __GENRE_LIST_H__

class GenreList
{
	public:
		GenreList();
		~GenreList();

		static const char* Genre(int value);
		static int Genre(const char* value);
		static int NumGenres();
		static int GenreSort(const char* g1, const char* g2);
};

#endif

