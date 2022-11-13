/*
 * Copyright 2000-2021, ArmyKnife Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __GUI_STRINGS_H__
#define __GUI_STRINGS_H__

#include <Catalog.h>

#include "appdefs.h"

#define ABOUT_TEXT B_TRANSLATE_CONTEXT("The all-in-one metadata editor for audio and MPEG files.", "About window")

#define WIN_TITLE B_TRANSLATE_SYSTEM_NAME(APPLICATION)
#define PREF_TITLE B_TRANSLATE_CONTEXT("ArmyKnife preferences", "Window title")
#define LOOKUP_TITLE B_TRANSLATE_CONTEXT("Online tag lookup", "Window title")

#define FILE_MENU B_TRANSLATE_CONTEXT("File", "File menu")
#define OPEN_ITEM B_TRANSLATE_CONTEXT("Open", "File menu")
#define ABOUT_ITEM B_TRANSLATE_CONTEXT("About ArmyKnife", "File menu")
#define QUIT_ITEM B_TRANSLATE_CONTEXT("Quit", "File menu")
#define OPEN_SHORTCUT 'O'
#define QUIT_SHORTCUT 'Q'

#define EDIT_MENU B_TRANSLATE_CONTEXT("Edit", "Edit menu: Copy/Paste")
#define CUT_ITEM B_TRANSLATE_CONTEXT("Cut", "Edit menu: Copy/Paste")
#define CUT_ITEM_SHORTCUT 'X'
#define COPY_ITEM B_TRANSLATE_CONTEXT("Copy", "Edit menu: Copy/Paste")
#define COPY_ITEM_SHORTCUT 'C'
#define PASTE_ITEM B_TRANSLATE_CONTEXT("Paste", "Edit menu: Copy/Paste")
#define PASTE_ITEM_SHORTCUT 'V'

#define FIRST_FILE_ITEM B_TRANSLATE_CONTEXT("First file", "Edit menu: Selecting files")
#define FIRST_FILE_SHORTCUT B_HOME
#define LAST_FILE_ITEM B_TRANSLATE_CONTEXT("Last file", "Edit menu: Selecting files")
#define LAST_FILE_SHORTCUT B_END
#define PREVIOUS_FILE_ITEM B_TRANSLATE_CONTEXT("Previous file", "Edit menu: Selecting files")
#define PREVIOUS_FILE_SHORTCUT B_UP_ARROW
#define NEXT_FILE_ITEM B_TRANSLATE_CONTEXT("Next file", "Edit menu: Selecting files")
#define NEXT_FILE_SHORTCUT B_DOWN_ARROW
#define RESET_ITEM B_TRANSLATE_CONTEXT("Reset", "Edit menu: Selecting files")
#define RESET_SHORTCUT 'R'
#define CLEAR_LIST_ITEM B_TRANSLATE_CONTEXT("Clear list", "Edit menu: Selecting files")
#define CLEAR_LIST_SHORTCUT 'L'
#define SELECT_ALL_ITEM B_TRANSLATE_CONTEXT("Select all", "Edit menu: Selecting files")
#define SELECT_ALL_UNSUPPORTED_ITEM B_TRANSLATE_CONTEXT("Select all unsupported", "Edit menu: Selecting files")
#define SELECT_ALL_SHORTCUT 'A'

#define MODE_MENU B_TRANSLATE_CONTEXT("Mode", "Mode menu")
#define PREVIOUS_MODE_NAME B_TRANSLATE_CONTEXT("Previous mode", "Mode menu")
#define PREVIOUS_MODE_SHORTCUT B_LEFT_ARROW
#define NEXT_MODE_NAME B_TRANSLATE_CONTEXT("Next mode", "Mode menu")
#define NEXT_MODE_SHORTCUT B_RIGHT_ARROW
#define EDITOR_MODE_NAME B_TRANSLATE_CONTEXT("Edit", "Mode menu")
#define TA_MODE_NAME B_TRANSLATE_CONTEXT("Copy", "Mode menu")
#define NA_MODE_NAME B_TRANSLATE_CONTEXT("Name", "Mode menu")
#define MPEG_MODE_NAME B_TRANSLATE_CONTEXT("MPEG", "Mode menu")
#define OGG_MODE_NAME B_TRANSLATE_CONTEXT("OGG", "Mode menu")
#define FLAC_MODE_NAME B_TRANSLATE_CONTEXT("FLAC", "Mode menu")
#define TT_INFO_MODE_NAME B_TRANSLATE_CONTEXT("Info", "Mode menu")

#define HELP_MENU B_TRANSLATE_CONTEXT("Help", "Help menu")
#define README_ITEM B_TRANSLATE_CONTEXT("Readme", "Help menu")
#define CHANGELOG_ITEM B_TRANSLATE_CONTEXT("Changelog", "Help menu")

#define OPTIONS_MENU B_TRANSLATE_CONTEXT("Options", "Options menu")
#define BEEP_ON_UNSUPPORTED B_TRANSLATE_CONTEXT("Beep on unsupported files", "Options menu")

#define ABOUT_BTN B_TRANSLATE_ALL("Excellent", "About message", "In context, this means \"dismiss\"")

#define ARTIST_PATTERN_INFO B_TRANSLATE_ALL("Artist = /a", "Pattern explanation", "Leave /a untouched.")
#define ALBUM_PATTERN_INFO B_TRANSLATE_ALL("Album = /n", "Pattern explanation", "Leave /n untouched.")
#define TITLE_PATTERN_INFO B_TRANSLATE_ALL("Title = /t", "Pattern explanation", "Leave /t untouched.")
#define YEAR_PATTERN_INFO B_TRANSLATE_ALL("Year = /y", "Pattern explanation", "Leave /y untouched.")
#define COMMENT_PATTERN_INFO B_TRANSLATE_ALL("Comment = /c", "Pattern explanation", "Leave /c untouched.")
#define TRACK_PATTERN_INFO B_TRANSLATE_ALL("Track = /k", "Pattern explanation", "Leave /k untouched.")
#define GENRE_PATTERN_INFO B_TRANSLATE_ALL("Genre = /g", "Pattern explanation", "Leave /g untouched.")
#define WILDCARD_PATTERN_INFO B_TRANSLATE_ALL("Wildcard = /*", "Pattern explanation", "Leave /* untouched.")

#define ARTIST_PATTERN "/a"
#define ALBUM_PATTERN "/n"
#define TRACK_PATTERN "/k"
#define TITLE_PATTERN "/t"
#define YEAR_PATTERN "/y"
#define COMMENT_PATTERN "/c"
#define GENRE_PATTERN "/g"

#define COVER_LABEL B_TRANSLATE_CONTEXT("Cover", "Tag type")
#define ARTIST_LABEL B_TRANSLATE_CONTEXT("Artist", "Tag type")
#define ALBUM_LABEL B_TRANSLATE_CONTEXT("Album", "Tag type")
#define TITLE_LABEL B_TRANSLATE_CONTEXT("Title", "Tag type")
#define YEAR_LABEL B_TRANSLATE_CONTEXT("Year", "Tag type")
#define COMMENT_LABEL B_TRANSLATE_CONTEXT("Comment", "Tag type")
#define TRACK_LABEL B_TRANSLATE_CONTEXT("Track", "Tag type")
#define GENRE_LABEL B_TRANSLATE_CONTEXT("Genre", "Tag type")

#define PATTERN_LABEL B_TRANSLATE_CONTEXT("Filename:", "Filename pattern label")
#define PATTERN_MENU_NEW B_TRANSLATE_CONTEXT("New" B_UTF8_ELLIPSIS, "New filename pattern")
#define PATTERN_MENU_DELETE B_TRANSLATE_CONTEXT("Delete current", "Delete filename pattern")

#ifdef __HAIKU__
#define A2T_LABEL B_TRANSLATE_CONTEXT("Attributes \xe2\x86\x92 Tags", "Conversion type")
#define T2A_LABEL B_TRANSLATE_CONTEXT("Tags \xe2\x86\x92 Attributes", "Conversion type")
#define A2N_LABEL B_TRANSLATE_CONTEXT("Attributes \xe2\x86\x92 Name", "Conversion type")
#define N2A_LABEL B_TRANSLATE_CONTEXT("Name \xe2\x86\x92 Attributes", "Conversion type")
#else
#define A2T_LABEL "Attributes -> Tags"
#define T2A_LABEL "Tags -> Attributes"
#define A2N_LABEL "Attributes -> Name"
#define N2A_LABEL "Name -> Attributes"
#endif

#define EDIT_LABEL B_TRANSLATE_CONTEXT("Edit", "Edit mode operation")
#define ATTRIBUTES_LABEL B_TRANSLATE_CONTEXT("Attributes", "Edit mode operation")
#define TAGS_LABEL B_TRANSLATE_CONTEXT("Tags", "Edit mode operation")
#define APPLY_TO_ATTRIBUTES B_TRANSLATE_CONTEXT("Also apply to attributes", "Edit mode operation")
#define APPLY_TO_TAGS B_TRANSLATE_CONTEXT("Also apply to tags", "Edit mode operation")

#define DRAG_FILES B_TRANSLATE_CONTEXT("Drag and drop files here", "Empty list")
#define NO_IMAGE B_TRANSLATE_CONTEXT("No image", "Status")
#define IDLE B_TRANSLATE_CONTEXT("idle", "Status")

#define ADD_BUTTON B_TRANSLATE_CONTEXT("Add", "Main action")
#define APPLY_BUTTON B_TRANSLATE_CONTEXT("Save", "Main action")
#define RESET_BUTTON B_TRANSLATE_CONTEXT("Reset", "Main action")
#define CANCEL_BUTTON B_TRANSLATE_CONTEXT("Cancel", "Main action")
#define SELECTED_TEXT B_TRANSLATE_CONTEXT(" selected", "Main action")

#define DEFAULT_NA_PATTERN_LABEL B_TRANSLATE_CONTEXT("Default name & attributes pattern", "Default")
#define DEFAULT_FUNCTION_LABEL B_TRANSLATE_CONTEXT("Default function", "Default")

#define MPEG_INFO_BOX_LABEL B_TRANSLATE_CONTEXT("Info", "MPEG information")
#define MPEG_FILES_STRING_DEFAULT B_TRANSLATE_CONTEXT("0 files in selection", "MPEG information")
#define MPEG_ID3V1_STRING_DEFAULT B_TRANSLATE_CONTEXT("0 have ID3v1 tags", "MPEG information")
#define MPEG_ID3V2_STRING_DEFAULT B_TRANSLATE_CONTEXT("0 have ID3v2 tags", "MPEG information")
#define MPEG_APE_STRING_DEFAULT B_TRANSLATE_CONTEXT("0 have APE tags", "MPEG information")

#define MPEG_ADD_REMOVE_BOX_LABEL B_TRANSLATE_CONTEXT("Add/Remove", "MPEG operation")
#define MPEG_ADD_RADIOBUTTON B_TRANSLATE_CONTEXT("Add", "MPEG operation")
#define MPEG_REMOVE_RADIOBUTTON B_TRANSLATE_CONTEXT("Remove", "MPEG operation")

#ifdef _TTE_
#define RATING_LABEL "Rating"
#define TEMPO_LABEL "Tempo"
#define COMPOSER_LABEL "Composer"
#define GENDER_LABEL "Gender"
#endif

#define QUERY_START_QUERY B_TRANSLATE_CONTEXT("Online tag lookup" B_UTF8_ELLIPSIS, "Editor view button")
#define QUERY_MORE B_TRANSLATE_CONTEXT("Show more", "Query lookup button")
#define QUERY_WORKING B_TRANSLATE_CONTEXT("Working" B_UTF8_ELLIPSIS, "Query lookup button")
#define QUERY_NO_MORE_RESULTS B_TRANSLATE_CONTEXT("No more results", "Query lookup button")

#endif
