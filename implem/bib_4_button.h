// Created by Ousmane Diakite on 17/05/2025.
//

#ifndef BIB_4_BUTTON_H
#define BIB_4_BUTTON_H

#include "interracteur.h"

// Déclaration de la fonction qui crée une classe de widget "button"
// Cette classe de widget est utilisée pour gérer les objets de type bouton dans l'interface graphique.
ei_widgetclass_t* create_button_widgetclass();

// Fonction qui permet de définir les valeurs par défaut pour un widget de type bouton.
// Cela permet de garantir qu'un bouton a un comportement initial cohérent.
void button_setdefaults(ei_widget_t widget);

/**
 * @brief Alloue dynamiquement un nouveau widget de type button.
 * 
 * Cette fonction crée un widget bouton vierge et l'initialise avec les valeurs par défaut.
 * Elle renvoie un pointeur vers ce widget.
 * 
 * @return ei_widget_t* Pointeur vers le widget nouvellement alloué.
 */
ei_widget_t button_alloc();

/**
 * @brief Libère la mémoire allouée pour un widget button et ses ressources associées.
 * 
 * Cette fonction permet de désallouer un widget de type bouton et de libérer toutes les
 * ressources qu'il utilise (comme les images, les couleurs, etc.).
 * 
 * @param widget Le widget à désallouer.
 */
void button_release(ei_widget_t widget);

/**
 * @brief Dessine un widget button sur une surface de dessin spécifiée.
 * 
 * Cette fonction prend un widget bouton et le dessine dans la surface donnée. Elle gère également
 * la détection des interactions avec le bouton (via la surface de picking).
 * Elle peut également restreindre la zone de dessin en fonction d'un rectangle de clipping.
 * 
 * @param widget Le widget à dessiner.
 * @param surface La surface sur laquelle dessiner le widget (écran, par exemple).
 * @param pick_surface Surface utilisée pour détecter les interactions utilisateur avec ce widget (associée à des couleurs uniques).
 * @param clip_rect Rectangle de clipping optionnel pour restreindre la zone de dessin.
 */
void button_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);

/**
 * @brief Configure un bouton en initialisant ses paramètres visuels et fonctionnels.
 * 
 * Cette fonction permet de configurer un bouton en lui attribuant des valeurs pour ses différentes
 * propriétés : taille, couleur, relief, texte, image, etc.
 * 
 * @param widget Le widget bouton à configurer.
 * @param requested_size Taille demandée du widget (largeur, hauteur).
 * @param color Couleur de fond du bouton.
 * @param border_width Largeur de la bordure du bouton.
 * @param corner_radius Rayon des coins arrondis du bouton.
 * @param relief Type de relief du bouton (plat, élevé, etc.).
 * @param text Texte à afficher sur le bouton.
 * @param text_font Police du texte à afficher.
 * @param text_color Couleur du texte.
 * @param text_anchor Ancrage du texte par rapport au bouton.
 * @param img Image à afficher sur le bouton (si présente).
 * @param img_rect Rectangle spécifiant la zone de l'image.
 * @param img_anchor Ancrage de l'image.
 * @param callback Fonction de rappel associée à une action sur le bouton.
 * @param user_param Paramètre utilisateur pour la fonction de rappel.
 */
void ei_button_configure(ei_widget_t widget, ei_size_t* requested_size, const ei_color_t* color, int* border_width, int* corner_radius, ei_relief_t* relief, ei_string_t* text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_ptr_t* img_rect, ei_anchor_t* img_anchor, ei_callback_t* callback, ei_user_param_t* user_param);

/**
 * @brief Met à jour la géométrie d'un widget button (placement, taille, etc.).
 * 
 * Cette fonction est appelée lorsque la géométrie d'un widget change (par exemple, la taille ou la position).
 * Elle met à jour l'apparence du bouton pour qu'il reflète les nouveaux paramètres géométriques.
 * 
 * @param widget Le widget à géométrer.
 * @param rect La zone rectangulaire cible pour la géométrie du widget.
 */
void button_geonotify(ei_widget_t widget);

/**
 * @brief Gère les événements internes du widget button.
 * 
 * Cette fonction est responsable de la gestion des événements internes comme les clics
 * ou les survols sur le bouton. Elle doit être appelée lors de l'interaction avec le widget.
 * 
 * @param widget Le widget bouton auquel l'événement est destiné.
 * @param event L'événement à traiter.
 * 
 * @return true si l'événement a été traité, false sinon.
 */
bool button_handle_intern(ei_widget_t widget, struct ei_event_t* event);

#endif // BIB_4_BUTTON_H
