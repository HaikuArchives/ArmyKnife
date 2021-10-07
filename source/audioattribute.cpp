/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fs_attr.h>
#include <fs_index.h>

#include <Debug.h>
#include <File.h>
#include <InterfaceDefs.h>
#include <Mime.h>
#include <NodeInfo.h>
#include <TypeConstants.h>
#include <String.h>

#include "audioattribute.h"


AudioAttribute::AudioAttribute(BFile* file, const char* name,
		const char* attribute, type_code type)
{
	PRINT(("AudioAttribute::AudioAttribute(BFile*,const char*)\n"));

	strcpy(this->m_name,name);
	strcpy(this->m_attribute,attribute);
	this->m_type = type;
	this->m_file = file;
	this->m_value = 0;
}

AudioAttribute::~AudioAttribute()
{
	PRINT(("AudioAttribute::~AudioAttribute()\n"));

	delete [] m_value;
	m_value = 0;
}

bool
AudioAttribute::Exists()
{
	PRINT(("AudioAttribute::Exists()\n"));

	BNodeInfo nodeInfo(m_file);
	if(nodeInfo.InitCheck() != B_OK)
	{
		return false;
	}

	char type[B_MIME_TYPE_LENGTH];
	if(nodeInfo.GetType(type) != B_OK)
	{
		return false;
	}

	BMimeType mimeType(type);
	if(mimeType.InitCheck() != B_OK)
	{
		return false;
	}

	BMessage attributes;
	if(mimeType.GetAttrInfo(&attributes) != B_OK)
	{
		return false;
	}

	const char* name;
	int32 index = 0;
	while(attributes.FindString("attr:name",index,&name) == B_OK)
	{
		if(strcmp(name,this->m_attribute) == 0)
		{
			return true;
		}
		index++;
	}

	return false;
}

int32
AudioAttribute::Create()
{
	PRINT(("AudioAttribute::Create()\n"));

	BNodeInfo nodeInfo(m_file);
	if(nodeInfo.InitCheck() != B_OK)
	{
		return B_ERROR;
	}

	char type[B_MIME_TYPE_LENGTH];
	if(nodeInfo.GetType(type) != B_OK)
	{
		return B_ERROR;
	}

	BMimeType mimeType(type);
	if(mimeType.InitCheck() != B_OK)
	{
		return B_ERROR;
	}

	BMessage attributes;
	if(mimeType.GetAttrInfo(&attributes) != B_OK)
	{
		return B_ERROR;
	}

	attributes.AddString("attr:name",this->m_attribute);
	attributes.AddString("attr:public_name",this->m_name);
	attributes.AddInt32("attr:type",this->m_type);
	attributes.AddBool("attr:public",true);
	attributes.AddBool("attr:viewable",true);
	attributes.AddBool("attr:editable",true);
	attributes.AddInt32("attr:width",30);
	attributes.AddInt32("attr:alignment",B_ALIGN_LEFT);
	attributes.AddBool("attr:extra",false);

	if(mimeType.SetAttrInfo(&attributes) != B_OK)
	{
		return B_ERROR;
	}

	return B_OK;
}

bool
AudioAttribute::IsIndexed(dev_t device)
{
	PRINT(("AudioAttribute::IsIndexed(dev_t)\n"));

	bool found = false;
	DIR* index_dir = fs_open_index_dir(device);
	if(index_dir)
	{
		dirent* entry;
		while(entry = fs_read_index_dir(index_dir))
		{
			if(strcmp(entry->d_name, this->m_attribute) == 0)
			{
				found = true;
				break;
			}
		}
		fs_close_index_dir(index_dir);
	}

	return found;
}

int32
AudioAttribute::Index(dev_t device)
{
	PRINT(("AudioAttribute::Index(dev_t)\n"));

	if(IsIndexed(device))
	{
		return B_OK;
	}

	if(fs_create_index(device, this->m_attribute, this->m_type, 0) != 0)
	{
		return B_ERROR;
	}

	return B_OK;
}

