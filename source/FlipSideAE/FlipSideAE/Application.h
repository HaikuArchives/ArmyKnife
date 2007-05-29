#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <be/app/Application.h>

class AppWindow;
class BMessage;

class Application : public BApplication
{
	public:
		Application();
		~Application();
		virtual void ArgvReceived(int32 argc, char** argv);
		virtual void MessageReceived(BMessage* message);
		virtual void RefsReceived(BMessage* message);
	private:
		AppWindow* window;
};

#endif
