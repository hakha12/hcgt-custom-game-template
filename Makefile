#**************************************************************************************************
#
#   A customized raylib makefile for Desktop platforms and Web (Wasm)
#
#   Copyright (c) 2013-2025 Ramon Santamaria (@raysan5)
#
#   This software is provided "as-is", without any express or implied warranty. In no event
#   will the authors be held liable for any damages arising from the use of this software.
#
#   Permission is granted to anyone to use this software for any purpose, including commercial
#   applications, and to alter it and redistribute it freely, subject to the following restrictions:
#
#     1. The origin of this software must not be misrepresented; you must not claim that you
#     wrote the original software. If you use this software in a product, an acknowledgment
#     in the product documentation would be appreciated but is not required.
#
#     2. Altered source versions must be plainly marked as such, and must not be misrepresented
#     as being the original software.
#
#     3. This notice may not be removed or altered from any source distribution.
#
#**************************************************************************************************

.PHONY: all clean

# Define required environment variables
#------------------------------------------------------------------------------------------------
# Define target platform: PLATFORM_DESKTOP, PLATFORM_WEB
PLATFORM              ?= PLATFORM_DESKTOP

# Define project variables
PROJECT_NAME          ?= hcgt-custom-game-template
PROJECT_VERSION       ?= 0.0.5
PROJECT_BUILD_PATH    ?= .
PROJECT_SOURCE_PATH	  ?= src
PROJECT_SCENE_PATH	  ?= scenes
PROJECT_SOURCE_FILES  ?= \
    $(PROJECT_SOURCE_PATH)/main.cpp \
	$(PROJECT_SOURCE_PATH)/engine.cpp \
	$(PROJECT_SOURCE_PATH)/directory.cpp \
	$(PROJECT_SOURCE_PATH)/window.cpp \
	$(PROJECT_SOURCE_PATH)/event.cpp \
	$(PROJECT_SOURCE_PATH)/state.cpp \
	$(PROJECT_SOURCE_PATH)/texture.cpp

PROJECT_SCENE_FILES  ?= \
	$(PROJECT_SCENE_PATH)/scene_template.cpp

PROJECT_OBJECT_PATH	  ?= temp
PROJECT_OBJECT_FILE    = $(patsubst $(PROJECT_SOURCE_PATH)/%.cpp, $(PROJECT_OBJECT_PATH)/%.o, $(PROJECT_SOURCE_FILES))\
						 $(patsubst $(PROJECT_SCENE_PATH)/%.cpp, $(PROJECT_OBJECT_PATH)/%.o, $(PROJECT_SCENE_FILES))

# raylib library variables
RAYLIB_SRC_PATH       ?= ../../raylib/src
RAYLIB_INCLUDE_PATH   ?= $(RAYLIB_SRC_PATH)
RAYLIB_LIB_PATH       ?= $(RAYLIB_SRC_PATH)

# Library type used for raylib: STATIC (.a) or SHARED (.so/.dll)
RAYLIB_LIBTYPE        ?= STATIC

# Define compiler path on Windows
COMPILER_PATH         ?= C:\raylib\w64devkit\bin

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= RELEASE

# PLATFORM_WEB: Default properties
BUILD_WEB_ASYNCIFY    ?= FALSE
BUILD_WEB_SHELL       ?= $(RAYLIB_SRC_PATH)/minshell.html
BUILD_WEB_HEAP_SIZE   ?= 128MB
BUILD_WEB_STACK_SIZE  ?= 1MB
BUILD_WEB_ASYNCIFY_STACK_SIZE ?= 1048576
BUILD_WEB_RESOURCES   ?= FALSE
BUILD_WEB_RESOURCES_PATH  ?= resources

