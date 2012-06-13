PROGRAMS = stepper  
DOCUMENTATIONS = doc

version = v0.1.8
version_rs232=`cat ../rs232/VERSION`
OPT = -Dcimg_display=0 -Dcimg_debug=2 -Dcimg_use_vt100 -DSTEPPER_VERSION=\"$(version)\"  -DRS232_VERSION=\"$(version_rs232)\"
OPT =                  -Dcimg_debug=2 -Dcimg_use_vt100 -DSTEPPER_VERSION=\"$(version)\"  -DRS232_VERSION=\"$(version_rs232)\" -I/usr/X11R6/include -Dcimg_use_xshm -L/usr/X11R6/lib -lpthread -lX11 -lXext
CC = gcc
CPP = g++

all: $(PROGRAMS) $(DOCUMENTATIONS)

prog:$(PROGRAMS)

stepper: stepper.cpp stepper.h Makefile ../rs232/serial_factory.h ../rs232/serial.h
	$(CPP) $(OPT) $@.cpp -o $@

doc: stepper.Doxygen Makefile stepper.h stepper.cpp
	echo ${version} > VERSION
	./doxIt.sh

clean:
	rm -rf $(DOCUMENTATIONS)/*
	rm -f *.o
	rm -rf .libs
	@list='$(PROGRAMS)'; for p in $$list; do \
	  rm -f $$p ; \
	done


