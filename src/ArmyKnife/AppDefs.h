#ifndef __APP_DEFS_H__
#define __APP_DEFS_H__

#define AUTHOR "Jason Burgess"
#define PROJECT "FlipSide Software"
#define PROJECT_WWW "http://www.beunited.org"
#define MAINTAINER "Jonas Sundström"
#define MAINTAINER_EMAIL "jonas@kirilla.com"

#ifdef _TTE_
#define APPLICATION "The Army Knife - TuneTracker Edition"
#define APPLICATION_DIR "ArmyKnifeTTE"
#define SIGNATURE "application/x-vnd.beunited.armyknife-tte"
#else
#define APPLICATION "The Army Knife"
#define APPLICATION_DIR "ArmyKnife"
#define SIGNATURE "application/x-vnd.beunited.armyknife"
#endif

#define VERSION "4.0.0"

#define PROJECT_DIR "FlipSide Software"
#define WINDOW_FILE "window"
#define PREFS_FILE "preferences"

#define MP3_MIME_TYPE "audio/x-mpeg"
#define OGG_MIME_TYPE "audio/x-vorbis"
#define FLAC_MIME_TYPE "audio/x-flac"

#endif
