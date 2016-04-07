

SRCS := $(shell ls *.cpp)
#SRCS_C := $(shell ls *.c)

ifndef VERSION
VERSION=release
endif

NAME=drosopi

PORTAUDIO=/home/ben/code/portaudio/portaudio

export PATH:=/usr/local/gcc-4.9.2/bin:$(PATH)

$(shell mkdir -p $(VERSION))

ifeq ($(VERSION),debug)
VERSION_FLAGS=-DDEBUG -O0 -fno-inline
VERSION_LD_FLAGS=
endif
ifeq ($(VERSION),release)
VERSION_FLAGS=-O3 -DNDEBUG -fno-strict-aliasing
VERSION_LD_FLAGS=-rdynamic
endif

LDFLAGS=-static-libstdc++ -static-libgcc -L$(PORTAUDIO)/lib -Wl,-Bstatic -lportaudio -Wl,-Bdynamic -lpthread -lrt

ifndef MKFLAGS
MKFLAGS=-j4
endif

CFLAGS= $(VERSION_FLAGS) -ggdb -Wall -fPIC -MD -msse2 -I$(PORTAUDIO)/include
CPPFLAGS= $(VERSION_FLAGS) -Wno-deprecated -ggdb -Wall -fPIC -MD -msse2 -std=c++11 -I$(PORTAUDIO)/include

OBJS = $(SRCS:%.cpp=$(VERSION)/%.o)

all : $(VERSION)/$(NAME)

$(VERSION)/$(NAME): $(OBJS)
	g++ -o $@ $(VERSION_LD_FLAGS) $(OBJS) $(DEPOBJS) $(LDFLAGS) 

$(OBJS): $(VERSION)/%.o: %.cpp 
	g++ -c $(CPPFLAGS) $< -o $@

clean:
	rm -f $(VERSION)/*.o $(VERSION)/*.a $(VERSION)/*.d $(VERSION)/$(NAME)

#configure:
#	mkdir -p $(VERSION)
#	echo SRCS = *.cpp > filelist

DEPS = $(SRCS:%.cpp=$(VERSION)/%.d)
sinclude $(DEPS)
	
