/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __AUDIO_ATTRIBUTE_H__
#define __AUDIO_ATTRIBUTE_H__

#include <SupportDefs.h>
#include <String.h>

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
		BFile*		m_file;
		char		m_name[256];
		char		m_attribute[B_ATTR_NAME_LENGTH];
		type_code	m_type;
		char*		m_value;

	private:
		void Trim(BString * string);
};

#endif

