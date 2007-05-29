#include <be/app/Message.h>
#include <be/app/Messenger.h>
#include <be/interface/Menu.h>
#include <be/kernel/fs_info.h>
#include <be/media/MediaDefs.h>
#include <be/media/MediaFile.h>
#include <be/media/MediaFormats.h>
#include <be/media/MediaTrack.h>
#include <be/storage/Entry.h>
#include <be/storage/NodeInfo.h>
#include <be/storage/Volume.h>
#include <be/support/Debug.h>
#include <AudioInfo/AudioAttributes.h>
#include <AudioInfo/ID3Tags.h>
#include <String/String.h>

#include "BeEncoder.h"
#include "MediaMenuItem.h"

BeEncoder::BeEncoder() : AEEncoder(ADDON_NAME)
{
	PRINT(("BeEncoder::BeEncoder()\n"));

	if(menu)
	{
		BMenuItem* marked = menu->FindMarked();
		if(!marked)
		{
			error = B_ERROR;
			return;
		}
		String markedItem = marked->Label();

		BMenuItem* item;
		int32 numItems = menu->CountItems();
		for(int i=0;i<numItems;i++)
		{
			item = menu->RemoveItem((int32)0);
			delete item;
			item = 0;
		}

		AddEncoderEntries(menu);

		numItems = menu->CountItems();
		for(int i=0;i<numItems;i++)
		{
			item = menu->ItemAt(i);
			if(item && markedItem == item->Label())
			{
				item->SetMarked(true);
				break;
			}
		}
	}
}

BeEncoder::~BeEncoder()
{
	PRINT(("BeEncoder::~BeEncoder()\n"));
}

