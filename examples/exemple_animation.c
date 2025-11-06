/**
 * @file exemple_animation.c
 * @brief Exemple d'animation simple
 * 
 * Cet exemple montre un cercle qui se déplace et rebondit sur les bords.
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
    if (graphics_create_window("Exemple Animation - Balle rebondissante", 800, 600) != 0) {
        fprintf(stderr, "Erreur de création de fenêtre\n");
        graphics_quit();
        return 1;
    }
    
    /* Variables pour l'animation */
    int running = 1;
    Event event;
    
    float ball_x = 400.0f;
    float ball_y = 300.0f;
    float ball_vx = 3.0f;
    float ball_vy = 2.5f;
    int ball_radius = 30;
    
    /* Boucle principale */
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
        
        /* Mettre à jour la position de la balle */
        ball_x += ball_vx;
        ball_y += ball_vy;
        
        /* Rebondir sur les bords */
        if (ball_x - ball_radius < 0 || ball_x + ball_radius > 800) {
            ball_vx = -ball_vx;
        }
        if (ball_y - ball_radius < 0 || ball_y + ball_radius > 600) {
            ball_vy = -ball_vy;
        }
        
        /* Effacer l'écran */
        graphics_set_color(COLOR_BLACK);
        graphics_clear();
        
        /* Dessiner la balle */
        graphics_set_color(COLOR_CYAN);
        graphics_fill_circle((int)ball_x, (int)ball_y, ball_radius);
        
        /* Dessiner un contour blanc autour de la balle */
        graphics_set_color(COLOR_WHITE);
        graphics_draw_circle((int)ball_x, (int)ball_y, ball_radius);
        
        /* Afficher les instructions */
        graphics_set_color(COLOR_YELLOW);
        graphics_draw_text("Animation - Balle rebondissante", 250, 20, 24);
        graphics_draw_text("Appuyez sur ESC pour quitter", 280, 560, 18);
        
        /* Rafraîchir l'affichage */
        graphics_present();
        
        /* Attendre pour maintenir ~60 FPS */
        graphics_delay(16);
    }
    
    /* Nettoyer et quitter */
    graphics_quit();
    
    return 0;
}
