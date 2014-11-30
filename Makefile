CC		=	gcc
CFLAGS	=	-g -Wall
OBJ		=	Lab9.o
SRC		=	$(OBJ:.o=.c)
EXE		=	Lab9

all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXE) $(OBJ)

run:
	./$(EXE) f1.txt f2.txt
