#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "String.h"

String::String()
{
	PRINT(("String::String()\n"));

	_string = 0;
	SetTo("");
}

String::String(const char *string)
{
	PRINT(("String::String(const char*)\n"));

	_string = 0;
	SetTo(string);
}

String::String(const String& string)
{
	PRINT(("String::String(const String&)\n"));

	_string = 0;
	SetTo(string.Value());
}

String::~String()
{
	PRINT(("String::~String()\n"));

	delete [] _string;
	_string = 0;
}

char String::CharAt(int index) const
{
	PRINT(("String::CharAt(int)\n"));

	if(InBounds(index))
	{
		return _string[index];
	}

	return 0;
}

int String::CompareTo(const char* string) const
{
	PRINT(("String::CompareTo(const char*)\n"));

	return strcmp(_string,string);
}

int String::CompareTo(const String* string) const
{
	PRINT(("String::CompareTo(const String*)\n"));

	if(!string)
	{
		return 1;
	}

	return CompareTo(string->Value());
}

String& String::Concat(const char* string)
{
	PRINT(("String::Concat(const char*)\n"));

	if(!string)
	{
		return *this;
	}

	int size = Length() + strlen(string) + 1;
	char* tmp = new char[size];
	strcpy(tmp,_string);
	strcat(tmp,string);

	SetTo(tmp);

	delete [] tmp;

	return *this;
}

String& String::Concat(const String* string)
{
	PRINT(("String::String(const String*)\n"));

	if(!string)
	{
		return *this;
	}

	return Concat(string->Value());
}

bool String::EndsWith(const char* string) const
{
	PRINT(("String::EndsWith(const char*)\n"));

	if(!string)
	{
		return false;
	}

	int index = Length() - strlen(string);
	if(!InBounds(index))
	{
		return false;
	}

	if(strcmp(&(_string[index]),string) == 0)
	{
		return true;
	}

	return false;
}

bool String::EndsWith(const String* string) const
{
	PRINT(("String::EndsWith(const String*)\n"));

	if(!string)
	{
		return false;
	}

	return EndsWith(string->Value());
}

int String::FindFirst(const char* string) const
{
	PRINT(("String::FindFirst(const char*)\n"));
	
	if((!string) || (string[0] == 0))
	{
		return NOT_FOUND;
	}

	int len = strlen(string);

	if(Length() < len)
	{
		return NOT_FOUND;
	}

	int i = 0;
	int index = len-2;
	if(index < 0)
	{
		index = 0;
	}

	char* tmp = _string;
	while((tmp[index] != 0) && (strncmp(tmp,string,len) != 0))
	{
		i++;
		tmp++;
	}

	if(strncmp(tmp,string,len) == 0)
	{
		return i;
	}

	return NOT_FOUND;
}

int String::FindFirst(const String* string) const
{
	PRINT(("String::FindFirst(const String*)\n"));

	if(!string)
	{
		return NOT_FOUND;
	}

	return FindFirst(string->Value());
}

int String::ICompareTo(const char* string) const
{
	PRINT(("String::ICompareTo(const char*)\n"));

	if(!string)
	{
		return 1;
	}

	String left(*this);
	String right(string);

	left.ToLowerCase();
	right.ToLowerCase();

	return left.CompareTo(&right);
}

int String::ICompareTo(const String* string) const
{
	PRINT(("String::ICompareTo(const String*)\n"));

	if(!string)
	{
		return 1;
	}

	return ICompareTo(string->Value());
}

int String::IFindFirst(const char* string) const
{
	PRINT(("String::IFindFirst(const char*)\n"));
	
	String str(string);
	str.ToUpperCase();
	String str2(*this);
	str2.ToUpperCase();

	return str.FindFirst(str2.Value());
}

int String::IFindFirst(const String* string) const
{
	PRINT(("String::IFindFirst(const String*)\n"));

	if(!string)
	{
		return NOT_FOUND;
	}

	return IFindFirst(string->Value());
}

bool String::InBounds(int index) const
{
	PRINT(("String::InBounds(int)\n"));

	if((index >= 0) && (index < Length()))
	{
		return true;
	}

	return false;
}

int String::IndexOf(char c) const
{
	PRINT(("String::IndexOf(char)\n"));

	return IndexOf(c,0);
}

int String::IndexOf(const char* string) const
{
	PRINT(("String::IndexOf(const char*)\n"));

	return IndexOf(string,0);
}

int String::IndexOf(const String* string) const
{
	PRINT(("String::IndexOf(const String*)\n"));

	return IndexOf(string,0);
}

