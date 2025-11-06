# Guide d'utilisation de la Bibliothèque Graphique en C

## Description

Cette bibliothèque graphique en C permet de créer facilement des programmes en mode fenêtré. Elle offre des fonctions simples pour :

- Créer et gérer des fenêtres
- Dessiner des formes géométriques (rectangles, cercles, lignes)
- Afficher du texte
- Gérer les événements (clavier, souris)
- Créer des animations

La bibliothèque utilise SDL2 en arrière-plan pour assurer la compatibilité multi-plateforme.

## Prérequis

Pour compiler et utiliser cette bibliothèque, vous devez avoir installé :

- **gcc** (compilateur C)
- **make** (outil de compilation)
- **SDL2** (bibliothèque graphique)
- **SDL2_ttf** (extension SDL2 pour le texte)

### Installation des dépendances

#### Sur Ubuntu/Debian :
```bash
sudo apt-get update
sudo apt-get install build-essential libsdl2-dev libsdl2-ttf-dev
```

#### Sur Fedora/RedHat :
```bash
sudo dnf install gcc make SDL2-devel SDL2_ttf-devel
```

#### Sur Arch Linux :
```bash
sudo pacman -S base-devel sdl2 sdl2_ttf
```

#### Sur macOS (avec Homebrew) :
```bash
brew install sdl2 sdl2_ttf
```

## Compilation

### Compiler la bibliothèque et les exemples

```bash
make
```

Cela crée :
- `lib/libgraphics.a` - Bibliothèque statique
- `lib/libgraphics.so` - Bibliothèque partagée
- Les exécutables des exemples dans le dossier `examples/`

### Compiler uniquement les exemples

```bash
make examples
```

### Nettoyer les fichiers compilés

```bash
make clean
```

## Installation (optionnel)

Pour installer la bibliothèque sur votre système :

```bash
sudo make install
```

Cela copie :
- Les bibliothèques dans `/usr/local/lib/`
- Le fichier d'en-tête dans `/usr/local/include/`

Pour désinstaller :

```bash
sudo make uninstall
```

## Utilisation

### Structure de base d'un programme

```c
#include <graphics.h>

int main(void) {
    /* 1. Initialiser la bibliothèque */
    if (graphics_init() != 0) {
        return 1;
    }
    
    /* 2. Créer une fenêtre */
    if (graphics_create_window("Mon Programme", 800, 600) != 0) {
        graphics_quit();
        return 1;
    }
    
    /* 3. Boucle principale */
    int running = 1;
    Event event;
    
    while (running) {
        /* Gérer les événements */
        while (graphics_poll_event(&event)) {
            if (event.type == EVENT_QUIT) {
                running = 0;
            }
        }
        
        /* Effacer l'écran */
        graphics_set_color(COLOR_BLACK);
        graphics_clear();
        
        /* Dessiner des formes */
        graphics_set_color(COLOR_WHITE);
        graphics_fill_circle(400, 300, 50);
        
        /* Rafraîchir l'affichage */
        graphics_present();
        
        /* Limiter la fréquence de rafraîchissement */
        graphics_delay(16);  /* ~60 FPS */
    }
    
    /* 4. Nettoyer et quitter */
    graphics_quit();
    
    return 0;
}
```

### Compilation d'un programme utilisant la bibliothèque

Si la bibliothèque est installée sur le système :

```bash
gcc -o mon_programme mon_programme.c -lgraphics -lSDL2 -lSDL2_ttf -lm
```

Si la bibliothèque n'est pas installée (utilisation locale) :

```bash
gcc -o mon_programme mon_programme.c -I./include -L./lib -lgraphics -lSDL2 -lSDL2_ttf -lm
```

## Fonctions disponibles

### Gestion de la fenêtre

- `int graphics_init(void)` - Initialise la bibliothèque
- `int graphics_create_window(const char* titre, int largeur, int hauteur)` - Crée une fenêtre
- `void graphics_clear(void)` - Efface la fenêtre
- `void graphics_present(void)` - Rafraîchit l'affichage
- `void graphics_quit(void)` - Ferme la fenêtre et libère les ressources

### Gestion des couleurs

