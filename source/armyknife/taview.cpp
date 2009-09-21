#include <be/app/Message.h>
#include <be/app/Messenger.h>
#include <be/interface/Box.h>
#include <be/interface/CheckBox.h>
#include <be/interface/RadioButton.h>
#include <be/interface/Rect.h>
#include <be/interface/Window.h>
#include <be/storage/Entry.h>
#include <be/storage/File.h>
#include <be/storage/NodeInfo.h>
#include <be/storage/Path.h>
#include <be/support/Debug.h>
#include <be/support/List.h>
#include <be/support/SupportDefs.h>
#include "audioattributes.h"
#include "id3tags.h"
#include "appdefs.h"
#include "commandconstants.h"
#include "entryrefitem.h"
#include "guistrings.h"
#include "taview.h"

TAView::TAView(BRect frame, Preferences * preferences)
 :	AddOnView		(frame, TA_MODE_NAME),
 	m_preferences	(preferences)
{
	PRINT(("TAView::TAView(BRect)\n"));

	InitView();
}

TAView::~TAView()
{
	PRINT(("TAView::~TAView()\n"));
}

void
TAView::InitView()
{
	PRINT(("TAView::InitView()\n"));

	int space = 6;
	float bottom = 0;
	BRect frame(0,0,0,0);

	m_a2t_radiobutton = new BRadioButton(frame,"m_a2t_radiobutton",A2T_LABEL,NULL);
	m_a2t_radiobutton->SetValue(B_CONTROL_ON);
	m_a2t_radiobutton->ResizeToPreferred();
	m_a2t_radiobutton->MoveTo(space,2*space);
	bottom = m_a2t_radiobutton->Frame().bottom;

	m_t2a_radiobutton = new BRadioButton(frame,"m_t2a_radiobutton",T2A_LABEL,NULL);
	m_t2a_radiobutton->ResizeToPreferred();
	m_t2a_radiobutton->MoveTo(space,space+bottom);

	float width = 2*space +
		max_c(m_a2t_radiobutton->Frame().Width(),m_a2t_radiobutton->Frame().Width());

	float height = 4*space + 2*m_a2t_radiobutton->Frame().Height();

	m_direction_box = new BBox(BRect(0,0,width,height),"m_direction_box");
	m_direction_box->MoveTo(space,space);
	bottom = m_direction_box->Frame().bottom;

	m_artist_checkbox = new BCheckBox(frame,"m_artist_checkbox",ARTIST_LABEL,NULL);
	m_artist_checkbox->ResizeToPreferred();
	m_artist_checkbox->MoveTo(space,space+bottom);
	bottom = m_artist_checkbox->Frame().bottom;
	
	m_album_checkbox = new BCheckBox(frame,"m_album_checkbox",ALBUM_LABEL,NULL);
	m_album_checkbox->ResizeToPreferred();
	m_album_checkbox->MoveTo(space,space+bottom);
	bottom = m_album_checkbox->Frame().bottom;
	
	m_title_checkbox = new BCheckBox(frame,"m_title_checkbox",TITLE_LABEL,NULL);
	m_title_checkbox->ResizeToPreferred();
	m_title_checkbox->MoveTo(space,space+bottom);
	bottom = m_title_checkbox->Frame().bottom;
	
	m_year_checkbox = new BCheckBox(frame,"m_year_checkbox",YEAR_LABEL,NULL);
	m_year_checkbox->ResizeToPreferred();
	m_year_checkbox->MoveTo(space,space+bottom);
	bottom = m_year_checkbox->Frame().bottom;
	
	m_comment_checkbox = new BCheckBox(frame,"m_comment_checkbox",COMMENT_LABEL,NULL);
	m_comment_checkbox->ResizeToPreferred();
	m_comment_checkbox->MoveTo(space,space+bottom);
	bottom = m_comment_checkbox->Frame().bottom;
	
	m_track_checkbox = new BCheckBox(frame,"m_track_checkbox",TRACK_LABEL,NULL);
	m_track_checkbox->ResizeToPreferred();
	m_track_checkbox->MoveTo(space,space+bottom);
	bottom = m_track_checkbox->Frame().bottom;

	m_genre_checkbox = new BCheckBox(frame,"m_genre_checkbox",GENRE_LABEL,NULL);
	m_genre_checkbox->ResizeToPreferred();
	m_genre_checkbox->MoveTo(space,space+bottom);

	ResizeToPreferred();

	AddChild(m_direction_box);
	m_direction_box->AddChild(m_a2t_radiobutton);
	m_direction_box->AddChild(m_t2a_radiobutton);

	AddChild(m_artist_checkbox);
	AddChild(m_album_checkbox);
	AddChild(m_title_checkbox);
	AddChild(m_year_checkbox);
	AddChild(m_comment_checkbox);
	AddChild(m_track_checkbox);
	AddChild(m_genre_checkbox);

	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
TAView::AttachedToWindow()
{
	AddOnView::AttachedToWindow();

	PRINT(("TAView::AttachedToWindow()\n"));

	m_a2t_radiobutton->SetTarget(this);
	m_t2a_radiobutton->SetTarget(this);
}

void
TAView::GetPreferredSize(float* width, float* height)
{
	*width = m_direction_box->Frame().right;
	*height = m_genre_checkbox->Frame().bottom;
}

void
TAView::Hide()
{
	AddOnView::Hide();

	PRINT(("TAView::Hide()\n"));
}

void
TAView::Show()
{
	AddOnView::Show();

	PRINT(("TAView::Show()\n"));
}

void
TAView::SelectionChanged(BList* list)
{
	AddOnView::SelectionChanged(list);

	PRINT(("TAView::SelectionChanged(BList* list)\n"));

	WidgetsSetEnabled();
}

void
TAView::Apply()
{
	AddOnView::Apply();

	PRINT(("TAView::Apply()\n"));

	thread_id thread = spawn_thread(TAView::ApplyFunction,"Names & Attributes",
			B_NORMAL_PRIORITY,(void*)this);
	resume_thread(thread);
}

void
TAView::Reset()
{
	AddOnView::Reset();

	PRINT(("TAView::Reset()\n"));
	WidgetsSetValues();
	WidgetsSetEnabled();
}

void
TAView::WidgetsSetValues()
{
	PRINT(("TAView::WidgetsSetValues()\n"));

	m_a2t_radiobutton->SetValue(B_CONTROL_ON);
	m_artist_checkbox->SetValue(B_CONTROL_ON);
	m_album_checkbox->SetValue(B_CONTROL_ON);
	m_title_checkbox->SetValue(B_CONTROL_ON);
	m_year_checkbox->SetValue(B_CONTROL_ON);
	m_comment_checkbox->SetValue(B_CONTROL_ON);
	m_track_checkbox->SetValue(B_CONTROL_ON);
	m_genre_checkbox->SetValue(B_CONTROL_ON);
}

void
TAView::WidgetsSetEnabled()
{
	PRINT(("TAView::WidgetsSetEnabled()\n"));

	int numSelected = m_selected_items->CountItems();
	if(numSelected > 0 && m_list_view->HasSelectionOfOnlyAcceptedItems())
	{
		m_a2t_radiobutton->SetEnabled(true);
		m_t2a_radiobutton->SetEnabled(true);
		m_artist_checkbox->SetEnabled(true);
		m_album_checkbox->SetEnabled(true);
		m_title_checkbox->SetEnabled(true);
		m_year_checkbox->SetEnabled(true);
		m_comment_checkbox->SetEnabled(true);
		m_track_checkbox->SetEnabled(true);
		m_genre_checkbox->SetEnabled(true);
	}
	else
	{
		m_a2t_radiobutton->SetEnabled(false);
		m_t2a_radiobutton->SetEnabled(false);
		m_artist_checkbox->SetEnabled(false);
		m_album_checkbox->SetEnabled(false);
		m_title_checkbox->SetEnabled(false);
		m_year_checkbox->SetEnabled(false);
		m_comment_checkbox->SetEnabled(false);
		m_track_checkbox->SetEnabled(false);
		m_genre_checkbox->SetEnabled(false);
	}
}

int32
TAView::ApplyFunction(void* args)
{
	PRINT(("TAView::ApplyFunction(void*)\n"));

	TAView* view = (TAView*)args;

	BView* appView = view->Window()->FindView("ArmyKnifeAppView");
	BMessenger messenger(appView);
	BMessage startMsg(START_APPLY);
	BMessage endMsg(END_APPLY);
	BMessage maxMsg(STATUS_BAR_SET_MAX_VALUE);
	BMessage fileMsg(STATUS_BAR_FILE);
	BMessage updateMsg(STATUS_BAR_UPDATE);
	BMessage resetMsg(STATUS_BAR_RESET);

	messenger.SendMessage(&startMsg);

	bool a2t = false;
	bool doArtist = false;
	bool doAlbum = false;
	bool doTitle = false;
	bool doYear = false;
	bool doComment = false;
	bool doTrack = false;
	bool doGenre = false;

	if(view->m_a2t_radiobutton->Value() == B_CONTROL_ON)
	{
		a2t = true;
	}

	if(view->m_artist_checkbox->Value() == B_CONTROL_ON)
	{
		doArtist = true;
	}

	if(view->m_album_checkbox->Value() == B_CONTROL_ON)
	{
		doAlbum = true;
	}

	if(view->m_title_checkbox->Value() == B_CONTROL_ON)
	{
		doTitle = true;
	}

	if(view->m_year_checkbox->Value() == B_CONTROL_ON)
	{
		doYear = true;
	}

	if(view->m_comment_checkbox->Value() == B_CONTROL_ON)
	{
		doComment = true;
	}

	if(view->m_track_checkbox->Value() == B_CONTROL_ON)
	{
		doTrack = true;
	}

	if(view->m_genre_checkbox->Value() == B_CONTROL_ON)
	{
		doGenre = true;
	}

	int numSelected = view->m_selected_items->CountItems();
	maxMsg.AddInt32("maxValue",numSelected);
	messenger.SendMessage(&maxMsg);
	for(int i=0; i<numSelected; i++)
	{
		EntryRefItem* refItem = (EntryRefItem*)view->m_selected_items->ItemAt(i);

		if(refItem->IsSupportedByTaglib() == false)
		{
			PRINT(("TAView::ApplyFunction():: FILE NOT SUPPORTED :: %s\n", refItem->EntryRef()->name));
		
			continue;
		}
		
		BFile audioFile(refItem->EntryRef(), B_READ_WRITE);

		AudioAttributes attributes(& audioFile);
		ID3Tags * tags = new ID3Tags (refItem); // XXX
		
		fileMsg.MakeEmpty();
		fileMsg.AddString("file", refItem->EntryRef()->name);
		messenger.SendMessage(&fileMsg);

		if(a2t)
		{
			if(doArtist)
			{
				tags->SetArtist(attributes.Artist());
				tags->WriteArtist();
			}
			if(doAlbum)
			{
				tags->SetAlbum(attributes.Album());
				tags->WriteAlbum();
			}
			if(doTitle)
			{
				tags->SetTitle(attributes.Title());
				tags->WriteTitle();
			}
			if(doYear)
			{
				tags->SetYear(attributes.Year());
				tags->WriteYear();
			}
			if(doComment)
			{
				tags->SetComment(attributes.Comment());
				tags->WriteComment();
			}
			if(doTrack)
			{
				tags->SetTrack(attributes.Track());
				tags->WriteTrack();
			}
			if(doGenre)
			{
				tags->SetGenre(attributes.Genre());
				tags->WriteGenre();
			}
		}
		else
		{
			if(doArtist)
			{
				attributes.SetArtist(tags->Artist());
				attributes.WriteArtist();
			}
			if(doAlbum)
			{
				attributes.SetAlbum(tags->Album());
				attributes.WriteAlbum();
			}
			if(doTitle)
			{
				attributes.SetTitle(tags->Title());
				attributes.WriteTitle();
			}
			if(doYear)
			{
				attributes.SetYear(tags->Year());
				attributes.WriteYear();
			}
			if(doComment)
			{
				attributes.SetComment(tags->Comment());
				attributes.WriteComment();
			}
			if(doTrack)
			{
				attributes.SetTrack(tags->Track());
				attributes.WriteTrack();
			}
			if(doGenre)
			{
				attributes.SetGenre(tags->Genre());
				attributes.WriteGenre();
			}
		}
		
		delete tags;
		refItem->UpdateTaglibMetadata();
		
		messenger.SendMessage(&updateMsg);
	}
	messenger.SendMessage(&resetMsg);
	messenger.SendMessage(&endMsg);
}

bool
TAView::AcceptListItem(EntryRefItem* listItem)
{
	PRINT(("TAView::AcceptListItem()\n"));
	
	if (!listItem->IsFSWritable())
		return false;
		
	if (!listItem->IsFSAttributable())
		return false;
	
	if(!listItem->IsSupportedByTaglib())
		return false;

	return true;
}

