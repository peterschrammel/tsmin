
SRC = example/tsmin.cpp

OBJ =

INCLUDES = 

CLEANFILES = tsmin

LIBS = 

###############################################################################
CXX = g++
CXXFLAGS = -Wall -O2 -g
CP_CXXFLAGS = -MMD -MP -DSTL_HASH_TR1

CP_CXXFLAGS += $(CXXFLAGS) $(INCLUDES)

LINKLIB = ld -r -o $@ $^
LINKBIN = $(CXX) $(LINKFLAGS) -o $@ $^ $(LIBS)

OBJ += $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRC)))

###############################################################################

all: example/tsmin

example/tsmin: $(OBJ)
	$(LINKBIN)

example/%.o: example/%.cpp
	$(CXX) -c $(CP_CXXFLAGS) -o $@ $<

clean:
	$(RM) $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRC))) \
		$(CLEANFILES)

regression:
	cd ../regression; ./regression.sh

../performance/test: ../performance/test.cpp
	$(CXX) $(CP_CXXFLAGS) -o $@ $<

performance: ../performance/test
	../performance/test
