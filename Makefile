PROGRAM = popcorn.exe

COMPILE = clang
C_FLAGS = -O2 -Wall -Wextra -Wpedantic -std=c17 -Wno-deprecated-declarations
L_FLAGS =

OBJ_DIR	= obj
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(COMPILE) $(C_FLAGS) $^ -o $@ $(L_FLAGS)

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	$(COMPILE) $(C_FLAGS) -c $< -o $@ $(L_FLAGS)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -rf obj $(PROGRAM)
