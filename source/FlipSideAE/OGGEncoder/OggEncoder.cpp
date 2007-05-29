#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <be/kernel/fs_info.h>
#include <be/kernel/image.h>
#include <be/app/Message.h>
#include <be/interface/Menu.h>
#include <be/interface/MenuItem.h>
#include <be/storage/File.h>
#include <be/storage/NodeInfo.h>
#include <be/storage/Volume.h>
#include <be/support/Debug.h>
#include <AudioInfo/AudioAttributes.h>

#include "OggEncoder.h"

OggEncoder::OggEncoder() : AEEncoder(ADDON_NAME)
{
	PRINT(("OggEncoder::OggEncoder()\n"));

	if(FindExecutable(OGG,oggencPath) != B_OK)
	{
		delete menu;
		menu = 0;
		error = FSS_EXE_NOT_FOUND;
	}
}

OggEncoder::~OggEncoder()
{
	PRINT(("OggEncoder::~OggEncoder()\n"));
}

int32
OggEncoder::Encode(BMessage* message)
{
	PRINT(("OggEncoder::Encode(BMessage*)\n"));

	argument args;
	memset(&args,0,sizeof(argument));
	char bitrate[4];

	if(GetArgs(&args,message) != B_OK)
	{
		message->AddString("error","Error getting arguments.\n");
		return B_ERROR;
	}

	if(GetBitrate(bitrate) != B_OK)
	{
		message->AddString("error","Error getting bitrate setting.\n");
		return B_ERROR;
	}

	//check if input file is of correct type
	BFile iFile(args.inputFile,B_READ_ONLY);
	if(iFile.InitCheck() != B_OK)
	{
		message->AddString("error","Error init'ing input file.\n");
		return B_ERROR;
	}
	BNodeInfo info(&iFile);
	if(info.InitCheck() != B_OK)
	{
		message->AddString("error","Error getting info on input file.\n");
		return B_ERROR;
	}
	char mime[B_MIME_TYPE_LENGTH];
	info.GetType(mime);
	if((strcmp(mime,WAV_MIME_TYPE) != 0)
			&& (strcmp(mime,RIFF_WAV_MIME_TYPE) != 0)
			&& (strcmp(mime,RIFF_MIME_TYPE) != 0)
			&& (strcmp(mime,AIFF_MIME_TYPE) != 0))
	{
		return FSS_INPUT_NOT_SUPPORTED;
	}

	//set up arg list
	int numTags = 0;
	if(args.artist) numTags += 2;
	if(args.album) numTags += 2;
	if(args.title) numTags += 2;
	if(args.year) numTags += 2;
	if(args.comment) numTags++;
	if(args.track) numTags += 2;
	if(args.genre) numTags++;

	int argc = 6 + numTags;
	const char* argv[argc+1];

	argv[0] = oggencPath;
	argv[1] = OUTPUT_PREFIX;
	argv[2] = args.outputFile;
	argv[3] = BITRATE_PREFIX;
	argv[4] = bitrate;

	int i = 5;
	if(args.artist)
	{
		argv[i] = strdup(ARTIST_PREFIX);
		argv[i+1] = strdup(args.artist);
		i += 2;
	}
	if(args.album)
	{
		argv[i] = strdup(ALBUM_PREFIX);
		argv[i+1] = strdup(args.album);
		i += 2;
	}
	if(args.title)
	{
		argv[i] = strdup(TITLE_PREFIX);
		argv[i+1] = strdup(args.title);
		i += 2;
	}
	if(args.year)
	{
		argv[i] = strdup(YEAR_PREFIX);
		argv[i+1] = strdup(args.year);
		i += 2;
	}
	if(args.comment)
	{
		BString comment = COMMENT_PREFIX;
		comment << args.comment;
		argv[i] = strdup(comment.String());
		i++;
	}
	if(args.track)
	{
		argv[i] = strdup(TRACK_PREFIX);
		argv[i+1] = strdup(args.track);
		i += 2;
	}
	if(args.genre)
	{
		BString genre = GENRE_PREFIX;
		genre << args.genre;
		argv[i] = strdup(genre.String());
		i++;
	}

	argv[argc-1] = args.inputFile;
	argv[argc] = NULL;

#ifdef DEBUG
	int j=0;
	while(argv[j] != NULL)
	{
		PRINT(("%s ",argv[j]));
		j++;
	}
	PRINT(("\n"));
#endif

	FILE* out;
	int filedes[2];
	thread_id oggenc = CommandIO(filedes,argc,argv);
	if(oggenc <= B_ERROR)
	{
		PRINT(("ERROR: can't load oggenc image\n"));
		return B_ERROR;
	}
	out = fdopen(filedes[0],"r");

	resume_thread(oggenc);

	int32 status = UpdateStatus(out,&messenger);
	fclose(out);
	close(filedes[1]);

	if(status == FSS_CANCEL_ENCODING)
	{
		status = send_signal(oggenc,SIGTERM);
		PRINT(("status = %d\n",status));
		return FSS_CANCEL_ENCODING;
	}

	status_t err;
	wait_for_thread(oggenc,&err);

	WriteDetails(&args);

	return B_OK;
}

