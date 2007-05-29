#ifndef __STATUS_BAR_FILTER_H__
#define __STATUS_BAR_FILTER_H__

#include <be/app/MessageFilter.h>

class BHandler;
class BMessage;

class StatusBarFilter : public BMessageFilter
{
	public:
		StatusBarFilter();
		virtual filter_result Filter(BMessage* message, BHandler** target);
};

#endif
