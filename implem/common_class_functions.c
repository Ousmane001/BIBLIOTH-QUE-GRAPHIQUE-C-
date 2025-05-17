//
// Created by Ousmane Diakite on 17/05/2025.
//

#include "interracteur.h"
#include "common_class_function.h"

/*####################################################################################################################*/


ei_point_t* surface_localistion(ei_rect_t screen_location, int width_z, int height_z, ei_anchor_t* encre, int bordure){

	ei_point_t* point = malloc(sizeof(ei_point_t));

	// on recupere les valeurs des zones
	int x0 = screen_location.top_left.x, y0 = screen_location.top_left.y;
	int width_s = screen_location.size.width, height_s = screen_location.size.height;



	// en fonction de l'encre on determine le point
	switch (*encre)
	{
		case ei_anc_center:
			point->x = x0 + (width_s - width_z)/2;
			point->y = y0+(height_s/2)-(height_z/2);
			break;
		case ei_anc_north:
			point->x = x0 + (width_s - width_z)/2;
			point->y = y0 + bordure;
			break;
		case ei_anc_northeast:
			point->x = x0 + width_s - (width_z + bordure);
			point->y = y0 + bordure;
			break;
		case ei_anc_east:
			point->x = point->x = x0 + width_s - (width_z + bordure);
			point->y = y0+(height_s/2)-(height_z/2);
			break;
		case ei_anc_southeast:
			point->x = x0 + width_s - (width_z + bordure);
			point->y = y0 + height_s - bordure - height_z;
			break;
		case ei_anc_south:
			point->x = x0 + (width_s - width_z)/2;
			point->y = y0 + height_s - bordure - height_z;
			break;
		case ei_anc_southwest:
			point->x = x0 + bordure;
			point->y = y0 + height_s - bordure - height_z;
			break;
		case ei_anc_west:
			point->x = x0 + bordure;
			point->y = y0+(height_s/2)-(height_z/2);
			break;
		case ei_anc_northwest:
			point->x = x0 + bordure;
			point->y = y0 + bordure;
			break;

		default:
			point->x = x0 + (width_s - width_z)/2;
			point->y = y0+(height_s/2)-(height_z/2);
			break;
	}
	return point;
}

/*####################################################################################################################*/

ei_color_t genere_couleur_suivante() {
	ei_color_t color;

	// Extraire R, G, B à partir de l'index
	color.red   = (get_key_actuel() >> 16) & 0xFF;
	color.green = (get_key_actuel() >> 8)  & 0xFF;
	color.blue  = (get_key_actuel())       & 0xFF;
	color.alpha = 255;

	// Incrémenter pour la prochaine couleur
	incremente();

	return color;
}

/*####################################################################################################################*/

unsigned int hacher(int cle, int capacite) {
	return (unsigned int)cle % capacite;
}
/*####################################################################################################################*/

// Créer un dictionnaire vide
dictionnaire creer_dictionnaire() {
	dictionnaire d;
	d.capacite = CAPACITE_INITIALE;
	d.taille = 0;
	d.seaux = calloc(d.capacite, sizeof(widget_element_dict*));
	return d;
}

/*####################################################################################################################*/

// Rechercher une widget à partir de la clé
ei_widget_t obtenir(dictionnaire* d, int cle) {
	unsigned int indice = hacher(cle, d->capacite);
	widget_element_dict* courant = d->seaux[indice];
	while (courant) {
		if (courant->cle == cle) {
			return courant->widget;
		}
		courant = courant->suivante;
	}
	return NULL;
}

/*####################################################################################################################*/

// Redimensionner le dictionnaire si nécessaire
void redimensionner(dictionnaire* d) {
	int ancienne_capacite = d->capacite;
	d->capacite *= 2;
	widget_element_dict** nouveaux_seaux = calloc(d->capacite, sizeof(widget_element_dict*));

	for (int i = 0; i < ancienne_capacite; i++) {
		widget_element_dict* courant = d->seaux[i];
		while (courant) {
			widget_element_dict* suivant = courant->suivante;
			unsigned int nouvel_indice = hacher(courant->cle, d->capacite);
			courant->suivante = nouveaux_seaux[nouvel_indice];
			nouveaux_seaux[nouvel_indice] = courant;
			courant = suivant;
		}
	}

	free(d->seaux);
	d->seaux = nouveaux_seaux;
}
/*####################################################################################################################*/

