#Please set XERCESDIR, READLINEDIR, and XMLUTILITIES_PATH
#Check XMLUtilities
ifndef XMLUTILITIES_PATH
XMLUTILITIES_PATH = ..
endif

CXXFLAGS = -I$(XMLUTILITIES_PATH)/include -I/$(XERCESDIR)/include -I/$(READLINEDIR)/include
LDFLAGS = -L/$(XERCESDIR)/lib -lxerces-c -L/$(READLINEDIR)/lib -lreadline

TARGETS = \
main_example \
xmlushow \
xmlumodify

TARGETS_OBJECTS = $(addsuffix .o, $(basename $(TARGETS)))
TARGETS_SOURCES = $(addsuffix .cpp, $(basename $(TARGETS)))

.PHONY : all

all : $(TARGETS)

$(TARGETS) : $(TARGETS_SOURCES)
	$(CXX) -g $(CXXFLAGS) -o $@ $@.cpp $(LDFLAGS)
	rm -r $@.dSYM
	        
install :
	rm -rf ../bin
	mkdir ../bin
	cp $(TARGETS) ../bin

clean :
	rm -rf $(TARGETS) $(addsuffix .o, $(TARGETS)) $(addsuffix .dSYM, $(TARGETS))
	rm -rf ../bin
