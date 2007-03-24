#ifndef __STRING_H__
#define __STRING_H__

#define NOT_FOUND -1

#include <Debug.h>

class String
{
public:
	String();
	String(const char* string);
	String(const String& string);
	~String();
	
	char CharAt(int index) const;
	int CompareTo(const char* string) const;
	int CompareTo(const String* string) const;
	String& Concat(const char* string);
	String& Concat(const String* string);
	bool EndsWith(const char* string) const;
	bool EndsWith(const String* string) const;
	int FindFirst(const char* string) const;
	int FindFirst(const String* string) const;
	int ICompareTo(const char* string) const;
	int ICompareTo(const String* string) const;
	int IFindFirst(const char* string) const;
	int IFindFirst(const String* string) const;
	bool InBounds(int index) const;
	int IndexOf(char c) const;
	int IndexOf(const char* string) const;
	int IndexOf(const String* string) const;
	int IndexOf(char c, int fromIndex) const;
	int IndexOf(const char* string, int fromIndex) const;
	int IndexOf(const String* string, int fromIndex) const;
	int LastIndexOf(char c) const;
	int LastIndexOf(const char* string) const;
	int LastIndexOf(const String* string) const;
	int LastIndexOf(char c, int fromIndex) const;
	int LastIndexOf(const char* string, int fromIndex) const;
	int LastIndexOf(const String* string, int fromIndex) const;
	int Length() const;
	String& ReplaceAll(char old, char newc);
	String& ReplaceAll(const char* old, const char* newc);
	String& ReplaceAll(const String* old, const String* newc);
	String& ReplaceFirst(char old, char newc);
	String& ReplaceFirst(const char* old, const char* newc);
	String& ReplaceFirst(const String* old, const String* newc);
	String& ReplaceLast(char old, char newc);
	String& ReplaceLast(const char* old, const char* newc);
	String& ReplaceLast(const String* old, const String* newc);
	String& SetTo(const char* string);
	String& SetTo(const String* string);
	bool StartsWith(const char* string) const;
	bool StartsWith(const String* string) const;
	String& Substring(int beginIndex);
	String& Substring(int beginIndex, int length);
	String& ToLowerCase();
	String& ToUpperCase();
	String& Trim();
	const char* Value() const;

	String& operator=(const int i);
	String& operator=(const long l);
	String& operator=(const float f);
	String& operator=(const double d);
	String& operator=(const char c);
	String& operator=(const char* string);
	String& operator=(const String& string);

	String& operator+=(const int i);
	String& operator+=(const long l);
	String& operator+=(const float f);
	String& operator+=(const double d);
	String& operator+=(const char c);
	String& operator+=(const char* string);
	String& operator+=(const String& string);

	String& operator<<(const int i);
	String& operator<<(const long l);
	String& operator<<(const float f);
	String& operator<<(const double d);
	String& operator<<(const char c);
	String& operator<<(const char* string);
	String& operator<<(const String& string);

	bool operator==(const char* string) const;
	bool operator==(const String& string) const;

	bool operator!=(const char* string) const;
	bool operator!=(const String& string) const;

	bool operator<(const char* string) const;
	bool operator<(const String& string) const;

	bool operator>(const char* string) const;
	bool operator>(const String& string) const;

	bool operator<=(const char* string) const;
	bool operator<=(const String& string) const;

	bool operator>=(const char* string) const;
	bool operator>=(const String& string) const;

	char operator[](const int index) const;

private:
	char* _string;
};

// declare global operators for convience
bool operator==(const char* left, const String& right);
bool operator!=(const char* left, const String& right);
bool operator<(const char* left, const String& right);
bool operator>(const char* left, const String& right);
bool operator<=(const char* left, const String& right);
bool operator>=(const char* left, const String& right);

// inline function definitions
inline int String::Length() const
{
	return strlen(_string);
}

inline const char* String::Value() const
{
	PRINT(("String Value = %s\n",_string));

	return _string;
}

#endif

