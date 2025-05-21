// Created by Ousmane Diakite on 17/05/2025.
//

#ifndef BIB_4_FRAME_H
#define BIB_4_FRAME_H

#include "interracteur.h"

/*####################################################################################################################*/
/**
 * @brief Alloue dynamiquement un widget de type "frame".
 *
 * Cette fonction crée un nouveau widget de type "frame" en mémoire et initialise ses valeurs par défaut.
 * Elle renvoie un pointeur vers ce widget, qui pourra ensuite être configuré et utilisé dans l'interface.
 *
 * @return Un pointeur vers une structure ei_widget_t correspondant à une frame.
 */
ei_widget_t frame_alloc();
/*####################################################################################################################*/


/**
 * @brief Libère la mémoire associée à un widget "frame" et à ses sous-structures.
 *
 * Cette fonction est utilisée pour désallouer un widget de type "frame" ainsi que toutes les ressources
 * associées (par exemple, l'espace mémoire utilisé pour ses propriétés internes ou ses sous-widgets).
 * Elle doit être appelée une fois que le widget n'est plus nécessaire pour éviter les fuites de mémoire.
 *
 * @param widget Le widget à libérer.
 */
void frame_release(ei_widget_t widget);
/*####################################################################################################################*/

/**
 * @brief Dessine un widget "frame" sur les surfaces de dessin et de picking.
 *
 * Cette fonction dessine un widget de type "frame" sur la surface principale d'affichage (par exemple, l'écran).
 * Elle prend également en compte la surface de picking, qui est utilisée pour détecter les événements
 * d'interaction utilisateur avec le widget (comme un clic ou un survol). Un rectangle de clipping optionnel
 * peut être fourni pour restreindre la zone de dessin.
 *
 * @param widget       Le widget à dessiner.
 * @param surface      La surface principale pour l'affichage (par exemple, la fenêtre de l'application).
 * @param pick_surface La surface utilisée pour la détection des événements (interactions utilisateur, comme le picking).
 * @param clipper      Rectangle définissant la zone de dessin autorisée, utilisé pour restreindre le dessin si nécessaire.
 */
void frame_draw(ei_widget_t widget,
                ei_surface_t surface,
                ei_surface_t pick_surface,
                ei_rect_t* clipper);
/*####################################################################################################################*/

/**
 * @brief Applique les valeurs par défaut à un widget "frame".
 *
 * Cette fonction initialise les paramètres par défaut pour un widget "frame". Cela peut inclure la couleur,
 * la taille, la position, et d'autres propriétés par défaut qui permettent au widget d'avoir un comportement
 * cohérent même s'il n'est pas explicitement configuré par l'utilisateur.
 *
 * @param widget Le widget auquel appliquer les valeurs par défaut.
 */
void frame_setdefaults(ei_widget_t widget);
/*####################################################################################################################*/


/**
 * @brief Notification de changement de géométrie d'un widget "frame".
 *
 * Lorsque la géométrie d'un widget change (par exemple, sa position ou sa taille), cette fonction
 * est appelée pour notifier le widget du changement. Elle permet de mettre à jour l'apparence du widget
 * pour qu'il s'ajuste aux nouvelles dimensions.
 *
 * @param widget Le widget concerné par le changement de géométrie.
 */
void frame_geonotify(ei_widget_t widget);
/*####################################################################################################################*/

/**
 * @brief Gère les événements envoyés à un widget "frame".
 *
 * Cette fonction est responsable du traitement des événements (comme des clics ou des déplacements de la souris)
 * envoyés à un widget de type "frame". Elle détermine si l'événement doit être géré par ce widget ou non.
 * Elle renvoie true si l'événement a été traité, false sinon.
 *
 * @param widget Le widget concerné par l'événement.
 * @param event  L'événement à traiter (par exemple, un clic, une touche, etc.).
 *
 * @return true si l'événement a été traité, false sinon.
 */
bool frame_handle(ei_widget_t widget,
                  struct ei_event_t* event);
/*####################################################################################################################*/

/**
 * @brief Alloue et initialise une structure ei_widgetclass_t pour le widget de type "frame".
 *
 * Cette fonction crée une nouvelle classe de widget "frame", ce qui permet de définir son comportement
 * (dessin, gestion des événements, etc.) dans l'application. Elle renvoie un pointeur vers la structure
 * `ei_widgetclass_t` qui décrit cette classe.
 *
 * @return Un pointeur vers la structure ei_widgetclass_t initialisée.
 */
ei_widgetclass_t* create_frame_widgetclass();
/*####################################################################################################################*/

#endif // BIB_4_FRAME_H
