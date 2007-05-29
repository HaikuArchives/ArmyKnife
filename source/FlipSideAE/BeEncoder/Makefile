ADDON = BeEncoder
INCLUDE_DIR = ../../../include
LIBRARY_DIR = ../../../lib
RELEASE_DIR = ../../../release/FlipSide\ A.E.
ADDON_DIR = $(RELEASE_DIR)/add-ons

HDRS =	BeEncoder.h\
			MediaMenuItem.h

SRCS =	BeEncoder.cpp\
			MediaMenuItem.cpp

OBJS = $(SRCS:.cpp=.o)

LIBS = -lbe -lroot -lmedia -laudioinfo -laeencoder -lstring -ltag
CC = g++
CFLAGS = -DDEBUG -g
LD = g++
LDFLAGS = -nostart -Xlinker -soname="$(ADDON)" -L$(LIBRARY_DIR)
MIMESET = mimeset

$(ADDON) : $(OBJS)
	$(LD) -o $(ADDON) $(OBJS) $(LDFLAGS) $(LIBS)
	$(MIMESET) -f "$(ADDON)"
	chmod 755 $(ADDON)

clean  :
	rm -f *.o $(ADDON)

install : $(ADDON)
	cp -f $(ADDON) $(ADDON_DIR)/$(ADDON)

release_clean : clean
	rm -f $(ADDON_DIR)/$(ADDON)

release : CFLAGS = -O3
release : release_clean install
	make clean

%.o : %.cpp
	$(CC) -c $< $(CFLAGS) -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/taglib -o $@
