#ifndef __GUI_STRINGS_H__
#define __GUI_STRINGS_H__

#include "AppDefs.h"

#define WIN_TITLE APPLICATION
#define PREF_TITLE "The Army Knife Preferences"

#define FILE_MENU "File"
#define ABOUT_ITEM "About..."
#define QUIT_ITEM "Quit"
#define QUIT_SHORTCUT 'Q'

#define EDIT_MENU "Edit"
#define FIRST_FILE_ITEM "First File"
#define FIRST_FILE_SHORTCUT B_HOME
#define LAST_FILE_ITEM "Last File"
#define LAST_FILE_SHORTCUT B_END
#define PREVIOUS_FILE_ITEM "Previous File"
#define PREVIOUS_FILE_SHORTCUT B_UP_ARROW
#define NEXT_FILE_ITEM "Next File"
#define NEXT_FILE_SHORTCUT B_DOWN_ARROW
#define RESET_ITEM "Reset"
#define RESET_SHORTCUT 'R'
#define CLEAR_LIST_ITEM "Clear List"
#define CLEAR_LIST_SHORTCUT 'L'
#define SELECT_ALL_ITEM "Select All"
#define SELECT_ALL_SHORTCUT 'A'

#define MODE_MENU "Mode"
#define PREVIOUS_MODE_NAME "Previous Mode"
#define PREVIOUS_MODE_SHORTCUT B_LEFT_ARROW
#define NEXT_MODE_NAME "Next Mode"
#define NEXT_MODE_SHORTCUT B_RIGHT_ARROW
#define EDITOR_MODE_NAME "Edit"
#define TA_MODE_NAME "Copy"
#define NA_MODE_NAME "Name" 
#define MPEG_MODE_NAME "MPEG"
#define OGG_MODE_NAME "OGG"
#define FLAC_MODE_NAME "FLAC"

#define OPTIONS_MENU "Options"
#define BEEP_ON_UNSUPPORTED "Beep On Unsupported Files"

#define ABOUT_BTN "Excellent"

#define ARTIST_PATTERN "Artist = /a"
#define ALBUM_PATTERN "Album = /n"
#define TITLE_PATTERN "Title = /t"
#define YEAR_PATTERN "Year = /y"
#define COMMENT_PATTERN "Comment = /c"
#define TRACK_PATTERN "Track = /k"
#define GENRE_PATTERN "Genre = /g"
#define WILDCARD_PATTERN "Wildcard = /*"

#define ARTIST_PAT "/a"
#define ALBUM_PAT "/n"
#define TRACK_PAT "/k"
#define TITLE_PAT "/t"
#define YEAR_PAT "/y"
#define COMMENT_PAT "/c"
#define GENRE_PAT "/g"

#define ARTIST_LABEL "Artist"
#define ALBUM_LABEL "Album"
#define TITLE_LABEL "Title"
#define YEAR_LABEL "Year"
#define COMMENT_LABEL "Comment"
#define TRACK_LABEL "Track"
#define GENRE_LABEL "Genre"

#define PATTERN_LABEL "Pattern:"

#define DIRECTION_LABEL "Choose Direction"
#define A2T_LABEL "Attributes -> Tags"
#define T2A_LABEL "Tags -> Attributes"
#define A2N_LABEL "Attributes -> Name"
#define N2A_LABEL "Name -> Attributes"

#define EDIT_LABEL "Edit"
#define ATTRIBUTES_LABEL "Attributes"
#define TAGS_LABEL "Tags"
#define APPLY_TO_ATTRIBUTES "Also Apply To Attributes"
#define APPLY_TO_TAGS "Also Apply To Tags"

#define ADD_BUTTON "Add"
#define APPLY_BUTTON "Apply"
#define RESET_BUTTON "Reset"
#define CANCEL_BUTTON "Cancel"
#define SELECTED_TEXT " selected"

#define DEFAULT_NA_PATTERN_LABEL "Default Name & Attributes Pattern"
#define DEFAULT_FUNCTION_LABEL "Default Function"

#define MPEG_INFO_BOX_LABEL "Info"
#define MPEG_FILES_STRING_DEFAULT "0 files in selection."
#define MPEG_ID3V1_STRING_DEFAULT "0 have ID3v1 tags."
#define MPEG_ID3V2_STRING_DEFAULT "0 have ID3v2 tags."
#define MPEG_APE_STRING_DEFAULT "0 have APE tags."

#define MPEG_ADD_REMOVE_BOX_LABEL "Add/Remove"
#define MPEG_ADD_RADIOBUTTON "Add"
#define MPEG_REMOVE_RADIOBUTTON "Remove"

#ifdef _TTE_
#define RATING_LABEL "Rating"
#define TEMPO_LABEL "Tempo"
#define COMPOSER_LABEL "Composer"
#endif

#endif