- `void graphics_set_color(Color couleur)` - Définit la couleur de dessin
- `Color graphics_make_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)` - Crée une couleur personnalisée

Couleurs prédéfinies : `COLOR_BLACK`, `COLOR_WHITE`, `COLOR_RED`, `COLOR_GREEN`, `COLOR_BLUE`, `COLOR_YELLOW`, `COLOR_CYAN`, `COLOR_MAGENTA`

### Fonctions de dessin

- `void graphics_draw_pixel(int x, int y)` - Dessine un pixel
- `void graphics_draw_line(int x1, int y1, int x2, int y2)` - Dessine une ligne
- `void graphics_draw_rectangle(int x, int y, int width, int height)` - Dessine un rectangle (contour)
- `void graphics_fill_rectangle(int x, int y, int width, int height)` - Dessine un rectangle rempli
- `void graphics_draw_circle(int x, int y, int radius)` - Dessine un cercle (contour)
- `void graphics_fill_circle(int x, int y, int radius)` - Dessine un cercle rempli
- `void graphics_draw_text(const char* text, int x, int y, int font_size)` - Affiche du texte

### Gestion des événements

- `int graphics_poll_event(Event* event)` - Récupère le prochain événement
- `void graphics_delay(int ms)` - Attend un délai en millisecondes

Types d'événements :
- `EVENT_QUIT` - L'utilisateur a fermé la fenêtre
- `EVENT_KEY_DOWN` - Une touche a été pressée
- `EVENT_KEY_UP` - Une touche a été relâchée
- `EVENT_MOUSE_BUTTON_DOWN` - Un bouton de souris a été pressé
- `EVENT_MOUSE_BUTTON_UP` - Un bouton de souris a été relâché
- `EVENT_MOUSE_MOTION` - La souris a bougé

## Exemples

### Exemple 1 : Fenêtre simple avec formes

```bash
./examples/exemple_simple
```

Affiche une fenêtre avec des rectangles, cercles et lignes de différentes couleurs.

### Exemple 2 : Dessin interactif

```bash
./examples/exemple_dessin
```

Permet de dessiner avec la souris. Cliquez et déplacez pour dessiner, appuyez sur 'C' pour effacer.

### Exemple 3 : Animation

```bash
./examples/exemple_animation
```

Montre une balle qui rebondit sur les bords de la fenêtre.

## Structure du projet

```
BIBLIOTH-QUE-GRAPHIQUE-C-/
├── include/
│   └── graphics.h          # Fichier d'en-tête de la bibliothèque
├── src/
│   └── graphics.c          # Implémentation de la bibliothèque
├── examples/
│   ├── exemple_simple.c    # Exemple simple
│   ├── exemple_dessin.c    # Exemple de dessin interactif
│   └── exemple_animation.c # Exemple d'animation
├── Makefile                # Fichier de compilation
├── GUIDE.md                # Ce guide
└── README.md               # Description du projet
```

## Conseils et astuces

1. **Gestion de la boucle principale** : Utilisez toujours une boucle avec `graphics_poll_event()` pour gérer les événements
2. **Fréquence de rafraîchissement** : Utilisez `graphics_delay()` pour contrôler la fréquence de rafraîchissement (16ms ≈ 60 FPS)
3. **Ordre des appels** : Effacez l'écran, dessinez vos formes, puis appelez `graphics_present()`
4. **Libération des ressources** : Appelez toujours `graphics_quit()` avant de terminer votre programme

## Résolution de problèmes

### Erreur : "Impossible de charger une police système"

Assurez-vous qu'une police TrueType est installée sur votre système. Sur Linux, installez :
```bash
sudo apt-get install fonts-dejavu
```

### Erreur de compilation : "SDL2/SDL.h: No such file or directory"

Installez les bibliothèques de développement SDL2 :
```bash
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```

### La fenêtre ne s'affiche pas

Vérifiez que :
- Vous avez bien appelé `graphics_init()` avant `graphics_create_window()`
- Votre système supporte l'affichage graphique (serveur X sur Linux)
- Il n'y a pas de messages d'erreur sur stderr

## Licence

Cette bibliothèque est un projet éducatif libre d'utilisation.

## Auteur

Bibliothèque Graphique en C - Projet de programmation système
