# Makefile pour la Bibliothèque Graphique en C

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
LDFLAGS = -lSDL2 -lSDL2_ttf -lm

# Répertoires
SRC_DIR = src
INC_DIR = include
EXAMPLES_DIR = examples
OBJ_DIR = obj
LIB_DIR = lib

# Fichiers
LIB_SRC = $(SRC_DIR)/graphics.c
LIB_OBJ = $(OBJ_DIR)/graphics.o
LIB_STATIC = $(LIB_DIR)/libgraphics.a
LIB_SHARED = $(LIB_DIR)/libgraphics.so

# Exemples
EXAMPLE_SRCS = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_BINS = $(EXAMPLE_SRCS:$(EXAMPLES_DIR)/%.c=$(EXAMPLES_DIR)/%)

# Règle par défaut
all: directories $(LIB_STATIC) $(LIB_SHARED) examples

# Créer les répertoires nécessaires
directories:
	@mkdir -p $(OBJ_DIR) $(LIB_DIR)

# Compilation de l'objet de la bibliothèque
$(OBJ_DIR)/graphics.o: $(LIB_SRC) $(INC_DIR)/graphics.h
	$(CC) $(CFLAGS) -fPIC -c $(LIB_SRC) -o $(OBJ_DIR)/graphics.o

# Création de la bibliothèque statique
$(LIB_STATIC): $(LIB_OBJ)
	ar rcs $(LIB_STATIC) $(LIB_OBJ)
	@echo "Bibliothèque statique créée: $(LIB_STATIC)"

# Création de la bibliothèque partagée
$(LIB_SHARED): $(LIB_OBJ)
	$(CC) -shared -o $(LIB_SHARED) $(LIB_OBJ) $(LDFLAGS)
	@echo "Bibliothèque partagée créée: $(LIB_SHARED)"

# Compilation des exemples
examples: $(EXAMPLE_BINS)

$(EXAMPLES_DIR)/%: $(EXAMPLES_DIR)/%.c $(LIB_STATIC)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -lgraphics $(LDFLAGS) -o $@
	@echo "Exemple compilé: $@"

# Installation de la bibliothèque (nécessite les droits root)
install: all
	@echo "Installation de la bibliothèque..."
	install -d /usr/local/lib
	install -m 644 $(LIB_STATIC) /usr/local/lib/
	install -m 755 $(LIB_SHARED) /usr/local/lib/
	install -d /usr/local/include
	install -m 644 $(INC_DIR)/graphics.h /usr/local/include/
	ldconfig
	@echo "Installation terminée"

# Désinstallation
uninstall:
	@echo "Désinstallation de la bibliothèque..."
	rm -f /usr/local/lib/libgraphics.a
	rm -f /usr/local/lib/libgraphics.so
	rm -f /usr/local/include/graphics.h
	ldconfig
	@echo "Désinstallation terminée"

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -f $(EXAMPLE_BINS)
	@echo "Nettoyage terminé"

# Aide
help:
	@echo "Makefile pour la Bibliothèque Graphique en C"
	@echo ""
	@echo "Utilisation:"
	@echo "  make              - Compile la bibliothèque et les exemples"
	@echo "  make examples     - Compile uniquement les exemples"
	@echo "  make install      - Installe la bibliothèque sur le système"
	@echo "  make uninstall    - Désinstalle la bibliothèque"
	@echo "  make clean        - Nettoie les fichiers compilés"
	@echo "  make help         - Affiche cette aide"

.PHONY: all directories examples install uninstall clean help
