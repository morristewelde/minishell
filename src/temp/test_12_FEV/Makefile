# Makefile pour compiler et exécuter le projet
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

# Nom de l'exécutable
TARGET = mypipe

# Fichier source
SRC = test_12.c

# Compilation
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(LDFLAGS)

# Nettoyage
clean:
	rm -f $(TARGET)

# Exécution
run: $(TARGET)
	./$(TARGET)

