#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <Application.h>

class AppWindow;
class BMessage;

class Application : public BApplication
{
	public:

		Application();
		virtual void ArgvReceived(int32 argc, char** argv);
		virtual void RefsReceived(BMessage* message);

	private:

		AppWindow*	m_window;

};

#endif
