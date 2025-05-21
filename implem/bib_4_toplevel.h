//
// Created by Ousmane Diakite on 17/05/2025.
//

#ifndef BIB_4_TOPLEVEL_H
#define BIB_4_TOPLEVEL_H

#include "interracteur.h"


#define TAILLE_BUTTON_RESIZE 15
#define RAYON_TOP_LEVEL 10
#define TAILLE_CADRE_HAUT 40
#define TAILLE_ENTETE_TOP_LEVEL 28
#define TAILLE_BOUTTON_CLOSE    15
#define DIMENSION_BOUTTON_CLOSE (ei_size_t){TAILLE_BOUTTON_CLOSE,TAILLE_BOUTTON_CLOSE}

/**
 * @brief CrÃ©e et initialise une classe de widget pour le type toplevel.
 *
 * @return Un pointeur vers la structure de classe de widget crÃ©Ã©e.
 */
ei_widgetclass_t* create_toplevel_widgetclass();


/**
 * @brief Applique les valeurs par dÃ©faut Ã  un widget toplevel.
 *
 * @param widget Le widget toplevel Ã  initialiser.
 */
void toplevel_setdefaults(ei_widget_t widget);

/**
 * @brief Alloue dynamiquement un widget toplevel.
 *
 * @return Un pointeur vers le widget nouvellement allouÃ©, ou NULL en cas d'erreur d'allocation.
 */
ei_widget_t toplevel_alloc();


/**
 * @brief LibÃ¨re toute la mÃ©moire allouÃ©e Ã  un widget toplevel.
 *
 * @param widget Le widget toplevel Ã  dÃ©truire.
 */
void toplevel_release(ei_widget_t widget);

/**
 * @brief Dessine un widget toplevel sur la surface donnÃ©e.
 *
 * @param widget        Le widget Ã  dessiner.
 * @param surface       Surface d'affichage principale.
 * @param pick_surface  Surface de dÃ©tection d'Ã©vÃ©nements (offscreen).
 * @param clipper       Rectangle de dÃ©coupe pour le dessin (zone visible).
 */
void toplevel_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);

/**
 * @brief Configure les propriÃ©tÃ©s d'un widget toplevel.
 *
 * @param widget            Le widget toplevel Ã  configurer.
 * @param requested_size    Taille souhaitÃ©e du widget.
 * @param color             Couleur de fond du widget.
 * @param border_width      Largeur de la bordure.
 * @param title             Titre Ã  afficher dans l'en-tÃªte.
 * @param closable          BoolÃ©en indiquant si la fenÃªtre peut Ãªtre fermÃ©e.
 * @param resizable         Axe(s) sur le(s)quel(s) le widget peut Ãªtre redimensionnÃ©.
 * @param min_size          Taille minimale autorisÃ©e du widget.
 */
void ei_toplevel_configure(ei_widget_t		widget,
                          ei_size_t*		requested_size,
                          const ei_color_t*	color,
                          int*			border_width,
                          ei_string_t*		title,
                          bool*			closable,
                          ei_axis_set_t*		resizable,
                          ei_size_ptr_t*		min_size);

/**
 * @brief Notifie le widget toplevel qu'il a Ã©tÃ© gÃ©omÃ©triquement modifiÃ©.
 *        RÃ©ajuste la position et la taille pour respecter les contraintes.
 *
 * @param widget Le widget toplevel concernÃ©.
 */
void toplevel_geonotify(ei_widget_t		widget);

/**
 * @brief GÃ¨re les Ã©vÃ©nements liÃ©s Ã  un widget toplevel (clic, dÃ©placement, redimensionnement).
 *
 * @param widget Le widget toplevel concernÃ©.
 * @param event  L'Ã©vÃ©nement Ã  traiter (clic, mouvement souris, relÃ¢chement).
 *
 * @return true si l'Ã©vÃ©nement a Ã©tÃ© traitÃ©, false sinon.
 */
bool toplevel_handle(ei_widget_t widget, struct ei_event_t* event);


void close_function(ei_widget_t widget, struct ei_event_t* event, ei_user_param_t user_param);


#endif //BIB_4_TOPLEVEL_H