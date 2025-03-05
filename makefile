#                        makefile for glclock
#
#		make clean
#		make all
#
#       June 2000 by Masa (Masaki Kawase) <masa@daionet.gr.jp>
#


         TARGET = glclock
    TARGET_MARK = glclockmark


       COMPILER = g++ -c
         LINKER = g++
            OPT = -O2

   INCLUDE_PATH = -I/usr/X11R6/include
       LIB_PATH = -L/usr/X11R6/lib

           LIBS = -lglut -lGLU -lGL -lXi -lXmu -lXext -lX11 -lm
      MARK_LIBS = -lm

#
#                 for SGI IRIS
#
#       COMPILER = CC -c
#         LINKER = CC
#            OPT = -O2
#           LIBS = -lglut -lGLU -lGL -lXmu -lXext -lX11 -lfastm -lm
#      MARK_LIBS = -lfastm -lm
#
#                 for SGI R1000 n32 mips4
#
#            OPT = -O2 -n32 -mips4 -r10000
#           LIBS = -lglut -lGLU -lGL -lXmu -lXext -lX11 -lfastm -lm
#      MARK_LIBS = -lfastm -lm
#
#                 for Mesa
#           LIBS = -lglut -lMesatk -lMesaGLU -lMesaGL -lXi -lXext -lXmu -lX11 -lm
#           LIBS = -lglut -lGLU -lGL -lXi -lXext -lXmu -lX11 -lm
#      MARK_LIBS = -lm
#


  COMPILE = $(COMPILER) $(OPT) $(INCLUDE_PATH)
     LINK = $(LINKER) $(OPT) $(LIB_PATH)


$(TARGET) : main.o glclock.o GLObject.o GLextensions.o PnmIO.o MString.o
	$(LINK) -o $(TARGET) main.o glclock.o GLObject.o GLextensions.o PnmIO.o MString.o $(LIBS)


$(TARGET_MARK) : glclockmark.o MString.o
	$(LINK) -o $(TARGET_MARK) glclockmark.o MString.o $(MARK_LIBS)

glclockmark.o : glclockmark.cpp glclockmark.H
	$(COMPILE) glclockmark.cpp


main.o : main.cpp glclock.H
	$(COMPILE) main.cpp


glclock.o : glclock.cpp glclock.H GLObject.H Option.H
	$(COMPILE) glclock.cpp


GLObject.o : GLObject.cpp GLObject.H GLextensions.H PnmIO.H
	$(COMPILE) GLObject.cpp

GLextensions.o : GLextensions.cpp GLextensions.H MString.H Math3D.H
	$(COMPILE) GLextensions.cpp

PnmIO.o : PnmIO.cpp PnmIO.H MString.H
	$(COMPILE) PnmIO.cpp

MString.o : MString.cpp MString.H Define.H
	$(COMPILE) MString.cpp


all : $(TARGET) $(TARGET_MARK)


clean :
	rm -f *.o $(TARGET) $(TARGET_MARK) *~ #*#
