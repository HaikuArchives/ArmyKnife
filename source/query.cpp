/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "query.h"

#include <Debug.h>

Query::Query()
	:
	m_buffer_size(-1),
	m_buffer_position(0)
{
	PRINT(("Query::Query()\n"));
}

Query::~Query()
{
	PRINT(("Query::~Query()\n"));
}

status_t
Query::HasNextResult()
{
	PRINT(("Query::HasNextResult()\n"));

	if (m_buffer_size == 0)
		return B_ERROR;

	if (m_buffer_position < m_buffer_size)
		return B_OK;

	m_buffer_position = 0;
	status_t result = RefillBuffer();
	m_buffer_size = m_buffer.size();
	return result;
}

Queryable
Query::GetNextResult()
{
	PRINT(("Query::GetNextResult()\n"));
	return m_buffer[m_buffer_position++];
}
