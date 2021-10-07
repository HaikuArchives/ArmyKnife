/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "albumpictureview.h"

#include <Bitmap.h>
#include <BitmapStream.h>
#include <Font.h>
#include <Path.h>
#include <Rect.h>
#include <TranslatorRoster.h>

#include "entryrefitem.h"
#include "guistrings.h"

AlbumPictureView::AlbumPictureView(const char *name)
	:	BView(name, B_WILL_DRAW),
		m_bitmap(NULL)
{
	SetExplicitPreferredSize(BSize(160, 160));
	SetExplicitMinSize(BSize(160, 160));
	SetExplicitMaxSize(BSize(160, 160));
	m_attached_to_track = false;
	NoImage();
}

AlbumPictureView::~AlbumPictureView()
{
}


void
AlbumPictureView::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case B_REFS_RECEIVED:
			// Supposed to fallthrough
		case B_SIMPLE_DATA:
			// If there wasn't just one image, we pass it further
			if (ProcessRefs(message))
				break;
		default:
			BView::MessageReceived(message);
	}
}


// Get image from the message and load it.
// If there is more than one file passed (or no files)
// or some error occured return false.
bool
AlbumPictureView::ProcessRefs(BMessage *message)
{
	// Count number of files passed.
	// If it does not equal one, return false
	int32 refCount;
	message->GetInfo("refs", NULL, &refCount);
	if (refCount != 1)
		return false;

	// Find first item in the refs list and
	// load it if it is a picture
	// or return false otherwise
	entry_ref tempRef;
	delete m_bitmap;
	m_bitmap = NULL;
	BPath filePath;

	if (message->FindRef("refs", &tempRef) == B_NO_ERROR &&
		BEntry(&tempRef).GetPath(&filePath) == B_OK &&
		(m_bitmap = BTranslationUtils::GetBitmap(filePath.Path())) != NULL)
	{
		// Draw only if we a track is chosen in the list
		if (m_attached_to_track)
			DrawBitmap(m_bitmap, BRect(0, 0,
				Bounds().Width(),
				Bounds().Height()));
		return true;
	}
	return false;
}


void
AlbumPictureView::Draw(BRect rect)
{
	Clear();
	if (m_bitmap != NULL)
		DrawBitmap(m_bitmap, BRect(0, 0,
			Bounds().Width(),
			Bounds().Height()));
	else
		NoImage();
}


// Clears the image
void
AlbumPictureView::Clear()
{
	SetHighColor(255,255,255);
	FillRect(Bounds());
}


// Sets album picture for the file specified
void
AlbumPictureView::SetPicture(const char *path)
{
	if (m_bitmap == NULL)
		return;

	TagLib::MPEG::File file(path);
	TagLib::ID3v2::Tag *fileTags = file.ID3v2Tag();
	TagLib::ID3v2::FrameList foundFrames = fileTags->frameList("APIC");

	// Delete all the covers in this file
	if (!foundFrames.isEmpty())
	{
		TagLib::ID3v2::FrameList::ConstIterator it = foundFrames.begin();
		for (;it != foundFrames.end(); ++it)
		{
			TagLib::ID3v2::AttachedPictureFrame *currentFrame =
				static_cast<TagLib::ID3v2::AttachedPictureFrame *>(*it);

			if (currentFrame->type() ==
				TagLib::ID3v2::AttachedPictureFrame::FrontCover)
				fileTags->removeFrame(currentFrame, true);
		}
	}

	// Find the translator type that suits us
	BTranslatorRoster *roster = BTranslatorRoster::Default();
	translator_id *translators;
	int32 numTranslators;
	uint32 foundType = 0;
	const char *mimeName = "image/jpeg";

	roster->GetAllTranslators(&translators, &numTranslators);

	for (int32 i = 0; i < numTranslators && foundType == 0; ++i)
	{
		const translation_format *formats;
		int32 numFormats;

		roster->GetOutputFormats(translators[i], &formats, &numFormats);

		for (int32 j = 0; j < numFormats && foundType == 0; ++j)
		{
			if (!strcasecmp(formats[j].MIME, mimeName))
				foundType = formats[j].type;
		}
	}

	// Convert bitmap to jpeg
	BMallocIO *buffer = new BMallocIO();
	BBitmapStream *stream = new BBitmapStream(m_bitmap);
	roster->Translate(stream, NULL, NULL, buffer, foundType);

	// Write jpeg to tags and save the file
	TagLib::ID3v2::AttachedPictureFrame *pictureFrame =
		new TagLib::ID3v2::AttachedPictureFrame();
	pictureFrame->setMimeType(mimeName);
	pictureFrame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
	pictureFrame->setPicture(
		TagLib::ByteVector(static_cast<const char*>(buffer->Buffer()), buffer->BufferLength() + 1));
	printf("drag:%i\n", fileTags->frameList("APIC").size());
	fileTags->addFrame(pictureFrame);
	printf("drag:%i\n", fileTags->frameList("APIC").size());
	file.save();

	delete buffer;
}


