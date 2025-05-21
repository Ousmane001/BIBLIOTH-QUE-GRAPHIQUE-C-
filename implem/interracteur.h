//
// Created by diakitao on 5/12/25.
//

#ifndef INTERRACTEUR_H
#define INTERRACTEUR_H


#include <stdio.h>
#include <stdlib.h>
#include "ei_implementation.h"
#include "ei_widget_attributes.h"
#include "ei_widget_configure.h"
#include "ei_application.h"
#include "ei_draw.h"
#include "ei_event.h"
#include "ei_placer.h"
#include "ei_utils.h"
#include "bib_4_frame.h"
#include "bib_4_button.h"


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  GESTION DU DICO    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#define CAPACITE_INITIALE 30     // on commence avec 30 widget de base et dès qu'on atteint le 27iem , on augement la taille de notre dictionnaire
#define TAUX_REMPLISSAGE_MAX 0.90


typedef struct widget_element_dict {

	int cle;
	ei_widget_t widget;
	struct widget_element_dict* suivante;
} widget_element_dict;

// Le dictionnaire lui-même : tableau de seaux, taille, capacité
typedef struct {
	widget_element_dict** seaux;

	int capacite;
	int taille;
} dictionnaire;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   Prototypes  du Dictionnaire   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**
 * @brief la fonction qui permet de hasher le cle capacité
 *
 * @param cle  hash de la clé correspondante
 *
 * @param  capacite capacite ou taille du dicco app
 */
unsigned int hacher(int cle, int capacite);

dictionnaire* get_dicco_app(void);

void initialise_dicco_app(void);

dictionnaire creer_dictionnaire();

ei_widget_t obtenir(dictionnaire* d, int cle);

void redimensionner(dictionnaire* d);

void ajouter(dictionnaire* d, int cle, ei_widget_t valeur);

void liberer(dictionnaire* d);

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   Gestion  Evenements   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

ei_surface_t get_offscreen_picking(void);

ei_color_t genere_couleur_suivante();

void incremente(void);

int get_key_actuel(void);

ei_widgetclass_t* get_widget_class_list(void);

ei_widget_t get_widget_by_pt(int x0, int y0);

bool est_dans_rect(ei_point_t point, ei_rect_t rect);


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   Gestion  Application   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

ei_surface_t ei_app_root_surface(void);

ei_linked_rect_t* get_invalidate_rect_list(void);

void ei_app_invalidate_rect(const ei_rect_t* rect);

void draw_invalidate_rect(void);

void ei_app_create(ei_size_t main_window_size,bool fullscreen);

ei_widget_t ei_widget_create(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data,  ei_widget_destructor_t destructor);

void effacer(ei_widget_t widget);

void initialise_surf_rect(void);

ei_rect_t* get_surf_app_rect(void);

ei_linked_rect_t** get_invalidate_rect_list_ptr();

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     Fonctions Utilisé par les bibs  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

ei_color_t reorder_color_channels(ei_color_t color, ei_surface_t surface);

ei_point_t* surface_localistion(ei_rect_t scree_location, int x, int y, ei_anchor_t* encre, int bordure);



// pas à sa place :
ei_widgetclass_t* create_toplevel_widgetclass();
#endif //INTERRACTEUR_H
