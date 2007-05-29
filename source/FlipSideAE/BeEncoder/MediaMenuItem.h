#ifndef __MEDIA_MENU_ITEM_H__
#define __MEDIA_MENU_ITEM_H__

#include <be/interface/MenuItem.h>
#include <be/media/MediaFormats.h>

class BArchivable;
class BMessage;

class MediaMenuItem : public BMenuItem
{
	public:
		MediaMenuItem(const char* label, media_file_format* mfi,
				media_codec_info* mci);
		MediaMenuItem(BMessage* archive);
		virtual status_t Archive(BMessage* archive, bool deep=true) const;
		static BArchivable* Instantiate(BMessage* archive);

		const media_file_format* MediaFileFormat();
		const media_codec_info* MediaCodecInfo();

	private:
		media_file_format mff;
		media_codec_info mci;
};

#endif
