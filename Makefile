# Paths
OPENCV_PATH=/usr/local

# Programs
CC=
CXX=clang++

# Flags
ARCH_FLAGS=
CFLAGS=-Wextra -Wall -pedantic-errors $(ARCH_FLAGS) -O3 -Wno-long-long
LDFLAGS=$(ARCH_FLAGS)
DEFINES=
INCLUDES=-I$(OPENCV_PATH)/include -Iinclude/
LIBRARIES=-L$(OPENCV_PATH)/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect

# Files which require compiling
SOURCE_FILES=\
	src/lib/IO.mm\
	src/lib/PDM.mm\
	src/lib/Patch.mm\
	src/lib/CLM.mm\
	src/lib/FDet.mm\
	src/lib/PAW.mm\
	src/lib/FCheck.mm\
	src/lib/Tracker.mm

# Source files which contain a int main(..) function
SOURCE_FILES_WITH_MAIN=src/exe/face_tracker.mm

# End Configuration
SOURCE_OBJECTS=$(patsubst %.mm,%.o,$(SOURCE_FILES))

ALL_OBJECTS=\
	$(SOURCE_OBJECTS) \
	$(patsubst %.mm,%.o,$(SOURCE_FILES_WITH_MAIN))

DEPENDENCY_FILES=\
	$(patsubst %.o,%.d,$(ALL_OBJECTS)) 

all: bin/face_tracker

%.o: %.mm Makefile
	@# Make dependecy file
	$(CXX) -MM -MT $@ -MF $(patsubst %.mm,%.d,$<) $(CFLAGS) $(DEFINES) $(INCLUDES) $<
	@# Compile
	$(CXX) $(CFLAGS) $(DEFINES) -c -o $@ $< $(INCLUDES)

-include $(DEPENDENCY_FILES)

bin/face_tracker: $(ALL_OBJECTS)
	$(CXX) $(LDFLAGS)  -o $@ $(ALL_OBJECTS) $(LIBRARIES)

.PHONY: clean
clean:
	@echo "Cleaning"
	@for pattern in '*~' '*.o' '*.d' ; do \
		find . -name "$$pattern" | xargs rm ; \
	done
