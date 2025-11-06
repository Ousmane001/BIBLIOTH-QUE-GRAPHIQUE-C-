/**
 * @file exemple_simple.c
 * @brief Exemple simple d'utilisation de la bibliothèque graphique
 * 
 * Cet exemple montre comment créer une fenêtre et dessiner des formes de base.
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
    if (graphics_create_window("Exemple Simple - Bibliothèque Graphique", 800, 600) != 0) {
        fprintf(stderr, "Erreur de création de fenêtre\n");
        graphics_quit();
        return 1;
    }
    
    /* Boucle principale */
    int running = 1;
    Event event;
    
    while (running) {
        /* Gérer les événements */
        while (graphics_poll_event(&event)) {
            if (event.type == EVENT_QUIT) {
                running = 0;
            } else if (event.type == EVENT_KEY_DOWN) {
                if (event.key_code == SDLK_ESCAPE) {
                    running = 0;
                }
            }
        }
        
        /* Effacer l'écran en noir */
        graphics_set_color(COLOR_BLACK);
        graphics_clear();
        
        /* Dessiner un rectangle rouge */
        graphics_set_color(COLOR_RED);
        graphics_fill_rectangle(100, 100, 200, 150);
        
        /* Dessiner un cercle vert */
        graphics_set_color(COLOR_GREEN);
        graphics_fill_circle(500, 200, 80);
        
        /* Dessiner des lignes bleues */
        graphics_set_color(COLOR_BLUE);
        graphics_draw_line(50, 400, 750, 400);
        graphics_draw_line(400, 50, 400, 550);
        
        /* Dessiner un cercle jaune (contour) */
        graphics_set_color(COLOR_YELLOW);
        graphics_draw_circle(650, 450, 60);
        
        /* Dessiner du texte blanc */
        graphics_set_color(COLOR_WHITE);
        graphics_draw_text("Bibliotheque Graphique en C", 250, 20, 24);
        graphics_draw_text("Appuyez sur ESC pour quitter", 250, 550, 18);
        
        /* Rafraîchir l'affichage */
        graphics_present();
        
        /* Attendre un peu pour limiter la fréquence de rafraîchissement */
        graphics_delay(16); /* ~60 FPS */
    }
    
    /* Nettoyer et quitter */
    graphics_quit();
    
    return 0;
}