int32
BeEncoder::Encode(BMessage* message)
{
	PRINT(("BeEncoder::Encode(BMessage*)\n"));

	status_t status;
	int32 cookie;

	BMessenger messenger;
	MediaMenuItem* markedItem;

	const char* inputPath;
	const char* outputPath;
	entry_ref inputRef;
	entry_ref outputRef;

	media_format infmt;
	media_format outfmt;
	media_format format;
	media_codec_info codec;

	if(message->FindMessenger("statusBarMessenger",&messenger) != B_OK)
	{
		message->AddString("error","Error getting status bar messenger.\n");
		return B_ERROR;
	}

	if(message->FindString("input file",&inputPath) != B_OK)
	{
		message->AddString("error","Error getting input path.\n");
		return B_ERROR;
	}

	if(message->FindString("output file",&outputPath) != B_OK)
	{
		message->AddString("error","Error getting output path.\n");
		return B_ERROR;
	}

	if(get_ref_for_path(inputPath,&inputRef) != B_OK)
	{
		message->AddString("error","Error getting input ref.\n");
		return B_ERROR;
	}

	if(get_ref_for_path(outputPath,&outputRef) != B_OK)
	{
		message->AddString("error","Error getting output ref.\n");
		return B_ERROR;
	}

	markedItem = (MediaMenuItem*)menu->FindMarked();
	if(!markedItem)
	{
		message->AddString("error","Error getting encoder preference.\n");
		return B_ERROR;
	}

	BMediaFile inputFile(&inputRef);
	if(inputFile.InitCheck() != B_OK)
	{
		message->AddString("error","Error initializing input file.\n");
		return B_ERROR;
	}

	BMediaFile outputFile(&outputRef,markedItem->MediaFileFormat());
	if(outputFile.InitCheck() != B_OK)
	{
		if(inputFile.InitCheck() == B_MEDIA_NO_HANDLER)
		{
			return FSS_INPUT_NOT_SUPPORTED;
		}
		message->AddString("error","Error initializing output file.\n");
		return B_ERROR;
	}
	
	BMediaTrack* inputTrack;
	int32 numTracks = inputFile.CountTracks();
	for(int i=0;i<numTracks;i++)
	{
		inputTrack = inputFile.TrackAt(i);
		if(!inputTrack)
		{
			message->AddString("error",
					"Error getting inputTrack from input file.\n");
			return B_ERROR;
		}
		
		if(inputTrack->EncodedFormat(&infmt) != B_OK)
		{
			message->AddString("error",
					"Error getting encoded fromat from inputTrack.\n");
			return B_ERROR;
		}

		if(!infmt.IsAudio())
		{
			inputFile.ReleaseTrack(inputTrack);
			inputTrack = 0;
		}

		if(inputTrack)
		{
			break;
		}
	}

	if(!inputTrack)
	{
		message->AddString("error","Error file has no audio tracks.\n");
		return B_ERROR;
	}

	memset(&infmt,0,sizeof(media_format));
	memset(&outfmt,0,sizeof(media_format));

	inputTrack->EncodedFormat(&infmt);
	outfmt.type = B_MEDIA_RAW_AUDIO;
	outfmt.u.raw_audio.format = media_raw_audio_format::B_AUDIO_UCHAR;
	outfmt.u.raw_audio.channel_count = 1;
	inputTrack->DecodedFormat(&outfmt);

	char* sound_buffer = new char[outfmt.u.raw_audio.buffer_size];

	cookie = 0;
	while((status = get_next_encoder(&cookie,markedItem->MediaFileFormat(),
				&outfmt,&format,&codec)) == B_OK)
	{
		if(strcmp(markedItem->MediaCodecInfo()->short_name,codec.short_name) == 0)
		{
			break;
		}
	}

	if(status != B_OK)
	{
		message->AddString("error","Error finding output media format.\n");
		PRINT(("%s\n",strerror(status)));
		delete [] sound_buffer;
		return B_ERROR;
	}

	BMediaTrack* outputTrack = outputFile.CreateTrack(&outfmt,&codec);
	if(!outputTrack)
	{
		message->AddString("error","Error creating output audio track.\n");
		delete [] sound_buffer;
		return B_ERROR;
	}

	if(outputFile.CommitHeader() != B_OK)
	{
		message->AddString("error","Error commiting header.\n");
		delete [] sound_buffer;
		return B_ERROR;
	}

	int64 framecount = 0;
	int64 numFrames = inputTrack->CountFrames();
	media_header mh;

	BMessage setMaxMessage(FSS_SETMAX_STATUS_BAR);
	setMaxMessage.AddFloat("max",numFrames);
	messenger.SendMessage(&setMaxMessage);
	int result = B_OK;
	int prev = 0;
	int curr = 0;
	for(int i=0;i<numFrames;i+=framecount)
	{
		if(CheckForCancel())
		{
			result = FSS_CANCEL_ENCODING;
			break;
		}

		if(inputTrack->ReadFrames(sound_buffer,&framecount,&mh) != B_OK)
		{
			message->AddString("error","Error reading frames from input track.\n");
			result = B_ERROR;
			break;
		}

		if(status = outputTrack->WriteFrames(sound_buffer,framecount) != B_OK)
		{
			message->AddString("error","Error writing frames to output track.\n");
			PRINT(("%s\n",strerror(status)));
			result = B_ERROR;
			break;
		}

		prev = curr;
		curr = i;
		BMessage updateMessage(B_UPDATE_STATUS_BAR);
		updateMessage.AddFloat("delta",(curr-prev));
		messenger.SendMessage(&updateMessage);
	}
	
	delete [] sound_buffer;
	outputFile.CloseFile();

	if(strcmp((markedItem->MediaFileFormat())->short_name,"mp3") == 0)
	{
		WriteDetails(message,(markedItem->MediaFileFormat())->mime_type,true);
	}
	else
	{
		WriteDetails(message,(markedItem->MediaFileFormat())->mime_type);
	}
	return result;
}

int32
BeEncoder::LoadDefaultPattern()
{
	PRINT(("BeEncoder::LoadDefaultPattern()\n"));

	pattern = "/boot/MP3/%a/%n/%a - %n - %k - %t.mp3";

	return B_OK;
}

int32
BeEncoder::LoadDefaultMenu()
{
	PRINT(("BeEncoder::LoadDefaultMenu()\n"));

	menu = new BMenu(GetName());
	menu->SetRadioMode(true);
	AddEncoderEntries(menu);

	return B_OK;
}