void ajouter(dictionnaire* d, int cle, ei_widget_t widget) {
    if ((float)d->taille / d->capacite > TAUX_REMPLISSAGE_MAX) {
        redimensionner(d);
    }

    // Cas 1 : pick_id == 0 -> insertion d’un nouveau maillon
    if (widget->pick_id == 0) {
        unsigned int indice = hacher(cle, d->capacite);

        widget_element_dict* nouvelle_widget_element_dict = malloc(sizeof(widget_element_dict));
        nouvelle_widget_element_dict->cle = cle;
        nouvelle_widget_element_dict->widget = widget;
        nouvelle_widget_element_dict->suivante = d->seaux[indice];
        d->seaux[indice] = nouvelle_widget_element_dict;

        d->taille++;
        return;
    }

    // Cas 2 : pick_id != 0 -> modifier la clé d’un maillon existant
    unsigned int ancien_indice = hacher(widget->pick_id, d->capacite);
    widget_element_dict* precedent = NULL;
    widget_element_dict* courant = d->seaux[ancien_indice];

    // Recherche du maillon avec la clé pick_id
    while (courant) {
        if (courant->cle == widget->pick_id) {
            // Supprimer le maillon de sa liste actuelle
            if (precedent) {
                precedent->suivante = courant->suivante;
            } else {
                d->seaux[ancien_indice] = courant->suivante;
            }

            // Modifier la clé et le widget
            courant->cle = cle;
            courant->widget = widget;

            // Réinsérer à la bonne position avec le nouvel indice
            unsigned int nouveau_indice = hacher(cle, d->capacite);
            courant->suivante = d->seaux[nouveau_indice];
            d->seaux[nouveau_indice] = courant;

            // Pas de modification de d->taille (pas un ajout)
            return;
        }
        precedent = courant;
        courant = courant->suivante;
    }

    // car pick id n'existe pas :
}

/*####################################################################################################################*/

// Libérer toute la mémoire
void liberer(dictionnaire* d) {
    for (int i = 0; i < d->capacite; i++) {
        widget_element_dict* courant = d->seaux[i];
        while (courant) {
            widget_element_dict* suivant = courant->suivante;
            free(courant);
            courant = suivant;
        }
    }
    free(d->seaux);
}

/*####################################################################################################################*/

const ei_rect_t*	ei_widget_get_content_rect	(ei_widget_t		widget){

	return widget->content_rect;
}

/*####################################################################################################################*/

void	 		ei_widget_set_content_rect	(ei_widget_t		widget, const ei_rect_t*	content_rect){

	if(content_rect)
	{
		printf("setcontentrect -> \n");
		widget->content_rect = &(ei_rect_t){content_rect->top_left, content_rect->size};
	}
	else{
		widget->content_rect = &(widget->screen_location);
	}
}

/*####################################################################################################################*/

