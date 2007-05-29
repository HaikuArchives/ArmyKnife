#ifndef __OGG_ENCODER_H__
#define __OGG_ENCODER_H__

#include <stdio.h>
#include <AEEncoder/AEEncoder.h>

#define ADDON_NAME "OGG Encoder"

#define BITRATE_STR "Bitrate"
#define _112KBPS "112 Kbps"
#define _128KBPS "128 Kbps"
#define _160KBPS "160 Kbps"
#define _192KBPS "192 Kbps"
#define _256KBPS "256 Kbps"
#define _350KBPS "320 Kbps"

#define OGG_MIME_TYPE "audio/x-vorbis"
#define WAV_MIME_TYPE "audio/wav"
#define RIFF_WAV_MIME_TYPE "audio/x-wav"
#define RIFF_MIME_TYPE "audio/x-riff"
#define AIFF_MIME_TYPE "audio/x-aiff"

#define OGG "oggenc"
#define BITRATE_PREFIX "-b"
#define OUTPUT_PREFIX "-o"
#define ARTIST_PREFIX "-a"
#define ALBUM_PREFIX "-l"
#define TITLE_PREFIX "-t"
#define YEAR_PREFIX "-d"
#define COMMENT_PREFIX "-c COMMENT="
#define TRACK_PREFIX "-N"
#define GENRE_PREFIX "-c GENRE="

extern char **environ;

struct argument
{
	//required
	const char* inputFile;
	const char* outputFile;
	int32 bitrate;

	//optional
	const char* artist;
	const char* album;
	const char* title;
	const char* year;
	const char* comment;
	const char* track;
	const char* genre;
};

class BMessage;

class OggEncoder : public AEEncoder
{
	public:
		OggEncoder();
		~OggEncoder();

		virtual int32 Encode(BMessage* encodeMessage);

	protected:
		virtual int32 LoadDefaultPattern();
		virtual int32 LoadDefaultMenu();

	private:
		char oggencPath[B_PATH_NAME_LENGTH+1];
		BMessenger messenger;

		int32 GetBitrate(char* bitrate);
		int32 GetArgs(argument* args, BMessage* encodeMessage);
		int32 UpdateStatus(FILE* out, BMessenger* messenger);
		int32 WriteDetails(argument* args);
		thread_id CommandIO(int* filedes, int argc, const char** argv);
};

#endif
