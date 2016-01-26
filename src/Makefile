GCC = nspire-g++
LD = nspire-ld
GENZEHN = genzehn

GCCFLAGS = -Os -Wall -W -marm -Wno-unused-parameter -I include
LDFLAGS = -L lib -lndls -lGravnix -lArchive
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
EXE = nGravnix-sdl
DISTDIR = ..

OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all:	prepare $(EXE).prg.tns

#Because JGit does not do symbolic links and TeamCity uses JGit
prepare:
	cd ArchiveLib; make; cd ..
	cd GravnixLib; make; cd ..
	mkdir -p include
	mkdir -p include/ArchiveLib
	mkdir -p include/GravnixLib
	mkdir -p lib
	ln -s ../../ArchiveLib/include/ArchiveLib.h include/ArchiveLib/ArchiveLib.h
	ln -s ../ArchiveLib/libArchive.a lib/libArchive.a
	ln -s ../../GravnixLib/include/GravnixLib.h include/GravnixLib/GravnixLib.h
	ln -s ../GravnixLib/libGravnix.a lib/libGravnix.a

%.o: %.cpp
	$(GCC) $(GCCFLAGS) -c $<

$(EXE).elf: $(OBJS)
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $(DISTDIR)/$^ --output $(DISTDIR)/$@ $(ZEHNFLAGS)

$(EXE).prg.tns: $(EXE).tns
	make-prg $(DISTDIR)/$^ $(DISTDIR)/$@

clean:
	cd ArchiveLib; make clean; cd ..
	cd GravnixLib; make clean; cd ..
	rm -rf include
	rm -rf lib
	rm -f *.o *.elf
	rm -f $(DISTDIR)/$(EXE).tns
	rm -f $(DISTDIR)/$(EXE).elf
	rm -f $(DISTDIR)/$(EXE).prg.tns