ei_widgetclass_t*	ei_widgetclass_from_name	(ei_const_string_t name){
	if (name == NULL) {
		printf("Erreur, classe Null dans ei_widgetclass_from_name\n");
		return NULL;
	}

	// on verifie si une classe avec le même nom est déjà enregistrée
	ei_widgetclass_t* current = get_widget_class_list();
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			// si on a trouvé une classe qui correspond à ce que le dev veux
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/*####################################################################################################################*/

ei_widget_t ei_widget_create(ei_const_string_t class_name,
			     ei_widget_t parent,
			     ei_user_param_t user_data,
			     ei_widget_destructor_t destructor)
{
	// On Cherche la classe de widget à partir de son nom
	ei_widgetclass_t* widget_class = ei_widgetclass_from_name(class_name);
	if (widget_class == NULL) {
		return NULL; // Classe inconnue
	}

	// Alloue une nouvelle instance de widget via allocfunc
	ei_widget_t widget = widget_class->allocfunc();
	if (widget == NULL) {
		return NULL; // Échec de l'allocation
	}

	// Initialise les champs génériques du widget
	widget->wclass = widget_class;
	widget->parent = parent;
	widget->destructor = destructor;
	widget->user_data = user_data;
	widget->pick_id = 0;


	// Ajoute le widget à la liste des enfants du parent (si parent non NULL)
	if (parent != NULL) {
		widget->next_sibling = parent->children_head;
		parent->children_head = widget;
	}

	// Applique les valeurs par défaut de la classe
	if (widget_class->setdefaultsfunc != NULL) {
		widget_class->setdefaultsfunc(widget);
	}


	return widget;
}

/*####################################################################################################################*/


const ei_rect_t* ei_widget_get_screen_location(ei_widget_t widget) {
	if (widget == NULL)
		return NULL;

	// On part de la position locale du widget
	ei_rect_t* screen_location = &(widget->screen_location);

	// Si le widget n’a pas de parent, sa position à l’écran est déjà correcte
	if (widget->parent == NULL) {
		return screen_location;
	}

	// On calcule les coordonnées absolues en sommant les positions des parents
	ei_widget_t parent = widget->parent;

	int abs_x = widget->screen_location.top_left.x;
	int abs_y = widget->screen_location.top_left.y;

	while (parent != NULL) {
		abs_x += parent->screen_location.top_left.x;
		abs_y += parent->screen_location.top_left.y;
		parent = parent->parent;
	}

	// On écrase sauvagement la position avec les coordonnées absolues calculées
	widget->screen_location.top_left.x = abs_x;
	widget->screen_location.top_left.y = abs_y;

	return screen_location;
}

ei_color_t reorder_color_channels(ei_color_t color, ei_surface_t surface) {
	int ir, ig, ib, ia;

	// Récupérer l'ordre des canaux de couleur pour cette machine
	hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);

	// Créer une nouvelle couleur pour stocker le résultat
	ei_color_t reordered_color;

	// En fonction des indices retournés, réorganiser les composants de la couleur
	reordered_color.red = (ir == 0) ? color.red : ((ir == 1) ? color.green : (ir == 2) ? color.blue : color.alpha);
	reordered_color.green = (ig == 0) ? color.red : ((ig == 1) ? color.green : (ig == 2) ? color.blue : color.alpha);
	reordered_color.blue = (ib == 0) ? color.red : ((ib == 1) ? color.green : (ib == 2) ? color.blue : color.alpha);
	reordered_color.alpha = (ia == 0) ? color.red : ((ia == 1) ? color.green : (ia == 2) ? color.blue : color.alpha);

	return reordered_color;
}

/*####################################################################################################################*/

void ei_impl_widget_draw_children(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
	if (widget != NULL && widget->wclass->drawfunc != NULL) {
		widget->wclass->drawfunc(widget, surface, pick_surface, clipper);
	}
}
/*####################################################################################################################*/

void ei_place (ei_widget_t widget,
    ei_anchor_t* anchor,
    int* x,
    int* y,
    int* width,
    int* height,
    float* rel_x,
    float* rel_y,
    float* rel_width,
    float* rel_height){
        // Allouer la structure si nécessaire et on véirifie si un des paramètres est NULL
        // Dans ce cas on lui donne sa valeur par défaut (voir déf dans ei_placer.h)
        if (widget->placer_params == NULL) {
            widget->placer_params = calloc(1, sizeof(ei_impl_placer_params_t));
            ei_impl_placer_params_t* params = (ei_impl_placer_params_t*)widget->placer_params;

            // Valeurs par défaut
            params->anchor = ei_anc_northwest;
            params->x = 0;
            params->y = 0;
            params->width = 0;
            params->height = 0;
            params->rel_x = 0.0f;
            params->rel_y = 0.0f;
            params->rel_width = 0.0f;
            params->rel_height = 0.0f;
        }

        ei_impl_placer_params_t* p = (ei_impl_placer_params_t*)widget->placer_params;

        // Mise à jour des paramètres si fournis
        if (anchor)       p->anchor = *anchor;
        if (x)            p->x = *x;
        if (y)            p->y = *y;
        if (width)        p->width = *width;
        if (height)       p->height = *height;
        if (rel_x)        p->rel_x = *rel_x;
        if (rel_y)        p->rel_y = *rel_y;
        if (rel_width)    p->rel_width = *rel_width;
        if (rel_height)   p->rel_height = *rel_height;

        // Recalcul de la géométrie
        ei_impl_placer_run(widget);
    }