int String::IndexOf(char c, int fromIndex) const
{
	PRINT(("String::IndexOf(char,int)\n"));

	int len = Length();
	for(int i=fromIndex;i<len;i++)
	{
		if(_string[i] == c)
		{
			return i;
		}
	}
	return NOT_FOUND;
}

int String::IndexOf(const char* string, int fromIndex) const
{
	PRINT(("String::IndexOf(const char*,int)\n"));

	if(!string)
	{
		return NOT_FOUND;
	}

	if(!InBounds(fromIndex))
	{
		return NOT_FOUND;
	}

	int len = Length();
	int strLen = strlen(string);
	int max = len - strLen;
	for(int i=fromIndex;i<=max;i++)
	{
		if(strncmp(&(_string[i]),string,strLen) == 0)
		{
			return i;
		}
	}

	return NOT_FOUND;
}

int String::IndexOf(const String* string, int fromIndex) const
{
	PRINT(("String::IndexOf(const String*,int)\n"));

	if(!string)
	{
		return NOT_FOUND;
	}

	return IndexOf(string->Value(),fromIndex);
}

int String::LastIndexOf(char c) const
{
	PRINT(("String::LastIndexOf(char)\n"));

	return LastIndexOf(c,Length()-1);
}

int String::LastIndexOf(const char* string) const
{
	PRINT(("String::LastIndexOf(const char*)\n"));

	return LastIndexOf(string,Length()-1);
}

int String::LastIndexOf(const String* string) const
{
	PRINT(("String::LastIndexOf(const String*)\n"));

	return LastIndexOf(string,Length()-1);
}

int String::LastIndexOf(char c, int fromIndex) const
{
	PRINT(("String::LastIndexOf(char,int)\n"));

	if(!InBounds(fromIndex))
	{
		return NOT_FOUND;
	}

	for(int i=fromIndex;i>=0;i--)
	{
		if(_string[i] == c)
		{
			return i;
		}
	}

	return NOT_FOUND;
}

int String::LastIndexOf(const char* string, int fromIndex) const
{
	PRINT(("String::LastIndexOf(const char*,int)\n"));

	if(!string)
	{
		return NOT_FOUND;
	}

	if(!InBounds(fromIndex))
	{
		return NOT_FOUND;
	}

	int len = Length();
	int strLen = strlen(string);
	int max = len - strLen;
	for(int i=fromIndex;i>=0;i--)
	{
		if(strncmp(&(_string[i]),string,strLen) == 0)
		{
			return i;
		}
	}

	return NOT_FOUND;
}

int String::LastIndexOf(const String* string, int fromIndex) const
{
	PRINT(("String::LastIndexOf(const String*,int)\n"));

	if(!string)
	{
		return NOT_FOUND;
	}

	return LastIndexOf(string->Value(),fromIndex);
}

String& String::ReplaceAll(char old, char newc)
{
	PRINT(("String::ReplaceAll(char,char)\n"));

	int index;
	while((index = IndexOf(old)) != NOT_FOUND)
	{
		_string[index] = newc;
	}

	return *this;
}

String& String::ReplaceAll(const char* old, const char* newc)
{
	PRINT(("String::ReplaceAll(const char*,const char*)\n"));

	return *this;
}

String& String::ReplaceAll(const String* old, const String* newc)
{
	PRINT(("String::ReplaceAll(const String*,const String*)\n"));

	return *this;
}

String& String::ReplaceFirst(char old, char newc)
{
	PRINT(("String::ReplaceFirst(char,char)\n"));

	int index = IndexOf(old);
	if(index != NOT_FOUND)
	{
		_string[index] = newc;
	}

	return *this;
}

String& String::ReplaceFirst(const char* old, const char* newc)
{
	PRINT(("String::ReplaceFirst(const char*,const char*)\n"));

	return *this;
}

String& String::ReplaceFirst(const String* old, const String* newc)
{
	PRINT(("String::ReplaceFirst(const String*,const String*)\n"));

	return *this;
}

String& String::ReplaceLast(char old, char newc)
{
	PRINT(("String::ReplaceLast(char,char)\n"));

	int index = LastIndexOf(old);
	if(index != NOT_FOUND)
	{
		_string[index] = newc;
	}

	return *this;
}

String& String::ReplaceLast(const char* old, const char* newc)
{
	PRINT(("String::ReplaceLast(const char*,const char*)\n"));

	return *this;
}

String& String::ReplaceLast(const String* old, const String* newc)
{
	PRINT(("String::ReplaceLast(const String*,const String*)\n"));

	return *this;
}

