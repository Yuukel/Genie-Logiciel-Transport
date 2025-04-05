SRC = ./src/
BIN = ./bin/
INC = ./include/
OBJ = ./obj/

CC = g++

EXE = main.exe

$(EXE): main.o trajet.o arret.o
	$(CC) $(OBJ)* -o $@

main.o: trajet.o arret.o
	$(CC) $(SRC)main.cpp -I $(INC) -c -o $(OBJ)$@

trajet.o:
	$(CC) $(SRC)trajet.cpp -I $(INC) -c -o $(OBJ)$@

arret.o:
	$(CC) $(SRC)arret.cpp -I $(INC) -c -o $(OBJ)$@