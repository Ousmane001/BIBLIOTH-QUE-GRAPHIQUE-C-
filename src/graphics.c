/**
 * @file graphics.c
 * @brief Implémentation de la bibliothèque graphique
 */

#include "../include/graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Variables globales privées */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* font = NULL;
static int current_font_size = 0;
static Color current_color = {255, 255, 255, 255};
static Color background_color = {0, 0, 0, 255};

/* Couleurs prédéfinies */
const Color COLOR_BLACK = {0, 0, 0, 255};
const Color COLOR_WHITE = {255, 255, 255, 255};
const Color COLOR_RED = {255, 0, 0, 255};
const Color COLOR_GREEN = {0, 255, 0, 255};
const Color COLOR_BLUE = {0, 0, 255, 255};
const Color COLOR_YELLOW = {255, 255, 0, 255};
const Color COLOR_CYAN = {0, 255, 255, 255};
const Color COLOR_MAGENTA = {255, 0, 255, 255};

/* Implémentation des fonctions de gestion de fenêtre */

int graphics_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur d'initialisation SDL: %s\n", SDL_GetError());
        return -1;
    }
    
    if (TTF_Init() < 0) {
        fprintf(stderr, "Erreur d'initialisation TTF: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }
    
    return 0;
}

int graphics_create_window(const char* titre, int largeur, int hauteur) {
    window = SDL_CreateWindow(
        titre,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        largeur,
        hauteur,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        fprintf(stderr, "Erreur de création de fenêtre: %s\n", SDL_GetError());
        return -1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        window = NULL;
        return -1;
    }
    
    /* Définir la couleur de fond par défaut (noir) */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    return 0;
}

void graphics_clear(void) {
    if (renderer) {
        SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
        SDL_RenderClear(renderer);
        /* Restaurer la couleur de dessin actuelle */
        SDL_SetRenderDrawColor(renderer, current_color.r, current_color.g, current_color.b, current_color.a);
    }
}

void graphics_present(void) {
    if (renderer) {
        SDL_RenderPresent(renderer);
    }
}

void graphics_quit(void) {
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    
    TTF_Quit();
    SDL_Quit();
}

/* Implémentation des fonctions de gestion des couleurs */

void graphics_set_color(Color couleur) {
    current_color = couleur;
    if (renderer) {
        SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a);
    }
}

Color graphics_make_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    Color couleur = {r, g, b, a};
    return couleur;
}

/* Implémentation des fonctions de dessin */

void graphics_draw_pixel(int x, int y) {
    if (renderer) {
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void graphics_draw_line(int x1, int y1, int x2, int y2) {
    if (renderer) {
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void graphics_draw_rectangle(int x, int y, int width, int height) {
    if (renderer) {
        SDL_Rect rect = {x, y, width, height};
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void graphics_fill_rectangle(int x, int y, int width, int height) {
    if (renderer) {
        SDL_Rect rect = {x, y, width, height};
        SDL_RenderFillRect(renderer, &rect);
    }
}

/* Algorithme de dessin de cercle (Bresenham) */
static void draw_circle_points(int xc, int yc, int x, int y, int fill) {
    if (fill) {
        graphics_draw_line(xc - x, yc + y, xc + x, yc + y);
        graphics_draw_line(xc - x, yc - y, xc + x, yc - y);
        graphics_draw_line(xc - y, yc + x, xc + y, yc + x);
        graphics_draw_line(xc - y, yc - x, xc + y, yc - x);
    } else {
        graphics_draw_pixel(xc + x, yc + y);
        graphics_draw_pixel(xc - x, yc + y);
        graphics_draw_pixel(xc + x, yc - y);
        graphics_draw_pixel(xc - x, yc - y);
        graphics_draw_pixel(xc + y, yc + x);
        graphics_draw_pixel(xc - y, yc + x);
        graphics_draw_pixel(xc + y, yc - x);
        graphics_draw_pixel(xc - y, yc - x);
    }
}

void graphics_draw_circle(int xc, int yc, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    
    draw_circle_points(xc, yc, x, y, 0);
    
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        draw_circle_points(xc, yc, x, y, 0);
    }
}

void graphics_fill_circle(int xc, int yc, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    
    draw_circle_points(xc, yc, x, y, 1);
    
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        draw_circle_points(xc, yc, x, y, 1);
    }
}

void graphics_draw_text(const char* text, int x, int y, int font_size) {
    if (!renderer || !text) {
        return;
    }
    
    /* Recharger la police si la taille change ou si elle n'est pas encore chargée */
    if (!font || current_font_size != font_size) {
        /* Fermer la police précédente si elle existe */
        if (font) {
            TTF_CloseFont(font);
            font = NULL;
        }
        
        /* Essayer de charger une police système par défaut */
        const char* font_paths[] = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/System/Library/Fonts/Helvetica.ttc",
            "C:\\Windows\\Fonts\\arial.ttf",
            NULL
        };
        
        for (int i = 0; font_paths[i] != NULL; i++) {
            font = TTF_OpenFont(font_paths[i], font_size);
            if (font) {
                current_font_size = font_size;
                break;
            }
        }
        
        if (!font) {
            fprintf(stderr, "Impossible de charger une police système\n");
            return;
        }
    }
    
    SDL_Color color = {current_color.r, current_color.g, current_color.b, current_color.a};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    
    if (!surface) {
        fprintf(stderr, "Erreur de rendu du texte: %s\n", TTF_GetError());
        return;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

/* Implémentation des fonctions de gestion des événements */

int graphics_poll_event(Event* event) {
    if (!event) {
        return 0;
    }
    
    SDL_Event sdl_event;
    if (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
            case SDL_QUIT:
                event->type = EVENT_QUIT;
                return 1;
                
            case SDL_KEYDOWN:
                event->type = EVENT_KEY_DOWN;
                event->key_code = sdl_event.key.keysym.sym;
                return 1;
                
            case SDL_KEYUP:
                event->type = EVENT_KEY_UP;
                event->key_code = sdl_event.key.keysym.sym;
                return 1;
                
            case SDL_MOUSEBUTTONDOWN:
                event->type = EVENT_MOUSE_BUTTON_DOWN;
                event->mouse_x = sdl_event.button.x;
                event->mouse_y = sdl_event.button.y;
                event->mouse_button = sdl_event.button.button;
                return 1;
                
            case SDL_MOUSEBUTTONUP:
                event->type = EVENT_MOUSE_BUTTON_UP;
                event->mouse_x = sdl_event.button.x;
                event->mouse_y = sdl_event.button.y;
                event->mouse_button = sdl_event.button.button;
                return 1;
                
            case SDL_MOUSEMOTION:
                event->type = EVENT_MOUSE_MOTION;
                event->mouse_x = sdl_event.motion.x;
                event->mouse_y = sdl_event.motion.y;
                return 1;
                
            default:
                event->type = EVENT_NONE;
                return 1;
        }
    }
    
    return 0;
}

void graphics_delay(int ms) {
    SDL_Delay(ms);
}
