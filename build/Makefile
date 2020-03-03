# Set project directory one level above the Makefile directory. $(CURDIR) is a GNU make variable containing the path to the current working directory
PROJDIR := $(realpath $(CURDIR)/..)
SOURCEDIR := $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build
BINDIR := $(PROJDIR)/bin
INCLUDEDIR := $(PROJDIR)/include
LIBDIR := $(PROJDIR)/lib

# Name of the final executable
TARGET = LeJeanJackJeuDeCarte.exe

# Decide whether the commands will be shown or not
VERBOSE = TRUE

# Generate the GCC includes parameters by adding -I before each source folder
INCLUDES = $(addprefix -I, $(INCLUDEDIR))

# Add this list to VPATH, the place make will look for the source files
VPATH = $(SOURCEDIR)

# Create a list of *.c sources in DIRS
SOURCES = $(foreach dir,$(SOURCEDIR),$(wildcard $(dir)/*.c))

# Define objects for all sources
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.c=.o))

# Define dependencies files for all objects
DEPS = $(OBJS:.o=.d)

# Name the compiler
CC = gcc

# OS specific part
ifeq ($(OS),Windows_NT)
    RM = del /F /Q 
    RMDIR = -RMDIR /S /Q
    MKDIR = -mkdir
    ERRIGNORE = 2>NUL || true
    SEP=\\
else
    RM = rm -rf 
    RMDIR = rm -rf 
    MKDIR = mkdir -p
    ERRIGNORE = 2>/dev/null
    SEP=/
endif

# Remove space after separator
PSEP = $(strip $(SEP))

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
    HIDE =  
else
    HIDE = @
endif

# Define the function that will generate each rule
define generateRules
$(1)/%.o: %.c
    @echo Building $$@
    $(HIDE)$(CC) -c $$(INCLUDES) -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<) -MMD
endef

# Indicate to make which targets are not files
.PHONY: all clean directories 

all: $(TARGET)

$(TARGET):$(OBJS)
	$(HIDE)echo Linking
	$(HIDE)$(CC) $(OBJS) -o $(TARGET)

# Include dependencies
-include $(DEPS)

# Generate rules
$(eval $(call generateRules, $(SOURCEDIR)))

# Remove all objects, dependencies and executable files generated during the build
clean:
    $(HIDE)$(RMDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)
    $(HIDE)$(RM) $(TARGET) $(ERRIGNORE)
    @echo Cleaning done ! 