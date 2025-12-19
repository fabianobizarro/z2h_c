TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))


run: build
	./$(TARGET) -n -f mydb.db
	./$(TARGET) -f mydb.db -a "Mayara Bizarro, William Sab 50, 1200"
	./$(TARGET) -f mydb.db -a "Fabiano Bizarro, William Sab 50, 9000"
	./$(TARGET) -f mydb.db -a "Beyonce Bizarro, William Sab 50, 50"
	./$(TARGET) -f mydb.db -d "Fabiano Bizarro" -l

delete: 
	./$(TARGET) -f mydb.db -d "Fabiano Bizarro"

build: clean default

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude
