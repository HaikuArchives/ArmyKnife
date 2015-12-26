#include <add-ons/tracker/TrackerAddOn.h>
#include <app/Application.h>
#include <app/Message.h>
#include <app/Roster.h>
#include <support/Debug.h>
#include <storage/Entry.h>

#include "appdefs.h"
#include "application.h"
#include "appwindow.h"
#include "createmimetypes.h"

Application::Application()
 :	BApplication	(SIGNATURE),
	m_window		(NULL)
{
	PRINT(("Application::Application()\n"));

#ifndef __HAIKU__
	CreateMimetypes();
#endif

	m_window = AppWindow::CreateWindow();
	m_window->Show();
}

void
Application::ArgvReceived(int32 argc, char** argv)
{
	PRINT(("Application::ArgvReceived()\n"));

	BMessage msg (B_REFS_RECEIVED);

	for (int32 i = 1;  i < argc;  i++)
	{
		BEntry entry (argv[i]);

		entry_ref ref;
		entry.GetRef (& ref);

		if (entry.Exists())
			msg.AddRef ("refs", & ref);
		else
			printf("File not found: %s\n", argv[i]);
	}

	RefsReceived(& msg);
}

void
Application::RefsReceived(BMessage* message)
{
	PRINT(("Application::RefsReceived()\n"));

	m_window->PostMessage(message);
}

void
main()
{
	PRINT(("main()\n"));

	new Application();
	be_app->Run();
	delete be_app;
}

void
process_refs(entry_ref dir_ref, BMessage* msg, void* reserved)
{
	PRINT(("process_refs()\n"));

	msg->what = B_REFS_RECEIVED;
	be_roster->Launch (SIGNATURE, msg );
}
