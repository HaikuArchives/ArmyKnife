/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */#ifndef __ENTRY_REF_ITEM_H__
#define __ENTRY_REF_ITEM_H__

#include <Entry.h>
#include <ListItem.h>
#include <String.h>
#include <View.h>


enum
{
	ENTRY_IS_FILE		=	1,
	ENTRY_IS_FOLDER		=	2,
	ENTRY_IS_SYMLINK	=	3,
};

enum
{
	ENTRY_IS_MP3	=	1,
	ENTRY_IS_OGG	=	2,
	ENTRY_IS_FLAC	=	3,
};

class EntryRefItem : public BStringItem
{
	public:
		EntryRefItem	(entry_ref * entryRef);
		~EntryRefItem	();

		void		SetEntryRef		(entry_ref * entryRef);
		entry_ref *	EntryRef		();
		status_t	TraverseSymlink	();

		void UpdateText();

		void SetAccepted (bool accepted);
		bool IsAccepted ();

		virtual void DrawItem (BView *owner, BRect frame, bool complete = false);
		virtual	void Update(BView* owner, const BFont* font);

		bool IsFile();
		bool IsDirectory();
		bool IsSymLink();

		bool IsFSWritable();
		bool IsFSAttributable();

		bool IsSupportedByTaglib();
		bool IsMP3();
		bool IsOGG();
		bool IsFLAC();

		// mpeg
		bool HasID3V1Tag();
		bool HasID3V2Tag();
		bool HasAPETag();
		// ogg
		// flac

		void UpdateTaglibMetadata();

	private:

		void Update();
		void UpdateEntryType();
		void UpdateMimetype();
		void UpdateMimetypeClass();
		void UpdateSupportedByTaglib();
		void UpdateFSProperties();

		entry_ref * m_ref;
		BString		m_mimetype;

		int32		m_entry_type;
		int32		m_mimetype_class;

		bool		m_taglib_supported;
		bool		m_fs_readonly;
		bool		m_fs_knows_attr;

		bool		m_file_accepted;

		// mpeg
		bool		m_has_id3v1;
		bool		m_has_id3v2;
		bool		m_has_ape;

		float		m_baseline_offset;
};

#endif
