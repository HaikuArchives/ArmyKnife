#include <be/app/Application.h>
#include <be/app/Message.h>
#include <be/support/Beep.h>
#include <be/support/Debug.h>
#include "AppDefs.h"
#include "Application.h"
#include "AppWindow.h"
#include "GUIStrings.h"
#include "Settings.h"

Application::Application() : BApplication(SIGNATURE)
{
	PRINT(("Application::Application()\n"));

	add_system_beep_event(SYSTEM_BEEP_ENCODING_DONE);

	window = AppWindow::GetInstance();
	window->Show();
}

Application::~Application()
{
	PRINT(("Application::~Application()\n"));

	delete settings;
}

void
Application::ArgvReceived(int32 argc, char** argv)
{
	PRINT(("Application::ArgvReceived(int32,char**)\n"));
}

void
Application::MessageReceived(BMessage* message)
{
	//PRINT(("Application::MessageReceived(BMessage*)\n"));

	switch(message->what)
	{
		default:
			BApplication::MessageReceived(message);
	}
}

void
Application::RefsReceived(BMessage* message)
{
	PRINT(("Application::RefsReceived(BMessage*)\n"));

	window->PostMessage(message);
}

void
main()
{
	PRINT(("main()\n"));

	new Application();
	be_app->Run();
	delete be_app;
}
