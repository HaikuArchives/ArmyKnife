EXECUTABLE = Army\ Knife-M
INCLUDE_DIR = ../../include
LIBRARY_DIR = ../../lib
RELEASE_DIR = ../../release/Army\ Knife

HDRS =	Application.h\
			Preferences.h\
			AppWindow.h\
			AppView.h\
			AddOnView.h\
			EditorView.h\
			TAView.h\
			NAView.h\
			CreateMimetypes.h\
			PickListView.h\
			AudioAttribute.h\
			AudioAttributes.h\
			AudioInfo.h\
			GenreList.h\
			ID3Tag.h\
			ID3Tags.h\
			EntryRefItem.h\
			NAWindow.h\
			Barberpole.h\
			MPEGView.h


SRCS =	Application.cpp\
			Preferences.cpp\
			AppWindow.cpp\
			AppView.cpp\
			AddOnView.cpp\
			EditorView.cpp\
			TAView.cpp\
			NAView.cpp\
			CreateMimetypes.cpp\
			PickListView.cpp\
			AudioAttribute.cpp\
			AudioAttributes.cpp\
			AudioInfo.cpp\
			GenreList.cpp\
			ID3Tag.cpp\
			ID3Tags.cpp\
			EntryRefItem.cpp\
			NAWindow.cpp\
			Barberpole.cpp\
			MPEGView.cpp

OBJS = $(SRCS:.cpp=.o)

RSRCS = ArmyKnife.rsrc

LIBS = -lbe -lroot -lsanta -lmatcher -ltag -lstring

CC = g++
CFLAGS = -DDEBUG -g
LD = g++
LDFLAGS = -Xlinker -L$(LIBRARY_DIR)
XRES = xres
MIMESET = mimeset

$(EXECUTABLE) : $(OBJS)
	$(LD) -o $(EXECUTABLE) $(OBJS) $(LDFLAGS) $(LIBS)
	$(XRES) -o ""$(EXECUTABLE)"" $(RSRCS)
	$(MIMESET) -f "$(EXECUTABLE)"
	chmod 755 $(EXECUTABLE)
	mimeset $(EXECUTABLE)

clean  :
	rm -f *.o $(EXECUTABLE)

compile : $(OBJS)

install : $(EXECUTABLE)
	cp -f $(EXECUTABLE) $(RELEASE_DIR)/$(EXECUTABLE)

release_clean : clean
	rm -f $(RELEASE_DIR)/$(EXECUTABLE)

cleansettings :
	rm -rf /boot/home/config/settings/FlipSide\ Software/ArmyKnife/

release : CFLAGS = -O3
release : release_clean install
	make clean

%.o : %.cpp
	$(CC) -c $< $(CFLAGS) -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/taglib -o $@
