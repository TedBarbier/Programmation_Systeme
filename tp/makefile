# Nom de l'exécutable final
TARGET = program

# Répertoires
SRC_DIR = src
BUILD_DIR = build

# Chercher tous les fichiers .c
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Options du compilateur
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

# Règle par défaut pour compiler l'exécutable
all: $(TARGET)

# Lier les fichiers objets pour créer l'exécutable
$(TARGET): $(OBJS)
	@echo "Linking $@..."
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Règle pour compiler chaque fichier .c en .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyer les fichiers générés
clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) $(TARGET)

# Pour regénérer tout de zéro
rebuild: clean all

# Dépendances
.PHONY: all clean rebuild
