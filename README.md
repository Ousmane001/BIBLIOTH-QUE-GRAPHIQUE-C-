# Bibliothèque Graphique en C

Codage d'une bibliothèque Graphique en langage C pour automatiser la création de programmes informatiques en mode fenêtré.

## Description

Cette bibliothèque fournit une interface simple et intuitive pour créer des applications graphiques en C. Elle encapsule SDL2 pour offrir des fonctions de haut niveau permettant de :

- ✨ Créer et gérer des fenêtres facilement
- 🎨 Dessiner des formes géométriques (rectangles, cercles, lignes, pixels)
- ✍️ Afficher du texte avec différentes tailles
- 🎯 Gérer les événements clavier et souris
- 🎬 Créer des animations fluides
- 🌈 Utiliser des couleurs prédéfinies ou personnalisées

## Installation rapide

### Prérequis

```bash
# Ubuntu/Debian
sudo apt-get install build-essential libsdl2-dev libsdl2-ttf-dev

# Fedora/RedHat
sudo dnf install gcc make SDL2-devel SDL2_ttf-devel

# macOS (Homebrew)
brew install sdl2 sdl2_ttf
```

### Compilation

```bash
make
```

## Utilisation

### Exemple simple

```c
#include <graphics.h>

int main(void) {
    graphics_init();
    graphics_create_window("Ma Fenêtre", 800, 600);
    
    int running = 1;
    Event event;
    
    while (running) {
        while (graphics_poll_event(&event)) {
            if (event.type == EVENT_QUIT) running = 0;
        }
        
        graphics_set_color(COLOR_BLACK);
        graphics_clear();
        
        graphics_set_color(COLOR_CYAN);
        graphics_fill_circle(400, 300, 100);
        
        graphics_present();
        graphics_delay(16);
    }
    
    graphics_quit();
    return 0;
}
```

### Compiler votre programme

```bash
gcc -o mon_programme mon_programme.c -I./include -L./lib -lgraphics -lSDL2 -lSDL2_ttf -lm
```

## Exemples fournis

Trois programmes d'exemple sont inclus :

1. **exemple_simple** - Démonstration des formes de base et du texte
2. **exemple_dessin** - Programme de dessin interactif avec la souris
3. **exemple_animation** - Balle rebondissante animée

Exécutez-les après compilation :
```bash
./examples/exemple_simple
./examples/exemple_dessin
./examples/exemple_animation
```

## Documentation complète

Consultez le fichier [GUIDE.md](GUIDE.md) pour :
- Documentation complète de toutes les fonctions
- Exemples d'utilisation détaillés
- Guide de résolution des problèmes
- Structure du projet

## Fonctionnalités principales

### Gestion de fenêtre
- `graphics_init()` - Initialisation
- `graphics_create_window()` - Création de fenêtre
- `graphics_clear()` - Effacement
- `graphics_present()` - Rafraîchissement
- `graphics_quit()` - Fermeture

### Dessin
- Points, lignes, rectangles (pleins ou contours)
- Cercles (pleins ou contours)
- Texte avec taille configurable

### Événements
- Fermeture de fenêtre
- Clavier (touches pressées/relâchées)
- Souris (clics, déplacements)

## Structure du projet

```
├── include/
│   └── graphics.h          # API publique
├── src/
│   └── graphics.c          # Implémentation
├── examples/
│   ├── exemple_simple.c    # Formes de base
│   ├── exemple_dessin.c    # Dessin interactif
│   └── exemple_animation.c # Animation
├── Makefile                # Compilation
├── GUIDE.md                # Documentation détaillée
└── README.md               # Ce fichier
```

## Licence

Projet éducatif libre d'utilisation.

## Contribution

Les contributions sont les bienvenues ! N'hésitez pas à proposer des améliorations ou de nouvelles fonctionnalités.
