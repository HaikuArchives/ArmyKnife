#include <ctype.h>
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
#include <AudioInfo/ID3Tags.h>

#include "MP3Lame.h"

MP3Lame::MP3Lame() : AEEncoder(ADDON_NAME)
{
	PRINT(("MP3Lame::MP3Lame()\n"));

	if(FindExecutable(LAME,lamePath) != B_OK)
	{
		delete menu;
		menu = 0;
		error = FSS_EXE_NOT_FOUND;
		return;
	}

	if(menu)
	{
		BMenuItem* item = menu->FindItem(_32KBPS);
		if(!item)
		{
			delete menu;
			menu = 0;
		}
	}
}

MP3Lame::~MP3Lame()
{
	PRINT(("MP3Lame::~MP3Lame()\n"));
}

int32
MP3Lame::Encode(BMessage* message)
{
	PRINT(("MP3Lame::Encode(BMessage*)\n"));

	const char* inputFile;
	const char* outputFile;
	char bitrate[4];
	bool vbr;
	char format[2];
	char psycho[2];
	BMessenger messenger;

	//set required fields; quit if not found or error
	if(message->FindString("input file",&inputFile) != B_OK)
	{
		message->AddString("error","Error getting input path.\n");
		return B_ERROR;
	}
	if(message->FindString("output file",&outputFile) != B_OK)
	{
		message->AddString("error","Error getting output path.\n");
		return B_ERROR;
	}
	if(message->FindMessenger("statusBarMessenger",&messenger) != B_OK)
	{
		message->AddString("error","Error getting status bar messenger.\n");
		return B_ERROR;
	}
	if(GetBitrate(bitrate,&vbr) != B_OK)
	{
		message->AddString("error","Error getting bitrate setting.\n");
		return B_ERROR;
	}
	if(GetFormat(format) != B_OK)
	{
		message->AddString("error","Error getting format setting.\n");
		return B_ERROR;
	}
	if(GetPsycho(psycho) != B_OK)
	{
		message->AddString("error","Error getting psychoacoustic setting.\n");
		return B_ERROR;
	}

	//check if input file is of correct type
	BFile iFile(inputFile,B_READ_ONLY);
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
			&& (strcmp(mime,RIFF_MIME_TYPE) != 0))
	{
		return FSS_INPUT_NOT_SUPPORTED;
	}

	//set up arg list
	int argc = 10;
	if(vbr)
	{
		argc = 11;
	}
	const char* argv[argc+1];

	argv[0] = lamePath;
	argv[1] = PSYCHO_PREFIX;
	argv[2] = psycho;
	argv[3] = FORMAT_PREFIX;
	argv[4] = format;
	argv[5] = "--nohist";
	if(vbr)
	{
		argv[6] = VBR_PREFIX;
		argv[7] = VBR_QUAL_PREFIX;
	}
	else
	{
		argv[6] = BITRATE_PREFIX;
	}
	argv[argc - 3] = bitrate;
	argv[argc - 2] = inputFile;
	argv[argc - 1] = outputFile;
	argv[argc] = 0;

#ifdef DEBUG
	{
		int j=0;
		while(argv[j] != NULL)
		{
			PRINT(("%s ",argv[j]));
			j++;
		}
		PRINT(("\n"));
	}
#endif

	FILE* out;
	int filedes[2];
	thread_id lame = CommandIO(filedes,argc,argv);
	if(lame <= B_ERROR)
	{
		PRINT(("ERROR: can't load lame image\n"));
		message->AddString("error","Error running lame.\n");
		return B_ERROR;
	}
	out = fdopen(filedes[0],"r");

	resume_thread(lame);

	int32 status = UpdateStatus(out,&messenger);

	close(filedes[1]);
	close(filedes[0]);

	if(status == FSS_CANCEL_ENCODING)
	{
		status = send_signal(lame,SIGTERM);
		PRINT(("status = %d\n",status));
		return FSS_CANCEL_ENCODING;
	}

	status_t err;
	wait_for_thread(lame,&err);

	WriteDetails(message);

	if(CheckForCancel())
	{
		PRINT(("Cancel Requested.\n"));
		return FSS_CANCEL_ENCODING;
	}

	return B_OK;
}