# Determine PLATFORM_OS in case PLATFORM_DESKTOP selected
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    # No uname.exe on MinGW!, but OS=Windows_NT on Windows!
    # ifeq ($(UNAME),Msys) -> Windows
    ifeq ($(OS),Windows_NT)
        PLATFORM_OS = WINDOWS
        export PATH := $(COMPILER_PATH):$(PATH)
        ifndef PLATFORM_SHELL
            PLATFORM_SHELL = cmd
        endif
    else
        UNAMEOS = $(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS = LINUX
        endif
        ifndef PLATFORM_SHELL
            PLATFORM_SHELL = sh
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    ifeq ($(OS),Windows_NT)
        PLATFORM_OS = WINDOWS
        ifndef PLATFORM_SHELL
            PLATFORM_SHELL = cmd
        endif
    else
        UNAMEOS = $(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS = LINUX
        endif
        ifndef PLATFORM_SHELL
            PLATFORM_SHELL = sh
        endif
    endif
endif

ifeq ($(PLATFORM_OS),WINDOWS)
    ifeq ($(PLATFORM),PLATFORM_WEB)
        # Emscripten required variables
        EMSDK_PATH         ?= C:/raylib/emsdk
        EMSCRIPTEN_PATH    ?= $(EMSDK_PATH)/upstream/emscripten
        CLANG_PATH          = $(EMSDK_PATH)/upstream/bin
        PYTHON_PATH         = $(EMSDK_PATH)/python/3.9.2-nuget_64bit
        NODE_PATH           = $(EMSDK_PATH)/node/20.18.0_64bit/bin
        export PATH         = $(EMSDK_PATH);$(EMSCRIPTEN_PATH);$(CLANG_PATH);$(NODE_PATH);$(PYTHON_PATH):$$(PATH)
    endif
endif

# Define default C++ compiler: CC
#------------------------------------------------------------------------------------------------
CC = g++

ifeq ($(PLATFORM),PLATFORM_WEB)
    # HTML5 emscripten compiler
    # WARNING: To compile to HTML5, code must be redesigned
    # to use emscripten.h and emscripten_set_main_loop()
    CC = em++
endif

# Define default make program: MAKE
#------------------------------------------------------------------------------------------------
MAKE ?= make

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        MAKE = mingw32-make
    endif
endif

# Define compiler flags: CFLAGS
#------------------------------------------------------------------------------------------------
#  -O1                  defines optimization level
#  -g                   include debug information on compilation
#  -s                   strip unnecessary data from build
#  -Wall                turns on most, but not all, compiler warnings
#  -std=c99             defines C language mode (standard C from 1999 revision)
#  -std=gnu99           defines C language mode (GNU C from 1999 revision)
#  -Wno-missing-braces  ignore invalid warning (GCC bug 53119)
#  -Wno-unused-value    ignore unused return values of some functions (i.e. fread())
#  -D_DEFAULT_SOURCE    use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
CFLAGS = -std=c++11 -Wall -Wno-missing-braces -Wno-unused-value -D_DEFAULT_SOURCE $(PROJECT_CUSTOM_FLAGS)
#CFLAGS += -Wextra -Wmissing-prototypes -Wstrict-prototypes

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -D_DEBUG
else
    ifeq ($(PLATFORM),PLATFORM_WEB)
        ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
            CFLAGS += -O3
        else
            CFLAGS += -Os
        endif
    else
        CFLAGS += -s -O2
    endif
endif

# Define include paths for required headers: INCLUDE_PATHS
#------------------------------------------------------------------------------------------------
INCLUDE_PATHS += -I. -Iexternal -I$(RAYLIB_INCLUDE_PATH)

# Define additional directories containing required header files

# Define library paths containing required libs: LDFLAGS
#------------------------------------------------------------------------------------------------
LDFLAGS = -L. -L$(RAYLIB_LIB_PATH)

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        # NOTE: The resource .rc file contains windows executable icon and properties
        LDFLAGS += $(RAYLIB_SRC_PATH)/raylib.rc.data
        # -Wl,--subsystem,windows hides the console window
        ifeq ($(BUILD_MODE), RELEASE)
            LDFLAGS += -Wl,--subsystem,windows
        endif
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        # Reset everything.
        # Precedence: immediately local, installed version, raysan5 provided libs
        #LDFLAGS += -L$(RAYLIB_RELEASE_PATH)
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    # -Os                        # size optimization
    # -O2                        # optimization level 2, if used, also set --memory-init-file 0
    # -sUSE_GLFW=3               # Use glfw3 library (context/input management)
    # -sALLOW_MEMORY_GROWTH=1    # to allow memory resizing -> WARNING: Audio buffers could FAIL!
    # -sTOTAL_MEMORY=16777216    # to specify heap memory size (default = 16MB) (67108864 = 64MB)
    # -sUSE_PTHREADS=1           # multithreading support
    # -sWASM=0                   # disable Web Assembly, emitted by default
    # -sASYNCIFY                 # lets synchronous C/C++ code interact with asynchronous JS
    # -sFORCE_FILESYSTEM=1       # force filesystem to load/save files data
    # -sASSERTIONS=1             # enable runtime checks for common memory allocation errors (-O1 and above turn it off)
    # -sMINIFY_HTML=0            # minify generated html from shell.html
    # --profiling                # include information for code profiling
    # --memory-init-file 0       # to avoid an external memory initialization code file (.mem)
    # --preload-file resources   # specify a resources folder for data compilation
    # --source-map-base          # allow debugging in browser with source map
    # --shell-file shell.html    # define a custom shell .html and output extension
    LDFLAGS += -sUSE_GLFW=3 -sTOTAL_MEMORY=$(BUILD_WEB_HEAP_SIZE) -sSTACK_SIZE=$(BUILD_WEB_STACK_SIZE) -sFORCE_FILESYSTEM=1 -sMINIFY_HTML=0
    
    # Build using asyncify
    ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
        LDFLAGS += -sASYNCIFY -sASYNCIFY_STACK_SIZE=$(BUILD_WEB_ASYNCIFY_STACK_SIZE)
    endif

    # Add resources building if required
    ifeq ($(BUILD_WEB_RESOURCES),TRUE)
        LDFLAGS += --preload-file $(BUILD_WEB_RESOURCES_PATH)
    endif

    # Add debug mode flags if required
    ifeq ($(BUILD_MODE),DEBUG)
        LDFLAGS += -sASSERTIONS=1 --profiling
    endif

    # Define a custom shell .html and output extension
    LDFLAGS += --shell-file $(BUILD_WEB_SHELL)
    EXT = .html
endif

# Define libraries required on linking: LDLIBS
# NOTE: To link libraries (lib<name>.so or lib<name>.a), use -l<name>
#------------------------------------------------------------------------------------------------
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        # Libraries for Windows desktop compilation
        # NOTE: WinMM library required to set high-res timer resolution
        LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -lcomdlg32 -lole32
        # Required for physac examples
        LDLIBS += -static -lpthread
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        # Libraries for Debian GNU/Linux desktop compiling
        # NOTE: Required packages: libegl1-mesa-dev
        LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt

        # On Wayland windowing system, additional libraries requires
        ifeq ($(USE_WAYLAND_DISPLAY),TRUE)
            LDLIBS += -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
        else
            # On X11 requires also below libraries
            LDLIBS += -lX11
            # NOTE: It seems additional libraries are not required any more, latest GLFW just dlopen them
            #LDLIBS += -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor
        endif
        # Explicit link to libc
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDLIBS += -lc
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    # Libraries for web (HTML5) compiling
    LDLIBS = $(RAYLIB_LIB_PATH)/libraylib.web.a
endif

# Define processes to execute
MAKEFILE_TARGET = $(PROJECT_NAME)


# Default target entry
# NOTE: We call this Makefile target or Makefile.Android target
all:
	$(MAKE) $(MAKEFILE_TARGET)

# Project target defined by PROJECT_NAME
$(PROJECT_NAME): $(PROJECT_OBJECT_FILE)
	$(CC) -o $(PROJECT_BUILD_PATH)/$(PROJECT_NAME)$(EXT) $(PROJECT_OBJECT_FILE) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM) -D$(PLATFORM_OS)

# Compile source files
# NOTE: This pattern will compile every module defined on $(PROJECT_OBJECT_FILE)
$(PROJECT_OBJECT_PATH)/%.o: $(PROJECT_SOURCE_PATH)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -D$(PLATFORM_OS)

$(PROJECT_OBJECT_PATH)/%.o: $(PROJECT_SCENE_PATH)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -D$(PLATFORM_OS)


.PHONY: clean_shell_cmd clean_shell_sh

# Clean everything
clean:	clean_shell_$(PLATFORM_SHELL)
	@echo Cleaning done

clean_shell_sh:
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
		find . -type f -executable -delete
		rm -fv $(PROJECT_OBJECT_PATH)/*.o
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    ifeq ($(PLATFORM_OS),LINUX)
		rm -fv $(PROJECT_OBJECT_PATH)/*.o $(PROJECT_NAME).data $(PROJECT_NAME).html $(PROJECT_NAME).js $(PROJECT_NAME).wasm
    endif
endif

clean_shell_cmd: SHELL=cmd
clean_shell_cmd:
	del $(PROJECT_OBJECT_PATH)\*.o *.exe $(PROJECT_NAME).data $(PROJECT_NAME).html $(PROJECT_NAME).js $(PROJECT_NAME).wasm /s