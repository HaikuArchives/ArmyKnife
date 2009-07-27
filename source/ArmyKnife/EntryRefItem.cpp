#include <be/storage/Node.h>
#include <be/storage/NodeInfo.h>
#include <be/storage/Mime.h>
#include <be/storage/Path.h>
#include <be/storage/Volume.h>
#include <be/support/Debug.h>

#include <Font.h>

#include <tfile.h>
#include <fileref.h>
#include <tag.h>
#include <mpegfile.h>
#include <oggfile.h>
#include <vorbisfile.h>
#include <flacfile.h>

#include "EntryRefItem.h"

EntryRefItem::EntryRefItem(entry_ref* entryRef)
:	BStringItem			(entryRef->name, 0, true),
	m_ref				(new entry_ref(*entryRef)),
	m_mimetype			(),
	m_entry_type		(-1),
	m_mimetype_class	(-1),
	m_taglib_supported	(false),
	m_fs_readonly		(false),
	m_fs_knows_attr		(false),
	m_file_accepted		(true),
	m_has_id3v1			(false),
	m_has_id3v2			(false),
	m_has_ape			(false)
	
{
	PRINT(("EntryRefItem::EntryRefItem()\n"));
	
	Update();
}

EntryRefItem::~EntryRefItem()
{
	PRINT(("EntryRefItem::~EntryRefItem()\n"));

	delete m_ref;
}

void
EntryRefItem::SetEntryRef(entry_ref * entryRef)
{
	PRINT(("EntryRefItem::SetEntryRef()\n"));

	delete m_ref;
	m_ref = new entry_ref(*entryRef);

	UpdateText();
	Update();
}

entry_ref*
EntryRefItem::EntryRef()
{
	PRINT(("EntryRefItem::EntryRef()\n"));

	return m_ref;
}

status_t
EntryRefItem::TraverseSymlink()
{
	BEntry entry (m_ref, true);
	entry_ref ref;
	
	if((entry.InitCheck() == B_OK)
	&& (entry.GetRef(& ref) == B_OK))
	{
		SetEntryRef(& ref);
		return B_OK;
	}
	else
		return B_ERROR;
}

// ***********************
// *** FILE PROPERTIES ***
// ***********************

bool
EntryRefItem::IsFile()
{
	PRINT(("EntryRefItem::IsFile()\n"));

	return (m_entry_type == ENTRY_IS_FILE);
}

bool
EntryRefItem::IsDirectory()
{
	PRINT(("EntryRefItem::IsDirectory()\n"));

	return (m_entry_type == ENTRY_IS_FOLDER);
}

bool
EntryRefItem::IsSymLink()
{
	PRINT(("EntryRefItem::IsSymLink()\n"));

	return (m_entry_type == ENTRY_IS_SYMLINK);
}

bool
EntryRefItem::IsFSWritable()
{
	PRINT(("EntryRefItem::IsFSWritable()\n"));

	return (! m_fs_readonly);
}

bool
EntryRefItem::IsFSAttributable()
{
	PRINT(("EntryRefItem::IsFSAttributable()\n"));

	return m_fs_knows_attr;
}


bool
EntryRefItem::IsSupportedByTaglib()
{
	PRINT(("EntryRefItem::IsSupportedByTaglib()\n"));

	if (m_taglib_supported)
		return true;
	else
		return false;
}

bool
EntryRefItem::IsMP3()
{
	PRINT(("EntryRefItem::IsMP3()\n"));
	
	if (m_mimetype_class == ENTRY_IS_MP3)
		return true;
	else
		return false;
}

bool
EntryRefItem::IsOGG()
{
	PRINT(("EntryRefItem::IsOGG()\n"));
	
	if (m_mimetype_class == ENTRY_IS_OGG)
		return true;
	else
		return false;
}

bool
EntryRefItem::IsFLAC()
{
	PRINT(("EntryRefItem::IsFLAC()\n"));
	
	if (m_mimetype_class == ENTRY_IS_FLAC)
		return true;
	else
		return false;
}

bool
EntryRefItem::HasID3V1Tag()
{
	PRINT(("EntryRefItem::HasID3V1Tag()\n"));
	
	return m_has_id3v1;
}

bool
EntryRefItem::HasID3V2Tag()
{
	PRINT(("EntryRefItem::HasID3V2Tag()\n"));
	
	return m_has_id3v2;
}

bool
EntryRefItem::HasAPETag()
{
	PRINT(("EntryRefItem::HasAPETag()\n"));
	
	return m_has_ape;
}

// ***********************
// **** INITIAL PROBE ****
// ***********************

void
EntryRefItem::Update()
{
	PRINT(("EntryRefItem::Update()\n"));

//	UpdateText();
	UpdateEntryType();
	UpdateMimetype();
	UpdateMimetypeClass();
	UpdateSupportedByTaglib();
	UpdateFSProperties();
}

void
EntryRefItem::UpdateText()
{
	PRINT(("EntryRefItem::UpdateText()\n"));

	SetText(m_ref->name);
}

void
EntryRefItem::UpdateEntryType()
{
	PRINT(("EntryRefItem::UpdateEntryType()\n"));

	BNode node(m_ref);

	if (node.IsFile())
	{
		m_entry_type = ENTRY_IS_FILE;
	}
	else if (node.IsDirectory())
	{
		m_entry_type = ENTRY_IS_FOLDER;
	}
	else if (node.IsSymLink())
	{
		m_entry_type = ENTRY_IS_SYMLINK;
	}
}

