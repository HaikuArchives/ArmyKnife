#ifndef __NAME_VALUE_MATCHER_H__
#define __NAME_VALUE_MATCHER_H__

class NameValueMatcher
{
	public:
		NameValueMatcher(const char* nameStr, const char* valueStr, char delim);
		~NameValueMatcher();

		int NextMatch(char* name, char* value);

	private:
		char _delim;
		char* _name;
		char* _value;
		char* name_ptr;
		char* value_ptr;
};

#endif
