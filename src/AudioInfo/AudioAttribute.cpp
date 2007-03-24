#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <be/interface/InterfaceDefs.h>
#include <be/kernel/fs_attr.h>
#include <be/kernel/fs_index.h>
#include <be/storage/File.h>
#include <be/storage/Mime.h>
#include <be/storage/NodeInfo.h>
#include <be/support/Debug.h>
#include <be/support/TypeConstants.h>

#include "String/String.h"
#include "AudioAttribute.h"


AudioAttribute::AudioAttribute(BFile* file, const char* name,
		const char* attribute, type_code type)
{
	PRINT(("AudioAttribute::AudioAttribute(BFile*,const char*)\n"));

	strcpy(this->name,name);
	strcpy(this->attribute,attribute);
	this->type = type;
	this->file = file;
	this->value = 0;
}

AudioAttribute::~AudioAttribute()
{
	PRINT(("AudioAttribute::~AudioAttribute()\n"));

	delete [] value;
	value = 0;
}

bool
AudioAttribute::Exists()
{
	PRINT(("AudioAttribute::Exists()\n"));

	BNodeInfo nodeInfo(file);
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
		if(strcmp(name,this->attribute) == 0)
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

	BNodeInfo nodeInfo(file);
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

	attributes.AddString("attr:name",this->attribute);
	attributes.AddString("attr:public_name",this->name);
	attributes.AddInt32("attr:type",this->type);
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
			if(strcmp(entry->d_name,this->attribute) == 0)
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

	if(fs_create_index(device,this->attribute,this->type,0) != 0)
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

	if(fs_remove_index(device,this->attribute) != 0)
	{
		return B_ERROR;
	}

	return B_OK;
}

const char*
AudioAttribute::Value()
{
	PRINT(("AudioAttribute::Value()\n"));

	return this->value;
}

void
AudioAttribute::SetValue(const char* val)
{
	PRINT(("AudioAttribute::SetValue(const char*)\n"));

	delete [] this->value;
	this->value = 0;

	if(val && (strcmp(val,"") != 0))
	{
		String tmp = val;
		tmp.Trim();
		this->value = new char[tmp.Length()+1];
		strcpy(this->value,tmp.Value());
	}
}

status_t
AudioAttribute::Read()
{
	PRINT(("AudioAttribute::Read()\n"));

	if(!file->IsReadable())
	{
		PRINT(("File isn't readable.\n"));
		return B_ERROR;
	}

	attr_info info;
	if(file->GetAttrInfo(attribute,&info) != B_OK)
	{
		PRINT(("Can't get attribute info.\n"));
		return B_ERROR;
	}

	switch(info.type)
	{
		case B_STRING_TYPE:
			{
				delete [] value;
				value = 0;
				value = new char[info.size+1];
				memset(value,0,info.size+1);

				if(file->ReadAttr(attribute,B_STRING_TYPE,0,value,info.size) <= 0)
				{
					PRINT(("Error reading B_STRING_TYPE attribute %s.\n",attribute));
					return B_ERROR;
				}
			}
			break;
		case B_INT32_TYPE:
			{
				int32 buf;

				delete [] value;
				value = 0;
				value = new char[12]; //2^(32-1) is 10 chars + sign + null
				memset(value,0,12);

				if(file->ReadAttr(attribute,B_INT32_TYPE,0,&buf,info.size) <= 0)
				{
					PRINT(("Error reading B_INT32_TYPE attribute %s.\n",attribute));
					return B_ERROR;
				}
				sprintf(value,"%d",buf);
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

	if(!file->IsWritable())
	{
		PRINT(("File isn't writable.\n"));
		return B_ERROR;
	}

	switch(this->type)
	{
		case B_STRING_TYPE:
			{
				int len = 0;
				if(value)
				{
					len = strlen(value)+1;
					PRINT(("STR BUF = %s\n",value));
				}
				if(file->WriteAttr(attribute,B_STRING_TYPE,0,value,len) < len)
				{
					PRINT(("Error writing B_STRING_TYPE attribute %s\n",attribute));
					return B_ERROR;
				}
			}
			break;
		case B_INT32_TYPE:
			{
				int len = 0;
				int buf = 0;
				if(value)
				{
					len = sizeof(int32);
					buf = atol(value);
					PRINT(("STR = %s\n",value));
					PRINT(("INT BUF = %d\n",buf));
				}
				if(file->WriteAttr(attribute,B_INT32_TYPE,0,&buf,len) < len)
				{
					PRINT(("Error writing B_INT32_TYPE attribute %s\n",attribute));
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

