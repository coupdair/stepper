PROGRAMS = version stepper
DOCUMENTATIONS = doc

version = v0.1.9
#version_cimg = 125
version_cimg = 149
version_rs232=`cat ../rs232/VERSION`
OPT_XWINDOWS = -I/usr/X11R6/include -Dcimg_use_xshm -L/usr/X11R6/lib -lpthread -lX11 -lXext
#OPT = -Dcimg_display=0 -Dcimg_debug=2 -Dcimg_use_vt100 -DSTEPPER_VERSION=\"$(version)\"  -DRS232_VERSION=\"$(version_rs232)\"
OPT = $(OPT_XWINDOWS)  -Dcimg_debug=2 -Dcimg_use_vt100 -DSTEPPER_VERSION=\"$(version)\"  -DRS232_VERSION=\"$(version_rs232)\"
CC = gcc
CPP = g++

all: $(PROGRAMS) $(DOCUMENTATIONS)

prog:$(PROGRAMS)

version: Makefile
	echo ${version} > VERSION
	cd ..; rm CImg; ln -s CImg-1.4.9 CImg; cd ./stepper/
#	cd ..; rm CImg; ln -s CImg-1.2.5 CImg; cd ./stepper/

stepper: stepper.cpp stepper_factory.h stepper.h Makefile ../rs232/serial_factory.h ../rs232/serial.h
	$(CPP) $(OPT) $@.cpp -o $@

doc: stepper.Doxygen Makefile stepper.h stepper_factory.h stepper.cpp
	./doxIt.sh

clean:
	rm -rf $(DOCUMENTATIONS)/*
	rm -f *.o
	rm -rf .libs
	@list='$(PROGRAMS)'; for p in $$list; do \
	  rm -f $$p ; \
	done


