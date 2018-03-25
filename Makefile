debug=0
ifeq ($(OS),Windows_NT)
	platform=windows
else
	UNAME:=$(shell uname -s)
	ifeq ($(UNAME),Linux)
		platform=linux
	endif
	ifeq ($(UNAME),Darwin)
		platform=macos
	endif
endif
CFLAGS=-c -Wall -O2
LDFLAGS=-O2
ifeq ($(platform),windows)
LDFLAGS+=-u WinMain -ldinput8.lib -ldxguid.lib
else ifeq ($(platform),linux)
else ifeq ($(platform),macos)
LDFLAGS+=-framework Cocoa -framework Foundation -framework IOKit
endif

SOURCES=src/main.c \
	src/input.c \
	src/thread.c
ifeq ($(platform),windows)
SOURCES+=src/application_windows.c \
	src/input_dinput.c \
	src/window_windows.c
else ifeq ($(platform),linux)
else ifeq ($(platform),macos)
SOURCES+=src/application_macos.c \
	src/input_iokit.c \
	src/window_macos.c
endif
BASE_NAMES=$(basename $(SOURCES))
OBJECTS=$(BASE_NAMES:=.o)
DEPENDENCIES=$(OBJECTS:.o=.d)
EXECUTABLE=gamepads

.PHONY: all
ifeq ($(debug),1)
all: CFLAGS+=-DDEBUG -g
endif
all: bundle

.PHONY: bundle
bundle: $(EXECUTABLE)
ifeq ($(platform),macos)
bundle:
	mkdir -p $(EXECUTABLE).app
	mkdir -p $(EXECUTABLE).app/Contents
	cp -f macos/Info.plist $(EXECUTABLE).app/Contents
	mkdir -p $(EXECUTABLE).app/Contents/MacOS
	cp -f $(EXECUTABLE) $(EXECUTABLE).app/Contents/MacOS
	sed -e s/'$$(DEVELOPMENT_LANGUAGE)'/en/ \
		-e s/'$$(EXECUTABLE_NAME)'/gamepads/ \
		-e s/'$$(PRODUCT_BUNDLE_IDENTIFIER)'/lv.elviss.gamepads/ \
		-e s/'$$(PRODUCT_NAME)'/gamepads/ \
		-e s/'$$(MACOSX_DEPLOYMENT_TARGET)'/10.13/ \
		macos/Info.plist > gamepads.app/Contents/Info.plist
endif

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP $< -o $@