/*####################################################################################################################*/

void ei_impl_placer_run(ei_widget_t widget) {
    if (widget == NULL || widget->placer_params == NULL || widget->parent == NULL)
        return;

    ei_impl_placer_params_t* p = (ei_impl_placer_params_t*)widget->placer_params;
    ei_rect_t parent_rect = widget->parent->screen_location;

    // Calcul des coordonnées absolues (position + position relative)
    int abs_x = parent_rect.top_left.x + (int)(p->rel_x * parent_rect.size.width) + p->x;
    int abs_y = parent_rect.top_left.y + (int)(p->rel_y * parent_rect.size.height) + p->y;

    // Détermination de la taille finale
    int width  = (p->width > 0)  ? p->width  : widget->requested_size.width;
    int height = (p->height > 0) ? p->height : widget->requested_size.height;

    // Ajustement de la position selon l'ancrage
    switch (p->anchor) {
        case ei_anc_center:
            abs_x -= width / 2;
            abs_y -= height / 2;
            break;
        case ei_anc_north:
            abs_x -= width / 2;
            break;
        case ei_anc_northeast:
            abs_x -= width;
            break;
        case ei_anc_east:
            abs_x -= width;
            abs_y -= height / 2;
            break;
        case ei_anc_southeast:
            abs_x -= width;
            abs_y -= height;
            break;
        case ei_anc_south:
            abs_x -= width / 2;
            abs_y -= height;
            break;
        case ei_anc_southwest:
            abs_y -= height;
            break;
        case ei_anc_west:
            abs_y -= height / 2;
            break;
        case ei_anc_northwest:
            // Aucun ajustement
            break;
        case ei_anc_none:
        default:
            // Comportement par défaut identique à northwest
            break;
    }

    // Mise à jour de la géométrie réelle du widget
    widget->screen_location = (ei_rect_t){{abs_x, abs_y}, {width, height}};
    // Il faut fiare ceci aux enfants aussi sinon le placeur va générer des erreurs visuelles
    ei_widget_t enfant = widget->children_head;
    while (enfant != NULL) {
        ei_impl_placer_run(enfant);  // Appel récursif sur chaque enfant
        enfant = enfant->next_sibling;
    }
}
/*####################################################################################################################*/

ei_widgetclass_t* ei_widget_get_class(ei_widget_t widget){
	return widget->wclass;
}
/*####################################################################################################################*/

const ei_color_t* ei_widget_get_pick_color(ei_widget_t widget){
	return &(widget->pick_color);
}
/*####################################################################################################################*/

ei_widget_t ei_widget_get_parent(ei_widget_t widget){
	return widget->parent;
}
/*####################################################################################################################*/

ei_widget_t ei_widget_get_first_child(ei_widget_t widget){
	return widget->children_head;
}
/*####################################################################################################################*/

ei_widget_t ei_widget_get_last_child(ei_widget_t widget){
	return widget->children_tail;
}
/*####################################################################################################################*/

ei_widget_t ei_widget_get_next_sibling(ei_widget_t widget){
	return widget->next_sibling;
}
/*####################################################################################################################*/

void* ei_widget_get_user_data(ei_widget_t widget){
	return widget->user_data;
}
/*####################################################################################################################*/

const ei_size_t* ei_widget_get_requested_size(ei_widget_t widget){
	return &(widget->requested_size);
}
/*####################################################################################################################*/

ei_relief_t inverse_relief(ei_relief_t relief){


	if (relief == ei_relief_raised){
		return ei_relief_sunken;
	}

	else{
		return ei_relief_raised;
	}
}
/*####################################################################################################################*/

bool est_dans_rect(ei_point_t point, ei_rect_t rect){
	if(point.x >= rect.top_left.x && point.x <= rect.top_left.x + rect.size.width && point.y >= rect.top_left.y && point.y <= rect.top_left.y + rect.size.height){
		return true;
	}
	return false;
}

/*####################################################################################################################*/
