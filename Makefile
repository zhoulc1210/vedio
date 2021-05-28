##### Make sure all is the first target.
all:

CXX ?= g++
CC  ?= gcc

CFLAGS  += -g -pthread -Wall -Werror -Wno-write-strings -Wno-sign-compare -Wno-unused-but-set-variable -Wno-unused-variable -Werror=unused-value
LDFLAGS += -lpthread -pthread -ldl -lm -lz
INCLUDE += -I /usr/local/ffmpeg/include
LIBPATH += -L /usr/local/ffmpeg/lib 
LIBVAR += -lavcodec -lavformat -lavutil -lswresample

CXX_SRC=

CXX_SRC += main.cpp

MK := $(word 1,$(MAKEFILE_LIST))
ME := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))

OBJ=$(CXX_SRC:.cpp=.o) $(C_SRC:.c=.o)
DEP=$(OBJ:.o=.d) $(TARGET_OBJ:.o=.d)

CXXFLAGS += -std=c++11

TARGETS = test

.PHONY: all clean distclean

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	find . -name "*.o" -delete
	find . -name "*.d" -delete

distclean:
	rm -f $(TARGETS)
	find . -name "*.o" -delete
	find . -name "*.d" -delete

-include $(DEP)

%.o: %.c $(MK) $(ME)
	@$(CC) -c $< -MM -MT $@ -MF $(@:.o=.d) $(CFLAGS)
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.cpp $(MK) $(ME)
	@$(CXX) -c $< -MM -MT $@ -MF $(@:.o=.d) $(CXXFLAGS) $(CFLAGS)
	$(CXX) -c $< -o $@ $(INCLUDE) $(CXXFLAGS) $(CFLAGS)

$(TARGETS): $(OBJ)
	$(CXX) -o $(TARGETS) $^ $(CXXFLAGS) $(CFLAGS) $(LDFLAGS) $(LIBPATH) $(LIBVAR) 
	find . -name "*.o" -delete
	find . -name "*.d" -delete
