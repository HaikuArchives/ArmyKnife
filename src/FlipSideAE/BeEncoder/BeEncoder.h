#ifndef __BE_ENCODER_H__
#define __BE_ENCODER_H__

#include <stdio.h>
#include <AEEncoder/AEEncoder.h>

#define ADDON_NAME "BeEncoder"

#define ENCODER_SUBMENU "Encoder"

class BMessage;
class BMenuItem;

class BeEncoder : public AEEncoder
{
	public:
		BeEncoder();
		~BeEncoder();

		virtual int32 Encode(BMessage* encodeMessage);

	protected:
		virtual int32 LoadDefaultPattern();
		virtual int32 LoadDefaultMenu();

	private:
		int32 AddEncoderEntries(BMenu* encoderMenu);
		int32 WriteDetails(BMessage* msg, const char* mime, bool tags=false);
};

#endif
