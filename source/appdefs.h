#ifndef __APP_DEFS_H__
#define __APP_DEFS_H__

#define AUTHOR "Jason Burgess"
#define PROJECT "FlipSide Software"
#define PROJECT_WWW "http://dev.osdrawer.net/projects/show/armyknife"
#define MAINTAINER "Jonas Sundström"
#define MAINTAINER_EMAIL "jonas@kirilla.com"

#ifdef _TTE_
#define APPLICATION "The Army Knife - TuneTracker Edition"
#define APPLICATION_DIR "ArmyKnifeTTE"
#define SIGNATURE "application/x-vnd.armyknife-tte"
#else
#define APPLICATION "The Army Knife"
#define APPLICATION_DIR "ArmyKnife"
#define SIGNATURE "application/x-vnd.armyknife"
#endif

#define VERSION "5.1.0"

#define PROJECT_DIR "FlipSide Software"
#define WINDOW_FILE "window"
#define PREFS_FILE "preferences"
#define DOCUMENTATION_DIR "Documentation"
#define README_FILE "readme.html"
#define CHANGELOG_FILE "changelog.txt"

#define MP3_MIME_TYPE "audio/mpeg"
#define OGG_MIME_TYPE "audio/ogg"
#define FLAC_MIME_TYPE "audio/flac"

#endif
