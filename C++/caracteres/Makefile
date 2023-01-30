EXECUTABLE = caracteres
SOURCES = main.cpp plate.cpp plateCharacters.cpp
CC = g++
ODIR=./obj/
SRCDIR=./src/
INCLUDE = -I./include `pkg-config --cflags opencv`
CFLAGS=-c -Wall $(INCLUDE)
LIBS = -L/usr/local/lib -lm `pkg-config --libs opencv`

OBJ = $(SOURCES:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)%,$(OBJ))

${EXECUTABLE}: ${OBJS}
	${CC} -o ${EXECUTABLE} ${OBJS} ${LIBS} ${COMMON_LIBS} 

$(ODIR)%.o : $(SRCDIR)%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ${OBJS} $(EXECUTABLE) dat/result*.png