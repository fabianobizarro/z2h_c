TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	./$(TARGET) -n -f mydb.db
	./$(TARGET) -f mydb.db
	./$(TARGET) -f mydb.db -a "Fabiano Bizarro, William Sab 50, 900"
	./$(TARGET) -f mydb.db -a "Mayara Bizarro, William Sab 50, 1200"


default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc  -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude
