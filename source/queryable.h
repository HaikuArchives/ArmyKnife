#ifndef _QUERYABLE_H__
#define _QUERYABLE_H__

#include <Archivable.h>

class Queryable : public BArchivable
{
	public:
		Queryable();
		Queryable(const Queryable& other);
		Queryable(BMessage* from);
		~Queryable();

		status_t Archive(BMessage* into, bool deep = true) const;
		static BArchivable* Instantiate(BMessage* archive);

		const char* Artist() const;
		const char* Album() const;
		const char* Title() const;
		const char* Year() const;
		const char* Comment() const;
		const char* Track() const;

		void SetArtist(const char* value);
		void SetAlbum(const char* value);
		void SetTitle(const char* value);
		void SetYear(const char* value);
		void SetComment(const char* value);
		void SetTrack(const char* value);

		void PrintToStream();

	private:
		char* m_artist;
		char* m_album;
		char* m_title;
		char* m_year;
		char* m_comment;
		char* m_track;
};

#endif

