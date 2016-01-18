#ifndef __QUERY_WINDOW_H__
#define __QUERY_WINDOW_H__

#include <Messenger.h>
#include <ObjectList.h>
#include <Window.h>

#include "queryable.h"

class BButton;
class BColumnListView;
class Query;

class QueryWindow : public BWindow
{
	public:
		QueryWindow(Query* query, BMessenger replyTo);
		~QueryWindow();

		void MessageReceived(BMessage* message);

	private:
		void ShowMore();

		static const int32 k_update_size = 20;

		BMessenger m_reply_to;
		BColumnListView* m_result_view;
		BButton* m_more;
		BButton* m_apply;

		class Worker : public BLooper
		{
			friend class QueryWindow;

			public:
				Worker(Query* query, BMessenger reply_to);
				~Worker();

				void MessageReceived(BMessage* message);

			private:
				BMessenger m_reply_to;
				Query* m_query;
				BObjectList<Queryable> m_records;
		};

		Worker* m_worker;
};

#endif

