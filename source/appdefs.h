/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __APP_DEFS_H__
#define __APP_DEFS_H__

#define AUTHOR "Jason Burgess"
#define PROJECT "FlipSide Software"
#define PROJECT_WWW "github.com/HaikuArchives/ArmyKnife"
#define MAINTAINER "Scott McCreary"
#define MAINTAINER_EMAIL "scottmc2@gmail.com"

#ifdef _TTE_
#define APPLICATION "ArmyKnife - TuneTracker Edition"
#define APPLICATION_DIR "ArmyKnifeTTE"
#define SIGNATURE "application/x-vnd.armyknife-tte"
#else
#define APPLICATION "ArmyKnife"
#define APPLICATION_DIR "ArmyKnife"
#define SIGNATURE "application/x-vnd.armyknife"
#endif

#define VERSION "5.1.1"

#define PROJECT_DIR "FlipSide Software"
#define WINDOW_FILE "window"
#define PREFS_FILE "preferences"
#define DOCUMENTATION_DIR "documentation"
#define README_FILE "armyknife/readme.html"
#define CHANGELOG_FILE "changelog.txt"

#define MP3_MIME_TYPE "audio/mpeg"
#define OGG_MIME_TYPE "audio/ogg"
#define FLAC_MIME_TYPE "audio/flac"

#endif
