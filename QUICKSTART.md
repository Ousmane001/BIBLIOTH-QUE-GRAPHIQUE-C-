# Quick Start - Bibliothèque Graphique en C

## Installation en 3 étapes

### 1. Installer les dépendances

```bash
# Ubuntu/Debian
sudo apt-get install build-essential libsdl2-dev libsdl2-ttf-dev
```

### 2. Compiler la bibliothèque

```bash
cd BIBLIOTH-QUE-GRAPHIQUE-C-
make
```

### 3. Tester avec les exemples

```bash
# Exemple simple avec formes géométriques
./examples/exemple_simple

# Dessin interactif avec la souris
./examples/exemple_dessin

# Animation d'une balle rebondissante
./examples/exemple_animation
```

## Créer votre premier programme

### 1. Créez un fichier `hello_graphics.c` :

```c
#include <graphics.h>

int main(void) {
    /* Initialiser */
    graphics_init();
    graphics_create_window("Hello Graphics!", 640, 480);
    
    /* Boucle principale */
    int running = 1;
    Event event;
    
    while (running) {
        /* Gérer les événements */
        while (graphics_poll_event(&event)) {
            if (event.type == EVENT_QUIT) {
                running = 0;
            }
        }
        
        /* Dessiner */
        graphics_set_color(COLOR_BLACK);
        graphics_clear();
        
        graphics_set_color(COLOR_CYAN);
        graphics_fill_circle(320, 240, 100);
        
        graphics_set_color(COLOR_WHITE);
        graphics_draw_text("Hello, Graphics!", 240, 50, 32);
        
        graphics_present();
        graphics_delay(16);
    }
    
    /* Nettoyer */
    graphics_quit();
    return 0;
}
```

### 2. Compilez :

```bash
gcc -o hello_graphics hello_graphics.c \
    -I./include -L./lib -lgraphics \
    -lSDL2 -lSDL2_ttf -lm
```

### 3. Exécutez :

```bash
LD_LIBRARY_PATH=./lib ./hello_graphics
```

## Fonctions essentielles

### Initialisation et fenêtre
- `graphics_init()` - Initialiser la bibliothèque
- `graphics_create_window(titre, largeur, hauteur)` - Créer une fenêtre
- `graphics_quit()` - Fermer et nettoyer

### Dessin
- `graphics_set_color(couleur)` - Définir la couleur
- `graphics_clear()` - Effacer l'écran
- `graphics_fill_circle(x, y, rayon)` - Cercle plein
- `graphics_fill_rectangle(x, y, largeur, hauteur)` - Rectangle plein
- `graphics_draw_line(x1, y1, x2, y2)` - Ligne
- `graphics_draw_text(texte, x, y, taille)` - Texte
- `graphics_present()` - Afficher

### Événements
- `graphics_poll_event(&event)` - Récupérer les événements
- `graphics_delay(ms)` - Attendre

### Couleurs prédéfinies
`COLOR_BLACK`, `COLOR_WHITE`, `COLOR_RED`, `COLOR_GREEN`, `COLOR_BLUE`, `COLOR_YELLOW`, `COLOR_CYAN`, `COLOR_MAGENTA`

## Conseils

1. **Toujours** appeler `graphics_present()` après avoir dessiné
2. **Toujours** appeler `graphics_quit()` avant de terminer
3. Utilisez `graphics_delay(16)` pour ~60 FPS
4. Gérez l'événement `EVENT_QUIT` pour permettre de fermer la fenêtre

## Aide

Pour plus de détails, consultez :
- [GUIDE.md](GUIDE.md) - Documentation complète
- [README.md](README.md) - Vue d'ensemble du projet
- Dossier `examples/` - Code source des exemples
