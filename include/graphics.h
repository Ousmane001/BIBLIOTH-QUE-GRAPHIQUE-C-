/**
 * @file graphics.h
 * @brief Bibliothèque Graphique en C pour la création de programmes en mode fenêtré
 * 
 * Cette bibliothèque fournit des fonctions simples pour créer des fenêtres,
 * dessiner des formes géométriques et gérer les événements utilisateur.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* Types de base */
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Rectangle;

typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} Color;

/* Couleurs prédéfinies */
extern const Color COLOR_BLACK;
extern const Color COLOR_WHITE;
extern const Color COLOR_RED;
extern const Color COLOR_GREEN;
extern const Color COLOR_BLUE;
extern const Color COLOR_YELLOW;
extern const Color COLOR_CYAN;
extern const Color COLOR_MAGENTA;

/* Types d'événements */
typedef enum {
    EVENT_NONE,
    EVENT_QUIT,
    EVENT_KEY_DOWN,
    EVENT_KEY_UP,
    EVENT_MOUSE_BUTTON_DOWN,
    EVENT_MOUSE_BUTTON_UP,
    EVENT_MOUSE_MOTION
} EventType;

typedef struct {
    EventType type;
    int key_code;        /* Pour les événements clavier */
    int mouse_x;         /* Pour les événements souris */
    int mouse_y;
    int mouse_button;    /* Bouton de souris (1=gauche, 2=milieu, 3=droit) */
} Event;

/* Fonctions de gestion de la fenêtre */

/**
 * @brief Initialise la bibliothèque graphique
 * @return 0 en cas de succès, -1 en cas d'erreur
 */
int graphics_init(void);

/**
 * @brief Crée une fenêtre
 * @param titre Titre de la fenêtre
 * @param largeur Largeur de la fenêtre en pixels
 * @param hauteur Hauteur de la fenêtre en pixels
 * @return 0 en cas de succès, -1 en cas d'erreur
 */
int graphics_create_window(const char* titre, int largeur, int hauteur);

/**
 * @brief Efface la fenêtre avec la couleur de fond
 */
void graphics_clear(void);

/**
 * @brief Définit la couleur de fond utilisée par graphics_clear()
 * @param couleur Couleur de fond à utiliser
 */
void graphics_set_background_color(Color couleur);

/**
 * @brief Rafraîchit l'affichage de la fenêtre
 */
void graphics_present(void);

/**
 * @brief Ferme la fenêtre et libère les ressources
 */
void graphics_quit(void);

/* Fonctions de gestion des couleurs */

/**
 * @brief Définit la couleur de dessin courante
 * @param couleur Couleur à utiliser
 */
void graphics_set_color(Color couleur);

/**
 * @brief Crée une couleur à partir des composantes RGBA
 * @param r Composante rouge (0-255)
 * @param g Composante verte (0-255)
 * @param b Composante bleue (0-255)
 * @param a Composante alpha/transparence (0-255)
 * @return Structure Color
 */
Color graphics_make_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Fonctions de dessin */

/**
 * @brief Dessine un pixel
 * @param x Coordonnée X
 * @param y Coordonnée Y
 */
void graphics_draw_pixel(int x, int y);

/**
 * @brief Dessine une ligne
 * @param x1 Coordonnée X du point de départ
 * @param y1 Coordonnée Y du point de départ
 * @param x2 Coordonnée X du point d'arrivée
 * @param y2 Coordonnée Y du point d'arrivée
 */
void graphics_draw_line(int x1, int y1, int x2, int y2);

/**
 * @brief Dessine un rectangle (contour)
 * @param x Coordonnée X du coin supérieur gauche
 * @param y Coordonnée Y du coin supérieur gauche
 * @param width Largeur du rectangle
 * @param height Hauteur du rectangle
 */
void graphics_draw_rectangle(int x, int y, int width, int height);

/**
 * @brief Dessine un rectangle rempli
 * @param x Coordonnée X du coin supérieur gauche
 * @param y Coordonnée Y du coin supérieur gauche
 * @param width Largeur du rectangle
 * @param height Hauteur du rectangle
 */
void graphics_fill_rectangle(int x, int y, int width, int height);

/**
 * @brief Dessine un cercle (contour)
 * @param x Coordonnée X du centre
 * @param y Coordonnée Y du centre
 * @param radius Rayon du cercle
 */
void graphics_draw_circle(int x, int y, int radius);

/**
 * @brief Dessine un cercle rempli
 * @param x Coordonnée X du centre
 * @param y Coordonnée Y du centre
 * @param radius Rayon du cercle
 */
void graphics_fill_circle(int x, int y, int radius);

/**
 * @brief Dessine du texte
 * @param text Texte à afficher
 * @param x Coordonnée X
 * @param y Coordonnée Y
 * @param font_size Taille de la police
 */
void graphics_draw_text(const char* text, int x, int y, int font_size);

/* Fonctions de gestion des événements */

/**
 * @brief Vérifie s'il y a des événements en attente
 * @param event Pointeur vers la structure Event à remplir
 * @return 1 s'il y a un événement, 0 sinon
 */
int graphics_poll_event(Event* event);

/**
 * @brief Attend un délai en millisecondes
 * @param ms Délai en millisecondes
 */
void graphics_delay(int ms);

#endif /* GRAPHICS_H */
