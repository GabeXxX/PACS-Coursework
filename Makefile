CXX      ?= g++
CXXFLAGS ?= -std=c++20
LINK.o := $(LINK.cc) 

CPPFLAGS += -O3 -Wall -I.

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.hpp)

exe_sources = $(filter main%.cpp,$(SRCS))
EXEC = $(exe_sources:.cpp=)

.PHONY = all parallel clean distclean

.DEFAULT_GOAL = all

all: $(EXEC)

parallel: CPPFLAGS += -DPARALLELEXEC -I$(mkTbbInc)
parallel: LDLIBS += -L$(mkTbbLib) -ltbb
parallel: all

$(EXEC): $(OBJS)

$(OBJS): $(SRCS) $(HEADERS)

clean:
	$(RM) -f $(OBJS)

distclean: clean
	$(RM) -f $(EXEC)
	$(RM) *.out *.bak *~