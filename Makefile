CXXFLAGS        = -g -Wall
OBJS            = main.o
PROG            = main
INCLUDES        = -I /usr/local/include -I /usr/include/GL
LIBS            = -L /usr/local/lib -lglfw -lGLEW -lGL 

all:            $(PROG)

${PROG}:        $(OBJS)
		$(CXX) $(INCLUDES) -o $(PROG) $(OBJS) $(LIBS)

clean:;         $(RM) -f $(PROG) core *.o
