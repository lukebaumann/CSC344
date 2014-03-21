CC = g++
CFLAGS = -g -Wall -Werror
OS = $(shell uname -s)
PROC = $(shell uname -p)
EXEC_SUFFIX=$(OS)-$(PROC)

ifeq ("$(OS)", "SunOS")
   OSLIB=-lsfml-graphics -lsfml-window -lsfml-system -lsndfile 
   OSINC=
   OSDEF=-DSOLARIS
else
ifeq ("$(OS)", "Darwin")
   OSLIB=-lsfml-graphics -lsfml-window -lsfml-system -lsndfile 
   OSINC=
   OSDEF=-DDARWIN
else
   OSLIB=-lsfml-graphics -lsfml-window -lsfml-system -lsndfile 
   OSINC=
   OSDEF=-DLINUX
endif
endif

all:  fft-$(EXEC_SUFFIX)

fft-$(EXEC_SUFFIX): HistogramDrawing.cpp FourierTransform.cpp FinalProject.hpp
	$(CC) $(CFLAGS) $(OSINC) $(OSLIB) $(OSDEF) -o $@ HistogramDrawing.cpp FourierTransform.cpp

clean:
	rm -f fft-*