String& String::SetTo(const char* string)
{
	PRINT(("String::SetTo(const char*)\n"));

	int len;
	const char* value;
	if(string)
	{
		value = string;
	}
	else
	{
		value = "";
	}
	len = strlen(value);

	delete [] _string;
	_string = new char[len + 1];
	strcpy(_string,value);
	return *this;
}

String& String::SetTo(const String* string)
{
	PRINT(("String::SetTo(const String*)\n"));

	if(!string)
	{
		return SetTo("");
	}

	return SetTo(string->Value());
}

bool String::StartsWith(const char* string) const
{
	PRINT(("String::StartsWith(const char*)\n"));

	if(!string)
	{
		return false;
	}

	int len = strlen(string);
	if(strncmp(_string,string,len) == 0)
	{
		return true;
	}

	return false;
}

bool String::StartsWith(const String* string) const
{
	PRINT(("String::StartsWith(const String*)\n"));

	if(!string)
	{
		return false;
	}

	return StartsWith(string->Value());
}

String& String::Substring(int beginIndex)
{
	PRINT(("String::Substring(int)\n"));

	return Substring(beginIndex,0);
}

String& String::Substring(int beginIndex, int length)
{
	PRINT(("String::Substring(int,int)\n"));

	if((!InBounds(beginIndex)) || (length < 0))
	{
		return *this;
	}

	if(length == 0)
	{
		length = Length() - beginIndex;
	}

	char* tmp = new char[length + 1];
	memset(tmp,0,length+1);
	strncpy(tmp,&(_string[beginIndex]),length);
	SetTo(tmp);
	delete [] tmp;

	return *this;
}

String& String::ToLowerCase()
{
	PRINT(("String::ToLowerCase()\n"));

	int len = Length();
	for(int i=0;i<len;i++)
	{
		_string[i] = tolower(_string[i]);
	}
	return *this;
}

String& String::ToUpperCase()
{
	PRINT(("String::ToUpperCase()\n"));

	int len = Length();
	for(int i=0;i<len;i++)
	{
		_string[i] = toupper(_string[i]);
	}
	return *this;
}

String& String::Trim()
{
	PRINT(("String::Trim()\n"));

	int len = Length();

	if(len == 0)
	{
		return *this;
	}

	if((len == 1) && (!isspace(_string[0])))
	{
		return *this;
	}

	int start = 0;
	while(isspace(_string[start]) && (start < len))
	{
		start++;
	}

	int end = len - 1;
	while(isspace(_string[end]) && (end >= 0))
	{
		end--;
	}

	if(start >= end)
	{
		SetTo("");
	}
	else
	{
		Substring(start,(end-start+1));
	}

	return *this;
}

String& String::operator=(const int i)
{
	PRINT(("String::operator=(const int)\n"));

	char value[50];
	sprintf(value,"%d",i);
	return SetTo(value);
}

String& String::operator=(const long l)
{
	PRINT(("String::operator=(const long)\n"));

	char value[50];
	sprintf(value,"%d",l);
	return SetTo(value);
}

String& String::operator=(const float f)
{
	PRINT(("String::operator=(const float)\n"));

	char value[50];
	sprintf(value,"%.2f",f);
	return SetTo(value);
}

String& String::operator=(const double d)
{
	PRINT(("String::operator=(const double)\n"));

	char value[50];
	sprintf(value,"%.2f",d);
	return SetTo(value);
}

String& String::operator=(const char c)
{
	PRINT(("String::operator=(const char)\n"));

	char value[2];
	sprintf(value,"%c",c);
	return SetTo(value);
}

String& String::operator=(const char* string)
{
	PRINT(("String::operator=(const char*)\n"));

	return SetTo(string);
}

String& String::operator=(const String& string)
{
	PRINT(("String::operator=(const String&)\n"));

	return SetTo(&string);
}

String& String::operator+=(const int i)
{
	PRINT(("String::operator+=(const int)\n"));

	char value[50];
	sprintf(value,"%d",i);
	return Concat(value);
}

String& String::operator+=(const long l)
{
	PRINT(("String::operator+=(const long)\n"));

	char value[50];
	sprintf(value,"%d",l);
	return Concat(value);
}

String& String::operator+=(const float f)
{
	PRINT(("String::operator+=(const float)\n"));

	char value[50];
	sprintf(value,"%.2f",f);
	return Concat(value);
}

String& String::operator+=(const double d)
{
	PRINT(("String::operator+=(const double)\n"));

	char value[50];
	sprintf(value,"%.2f",d);
	return Concat(value);
}