int32
MP3Lame::LoadDefaultPattern()
{
	PRINT(("MP3Lame::LoadDefaultPattern()\n"));

	pattern = "/boot/MP3/%a/%n/%a - %n - %k - %t.mp3";

	return B_OK;
}

int32
MP3Lame::LoadDefaultMenu()
{
	PRINT(("MP3Lame::LoadDefaultMenu()\n"));

	BMenu* bitrateMenu;
	BMenuItem* item;

	menu = new BMenu(name.String());

	//Bitrate Menu
	bitrateMenu = new BMenu(BITRATE_STR);
	bitrateMenu->SetRadioMode(true);
	item = new BMenuItem(_32KBPS,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(_48KBPS,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(_64KBPS,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(_96KBPS,NULL);
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
	item = new BMenuItem(_320KBPS,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_0,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_1,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_2,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_3,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_4,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_5,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_6,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_7,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_8,NULL);
	bitrateMenu->AddItem(item);
	item = new BMenuItem(VBR_9,NULL);
	bitrateMenu->AddItem(item);
	menu->AddItem(bitrateMenu);

	//Output Format Menu
	BMenu* outputFormatMenu = new BMenu(OUTPUT_FORMAT_STR);
	outputFormatMenu->SetRadioMode(true);
	item = new BMenuItem(STEREO,NULL);
	item->SetMarked(true);
	outputFormatMenu->AddItem(item);
	item = new BMenuItem(MONO,NULL);
	outputFormatMenu->AddItem(item);
	item = new BMenuItem(JSTEREO,NULL);
	outputFormatMenu->AddItem(item);
	menu->AddItem(outputFormatMenu);

	//Misc Items
	BMenu* psychoMenu = new BMenu(PSYCHO_ACOUSTICS_STR);
	psychoMenu->SetRadioMode(true);
	item = new BMenuItem(PSY_0,NULL);
	psychoMenu->AddItem(item);
	item = new BMenuItem(PSY_1,NULL);
	psychoMenu->AddItem(item);
	item = new BMenuItem(PSY_2,NULL);
	psychoMenu->AddItem(item);
	item = new BMenuItem(PSY_3,NULL);
	psychoMenu->AddItem(item);
	item = new BMenuItem(PSY_4,NULL);
	psychoMenu->AddItem(item);
	item = new BMenuItem(PSY_5,NULL);
	item->SetMarked(true);
	psychoMenu->AddItem(item);
	item = new BMenuItem(PSY_6,NULL);
	psychoMenu->AddItem(item);
	item = new BMenuItem(PSY_7,NULL);
	psychoMenu->AddItem(item);
	item = new BMenuItem(PSY_8,NULL);
	psychoMenu->AddItem(item);
	item = new BMenuItem(PSY_9,NULL);
	psychoMenu->AddItem(item);
	menu->AddItem(psychoMenu);
}

int32
MP3Lame::GetBitrate(char* bitrate, bool* vbr)
{
	PRINT(("MP3Lame::GetBitrate(char*,bool*)\n"));

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
	if(strcmp(label,_32KBPS) == 0)
	{
		*vbr = false;
		strcpy(bitrate,"32");
	}
	if(strcmp(label,_48KBPS) == 0)
	{
		*vbr = false;
		strcpy(bitrate,"48");
	}
	if(strcmp(label,_64KBPS) == 0)
	{
		*vbr = false;
		strcpy(bitrate,"64");
	}
	else if(strcmp(label,_96KBPS) == 0)
	{
		*vbr = false;
		strcpy(bitrate,"96");
	}
	else if(strcmp(label,_128KBPS) == 0)
	{
		*vbr = false;
		strcpy(bitrate,"128");
	}
	else if(strcmp(label,_160KBPS) == 0)
	{
		*vbr = false;
		strcpy(bitrate,"160");
	}
	else if(strcmp(label,_192KBPS) == 0)
	{
		*vbr = false;
		strcpy(bitrate,"192");
	}
	else if(strcmp(label,_256KBPS) == 0)
	{
		*vbr = false;
		strcpy(bitrate,"256");
	}
	else if(strcmp(label,_320KBPS) == 0)
	{
		*vbr = false;
		strcpy(bitrate,"320");
	}
	else if(strcmp(label,VBR_0) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"0");
	}
	else if(strcmp(label,VBR_1) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"1");
	}
	else if(strcmp(label,VBR_2) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"2");
	}
	else if(strcmp(label,VBR_3) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"3");
	}
	else if(strcmp(label,VBR_4) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"4");
	}
	else if(strcmp(label,VBR_5) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"5");
	}
	else if(strcmp(label,VBR_6) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"6");
	}
	else if(strcmp(label,VBR_7) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"7");
	}
	else if(strcmp(label,VBR_8) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"8");
	}
	else if(strcmp(label,VBR_9) == 0)
	{
		*vbr = true;
		strcpy(bitrate,"9");
	}

	return B_OK;
}

