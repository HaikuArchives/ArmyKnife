/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <Debug.h>
#include <String.h>
#include "genrelist.h"

#define NUM_GENRES 115
#define OTHER_INDEX 12

static BString genres[NUM_GENRES] = {
	"Blues",  "Classic Rock",  "Country", "Dance",  "Disco", "Funk","Grunge",
	"Hip-Hop","Jazz", "Metal", "New Age", "Oldies","Other","Pop","R&B","Rap",
	"Reggae","Rock", "Techno","Industrial","Alternative","Ska","Death Metal",
	"Pranks",  "Soundtrack", "Euro-Techno",  "Ambient",  "Trip-Hop", "Vocal",
	"Jazz+Funk", "Fusion","Trance","Classical","Instrumental","Acid","House",
	"Games","Sound Clip", "Gospel","Noise","Alternative Rock","Bass", "Soul",
	"Punk", "Space",  "Meditative", "Instrumental Pop",  "Instrumental Rock",
	"Ethnic",   "Gothic",   "Darkwave",   "Techno-Industrial",  "Electronic",
	"Pop-Folk","Eurodance","Dream","Southern Rock","Comedy","Cult","Gangsta",
	"Top 40",   "Christian Rap",   "Jungle",  "Native American",   "Cabaret",
	"New Wave","Psychadelic","Rave","Showtunes", "Trailer", "Lo-Fi","Tribal",
	"Acid Punk",  "Acid Jazz",  "Polka",  "Retro", "Musical",  "Rock & Roll",
	"Hard Rock","Folk","Folk/Rock",	"National Folk", "Swing","Bebob","Latin",
	"Revival",    "Celtic",     "Bluegrass",   "Avantgarde",   "Gothic Rock",
	"Progressive Rock",  "Psychadelic Rock",  "Symphonic Rock",  "Slow Rock",
	"Big Band", "Chorus", "Easy Listening",  "Acoustic",  "Humour", "Speech",
	"Chanson", "Opera", "Chamber Music",  "Sonata",	"Symphony", "Booty Bass",
	"Primus", "Porn Groove", "Satire", "Slow Jam",  "Club", "Tango", "Samba",
	"Folklore"
};

GenreList::GenreList() {
}

GenreList::~GenreList(){
}

BString GenreList::Genre(int genreindex){
	if((genreindex >= 0) && (genreindex < NUM_GENRES)){
		return genres[genreindex];
	}
	return genres[OTHER_INDEX];
}

int GenreList::Genre(BString genreToFind){
	if(!genreToFind){
		return OTHER_INDEX;
	}
	for(int i=0; i < NUM_GENRES; i++){
		if(strcmp(genreToFind,genres[i]) == 0){
			return i;
		}
	}
	return OTHER_INDEX;
}

int GenreList::NumGenres(){
	return NUM_GENRES;
}


int GenreList::GenreSort(const char* g1, const char* g2) {
	return strcmp(g1,g2);
}

/*
int GenreList::GenreSort(const void* g1, const void* g2) {
	char* genre1 = *((char**) g1);
	char* genre2 = *((char**) g2);
	return strcmp(genre1, genre2);
}
*/
