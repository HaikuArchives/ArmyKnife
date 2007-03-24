#include <stdio.h>
#include <be/support/Debug.h>
#include "AudioInfo.h"

AudioInfo::AudioInfo()
{
	PRINT(("AudioInfo::AudioInfo()\n"));
}

AudioInfo::~AudioInfo()
{
	PRINT(("AudioInfo::~AudioInfo()\n"));
}

const char*
AudioInfo::Artist()
{
	PRINT(("AudioInfo::Artist()\n"));

	return 0;
}

const char*
AudioInfo::Album()
{
	PRINT(("AudioInfo::Album()\n"));

	return 0;
}

const char*
AudioInfo::Title()
{
	PRINT(("AudioInfo::Title()\n"));

	return 0;
}

const char*
AudioInfo::Year()
{
	PRINT(("AudioInfo::Year()\n"));

	return 0;
}

const char*
AudioInfo::Comment()
{
	PRINT(("AudioInfo::Comment()\n"));

	return 0;
}

const char*
AudioInfo::Track()
{
	PRINT(("AudioInfo::Track()\n"));

	return 0;
}

const char*
AudioInfo::Genre()
{
	PRINT(("AudioInfo::Genre()\n"));

	return 0;
}

void
AudioInfo::SetArtist(const char* value)
{
	PRINT(("AudioInfo::SetArtist(const char*)\n"));
}

void
AudioInfo::SetAlbum(const char* value)
{
	PRINT(("AudioInfo::SetAlbum(const char*)\n"));
}

void
AudioInfo::SetTitle(const char* value)
{
	PRINT(("AudioInfo::SetTitle(const char*)\n"));
}

void
AudioInfo::SetYear(const char* value)
{
	PRINT(("AudioInfo::SetYear(const char*)\n"));
}

void
AudioInfo::SetComment(const char* value)
{
	PRINT(("AudioInfo::SetComment(const char*)\n"));
}

void
AudioInfo::SetTrack(const char* value)
{
	PRINT(("AudioInfo::SetTrack(const char*)\n"));
}

void
AudioInfo::SetGenre(const char* value)
{
	PRINT(("AudioInfo::SetGenre(const char*)\n"));
}

status_t
AudioInfo::Read()
{
	PRINT(("AudioInfo::Read()\n"));

	int status;
	int retcode = B_OK;

	status = ReadArtist();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = ReadAlbum();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = ReadTitle();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = ReadYear();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = ReadComment();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = ReadTrack();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = ReadGenre();
	if(status != B_OK)
	{
		retcode = status;
	}

	return retcode;
}

status_t
AudioInfo::ReadArtist()
{
	PRINT(("AudioInfo::ReadArtist()\n"));

	return B_OK;
}

status_t
AudioInfo::ReadAlbum()
{
	PRINT(("AudioInfo::ReadAlbum()\n"));

	return B_OK;
}

status_t
AudioInfo::ReadTitle()
{
	PRINT(("AudioInfo::ReadTitle()\n"));

	return B_OK;
}

status_t
AudioInfo::ReadYear()
{
	PRINT(("AudioInfo::ReadYear()\n"));

	return B_OK;
}

status_t
AudioInfo::ReadComment()
{
	PRINT(("AudioInfo::ReadComment()\n"));

	return B_OK;
}

status_t
AudioInfo::ReadTrack()
{
	PRINT(("AudioInfo::ReadTrack()\n"));

	return B_OK;
}

status_t
AudioInfo::ReadGenre()
{
	PRINT(("AudioInfo::ReadGenre()\n"));

	return B_OK;
}

status_t
AudioInfo::Write()
{
	PRINT(("AudioInfo::Write()\n"));

	int status;
	int retcode = B_OK;

	status = WriteArtist();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = WriteAlbum();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = WriteTitle();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = WriteYear();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = WriteComment();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = WriteTrack();
	if(status != B_OK)
	{
		retcode = status;
	}
	status = WriteGenre();
	if(status != B_OK)
	{
		retcode = status;
	}
	return retcode;
}

status_t
AudioInfo::WriteArtist()
{
	PRINT(("AudioInfo::WriteArtist()\n"));

	return B_OK;
}

status_t
AudioInfo::WriteAlbum()
{
	PRINT(("AudioInfo::WriteAlbum()\n"));

	return B_OK;
}

status_t
AudioInfo::WriteTitle()
{
	PRINT(("AudioInfo::WriteTitle()\n"));

	return B_OK;
}

status_t
AudioInfo::WriteYear()
{
	PRINT(("AudioInfo::WriteYear()\n"));

	return B_OK;
}

status_t
AudioInfo::WriteComment()
{
	PRINT(("AudioInfo::WriteComment()\n"));

	return B_OK;
}

status_t
AudioInfo::WriteTrack()
{
	PRINT(("AudioInfo::WriteTrack()\n"));

	return B_OK;
}

status_t
AudioInfo::WriteGenre()
{
	PRINT(("AudioInfo::WriteGenre()\n"));

	return B_OK;
}

void
AudioInfo::PrintToStream()
{
	printf("\n");
	printf("Artist  = %s\n",Artist());
	printf("Album   = %s\n",Album());
	printf("Title   = %s\n",Title());
	printf("Year    = %s\n",Year());
	printf("Comment = %s\n",Comment());
	printf("Track   = %s\n",Track());
	printf("Genre   = %s\n",Genre());
}