String& String::operator+=(const char c)
{
	PRINT(("String::operator+=(const char)\n"));

	char value[2];
	sprintf(value,"%c",c);
	return Concat(value);
}

String& String::operator+=(const char* string)
{
	PRINT(("String::operator+=(const char*)\n"));

	return Concat(string);
}

String& String::operator+=(const String& string)
{
	PRINT(("String::operator+=(const String&)\n"));

	return Concat(&string);
}

String& String::operator<<(const int i)
{
	PRINT(("String::operator<<(const int)\n"));

	char value[50];
	sprintf(value,"%d",i);
	return Concat(value);
}

String& String::operator<<(const long l)
{
	PRINT(("String::operator<<(const long)\n"));

	char value[50];
	sprintf(value,"%d",l);
	return Concat(value);
}

String& String::operator<<(const float f)
{
	PRINT(("String::operator<<(const float)\n"));

	char value[50];
	sprintf(value,"%.2f",f);
	return Concat(value);
}

String& String::operator<<(const double d)
{
	PRINT(("String::operator<<(const double)\n"));

	char value[50];
	sprintf(value,"%.2f",d);
	return Concat(value);
}

String& String::operator<<(const char c)
{
	PRINT(("String::operator<<(const char)\n"));

	char value[2];
	sprintf(value,"%c",c);
	return Concat(value);
}

String& String::operator<<(const char* string)
{
	PRINT(("String::operator<<(const char*)\n"));

	return Concat(string);
}

String& String::operator<<(const String& string)
{
	PRINT(("String::operator<<(const String&)\n"));

	return Concat(&string);
}

bool String::operator==(const char* string) const
{
	PRINT(("String::operator==(const char*)\n"));

	return (CompareTo(string) == 0);
}

bool String::operator==(const String& string) const
{
	PRINT(("String::operator==(const String&)\n"));

	return (CompareTo(&string) == 0);
}

bool String::operator!=(const char* string) const
{
	PRINT(("String::operator!=(const char*)\n"));

	return (CompareTo(string) != 0);
}

bool String::operator!=(const String& string) const
{
	PRINT(("String::operator!=(const String&)\n"));

	return (CompareTo(&string) != 0);
}

bool String::operator<(const char* string) const
{
	PRINT(("String::operator<(const char*)\n"));

	return (CompareTo(string) < 0);
}

bool String::operator<(const String& string) const
{
	PRINT(("String::operator<(const String&)\n"));

	return (CompareTo(&string) < 0);
}

bool String::operator>(const char* string) const
{
	PRINT(("String::operator>(const char*)\n"));

	return (CompareTo(string) > 0);
}

bool String::operator>(const String& string) const
{
	PRINT(("String::operator>(const String&)\n"));

	return (CompareTo(&string) > 0);
}

bool String::operator<=(const char* string) const
{
	PRINT(("String::operator<=(const char*)\n"));

	return (CompareTo(string) <= 0);
}

bool String::operator<=(const String& string) const
{
	PRINT(("String::operator<=(const String&)\n"));

	return (CompareTo(&string) <= 0);
}

bool String::operator>=(const char* string) const
{
	PRINT(("String::operator>=(const char*)\n"));

	return (CompareTo(string) >= 0);
}

bool String::operator>=(const String& string) const
{
	PRINT(("String::operator>=(const String&)\n"));

	return (CompareTo(&string) >= 0);
}

char String::operator[](const int index) const
{
	PRINT(("String::operator[](const int)\n"));

	return CharAt(index);
}

bool operator==(const char* left, const String& right)
{
	PRINT(("operator==(const char*,const String&\n"));

	return strcmp(left,right.Value()) == 0;
}

bool operator!=(const char* left, const String& right)
{
	PRINT(("operator!=(const char*,const String&\n"));

	return strcmp(left,right.Value()) != 0;
}

bool operator<(const char* left, const String& right)
{
	PRINT(("operator<(const char*,const String&\n"));

	return strcmp(left,right.Value()) < 0;
}

bool operator>(const char* left, const String& right)
{
	PRINT(("operator>(const char*,const String&\n"));

	return strcmp(left,right.Value()) > 0;
}

bool operator<=(const char* left, const String& right)
{
	PRINT(("operator<=(const char*,const String&\n"));

	return strcmp(left,right.Value()) <= 0;
}

bool operator>=(const char* left, const String& right)
{
	PRINT(("operator>=(const char*,const String&\n"));

	return strcmp(left,right.Value()) >= 0;
}

