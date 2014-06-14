
HFILES = 

CFILES = 
CPPFILES = logger.cpp 
CPPFILES += loader.cpp objectptr.cpp objectimpl.cpp class.cpp
CPPFILES += classregistration.cpp mutex.cpp level.cpp
OFILES = $(CPPFILES:.cpp=.o)

THRDPTY= ../../fmcc/ThirdParty/host-native

CFLAGS = -Wall -I.. -I$(THRDPTY)/include -g

testlink: $(OFILES) testlink.o
	g++ -o $@ $(OFILES) testlink.o -L$(THRDPTY)/lib -lexpat

%.o: %.c $(HFILES)
	gcc -c $(CFLAGS) $<

%.o: %.cpp $(HFILES)
	g++ -c $(CFLAGS) $<

