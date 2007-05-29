LIBRARY = libaeencoder.a
HEADER_DIR = ../../../include/AEEncoder
INCLUDE_DIR = ../../../include
LIBRARY_DIR = ../../../lib
HEADER_INSTALL_DIR = ../../../release/FlipSide\ A.E./SDK/include
LIBRARY_INSTALL_DIR = ../../../release/FlipSide\ A.E./SDK/lib

HDRS =	AEEncoder.h

SRCS =	AEEncoder.cpp

OBJS = $(SRCS:.cpp=.o)

CC = g++
CFLAGS = -DDEBUG -D__FSS_BUILD -g
AR = ar
AR_FLAGS = -rcus
MIMESET = mimeset

$(LIBRARY) : $(OBJS)
	$(AR) $(AR_FLAGS) "$(LIBRARY)" *.o
	$(MIMESET) -f "$(LIBRARY)"
	chmod 755 $(LIBRARY)

clean  :
	rm -f *.o $(LIBRARY)

install : $(LIBRARY)
	mkdir -p $(HEADER_DIR)
	mkdir -p $(HEADER_INSTALL_DIR)
	mkdir -p $(LIBRARY_INSTALL_DIR)
	cp -f $(HDRS) $(HEADER_DIR)
	cp -f $(LIBRARY) $(LIBRARY_DIR)
	cp -f $(HDRS) $(HEADER_INSTALL_DIR)
	cp -f $(LIBRARY) $(LIBRARY_INSTALL_DIR)

release_clean : clean
	rm -rf $(HEADER_DIR)
	rm -f $(LIBRARY_DIR)/$(LIBRARY)
	rm -rf $(HEADER_INSTALL_DIR)
	rm -rf $(LIBRARY_INSTALL_DIR)

release : CFLAGS = -D__FSS_BUILD -O3
release : release_clean install
	make clean

%.o : %.cpp
	$(CC) -c $< $(CFLAGS) -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/taglib -o $@