int32
OggEncoder::LoadDefaultPattern()
{
	PRINT(("OggEncoder::LoadDefaultPattern()\n"));

	pattern = "/boot/OGG/%a/%n/%a - %n - %k - %t.ogg";

	return B_OK;
}

int32
OggEncoder::LoadDefaultMenu()
{
	PRINT(("OggEncoder::LoadDefaultMenu()\n"));

	BMenu* bitrateMenu;
	BMenuItem* item;

	menu = new BMenu(name.String());

	//Bitrate Menu
	bitrateMenu = new BMenu(BITRATE_STR);
	bitrateMenu->SetRadioMode(true);

	item = new BMenuItem(_112KBPS,NULL);
	bitrateMenu->AddItem(item);

	item = new BMenuItem(_128KBPS,NULL);
	item->SetMarked(true);
	bitrateMenu->AddItem(item);

	item = new BMenuItem(_160KBPS,NULL);
	bitrateMenu->AddItem(item);

	item = new BMenuItem(_192KBPS,NULL);
	bitrateMenu->AddItem(item);

	item = new BMenuItem(_256KBPS,NULL);
	bitrateMenu->AddItem(item);
	
	item = new BMenuItem(_350KBPS,NULL);
	bitrateMenu->AddItem(item);

	menu->AddItem(bitrateMenu);
}

int32
OggEncoder::GetBitrate(char* bitrate)
{
	PRINT(("OggEncoder::GetBitrate(char*)\n"));

	BMenuItem* item;
	BMenu* bitrateMenu;

	item = menu->FindItem(BITRATE_STR);
	if(!item)
	{
		return B_ERROR;
	}
	bitrateMenu = item->Submenu();
	if(!bitrateMenu)
	{
		return B_ERROR;
	}

	item = bitrateMenu->FindMarked();
	if(!item)
	{
		return B_ERROR;
	}

	const char* label = item->Label();
	if(strcmp(label,_112KBPS) == 0)
	{
		strcpy(bitrate,"112");
	}
	else if(strcmp(label,_128KBPS) == 0)
	{
		strcpy(bitrate,"128");
	}
	else if(strcmp(label,_160KBPS) == 0)
	{
		strcpy(bitrate,"160");
	}
	else if(strcmp(label,_192KBPS) == 0)
	{
		strcpy(bitrate,"192");
	}
	else if(strcmp(label,_256KBPS) == 0)
	{
		strcpy(bitrate,"256");
	}
	else if(strcmp(label,_350KBPS) == 0)
	{
		strcpy(bitrate,"350");
	}

	return B_OK;
}

