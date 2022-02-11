SYSTEMC_DIR = /usr/local/systemc-2.3.3
TARGET     = DMA
BUILDFLAGS  = -g3
CXX         = g++ -pthread
INCFLAGS    = -I. -I${SYSTEMC_DIR}/include
LDFLAGS     = -L${SYSTEMC_DIR}/lib-linux64 -lsystemc -lm
SRC_CPPHEAD = DMA Memory CPU
SRC_CPP     = $(SRC_CPPHEAD:=.cpp}
MAIN        = main.cpp
OBJECTS     = $(SRC_CPPHEAD:=.o)

EXE = $(TARGET)

all: $(EXE)

$(EXE): $(MAIN) $(OBJECTS) $(HEADERS)
	@echo "$@ building..."
	$(CXX) $(INCFLAGS) $(MAIN) $(OBJECTS) $(LDFLAGS) -o $@
	@echo ""
	@echo "$@ build done successfully..."
	@echo ""

%.o:%.cpp %.h
	$(CXX) -c $< $(INCFLAGS)

clean:
	rm -f $(EXE) \
	rm -f *.o
    
 
