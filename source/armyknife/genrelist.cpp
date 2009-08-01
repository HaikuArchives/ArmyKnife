#include <be/support/Debug.h>
#include <string.h>
#include "GenreList.h"

#define NUM_GENRES 115
#define OTHER_INDEX 12

char* genre_list[] =
{
	"Blues",
	"Classic Rock",
	"Country",
	"Dance",
	"Disco",
	"Funk",
	"Grunge",
	"Hip-Hop",
	"Jazz",
	"Metal",
	"New Age",
	"Oldies",
	"Other",
	"Pop",
	"R&B",
	"Rap",
	"Reggae",
	"Rock",
	"Techno",
	"Industrial",
	"Alternative",
	"Ska",
	"Death Metal",
	"Pranks",
	"Soundtrack",
	"Euro-Techno",
	"Ambient",
	"Trip-Hop",
	"Vocal",
	"Jazz+Funk",
	"Fusion",
	"Trance",
	"Classical",
	"Instrumental",
	"Acid",
	"House",
	"Games",
	"Sound Clip",
	"Gospel",
	"Noise",
	"Alternative Rock",
	"Bass",
	"Soul",
	"Punk",
	"Space",
	"Meditative",
	"Instrumental Pop",
	"Instrumental Rock",
	"Ethnic",
	"Gothic",
	"Darkwave",
	"Techno-Industrial",
	"Electronic",
	"Pop-Folk",
	"Eurodance",
	"Dream",
	"Southern Rock",
	"Comedy",
	"Cult",
	"Gangsta",
	"Top 40",
	"Christian Rap",
	"Pop/Funk",
	"Jungle",
	"Native American",
	"Cabaret",
	"New Wave",
	"Psychadelic",
	"Rave",
	"Showtunes",
	"Trailer",
	"Lo-Fi",
	"Tribal",
	"Acid Punk",
	"Acid Jazz",
	"Polka",
	"Retro",
	"Musical",
	"Rock & Roll",
	"Hard Rock",
	"Folk",
	"Folk/Rock",
	"National Folk",
	"Swing",
	"Bebob",
	"Latin",
	"Revival",
	"Celtic",
	"Bluegrass",
	"Avantgarde",
	"Gothic Rock",
	"Progressive Rock",
	"Psychadelic Rock",
	"Symphonic Rock",
	"Slow Rock",
	"Big Band",
	"Chorus",
	"Easy Listening",
	"Acoustic",
	"Humour",
	"Speech",
	"Chanson",
	"Opera",
	"Chamber Music",
	"Sonata",
	"Symphony",
	"Booty Bass",
	"Primus",
	"Porn Groove",
	"Satire",
	"Slow Jam",
	"Club",
	"Tango",
	"Samba",
	"Folklore"
};

GenreList::GenreList()
{
	PRINT(("GenreList::GenreList()\n"));
}

GenreList::~GenreList()
{
	PRINT(("GenreList::~GenreList()\n"));
}

char*
GenreList::Genre(int value)
{
	PRINT(("GenreList::Genre(int)\n"));

	if((value >= 0) && (value < NUM_GENRES))
	{
		return genre_list[value];
	}
	return genre_list[OTHER_INDEX];
}

int
GenreList::Genre(const char* value)
{
	PRINT(("GenreList::Genre(const char*)\n"));

	if(!value)
	{
		return OTHER_INDEX;
	}

	for(int i=0; i < NUM_GENRES; i++)
	{
		if(strcmp(value,genre_list[i]) == 0)
		{
			return i;
		}
	}
	return OTHER_INDEX;
}

int
GenreList::NumGenres()
{
	PRINT(("GenreList::NumGenres()\n"));

	return NUM_GENRES;
}

int
GenreList::GenreSort(const void* g1, const void* g2)
{
	//PRINT(("GenreList::GenreSort()\n"));

	char* genre1 = *((char**) g1);
	char* genre2 = *((char**) g2);
	return strcmp(genre1,genre2);
}

