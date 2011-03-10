TARGET = output.elf
OBJS = main.o  
 
INCDIR = 
CFLAGS = -G4 -Wall -O2 
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
 
LIBDIR =
LDFLAGS =
INCDIR = c:/pspsdk/psp/include/oslib
STDLIBS = -losl -lpng -lz \
  -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspaudiolib -lpspaudio -lm -ljpeg -lpsphprm
LIBS=$(STDLIBS)$(YOURLIBS)
 
 
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Rainbow Blaster
PSP_EBOOT_ICON = rainbowblaster_menu_icon.png
 
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak