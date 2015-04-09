#The Target Binary Program
TARGET        := PolyBackgammon

#Compiler
CC            := gcc

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR        := src
INCDIR        := include
BUILDDIR      := obj
TARGETDIR     := bin
DOCDIR        := doc

#Flags, Libraries and Includes
CFLAGS        += $(shell sdl-config --cflags)
CFLAGS_DEBUG  +=
CFLAGS_PROD   +=
CPPFLAGS      +=
LDFLAGS       +=
LDLIBS        += -ldl $(shell sdl-config --libs) -lSDL_ttf


#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
DEBUG         ?= 1

ifeq ($(DEBUG), 1)
	CFLAGS += $(CFLAGS_DEBUG)
	BUILDDIR  := $(BUILDDIR)/debug
	TARGET    := $(TARGET)-dev
else
	CFLAGS += $(CFLAGS_PROD)
	BUILDDIR  := $(BUILDDIR)/prod
endif

SOURCES       := $(shell find $(SRCDIR) -type f -name *.c)
OBJECTS       := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))

all: $(TARGETDIR)/$(TARGET)

-include $(OBJECTS:.o=.d)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	@echo "  CC     $@"
	@$(COMPILE.c) $(OUTPUT_OPTION) $<

$(TARGETDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	@echo "Linking  $@"
	@$(LINK.o) $(OUTPUT_OPTION) $^ $(LDLIBS)

clean:
	$(RM) -r obj $(TARGETDIR) $(DOCDIR)

help:
	@echo "Commandes disponibles :"
	@echo "    (all)      tout compiler."
	@echo "    clean      supprimer tous les fichiers générés."
	@echo "    doc        génération de la doc doxygen."
	@echo "    check      lancer les tests."

check: all

.PHONY: all clean check doc help
