# Architecture de la Bibliothèque Graphique en C

## Vue d'ensemble

Cette bibliothèque graphique en C fournit une abstraction simple au-dessus de SDL2 pour faciliter la création d'applications graphiques en mode fenêtré.

## Architecture en couches

```
┌─────────────────────────────────────────┐
│   Applications Utilisateur              │
│   (exemples et programmes personnalisés) │
└─────────────────────────────────────────┘
                  ↓
┌─────────────────────────────────────────┐
│   API Bibliothèque Graphique            │
│   (graphics.h)                          │
│   - Gestion fenêtre                     │
│   - Dessin de formes                    │
│   - Gestion événements                  │
│   - Gestion couleurs                    │
└─────────────────────────────────────────┘
                  ↓
┌─────────────────────────────────────────┐
│   Implémentation (graphics.c)           │
│   - Algorithmes de dessin               │
│   - Gestion d'état                      │
│   - Wrapper SDL2                        │
└─────────────────────────────────────────┘
                  ↓
┌─────────────────────────────────────────┐
│   SDL2 / SDL2_ttf                       │
│   (Bibliothèques système)               │
└─────────────────────────────────────────┘
```

## Modules principaux

### 1. Gestion de fenêtre
**Fichier**: `src/graphics.c` (fonctions d'initialisation)

**Responsabilités**:
- Initialisation de SDL2 et SDL2_ttf
- Création et destruction de fenêtre
- Gestion du renderer
- Rafraîchissement de l'affichage

**Fonctions clés**:
- `graphics_init()` - Initialise SDL2
- `graphics_create_window()` - Crée une fenêtre SDL
- `graphics_quit()` - Libère toutes les ressources

### 2. Système de dessin
**Fichier**: `src/graphics.c` (fonctions de dessin)

**Responsabilités**:
- Dessin de primitives géométriques
- Rendu de texte
- Gestion du buffer de dessin

**Implémentations**:
- **Pixels et lignes**: Appels directs à SDL
- **Rectangles**: Utilisation de SDL_Rect
- **Cercles**: Algorithme de Bresenham (implémentation personnalisée)
- **Texte**: Rendu via SDL_ttf avec gestion de police

### 3. Gestion des couleurs
**Fichier**: `src/graphics.c` (variables globales et fonctions de couleur)

**Responsabilités**:
- Définition de couleurs (RGBA)
- Couleurs prédéfinies
- Application de couleur au renderer

**Structures**:
```c
typedef struct {
    Uint8 r, g, b, a;
} Color;
```

### 4. Système d'événements
**Fichier**: `src/graphics.c` (fonctions d'événements)

**Responsabilités**:
- Polling des événements SDL
- Translation des événements SDL vers format simplifié
- Gestion du timing

**Types d'événements supportés**:
- Fermeture de fenêtre (EVENT_QUIT)
- Clavier (EVENT_KEY_DOWN, EVENT_KEY_UP)
- Souris (EVENT_MOUSE_BUTTON_DOWN, EVENT_MOUSE_BUTTON_UP, EVENT_MOUSE_MOTION)

## Structures de données

### Color
```c
typedef struct {
    Uint8 r;  // Rouge (0-255)
    Uint8 g;  // Vert (0-255)
    Uint8 b;  // Bleu (0-255)
    Uint8 a;  // Alpha/Transparence (0-255)
} Color;
```

### Point
```c
typedef struct {
    int x;
    int y;
} Point;
```

### Rectangle
```c
typedef struct {
    int x;
    int y;
    int width;
    int height;
} Rectangle;
```

### Event
```c
typedef struct {
    EventType type;
    int key_code;      // Pour événements clavier
    int mouse_x;       // Pour événements souris
    int mouse_y;
    int mouse_button;  // 1=gauche, 2=milieu, 3=droit
} Event;
```

## État interne

La bibliothèque maintient un état global privé:

```c
static SDL_Window* window = NULL;      // Fenêtre principale
static SDL_Renderer* renderer = NULL;  // Renderer SDL
static TTF_Font* font = NULL;          // Police de caractères
static Color current_color;            // Couleur actuelle
```

## Algorithmes notables

### Algorithme de cercle de Bresenham
Utilisé pour `graphics_draw_circle()` et `graphics_fill_circle()`:
- Évite les calculs en virgule flottante
- Exploite la symétrie du cercle (8 points par itération)
- Complexité O(r) où r est le rayon

### Rendu de texte
Processus en plusieurs étapes:
1. Chargement de police TrueType (première utilisation)
2. Rendu du texte sur une surface SDL
3. Conversion de la surface en texture
4. Copie de la texture sur le renderer
5. Libération des ressources temporaires

## Gestion de la mémoire

### Allocation
- Fenêtre et renderer: alloués lors de `graphics_create_window()`
- Police: chargée paresseusement lors du premier appel à `graphics_draw_text()`
- Textures de texte: créées et détruites dans `graphics_draw_text()`

### Libération
- Tout est libéré dans `graphics_quit()`
- Ordre de libération: police → renderer → fenêtre → SDL

## Flux d'exécution typique

```
1. graphics_init()
   ↓
2. graphics_create_window()
   ↓
3. Boucle principale:
   ├─ graphics_poll_event()
   ├─ graphics_set_color()
   ├─ graphics_clear()
   ├─ graphics_draw_*() / graphics_fill_*()
   ├─ graphics_present()
   └─ graphics_delay()
   ↓
4. graphics_quit()
```

## Compilation

### Bibliothèque statique
```bash
gcc -fPIC -c src/graphics.c -o obj/graphics.o
ar rcs lib/libgraphics.a obj/graphics.o
```

### Bibliothèque partagée
```bash
gcc -shared -o lib/libgraphics.so obj/graphics.o -lSDL2 -lSDL2_ttf -lm
```

### Application
```bash
gcc mon_app.c -Iinclude -Llib -lgraphics -lSDL2 -lSDL2_ttf -lm
```

## Dépendances

### Externes
- **SDL2** (≥2.0): Gestion de fenêtre et rendu
- **SDL2_ttf** (≥2.0): Rendu de texte TrueType
- **libm**: Fonctions mathématiques (pour algorithmes de cercle)

### Internes
- `graphics.c` dépend de `graphics.h`
- Tous les exemples dépendent de la bibliothèque compilée

## Extensions possibles

### Futures améliorations
1. Support d'images (PNG, JPG) via SDL2_image
2. Support de formes additionnelles (ellipses, polygones)
3. Transformations (rotation, échelle)
4. Gestion de plusieurs fenêtres
5. Support OpenGL pour accélération 3D
6. Gestion de sprites et animations
7. Support audio via SDL2_mixer

### Points d'extension
- Nouvelles formes dans `graphics.c`
- Nouveaux types d'événements dans `Event`
- Nouvelles propriétés de rendu (blend modes, etc.)

## Considérations de performance

### Optimisations actuelles
- Utilisation du renderer accéléré SDL (GPU)
- VSync activé pour synchronisation
- Algorithmes efficaces (Bresenham)

### Limitations
- Un seul renderer (pas de multi-threading)
- Pas de culling ou clipping manuel
- Pas de batching de commandes de dessin

## Compatibilité

### Systèmes supportés
- Linux (testé sur Ubuntu 24.04)
- macOS (avec SDL2 via Homebrew)
- Windows (avec SDL2 installé)

### Compilateurs
- GCC (recommandé)
- Clang
- MSVC (avec adaptations mineures)

## Licence

MIT License - Voir fichier LICENSE pour détails.
