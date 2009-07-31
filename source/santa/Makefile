LIBRARY = libsanta.a
HEADER_DIR = ../../include/Santa
INCLUDE_DIR = ../../include
LIBRARY_DIR = ../../lib

HDRS =	BetterScrollView.h\
			Colors.h\
			ScrollViewCorner.h

SRCS =	BetterScrollView.cpp\
			ScrollViewCorner.cpp

OBJS = $(SRCS:.cpp=.o)

CC = g++
CFLAGS = -DDEBUG -g
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
	cp -f $(HDRS) $(HEADER_DIR)
	cp -f $(LIBRARY) $(LIBRARY_DIR)

release_clean : clean
	rm -rf $(HEADER_DIR)
	rm -f $(LIBRARY_DIR)/$(LIBRARY)

release : CFLAGS = -O3
release : release_clean install
	make clean

%.o : %.cpp
	$(CC) -c $< $(CFLAGS) -I$(INCLUDE_DIR) -o $@