int32
OggEncoder::UpdateStatus(FILE* out, BMessenger* messenger)
{
	PRINT(("OggEncoder::UpdateStatus(FILE*,BMessenger*)\n"));

	float prev = 0.0;
	float curr = 0.0;
	char buffer[2048];
	unsigned char c = 0;
	unsigned char prevc = 0;
	int i = 0;
	bool lineStarted = false;

	while(1)
	{
		if(CheckForCancel())
		{
			PRINT(("Cancel requested.\n"));
			return FSS_CANCEL_ENCODING;
		}

		if(feof(out))
		{
			PRINT(("ERROR IN ENCODING STREAM - EOF ENCOUNTERED\n"));
			return B_ERROR;
		}

		if(ferror(out))
		{
			PRINT(("ERROR IN ENCODING STREAM\n"));
			return B_ERROR;
		}

		prevc = c;
		c = fgetc(out);

		if(c != 0 && c != 13)
		{
			if(c == 'E' && prevc == 13)
			{
				i = 0;
			}
			else if(c == '%')
			{
				buffer[i] = '\0';
				char* tmp;
				if(buffer[i-5] != ' ')
				{
					tmp = &(buffer[i-5]);
				}
				else if(buffer[i-4] != ' ')
				{
					tmp = &(buffer[i-4]);
				}
				else
				{
					tmp = &(buffer[i-3]);
				}
				curr = atof(tmp);
				float delta = curr - prev;
				prev = curr;
				BMessage updateMessage(B_UPDATE_STATUS_BAR);
				updateMessage.AddFloat("delta",delta);
				messenger->SendMessage(&updateMessage);
			}
			buffer[i] = c;
			i++;
		}
		else if(c == 13)
		{
			buffer[i] = 0;
			PRINT(("%s\n",buffer));
		}

		if(c == 'D' && prevc == 10)
		{
			break;
		}
	}
	PRINT(("DONE ENCODING\n"));
	return B_OK;
}

int32
OggEncoder::GetArgs(argument* args, BMessage* encodeMessage)
{
	PRINT(("OggEncoder::GetArgs(argument*,BMessage*)\n"));

	if(encodeMessage->FindString("input file",&(args->inputFile)) != B_OK)
	{
		return B_ERROR;
	}
	if(encodeMessage->FindString("output file",&(args->outputFile)) != B_OK)
	{
		return B_ERROR;
	}
	if(encodeMessage->FindMessenger("statusBarMessenger",&messenger) != B_OK)
	{
		return B_ERROR;
	}
	if(encodeMessage->FindString("artist",&(args->artist)) != B_OK)
	{
		args->artist = NULL;
	}
	if(encodeMessage->FindString("album",&(args->album)) != B_OK)
	{
		args->album = NULL;
	}
	if(encodeMessage->FindString("title",&(args->title)) != B_OK)
	{
		args->title = NULL;
	}
	if(encodeMessage->FindString("year",&(args->year)) != B_OK)
	{
		args->year = NULL;
	}
	if(encodeMessage->FindString("comment",&(args->comment)) != B_OK)
	{
		args->comment = NULL;
	}
	if(encodeMessage->FindString("track",&(args->track)) != B_OK)
	{
		args->track = NULL;
	}
	if(encodeMessage->FindString("genre",&(args->genre)) != B_OK)
	{
		args->genre = NULL;
	}

	return B_OK;
}

int32
OggEncoder::WriteDetails(argument* args)
{
	PRINT(("OggEncoder::WriteDetails(argument*)\n"));

	dev_t device = dev_for_path(args->outputFile);
	BVolume volume(device);
	if(volume.InitCheck() != B_OK)
	{
		return B_ERROR;
	}

	BFile oggFile(args->outputFile,B_READ_WRITE);
	if(oggFile.InitCheck() != B_OK)
	{
		return B_ERROR;
	}

	if(volume.KnowsMime())
	{
		BNodeInfo info(&oggFile);
		if(info.InitCheck() != B_OK)
		{
			return B_ERROR;
		}
		if(info.SetType(OGG_MIME_TYPE) != B_OK)
		{
			return B_ERROR;
		}
	}

	if(volume.KnowsAttr())
	{
		AudioAttributes attributes(&oggFile);
		attributes.SetArtist(args->artist);
		attributes.SetAlbum(args->album);
		attributes.SetTitle(args->title);
		attributes.SetYear(args->year);
		attributes.SetComment(args->comment);
		attributes.SetTrack(args->track);
		attributes.SetGenre(args->genre);
		if(attributes.Write() != B_OK)
		{
			return B_ERROR;
		}
	}

	return B_OK;
}

thread_id
OggEncoder::CommandIO(int* filedes, int argc, const char** argv)
{
	PRINT(("OggEncoder::CommandIO(int*,int,const char**)\n"));

	int oldstderr;

	pipe(filedes);
	oldstderr = dup(STDERR_FILENO);
	close(STDERR_FILENO);
  	dup2(filedes[1],STDERR_FILENO);

	thread_id ret = load_image(argc,argv,(const char**)environ);

  	dup2(oldstderr,STDERR_FILENO);
	close(oldstderr);

	return ret;
}

//function called by Flipside A.E. to get new AEEncoder subclass
AEEncoder*
load_encoder()
{
	return new OggEncoder();
}