// Loads album art for the song, path to which is specified
void
AlbumPictureView::UpdatePicture(const char *path)
{
	Clear();
	if (path == NULL)
	{
		NoImage();
		return;
	}
	bool success = true;
	const char *pictureTagIdName = "APIC";
	TagLib::MPEG::File file(path);
	TagLib::ID3v2::Tag *fileTags = file.ID3v2Tag();
	TagLib::ID3v2::FrameList frame;
	TagLib::ID3v2::AttachedPictureFrame *pictureFrame = NULL;

	if (fileTags != NULL)
	{
		// Find frame containing pictures
		frame = fileTags->frameList(pictureTagIdName);
		if (!frame.isEmpty())
		{
			// Search for the album image
			// (one frame can contain multiple images)
			TagLib::ID3v2::FrameList::ConstIterator it = frame.begin();
			for (;it != frame.end(); ++it)
			{
				TagLib::ID3v2::AttachedPictureFrame *currentFrame =
					static_cast<TagLib::ID3v2::AttachedPictureFrame *>(*it);
				if (currentFrame->type() ==
					TagLib::ID3v2::AttachedPictureFrame::FrontCover)
					pictureFrame = currentFrame;
			}
			if (pictureFrame != NULL)
			{
				// WARNING: HACK
				// This loop is needed because
				// sometimes the image cannot be loaded
				// in first few runs (idk why that happens)
				// P.S. It still fails sometimes
				delete m_bitmap;
				m_bitmap = NULL;
				const uint32 maxCount = 50;
				for (int counter = 0; counter < maxCount && m_bitmap == NULL;
					++counter)
				{
					delete m_bitmap;

					BMemoryIO *memStream = new BMemoryIO(
						pictureFrame->picture().data(),
						pictureFrame->picture().size());

					m_bitmap = BTranslationUtils::GetBitmap(memStream);

					delete memStream;
				}
				if (m_bitmap != NULL)
				{
					DrawBitmap(m_bitmap,
						BRect(0,0,
						Bounds().Width(),
						Bounds().Height()));
				}
				else
				{
					success = false;
				}
			}
			else
			{
				success = false;
			}
		}
		else
		{
			success = false;
		}
	}
	if (!success)
	{
		NoImage();
	}
	m_attached_to_track = true;
}

// Notify user that track does not contain artwork
void
AlbumPictureView::NoImage()
{
	delete m_bitmap;
	m_bitmap = NULL;
	Clear();
	SetHighColor(0,0,0);
	BFont font;
	font.SetSize(12.0);
	SetFont(&font);

	const char* noImageText = NO_IMAGE;
	DrawString(noImageText, 
		BPoint(Bounds().Width() / 2 - StringWidth(noImageText) / 2,
		Bounds().Height() / 2));
	m_attached_to_track = false;
}
