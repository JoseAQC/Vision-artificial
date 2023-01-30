EXECUTABLE = matricula
SOURCES = main.cpp plate.cpp
CC = g++
ODIR=./obj/
SRCDIR=./src/
INCLUDE = -I./include `pkg-config --cflags opencv`
CFLAGS=-c -fPIC -Wall $(INCLUDE)
LIBS = -L/usr/local/lib -lm `pkg-config --libs opencv`

OBJ = $(SOURCES:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)%,$(OBJ))

${EXECUTABLE}: ${OBJS}
	${CC} -o ${EXECUTABLE} ${OBJS} ${LIBS} ${COMMON_LIBS} 

$(ODIR)%.o : $(SRCDIR)%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ${OBJS} $(EXECUTABLE) #dat/*.png