void
EntryRefItem::UpdateMimetype()
{
	PRINT(("EntryRefItem::UpdateMimetype()\n"));

	BNode node(m_ref);
	BNodeInfo nodeinfo(& node);
	char type [B_MIME_TYPE_LENGTH];
	if (nodeinfo.GetType(type) == B_OK)
		m_mimetype = type;
	else
	{
		BPath path (m_ref);
		update_mime_info(path.Path(), false, true, false); 
	
		if (nodeinfo.GetType(type) == B_OK)
			m_mimetype = type;
		else
			m_mimetype.Truncate(0);
	}
}

void
EntryRefItem::UpdateMimetypeClass()
{
	PRINT(("EntryRefItem::UpdateMimetypeClass()\n"));
	
	if (	m_mimetype == "audio/mp3"
		||	m_mimetype == "audio/x-mp3"	
		||	m_mimetype == "audio/mpeg"
		||	m_mimetype == "audio/x-mpeg" 
		||	m_mimetype == "audio/mpg"
		||	m_mimetype == "audio/x-mpg")
	{
		m_mimetype_class = ENTRY_IS_MP3;
	}

	
	if (	m_mimetype == "audio/ogg"
		||	m_mimetype == "audio/x-ogg"
		||	m_mimetype == "audio/vorbis"
		||	m_mimetype == "audio/x-vorbis"
		||	m_mimetype == "application/ogg"
		||	m_mimetype == "application/x-ogg"	// the correct one ?
		||	m_mimetype == "application/vorbis"
		||	m_mimetype == "application/x-vorbis")
	{
		m_mimetype_class = ENTRY_IS_OGG;
	}
	
	if (	m_mimetype == "audio/flac"
		||	m_mimetype == "audio/x-flac"
		||	m_mimetype == "application/flac"
		||	m_mimetype == "application/x-flac")		// the correct one ?
	{
		m_mimetype_class = ENTRY_IS_FLAC;
	}
}

void
EntryRefItem::UpdateSupportedByTaglib()
{
	PRINT(("EntryRefItem::UpdateSupportedByTaglib()\n"));

	BPath  filename(m_ref);
	TagLib::FileRef	*	file;
	TagLib::Tag		*	tag;
	
	if (IsMP3())
	{
		file = new TagLib::FileRef(new TagLib::MPEG::File(filename.Path()));
	}
	else if (IsOGG())
	{
		file = new TagLib::FileRef(new TagLib::Vorbis::File(filename.Path()));
	}
	else if (IsFLAC())
	{
		file = new TagLib::FileRef(new TagLib::FLAC::File(filename.Path()));
	}
	else
	{
		file = new TagLib::FileRef (filename.Path());
	}
	
	if (file->file() == NULL)
	{
		PRINT(("file->file() == NULL\n"));
		
		m_taglib_supported = false;
		delete file;
		return;
	}
	
	if (file->file()->isValid())
	{
		m_taglib_supported = true;
	}
	else
	{
		m_taglib_supported = false;
	}

	// tags	in file
	if (IsMP3() && m_taglib_supported)
	{
		TagLib::ID3v1::Tag * id3v1_tag = ((TagLib::MPEG::File *) file->file())->ID3v1Tag();
		TagLib::ID3v2::Tag * id3v2_tag = ((TagLib::MPEG::File *) file->file())->ID3v2Tag();
		TagLib::APE::Tag * ape_tag = ((TagLib::MPEG::File *) file->file())->APETag();
	
		m_has_id3v1 = id3v1_tag;
		m_has_id3v2 = id3v2_tag;
		m_has_ape = ape_tag;
	}

	delete file;
}

void
EntryRefItem::UpdateFSProperties()
{
	PRINT(("EntryRefItem::UpdateFSProperties()\n"));
	
	BVolume volume (m_ref->device);
	
	m_fs_readonly = volume.IsReadOnly();
	m_fs_knows_attr = volume.KnowsAttr();
}

void
EntryRefItem::UpdateTaglibMetadata()
{
	PRINT(("EntryRefItem::UpdateTaglibMetadata()\n"));

	UpdateSupportedByTaglib();
}

// ***********************
// **** LIST ITEM GUI ****
// ***********************

void
EntryRefItem::SetAccepted(bool accepted)
{
	m_file_accepted = accepted;
}

bool
EntryRefItem::IsAccepted()
{
	return m_file_accepted;
}

void
EntryRefItem::DrawItem(BView *owner, BRect frame, bool complete = false)
{
	if (! m_file_accepted)
	{
		rgb_color original_high = owner->HighColor();
		rgb_color original_low = owner->LowColor();
		
		rgb_color color_a = {180, 110, 110, 0};
		rgb_color color_b = {255, 150, 150, 0};
		rgb_color black = {0, 0, 0, 0};
		rgb_color color;
			
		if (IsSelected())
		{
			color = color_a;
		}
		else
		{
			color = color_b;
		}
		
		owner->SetHighColor(color);
		owner->SetLowColor(color);
		owner->FillRect(frame);

		owner->SetHighColor(black);
		
/*		These font changes make redraws lag.
		BFont originalFont;
		BFont italicFont;
		owner->GetFont(& originalFont);
		italicFont = originalFont;
		italicFont.SetFace(B_BOLD_FACE);
		owner->SetFont(& italicFont);
*/		
		owner->MovePenTo(frame.left, frame.top + m_baseline_offset);
		owner->DrawString(Text());

		owner->SetHighColor(original_high);
		owner->SetLowColor(original_low);
//		owner->SetFont(& originalFont);
	}
	else
		BStringItem::DrawItem(owner, frame, true);
}


void
EntryRefItem::Update(BView *owner, const BFont *font)
{
	font_height height;
	font->GetHeight(&height);
	m_baseline_offset = ceilf(height.ascent + height.leading / 2) + 2;

	BStringItem::Update(owner, font);
}