int32
MP3Lame::GetFormat(char* format)
{
	PRINT(("MP3Lame::GetFormat(char*)\n"));

	BMenuItem* item;
	BMenu* formatMenu;

	item = menu->FindItem(OUTPUT_FORMAT_STR);
	if(!item)
	{
		return B_ERROR;
	}
	formatMenu = item->Submenu();
	if(!formatMenu)
	{
		return B_ERROR;
	}

	item = formatMenu->FindMarked();
	if(!item)
	{
		return B_ERROR;
	}

	const char* label = item->Label();

	if(strcmp(label,STEREO) == 0)
	{
		strcpy(format,STEREO_CODE);
	}
	else if(strcmp(label,MONO) == 0)
	{
		strcpy(format,MONO_CODE);
	}
	else if(strcmp(label,JSTEREO) == 0)
	{
		strcpy(format,JSTEREO_CODE);
	}

	return B_OK;
}

int32
MP3Lame::GetPsycho(char* psycho)
{
	PRINT(("MP3Lame::GetPsycho(char*)\n"));
	BMenuItem* item;
	BMenu* psychoMenu;

	item = menu->FindItem(PSYCHO_ACOUSTICS_STR);
	if(!item)
	{
		return B_ERROR;
	}
	psychoMenu = item->Submenu();
	if(!psychoMenu)
	{
		return B_ERROR;
	}

	item = psychoMenu->FindMarked();
	if(!item)
	{
		return B_ERROR;
	}

	const char* label = item->Label();

	if(strcmp(label,PSY_0) == 0)
	{
		strcpy(psycho,"0");
	}
	else if(strcmp(label,PSY_1) == 0)
	{
		strcpy(psycho,"1");
	}
	else if(strcmp(label,PSY_2) == 0)
	{
		strcpy(psycho,"2");
	}
	else if(strcmp(label,PSY_3) == 0)
	{
		strcpy(psycho,"3");
	}
	else if(strcmp(label,PSY_4) == 0)
	{
		strcpy(psycho,"4");
	}
	else if(strcmp(label,PSY_5) == 0)
	{
		strcpy(psycho,"5");
	}
	else if(strcmp(label,PSY_6) == 0)
	{
		strcpy(psycho,"6");
	}
	else if(strcmp(label,PSY_7) == 0)
	{
		strcpy(psycho,"7");
	}
	else if(strcmp(label,PSY_8) == 0)
	{
		strcpy(psycho,"8");
	}
	else if(strcmp(label,PSY_9) == 0)
	{
		strcpy(psycho,"9");
	}

	return B_OK;
}

