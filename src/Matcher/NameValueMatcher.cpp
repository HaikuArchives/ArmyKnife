#include <stdio.h>
#include <string.h>
#include "NameValueMatcher.h"

NameValueMatcher::NameValueMatcher(const char* nameStr, const char* valueStr,
		char delim)
{
	_delim = delim;
	_name = 0;
	_value = 0;

	if(nameStr)
	{
		int len = strlen(nameStr);
		_name = new char[len+1];
		strcpy(_name,nameStr);
	}

	if(valueStr)
	{
		int len = strlen(valueStr);
		_value = new char[len+1];
		strcpy(_value,valueStr);
	}

	name_ptr = _name;
	value_ptr = _value;

	if(name_ptr && value_ptr)
	{
		while(*name_ptr == *value_ptr)
		{
			name_ptr++;
			value_ptr++;
		}
	}
}

NameValueMatcher::~NameValueMatcher()
{
	delete [] _name;
	delete [] _value;
}

int
NameValueMatcher::NextMatch(char* name, char* value)
{
	int iVal;
	int iTmp;
	int len;
	char* tmp = 0;

	if(!name_ptr || !value_ptr || (*name_ptr != _delim))
	{
		return -1;
	}

	name[0] = name_ptr[1];
	name[1] = 0;

	name_ptr += 2;

	iVal = 0;
	while((*name_ptr != *value_ptr) && (*value_ptr != 0))
	{
		value[iVal] = *value_ptr;
		value_ptr++;
		iVal++;
	}

	if(*name_ptr == *value_ptr == 0)
	{
		name_ptr = 0;
		value_ptr = 0;
	}

	if(name_ptr)
	{
		tmp = new char[strlen(name_ptr)+1];
		memset(tmp,0,strlen(name_ptr)+1);

		iTmp = 0;
		while((*name_ptr != _delim) && (*name_ptr != 0))
		{
			tmp[iTmp] = *name_ptr;
			name_ptr++;
			iTmp++;
		}
		tmp[iTmp] = 0;
	}

	if(tmp)
	{
		len = strlen(tmp);
		while((strncmp(value_ptr,tmp,len) != 0) && (*value_ptr != 0))
		{
			value[iVal] = *value_ptr;
			value_ptr++;
			iVal++;
		}
		value_ptr += len;
	}

	value[iVal] = 0;

	delete [] tmp;

	return 0;
}

