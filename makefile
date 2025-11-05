# Compilador
CC = gcc

# Directorios
LIB_DIR = D:/C_Libs
INCLUDE_DIR = $(LIB_DIR)/include
LIBRARY_DIR = $(LIB_DIR)/lib
SRC_DIR = $(LIB_DIR)/src

# Flags de compilación
CFLAGS = -I$(INCLUDE_DIR)

# Flags de enlazado
LDFLAGS = -L$(LIBRARY_DIR) -lglfw3 -lopengl32 -lgdi32

# Archivos fuente
SOURCES = main.c $(SRC_DIR)/glad.c stb_image_impl.c

# Nombre del ejecutable
TARGET = main.exe

# Regla principal
$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Limpiar
clean:
	rm -f $(TARGET)

# Compilar y ejecutar
all: $(TARGET) run

# Debug con más información
debug: CFLAGS += -g -Wall -Wextra
debug: $(TARGET)

.PHONY: run clean all debug