int32
MP3Lame::UpdateStatus(FILE* out, BMessenger* messenger)
{
	PRINT(("MP3Lame::UpdateStatus(FILE*,BMessenger*)\n"));

	float prev = 0.0;
	float curr = 0.0;
	char buffer[8];
	char begin_buf[2048];
	unsigned char c = 0;
	int i = 0;
	bool save = false;
	bool begin = false;

	while(1)
	{
		if(CheckForCancel())
		{
			PRINT(("Cancel Requested.\n"));
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

		c = fgetc(out);
		if(!begin)
		{
			if(c == ' ')
			{
				begin_buf[i] = 0;
				if(strcmp(begin_buf,"ETA") == 0)
				{
					begin = true;
				}
				i=0;
			}
			else
			{
				begin_buf[i] = c;
				i++;
			}
		}
		else
		{
			if(c == '(')
			{
				save = true;
				i=0;
			}
			else if(c == '/')
			{
				save = false;
				buffer[i] = 0;
				prev = curr;
				curr = atoi(buffer);
				BMessage updateMsg(B_UPDATE_STATUS_BAR);
				updateMsg.AddFloat("delta",(curr-prev));
				messenger->SendMessage(&updateMsg);
				if(curr > 99)
				{
					break;
				}
			}
			else if(save)
			{
				if(isdigit(c) || c == '.')
				{
					buffer[i] = c;
					i++;
				}
			}
		}
	}
	PRINT(("DONE ENCODING\n"));
	return B_OK;
}

int32
MP3Lame::WriteDetails(BMessage* message)
{
	PRINT(("MP3Lame::WriteDetails(BMessage*)\n"));

	int32 status = B_OK;

	const char* outputFile;
	const char* artist;
	const char* album;
	const char* title;
	const char* year;
	const char* comment;
	const char* track;
	const char* genre;

	if(message->FindString("output file",&outputFile) != B_OK)
	{
		return B_ERROR;
	}

	message->FindString("artist",&artist);
	message->FindString("album",&album);
	message->FindString("title",&title);
	message->FindString("year",&year);
	message->FindString("comment",&comment);
	message->FindString("track",&track);
	message->FindString("genre",&genre);

	dev_t device = dev_for_path(outputFile);
	BVolume volume(device);
	if(volume.InitCheck() != B_OK)
	{
		return B_ERROR;
	}

	BFile mp3File(outputFile,B_READ_WRITE);
	if(mp3File.InitCheck() != B_OK)
	{
		return B_ERROR;
	}

	if(volume.KnowsMime())
	{
		BNodeInfo info(&mp3File);
		if(info.InitCheck() != B_OK)
		{
			return B_ERROR;
		}
		if(info.SetType(MP3_MIME_TYPE) != B_OK)
		{
			status = B_ERROR;
		}
	}

	if(volume.KnowsAttr())
	{
		AudioAttributes attributes(&mp3File);
		attributes.SetArtist(artist);
		attributes.SetAlbum(album);
		attributes.SetTitle(title);
		attributes.SetYear(year);
		attributes.SetComment(comment);
		attributes.SetTrack(track);
		attributes.SetGenre(genre);
		if(attributes.Write() != B_OK)
		{
			status = B_ERROR;
		}
	}

	ID3Tags tags(outputFile);
	tags.SetArtist(artist);
	tags.SetAlbum(album);
	tags.SetTitle(title);
	tags.SetYear(year);
	tags.SetComment(comment);
	tags.SetTrack(track);
	tags.SetGenre(genre);
	if(tags.Write() != B_OK)
	{
		status = B_ERROR;
	}
	
	return status;
}

thread_id
MP3Lame::CommandIO(int* filedes, int argc, const char** argv)
{
	PRINT(("MP3Lame::CommandIO(int*,int,const char**)\n"));

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
	return new MP3Lame();
}
