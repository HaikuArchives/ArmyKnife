LIBRARY = libaudioinfo.a
HEADER_DIR = ../../include/AudioInfo
INCLUDE_DIR = ../../include
LIBRARY_DIR = ../../lib

HDRS =	AudioAttribute.h\
			AudioAttributes.h\
			AudioInfo.h\
			GenreList.h\
			ID3Tag.h\
			ID3Tags.h

SRCS =	AudioAttribute.cpp\
			AudioAttributes.cpp\
			AudioInfo.cpp\
			GenreList.cpp\
			ID3Tag.cpp\
			ID3Tags.cpp

OBJS = $(SRCS:.cpp=.o)

LIBS = $(LIBRARY_DIR)/libstring.a
CC = g++
CFLAGS = -DDEBUG -g
EXTRACT = ar
EXTRACT_FLAGS = -x
AR = ar
AR_FLAGS = -rcus
MIMESET = mimeset

$(LIBRARY) : $(OBJS)
	$(EXTRACT) $(EXTRACT_FLAGS) $(LIBS)
	$(AR) $(AR_FLAGS) "$(LIBRARY)" *.o
	$(MIMESET) -f "$(LIBRARY)"
	chmod 755 $(LIBRARY)

clean  :
	rm -f *.o $(LIBRARY)

install : $(LIBRARY)
	mkdir -p $(HEADER_DIR)
	cp -f $(HDRS) $(HEADER_DIR)
	cp -f $(LIBRARY) $(LIBRARY_DIR)

release_clean : clean
	rm -rf $(HEADER_DIR)
	rm -f $(LIBRARY_DIR)/$(LIBRARY)

release : CFLAGS = -O3
release : release_clean install
	make clean

%.o : %.cpp
	$(CC) -c $< -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/taglib -o $@ $(CFLAGS)