int32
BeEncoder::AddEncoderEntries(BMenu* encoderMenu)
{
	PRINT(("BeEncoder::AddEncoderEntries(BMenu*)\n"));

	BMenuItem* item;
	media_format infmt;
	media_format outfmt;
	media_codec_info mci;
	media_file_format mff;

	infmt.type = B_MEDIA_RAW_AUDIO;
	infmt.u.raw_audio.format = media_raw_audio_format::B_AUDIO_UCHAR;
	infmt.u.raw_audio.channel_count = 1;

	int32 format_cookie = 0;
	while(get_next_file_format(&format_cookie,&mff) == B_OK)
	{
		if(!(mff.capabilities & media_file_format::B_KNOWS_RAW_VIDEO) &&
			!(mff.capabilities & media_file_format::B_KNOWS_ENCODED_VIDEO))
		{
			int32 encoder_cookie = 0;
			while(get_next_encoder(&encoder_cookie,&mff,&infmt,&outfmt,&mci)
					== B_OK)
			{
				//create the item
				String name = mff.short_name;
				name += "/";
				name += mci.pretty_name;
				item = new MediaMenuItem(name.Value(),&mff,&mci);
				encoderMenu->AddItem(item);
			}
		}
	}

	item = encoderMenu->ItemAt(0);
	if(item)
	{
		item->SetMarked(true);
	}

	return B_OK;
}

int32
BeEncoder::WriteDetails(BMessage* msg, const char* mime, bool tags=false)
{
	PRINT(("BeEncoder::WriteDetails(BMessage*,bool)\n"));

	BFile file;
	const char* outputFile;
	const char* artist;
	const char* album;
	const char* title;
	const char* year;
	const char* comment;
	const char* track;
	const char* genre;

	if(msg->FindString("output file",&outputFile) != B_OK)
	{
		PRINT(("Error getting output file from message.\n"));
		return B_ERROR;
	}

	if(file.SetTo(outputFile,B_READ_WRITE) != B_OK)
	{
		PRINT(("Error opening output file.\n"));
		return B_ERROR;
	}

	msg->FindString("artist",&artist);
	msg->FindString("album",&album);
	msg->FindString("title",&title);
	msg->FindString("year",&year);
	msg->FindString("comment",&comment);
	msg->FindString("track",&track);
	msg->FindString("genre",&genre);

	dev_t dev = dev_for_path(outputFile);
	if(dev <= B_ERROR)
	{
		PRINT(("Error Can't determine volume for output file.\n"));
		return B_ERROR;
	}

	BVolume vol(dev);
	if(vol.InitCheck() != B_OK)
	{
		PRINT(("Error init'ing volume.\n"));
		return B_ERROR;
	}

	if(vol.KnowsMime())
	{
		BNodeInfo info(&file);
		if(info.InitCheck() != B_OK)
		{
			PRINT(("Error init'ing BNodeInfo.\n"));
			return B_ERROR;
		}

		if(info.SetType(mime) != B_OK)
		{
			PRINT(("Error setting mime type.\n"));
			return B_ERROR;
		}
	}

	if(vol.KnowsAttr())
	{
		AudioAttributes attributes(&file);
		attributes.SetArtist(artist);
		attributes.SetAlbum(album);
		attributes.SetTitle(title);
		attributes.SetYear(year);
		attributes.SetComment(comment);
		attributes.SetTrack(track);
		attributes.SetGenre(genre);

		if(attributes.Write() != B_OK)
		{
			PRINT(("Error writing file attributes.\n"));
			return B_ERROR;
		}
	}

	if(tags)
	{
		ID3Tags id3Tags(outputFile);
		id3Tags.SetArtist(artist);
		id3Tags.SetAlbum(album);
		id3Tags.SetTitle(title);
		id3Tags.SetYear(year);
		id3Tags.SetComment(comment);
		id3Tags.SetTrack(track);
		id3Tags.SetGenre(genre);

		if(id3Tags.Write() != B_OK)
		{
			PRINT(("Error writing id3 tags.\n"));
			return B_ERROR;
		}
	}
}

//function called by Flipside A.E. to get new AEEncoder subclass
AEEncoder*
load_encoder()
{
	return new BeEncoder();
}
