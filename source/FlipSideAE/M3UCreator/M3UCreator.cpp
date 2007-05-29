#include <stdlib.h>
#include <string.h>
#include <be/kernel/fs_info.h>
#include <be/app/Message.h>
#include <be/app/Messenger.h>
#include <be/interface/Menu.h>
#include <be/storage/File.h>
#include <be/storage/NodeInfo.h>
#include <be/support/Debug.h>

#include "M3UCreator.h"

M3UCreator::M3UCreator() : AEEncoder(ADDON_NAME)
{
	PRINT(("M3UCreator::M3UCreator()\n"));
}

M3UCreator::~M3UCreator()
{
	PRINT(("M3UCreator::~M3UCreator()\n"));

	// make sure we free strings
	int32 numFiles = fileList.CountItems();
	for(int32 i=0;i<numFiles;i++)
	{
		delete fileList.ItemAt(0);
	}
}

int32
M3UCreator::Encode(BMessage* message)
{
	PRINT(("M3UCreator::Encode(BMessage*)\n"));

	PRINT_OBJECT(*message);

	const char* audioFile;
	const char* outputFile;
	BMessenger messenger;
	BMessage updateMessage(B_UPDATE_STATUS_BAR);
	updateMessage.AddFloat("delta",20.0);
	
	//get arguments from message
	if(message->FindString("input file",&audioFile) != B_OK)
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

	//update the status bar
	messenger.SendMessage(&updateMessage);

	//check to see if we've written to this m3u during this session
	BFile m3uFile;
	bool found = false;
	int32 numFiles = fileList.CountItems();
	for(int32 i=0;i<numFiles;i++)
	{
		BString* file = (BString*)fileList.ItemAt(i);
		if(file->Compare(outputFile) == 0)
		{
			found = true;
			break;
		}
	}

	//update the status bar
	messenger.SendMessage(&updateMessage);

	if(found) //open for appending
	{
		if(m3uFile.SetTo(outputFile,B_READ_WRITE|B_OPEN_AT_END) != B_OK)
		{
			message->AddString("error","Error initializing output file.\n");
			return B_ERROR;
		}
	}
	else //open (create if doesn't exist) and erase contents
	{
		if(m3uFile.SetTo(outputFile,B_READ_WRITE|B_CREATE_FILE|B_ERASE_FILE)
				!= B_OK)
		{
			message->AddString("error","Error initializing output file.\n");
			return B_ERROR;
		}
		//add the item at 0 because it's likely to be written to next time
		fileList.AddItem(new BString(outputFile),0);
	}

	//update the status bar
	messenger.SendMessage(&updateMessage);

	//setup buffer
	BString buffer = audioFile;
	buffer << "\n";

	//write new entry to m3u file
	m3uFile.Write(buffer.String(),buffer.Length());

	//update the status bar
	messenger.SendMessage(&updateMessage);

	//Set mime type
	BNodeInfo info(&m3uFile);
	if(info.InitCheck() != B_OK)
	{
		message->AddString("error","Error init'ing BNodeInfo.\n");
		return B_ERROR;
	}
	if(info.SetType(M3U_MIME_TYPE) != B_OK)
	{
		message->AddString("error","Error setting type.\n");
		return B_ERROR;
	}

	//update the status bar
	messenger.SendMessage(&updateMessage);

	if(CheckForCancel())
	{
		return FSS_CANCEL_ENCODING;
	}

	return B_OK;
}

int32
M3UCreator::UninitEncoder()
{
	PRINT(("M3UCreator::UninitEncoder()\n"));

	// make sure we free strings
	int32 numFiles = fileList.CountItems();
	for(int32 i=0;i<numFiles;i++)
	{
		delete fileList.RemoveItem((int32)0);
	}

	return B_OK;
}

int32
M3UCreator::LoadDefaultPattern()
{
	PRINT(("M3UCreator::LoadDefaultPattern()\n"));

	pattern = "/boot/home/playlists/%a.m3u";

	return B_OK;
}

//function called by Flipside A.E. to get new AEEncoder subclass
AEEncoder*
load_encoder()
{
	return new M3UCreator();
}
