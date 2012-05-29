PROGRAMS = stepper  
DOCUMENTATIONS = doc

version = v0.1.2
OPT = -Dcimg_display=0 -Dcimg_debug=2 -Dcimg_use_vt100 -DVERSION=\"$(version)\"
CC = gcc
CPP = g++

all: $(PROGRAMS) $(DOCUMENTATIONS)

prog:$(PROGRAMS)

stepper: stepper.cpp stepper.h Makefile ../rs232/serial_factory.h ../rs232/serial.h
	$(CPP) $(OPT) stepper.cpp -o $@

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


