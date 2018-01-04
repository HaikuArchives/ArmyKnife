#ifndef __APP_WINDOW_H__
#define __APP_WINDOW_H__

#include <Menu.h>
#include <Window.h>
#include <String.h>

static const BString ReadmeLoc = "file:///boot/system/apps/ArmyKnife/Documentation/armyknife/readme.html";
static const BString ChangelogLoc = "file:///boot/system/apps/ArmyKnife/Documentation/armyknife_changelog.txt"; 

class AppView;
class BMessage;
class Preferences;

class AppWindow : public BWindow
{
	public:
		AppWindow();
		AppWindow(BMessage* archive);
		~AppWindow();
		static AppWindow* CreateWindow();
		virtual void MessageReceived(BMessage* message);
		virtual bool QuitRequested();
		virtual void FrameMoved(BPoint point);
		virtual void FrameResized(float width, float height);

		void DisableInterface();
		void EnableInterface();

	private:
		void InitWindow();
		void AboutRequested();

		AppView*	m_app_view;
		BMenuBar*	m_menu_bar;
		BMenu*		m_file_menu;
		BMenu*		m_edit_menu;
		BMenu*		m_mode_menu;
		BMenu*		m_options_menu;
		BMenu*		m_help_menu;
		BMenuItem*	m_about_menu_item;
		BMenuItem*	m_quit_menu_item;
		BMenuItem*	m_cut_menu_item;
		BMenuItem*	m_copy_menu_item;
		BMenuItem*	m_paste_menu_item;
		BMenuItem*	m_first_file_menu_item;
		BMenuItem*	m_last_file_menu_item;
		BMenuItem*	m_previous_file_menu_item;
		BMenuItem*	m_next_file_menu_item;
		BMenuItem*	m_reset_menu_item;
		BMenuItem*	m_clear_list_menu_item;
		BMenuItem*	m_select_all_menu_item;
		BMenuItem*	m_select_all_unsupported_menu_item;
		BMenuItem*	m_beep_menu_item;
		BMenuItem*	m_readme_item;
		BMenuItem*	m_changelog_item;

};

#endif
