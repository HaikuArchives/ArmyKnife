#ifndef _QUERY_H__
#define _QUERY_H__

#include <vector>

#include <SupportDefs.h>

#include "queryable.h"

class Query
{
	public:
		Query();
		virtual ~Query();

		virtual status_t HasNextResult();
		virtual Queryable GetNextResult();

	protected:
		virtual status_t RefillBuffer() = 0;

		std::vector<Queryable> m_buffer;
		int32 m_buffer_size;

	private:
		int32 m_buffer_position;
};

#endif

