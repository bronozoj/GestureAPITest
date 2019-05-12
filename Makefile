CXX = g++

CXXFLAGS =   
LDFLAGS = --static -s
LDLIBS = 
REZFLAGS = 

SRC = $(wildcard *.cc)
OBJ = $(SRC:.cc=.o)
PROGNAME = test.exe

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ)  $(LDLIBS)

$.o: %.c
	$(CXX) -o $@ -mc $^ $(CXXFLAGS)

clean:
	rm -f $(OBJ)
	rm -f $(PROGNAME)
