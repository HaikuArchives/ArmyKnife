#ifndef __AUDIO_ATTRIBUTE_H__
#define __AUDIO_ATTRIBUTE_H__

#include <be/support/SupportDefs.h>

class BFile;

class AudioAttribute
{
	public:
		AudioAttribute(BFile* file, const char* name,
				const char* attribute, type_code type);
		virtual ~AudioAttribute();

		bool Exists();
		int32 Create();

		bool IsIndexed(dev_t device);
		int32 Index(dev_t device);
		int32 RemoveIndex(dev_t device);

		virtual const char* Value();
		virtual void SetValue(const char* value);

		virtual status_t Read();
		virtual status_t Write();

	protected:
		BFile* file;
		char name[256];
		char attribute[B_ATTR_NAME_LENGTH];
		type_code type;
		char* value;
};

#endif

