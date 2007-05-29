#include <string.h>
#include <be/app/Message.h>
#include <be/support/Archivable.h>
#include <be/support/Debug.h>
#include <be/support/TypeConstants.h>
#include "MediaMenuItem.h"

MediaMenuItem::MediaMenuItem(const char* label, media_file_format* mfi,
				media_codec_info* mci) : BMenuItem(label,NULL)
{
	PRINT(("MediaMenuItem::MediaMenuItem(const char*,media_file_format*,media_codec_info*)\n"));

	memcpy(&(this->mff),mfi,sizeof(this->mff));
	memcpy(&(this->mci),mci,sizeof(this->mci));
}

MediaMenuItem::MediaMenuItem(BMessage* archive) : BMenuItem(archive)
{
	PRINT(("MediaMenuItem::MediaMenuItem(BMessage*)\n"));

	media_file_format* format;
	media_codec_info* codec;
	ssize_t bytes;
	if(archive->FindData("format",(type_code)B_ANY_TYPE,
				(const void**)&format,&bytes) != B_OK)
	{
		PRINT(("Error retrieving format.\n"));
	}
	memcpy(&mff,format,sizeof(media_file_format));

	if(archive->FindData("codec",(type_code)B_ANY_TYPE,
				(const void**)&codec,&bytes) != B_OK)
	{
		PRINT(("Error retrieving codec.\n"));
	}
	memcpy(&mci,codec,sizeof(media_codec_info));
}

BArchivable*
MediaMenuItem::Instantiate(BMessage* archive)
{
	PRINT(("MediaMenuItem::Instantiate(BMessage*)\n"));

	if(!validate_instantiation(archive,"MediaMenuItem"))
	{
		return 0;
	}

	return new MediaMenuItem(archive);
}

status_t
MediaMenuItem::Archive(BMessage* archive, bool deep=true) const
{
	PRINT(("MediaMenuItem::Archive(BMessage*,bool)\n"));

	status_t status = BMenuItem::Archive(archive,deep);

	if(status == B_OK)
	{
		status = archive->AddData("format",B_MEDIA_PARAMETER_TYPE,&mff,
				sizeof(media_file_format));
	}

	if(status == B_OK)
	{
		status = archive->AddData("codec",B_MEDIA_PARAMETER_TYPE,&mci,
				sizeof(media_codec_info));
	}

	return status;
}

const media_file_format*
MediaMenuItem::MediaFileFormat()
{
	PRINT(("MediaMenuItem::MediaFileFormat()\n"));

	return &mff;
}

const media_codec_info*
MediaMenuItem::MediaCodecInfo()
{
	PRINT(("MediaMenuItem::MediaCodecInfo()\n"));

	return &mci;
}

