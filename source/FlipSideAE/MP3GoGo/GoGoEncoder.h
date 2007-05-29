#ifndef __GOGO_ENCODER_H__
#define __GOGO_ENCODER_H__

#include <stdio.h>
#include <AEEncoder/AEEncoder.h>

#define ADDON_NAME "MP3 GoGo"

#define BITRATE_STR "Bitrate"
#define _32KBPS  " 32 Kbps                             "
#define _48KBPS  " 48 Kbps                             "
#define _64KBPS  " 64 Kbps                             "
#define _96KBPS  " 96 Kbps                             "
#define _128KBPS "128 Kbps                             "
#define _160KBPS "160 Kbps                             "
#define _192KBPS "192 Kbps                             "
#define _256KBPS "256 Kbps                             "
#define _320KBPS "320 Kbps                             "
#define VBR_0    "Variable Bitrate 0 (High Quality)    "
#define VBR_1    "Variable Bitrate 1                   "
#define VBR_2    "Variable Bitrate 2                   "
#define VBR_3    "Variable Bitrate 3                   "
#define VBR_4    "Variable Bitrate 4                   "
#define VBR_5    "Variable Bitrate 5                   "
#define VBR_6    "Variable Bitrate 6                   "
#define VBR_7    "Variable Bitrate 7                   "
#define VBR_8    "Variable Bitrate 8                   "
#define VBR_9    "Variable Bitrate 9 (High Compression)"

#define OUTPUT_FORMAT_STR "Output Format"
#define STEREO  "Stereo  "
#define MONO    "Mono    "
#define JSTEREO "J-Stereo"

#define PSYCHO_ACOUSTICS_STR "Psycho Acoustics"

#define MP3_MIME_TYPE "audio/x-mpeg"
#define WAV_MIME_TYPE "audio/wav"
#define RIFF_WAV_MIME_TYPE "audio/x-wav"
#define RIFF_MIME_TYPE "audio/x-riff"

#define GOGO "gogo"
#define BITRATE_PREFIX "-b"
#define VBR_PREFIX "-v"
#define FORMAT_PREFIX "-m"
#define STEREO_CODE "s"
#define MONO_CODE "m"
#define JSTEREO_CODE "j"

class BMessage;
class BMessenger;

class GoGoEncoder : public AEEncoder
{
	public:
		GoGoEncoder();
		~GoGoEncoder();

		virtual int32 Encode(BMessage* encodeMessage);

	protected:
		virtual int32 LoadDefaultPattern();
		virtual int32 LoadDefaultMenu();

	private:
		char gogoPath[B_PATH_NAME_LENGTH+1];

		int32 GetBitrate(char* bitrate,bool* vbr);
		int32 GetFormat(char* format);
		int32 GetPsycho(bool* psycho);
		int32 UpdateStatus(FILE* out, BMessenger* messenger);
		int32 WriteDetails(BMessage* encodeMessage);
		thread_id CommandIO(int* filedes, int argc, const char** argv);
};

extern char **environ;

#endif