int32
AudioAttribute::RemoveIndex(dev_t device)
{
	PRINT(("AudioAttribute::RemoveIndex(dev_t)\n"));

	if(!IsIndexed(device))
	{
		return B_OK;
	}

	if(fs_remove_index(device, this->m_attribute) != 0)
	{
		return B_ERROR;
	}

	return B_OK;
}

const char*
AudioAttribute::Value()
{
	PRINT(("AudioAttribute::Value()\n"));

	return this->m_value;
}

void
AudioAttribute::SetValue(const char* val)
{
	PRINT(("AudioAttribute::SetValue(const char*)\n"));

	delete [] this->m_value;
	this->m_value = 0;

	if(val && (strcmp(val,"") != 0))
	{
		BString tmp = val;
		Trim(&tmp);
		this->m_value = new char[tmp.Length()+1];
		strcpy(this->m_value, tmp.String());
	}
}

status_t
AudioAttribute::Read()
{
	PRINT(("AudioAttribute::Read()\n"));

	if(! m_file->IsReadable())
	{
		PRINT(("File isn't readable.\n"));
		return B_ERROR;
	}

	attr_info info;
	if(m_file->GetAttrInfo(m_attribute, &info) != B_OK)
	{
		PRINT(("Can't get attribute info.\n"));
		return B_ERROR;
	}

	switch(info.type)
	{
		case B_STRING_TYPE:
			{
				delete [] m_value;
				m_value = 0;
				m_value = new char[info.size+1];
				memset(m_value, 0, info.size+1);

				if(m_file->ReadAttr(m_attribute, B_STRING_TYPE, 0, m_value, info.size) <= 0)
				{
					PRINT(("Error reading B_STRING_TYPE attribute %s.\n", m_attribute));
					return B_ERROR;
				}
			}
			break;
		case B_INT32_TYPE:
			{
				int32 buf;

				delete [] m_value;
				m_value = 0;
				m_value = new char[12]; //2^(32-1) is 10 chars + sign + null
				memset(m_value, 0, 12);

				if(m_file->ReadAttr(m_attribute, B_INT32_TYPE, 0, &buf, info.size) <= 0)
				{
					PRINT(("Error reading B_INT32_TYPE attribute %s.\n", m_attribute));
					return B_ERROR;
				}
				sprintf(m_value,"%d",buf);
			}
			break;
		default:
			PRINT(("Attribute type isn't B_STRING_TYPE or B_INT32_TYPE.\n"));
			return B_ERROR;
	}

	return B_OK;
}

status_t
AudioAttribute::Write()
{
	PRINT(("AudioAttribute::Write()\n"));

	if(! m_file->IsWritable())
	{
		PRINT(("File isn't writable.\n"));
		return B_ERROR;
	}

	switch(this->m_type)
	{
		case B_STRING_TYPE:
			{
				int len = 0;
				if(m_value)
				{
					len = strlen(m_value)+1;
					PRINT(("STR BUF = %s\n", m_value));
				}
				if(m_file->WriteAttr(m_attribute, B_STRING_TYPE, 0, m_value, len) < len)
				{
					PRINT(("Error writing B_STRING_TYPE attribute %s\n", m_attribute));
					return B_ERROR;
				}
			}
			break;
		case B_INT32_TYPE:
			{
				int len = 0;
				int buf = 0;
				if(m_value)
				{
					len = sizeof(int32);
					buf = atol(m_value);
					PRINT(("STR = %s\n", m_value));
					PRINT(("INT BUF = %d\n", buf));
				}
				if(m_file->WriteAttr(m_attribute, B_INT32_TYPE, 0, &buf, len) < len)
				{
					PRINT(("Error writing B_INT32_TYPE attribute %s\n", m_attribute));
					return B_ERROR;
				}
			}
			break;
		default:
			PRINT(("Attribute type isn't B_STRING_TYPE or B_INT32_TYPE.\n"));
			return B_ERROR;
	}

	return B_OK;
}

void AudioAttribute::Trim(BString * string)
{
	while(string->FindFirst(' ') == 0)
		if (string->Length() > 0)
			string->Remove(0,1);
		else
			break;

	while(string->FindLast(' ') == (string->Length()-1))
		if (string->Length() > 0)
			string->RemoveLast(" ");
		else
			break;
}

