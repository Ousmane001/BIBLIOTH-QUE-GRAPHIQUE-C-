/**
 * @file exemple_dessin.c
 * @brief Exemple de dessin interactif avec la souris
 * 
 * Cet exemple permet de dessiner avec la souris en cliquant et en déplaçant.
 */

#include "../include/graphics.h"
#include <stdio.h>

int main(void) {
    /* Initialiser la bibliothèque */
    if (graphics_init() != 0) {
        fprintf(stderr, "Erreur d'initialisation\n");
        return 1;
    }
    
    /* Créer une fenêtre */
    if (graphics_create_window("Exemple Dessin - Cliquez et déplacez pour dessiner", 800, 600) != 0) {
        fprintf(stderr, "Erreur de création de fenêtre\n");
        graphics_quit();
        return 1;
    }
    
    /* Variables pour le dessin */
    int running = 1;
    int drawing = 0;
    int last_x = 0, last_y = 0;
    Event event;
    
    /* Effacer l'écran en noir une fois */
    graphics_set_color(COLOR_BLACK);
    graphics_clear();
    
    /* Afficher les instructions */
    graphics_set_color(COLOR_WHITE);
    graphics_draw_text("Cliquez et déplacez pour dessiner - ESC pour quitter", 150, 10, 20);
    graphics_present();
    
    /* Boucle principale */
    while (running) {
        /* Gérer les événements */
        while (graphics_poll_event(&event)) {
            if (event.type == EVENT_QUIT) {
                running = 0;
            } else if (event.type == EVENT_KEY_DOWN) {
                if (event.key_code == SDLK_ESCAPE) {
                    running = 0;
                } else if (event.key_code == SDLK_c) {
                    /* Effacer l'écran avec 'C' */
                    graphics_set_color(COLOR_BLACK);
                    graphics_clear();
                    graphics_set_color(COLOR_WHITE);
                    graphics_draw_text("Cliquez et déplacez pour dessiner - ESC pour quitter - C pour effacer", 100, 10, 20);
                    graphics_present();
                }
            } else if (event.type == EVENT_MOUSE_BUTTON_DOWN) {
                drawing = 1;
                last_x = event.mouse_x;
                last_y = event.mouse_y;
            } else if (event.type == EVENT_MOUSE_BUTTON_UP) {
                drawing = 0;
            } else if (event.type == EVENT_MOUSE_MOTION && drawing) {
                /* Dessiner une ligne de la dernière position à la position actuelle */
                graphics_set_color(COLOR_WHITE);
                graphics_draw_line(last_x, last_y, event.mouse_x, event.mouse_y);
                
                /* Dessiner aussi un petit cercle pour un trait plus épais */
                graphics_fill_circle(event.mouse_x, event.mouse_y, 2);
                
                graphics_present();
                
                last_x = event.mouse_x;
                last_y = event.mouse_y;
            }
        }
        
        /* Attendre un peu */
        graphics_delay(16);
    }
    
    /* Nettoyer et quitter */
    graphics_quit();
    
    return 0;
}
