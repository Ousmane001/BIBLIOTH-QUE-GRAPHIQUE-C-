//
// Created by diakitao on 5/12/25.
//

#include "interracteur.h"

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
	

// #include "ei_draw.h"
// #include <stdio.h>
// #include "ei_implementation.h"
// #include "ei_widget.h"
// #include "ei_widgetclass.h"
// #include "ei_widget_attributes.h"
// #include "ei_widget_configure.h"
// #include "ei_application.h"

// surface total de l'ecran  et de l'offscreen qu'on va user pour le clipping vers la fin du projet
static ei_surface_t root_surface, offscreen;
// widget racine (frame root)
static ei_widget_t root_widget;

// Liste chaînée globale des classes de widgets
static ei_widgetclass_t* g_widgetclass_list = NULL; //c'est le début de la liste chainee des widgetclass

// dictionnaire de correspondance entre cle et widget
static dictionnaire dicco_app;

// variable qui determine la cle actuel a incrementer pour le suivant
static int key_actuel = 0;

static ei_linked_rect_t* ei_linked_rect_t_list = NULL;



dictionnaire* get_dicco_app(void){
    return &dicco_app;
}


ei_color_t genere_couleur_suivante() {
    ei_color_t color;

    // Extraire R, G, B à partir de l'index
    color.red   = (key_actuel >> 16) & 0xFF;
    color.green = (key_actuel >> 8)  & 0xFF;
    color.blue  = (key_actuel)       & 0xFF;
    color.alpha = 255;

    // Incrémenter pour la prochaine couleur
    key_actuel++;

    return color;
}

void initialise_dicco_app(void){
    dicco_app = creer_dictionnaire();
}

unsigned int hacher(int cle, int capacite) {
    return (unsigned int)cle % capacite;
}

// Créer un dictionnaire vide
dictionnaire creer_dictionnaire() {
    dictionnaire d;
    d.capacite = CAPACITE_INITIALE;
    d.taille = 0;
    d.seaux = calloc(d.capacite, sizeof(widget_element_dict*));
    return d;
}

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

// Ajouter ou mettre à jour une paire clé/widget
// void ajouter(dictionnaire* d, int cle, ei_widget_t widget) {
//     if ((float)d->taille / d->capacite > TAUX_REMPLISSAGE_MAX) {
//         redimensionner(d);
//     }

//     unsigned int indice = hacher(cle, d->capacite);
//     widget_element_dict* courant = d->seaux[indice];
//     while (courant) {
//         if (courant->cle == cle) {
//             courant->widget = widget;
//             return;
//         }
//         courant = courant->suivante;
//     }

//     widget_element_dict* nouvelle_widget_element_dict = malloc(sizeof(widget_element_dict));
//     nouvelle_widget_element_dict->cle = cle;
//     nouvelle_widget_element_dict->widget = widget;
//     nouvelle_widget_element_dict->suivante = d->seaux[indice];
//     d->seaux[indice] = nouvelle_widget_element_dict;
//     d->taille++;
// }

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



void ei_widgetclass_register(ei_widgetclass_t* widgetclass) {
    if (widgetclass == NULL) {
        return;
    }

    // on verifie si une classe avec le même nom est déjà enregistrée
    ei_widgetclass_t* current = g_widgetclass_list;
    while (current != NULL) {
        if (strcmp(current->name, widgetclass->name) == 0) {
            // Une classe avec ce nom est déjà enregistrée, on ne fait rien
            return;
        }
        current = current->next;
    }

    // Insérer la nouvelle classe en tête de liste
    widgetclass->next = g_widgetclass_list;
    g_widgetclass_list = widgetclass;
}

ei_widgetclass_t* create_frame_widgetclass(){

    ei_widgetclass_t* frame_widgetclass = malloc(sizeof(ei_widgetclass_t));
    strcpy(frame_widgetclass->name, "frame");
    frame_widgetclass->allocfunc = frame_alloc;
    frame_widgetclass->releasefunc = frame_release;
    frame_widgetclass->drawfunc = frame_draw;
    frame_widgetclass->setdefaultsfunc = frame_setdefaults;
    frame_widgetclass->geomnotifyfunc = frame_geonotify;
    frame_widgetclass->handlefunc = frame_handle;
    frame_widgetclass->next=NULL;
    return frame_widgetclass;
}

void ei_frame_configure(
    ei_widget_t widget,
    ei_size_t* requested_size,
    const ei_color_t* color,
    int* border_width,
    ei_relief_t* relief,
    ei_string_t* text,
    ei_font_t* text_font,
    ei_color_t* text_color,
    ei_anchor_t* text_anchor,
    ei_surface_t* img,
    ei_rect_ptr_t* img_rect,
    ei_anchor_t* img_anchor)
{
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;



    if (requested_size != NULL) {
        if (frame->requested_size == NULL)
            frame->requested_size = malloc(sizeof(ei_size_t));
        widget->requested_size=*requested_size;
        *(frame->requested_size) = *requested_size;
    }

    if (color != NULL) {
        if (frame->color == NULL)
            frame->color = malloc(sizeof(ei_color_t));
        *(frame->color) = *color;
    }

    if (text != NULL && *text != NULL) {
        if (frame->text != NULL)
            free(frame->text);
        frame->text = strdup(*text);  // on copie la chaîne
    }

    if (border_width != NULL) {
        if (frame->border_width == NULL)
            frame->border_width = malloc(sizeof(int));
        *(frame->border_width) = *border_width;
    }

    if (relief != NULL) {
        if (frame->relief == NULL)
            frame->relief = malloc(sizeof(ei_relief_t));
        *(frame->relief) = *relief;
    }

    if (text_font != NULL) {
        if (frame->text_font == NULL)
            frame->text_font = malloc(sizeof(ei_font_t));
        *(frame->text_font) = *text_font;
    }

    if (text_color != NULL) {
        if (frame->text_color == NULL)
            frame->text_color = malloc(sizeof(ei_color_t));
        *(frame->text_color) = *text_color;
    }

    if (text_anchor != NULL) {
        if (frame->text_anchor == NULL)
            frame->text_anchor = malloc(sizeof(ei_anchor_t));
        *(frame->text_anchor) = *text_anchor;
    }

    if (img != NULL) {
        if (frame->img == NULL)
            frame->img = malloc(sizeof(ei_surface_t));
        *(frame->img) = *img;
    }

    if (img_rect != NULL && *img_rect != NULL) {
        frame->img_rect = malloc(sizeof(ei_rect_t));
        *(frame->img_rect) = **img_rect;
    }

    if (img_anchor != NULL) {
        if (frame->img_anchor == NULL)
            frame->img_anchor = malloc(sizeof(ei_anchor_t));
        *(frame->img_anchor) = *img_anchor;
    }

    // on initialise content reect à screen location
    widget->content_rect = &(widget->screen_location);
}

/**************************************************************************************************************************** */
const ei_rect_t*	ei_widget_get_content_rect	(ei_widget_t		widget){
   
    return widget->content_rect;
}
/****************************************************************************************************************************/
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
/****************************************************************************************************************************/
void ei_app_create(ei_size_t main_window_size, bool fullscreen) {

    hw_init();
    root_surface = hw_create_window(main_window_size, fullscreen);
    hw_surface_lock(root_surface);
    


    // on cree une surface offscreen pour dessiner en mémoire
    ei_size_t surface_size = hw_surface_get_size(root_surface);
    offscreen = hw_surface_create(root_surface, surface_size, false);

    // on enregistre les classes de widgets (ici, "frame" pour le root) 
    ei_widgetclass_register(create_frame_widgetclass());

    // Meme chose pour la classe button
    ei_widgetclass_register(create_button_widgetclass());

    // Crée le widget racine (de type "frame"), sans parent
    root_widget = ei_widget_create("frame", NULL, NULL, NULL);
    ei_frame_configure(root_widget,&main_window_size,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    root_widget->screen_location.top_left=(ei_point_t) {0,0};
    root_widget->screen_location.size=main_window_size;
    root_widget->content_rect = &(root_widget->screen_location);
    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, get_invalidate_rect_list());

    // on initialise le dictionnaire et on rajoute:
    initialise_dicco_app();
}



void ei_app_run(){
    ei_size_t surface_size = hw_surface_get_size(root_surface);
    ei_frame_configure(root_widget,&surface_size,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    frame_draw(root_widget, root_surface, offscreen, NULL);
	// printf("tout est affichee\n");
    // getchar();
    ei_event_t event;
    event.type = ei_ev_none;
	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown)){
        
        switch (event.type)
        {
        case ei_ev_mouse_buttondown:
            ei_widget_t widget_pointee = get_widget_by_pt(event.param.mouse.where.x, event.param.mouse.where.y);
            widget_pointee->wclass->handlefunc(widget_pointee, &event);
            break;

           default:
            break;
        }
	    hw_event_wait_next(&event);

    }
}

ei_widgetclass_t*	ei_widgetclass_from_name	(ei_const_string_t name){
    if (name == NULL) {
        printf("Erreur, classe Null dans ei_widgetclass_from_name\n");
        return NULL;
    }

    // on verifie si une classe avec le même nom est déjà enregistrée
    ei_widgetclass_t* current = g_widgetclass_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            // si on a trouvé une classe qui correspond à ce que le dev veux 
            return current;
        }
        current = current->next;
    }
    return NULL;
}



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




static inline void ei_place_xy(ei_widget_t widget, int x, int y){}

void ei_app_free(void){

    // on libere le dicco de correspondance entre cle et widget : 
    liberer(get_dicco_app());
}

ei_widget_t ei_app_root_widget(){
    return root_widget;
}


void frame_setdefaults(ei_widget_t widget) {
    if (widget == NULL) 
        return;

    // Couleur de fond par défaut
    const ei_color_t color = ei_default_background_color;

    // Bordure
    int border_width = 0;

    // Relief
    ei_relief_t relief = ei_relief_none;

    // Texte
    ei_string_t text = NULL;

    // Police
    ei_font_t text_font = ei_default_font;

    // Couleur du texte
    ei_color_t text_color = ei_font_default_color;

    // Ancrage du texte
    ei_anchor_t text_anchor = ei_anc_center;

    // Image
    ei_surface_t img = NULL;

    // Rectangle de l’image
    ei_rect_ptr_t img_rect = NULL;

    // Ancrage de l’image
    ei_anchor_t img_anchor = ei_anc_center;

    // Appelle la fonction de configuration standard pour tout initialiser
    ei_frame_configure(
        widget,
        NULL,                // on attent le config pour mettre le requested size car on est pas des devin !!!
        &color,
        &border_width,
        &relief,
        &text,
        &text_font,
        &text_color,
        &text_anchor,
        &img,
        &img_rect,
        &img_anchor
    );
    ei_widget_set_content_rect(widget, NULL);
}


void frame_geonotify(ei_widget_t		widget){}

bool	frame_handle(ei_widget_t		widget,
    struct ei_event_t*	event){
        
    }


/*******************************************************************************************************************************************/


// ei_widget_t frame_alloc(){
//     ei_widget_t frame = malloc(sizeof(ei_impl_frame_t));
//     return frame;
// }

ei_widget_t frame_alloc() {
    ei_impl_frame_t* frame = malloc(sizeof(ei_impl_frame_t));
    if (frame == NULL) {
        fprintf(stderr, "Erreur\n");
        return NULL;
    }

    // On nettoie cette benne à ordures : tous les champs à zéro.
    memset(frame, 0, sizeof(ei_impl_frame_t));

    // Retourne ça comme un ei_widget_t parce que visiblement, tout est déguisé ici.
    return (ei_widget_t)frame;
}


//cette fonction prend en parametre un widget et pas un ei_impl_frame_t car elle doit etre "commune" à toutes les classes cou
void frame_release(ei_widget_t widget){
    ei_impl_frame_t* frame = (ei_impl_frame_t* ) widget;
    
    // desallocation de ei_impl_widget_t
    free(frame->widget.wclass);
    free(frame->widget.user_data);
    free(frame->widget.content_rect);
     
    // désallocation des autres structures contenues dans frame: 
    free(frame->requested_size);
    //free(frame->color);
    free(frame->border_width);
    free(frame->relief);
    free(frame->text_font);
    free(frame->text_color);
    free(frame->text_anchor);
    free(frame->img);
    free(frame->img_anchor);
    
    // on desaloue finaleemnt la frame:
    free(frame);
}

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


void frame_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){

    int	i, ir, ig, ib, ia;

    // on revient en type frame :
    ei_impl_frame_t* frame= (ei_impl_frame_t*) widget;


    // gestion des couleurs 
    ei_color_t couleur = reorder_color_channels(*(frame->color),root_surface);
    hw_surface_get_channel_indices(root_surface, &ir, &ig, &ib, &ia);

    // on gernere une couleur unique pour cette frame pour la pick surface :
    ei_color_t couleur_pick = genere_couleur_suivante();
    ajouter(get_dicco_app(), *(uint32_t *)&couleur_pick, widget);
    widget->pick_color = couleur_pick;
    widget->pick_id = *(uint32_t *)&couleur_pick;

    ei_color_t *claire = change_color(&couleur, false), *foncee = change_color(&couleur, true);

    // creation des points poour les figures a dessineer
    ei_rect_t zone = widget->screen_location;
    int bordure = *(frame->border_width);
    ei_point_t carre[5] = {
                            {zone.top_left.x, zone.top_left.y},
                            {zone.top_left.x + zone.size.width, zone.top_left.y},
                            {zone.top_left.x + zone.size.width, zone.top_left.y + zone.size.height},
                            {zone.top_left.x , zone.top_left.y + zone.size.height},
                            {zone.top_left.x, zone.top_left.y}
    };
    ei_point_t triangle[4] = {
                                {zone.top_left.x, zone.top_left.y},
                                {zone.top_left.x + zone.size.width, zone.top_left.y},
                                {zone.top_left.x , zone.top_left.y + zone.size.height},
                                {zone.top_left.x, zone.top_left.y}
    };
    ei_point_t centre[5] = {
        {zone.top_left.x + bordure, zone.top_left.y + bordure},
        {zone.top_left.x + zone.size.width - bordure, zone.top_left.y + bordure},
        {zone.top_left.x + zone.size.width - bordure, zone.top_left.y + zone.size.height - bordure},
        {zone.top_left.x + bordure, zone.top_left.y + zone.size.height - bordure},
        {zone.top_left.x + bordure, zone.top_left.y + bordure}
    };

    // on dessine d'abord  dans l'offscreen de picking
    hw_surface_lock(get_offscreen_picking());
    ei_draw_polygon(get_offscreen_picking(), carre, 5, couleur_pick, clipper);
    hw_surface_unlock(get_offscreen_picking());
    
    // on lock avant tous la surface 
    hw_surface_lock(surface);

    // on dessine la frame en profondeur en fonction du relief
    switch(*(frame->relief)){
        case ei_relief_raised:
            ei_draw_polygon(surface, carre, 5, *foncee, clipper);
            ei_draw_polygon(surface, triangle, 4, *claire, clipper);
            ei_draw_polygon(surface, centre, 5, couleur, clipper);

        break;

        case ei_relief_sunken:
            ei_draw_polygon(surface, carre, 5, *claire, clipper);
            ei_draw_polygon(surface, triangle, 4, *foncee, clipper);
            ei_draw_polygon(surface, centre, 5, couleur, clipper);

        break;

        default:
            ei_draw_polygon(surface, carre, 5, couleur, clipper);
    }


    

    
    ei_const_string_t texte =  frame->text;
    if (texte){
        int width_z = 0, height_z = 0;
        hw_text_compute_size(texte, *(frame->text_font), &width_z, &height_z);
        ei_draw_text(surface, surface_localistion(*(frame->widget.content_rect), width_z, height_z, frame->text_anchor, bordure), texte, *(frame->text_font), reorder_color_channels(*(frame->text_color), surface), widget->content_rect);
    }
    else{
        // ei_const_string_t* filename=(ei_const_string_t*)widget->user_data;
        // ei_surface_t image=hw_image_load(*filename, surface);
        // ei_rect_ptr_t* rect_img=frame->img_rect;
        // ei_anchor_t* img_anchor=frame->img_anchor;
    }
    ei_widget_t fils_cour = widget->children_head;

    // parcours en largeur des fils et dessins respectifs 
    // while (fils_cour!=NULL){
    //     ei_impl_widget_draw_children(fils_cour, surface, pick_surface, clipper);
    //     fils_cour=fils_cour->next_sibling;
    // }

    // on delock la surface : 
    hw_surface_unlock(surface);

    while (fils_cour!=NULL){
        printf("j'affiche un fils dans frame\n");
        ei_impl_widget_draw_children(fils_cour, surface, pick_surface, widget->content_rect);
        fils_cour=fils_cour->next_sibling;
    }
    hw_surface_update_rects(surface, get_invalidate_rect_list());

}


ei_surface_t get_offscreen_picking(void){
    return offscreen;
}



void ei_impl_widget_draw_children(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
	if (widget != NULL && widget->wclass->drawfunc != NULL) {
		widget->wclass->drawfunc(widget, surface, pick_surface, clipper);
	}
}


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


/*******************************************************************************************************************/

void button_setdefaults(ei_widget_t widget) {
    if (widget == NULL) 
        return;

    // Couleur de fond par défaut
    const ei_color_t color = ei_default_background_color;

    // Bordure
    int border_width = k_default_button_border_width;

    // Corner radius
    int corner_radius = k_default_button_corner_radius;

    // Relief
    ei_relief_t relief = ei_relief_raised;

    // Police
    ei_font_t text_font = ei_default_font;

    // Couleur du texte
    ei_color_t text_color = ei_font_default_color;

    // Ancrage du texte
    ei_anchor_t text_anchor = ei_anc_center;

    // Ancrage de l’image
    ei_anchor_t img_anchor = ei_anc_center;

    // Appelle la fonction de configuration standard pour tout initialiser
    ei_button_configure(widget,
                        NULL,
                        &color,
                        &border_width,
                        &corner_radius,
                        &relief,
                        NULL,
                        &text_font,
                        &text_color,
                        &text_anchor,
                        NULL,
                        NULL,
                        &img_anchor,
                        NULL,
                        NULL);

    ei_widget_set_content_rect(widget, NULL);
    
}


ei_widget_t button_alloc() {
    ei_impl_button_t* button = malloc(sizeof(ei_impl_button_t));
    if (button == NULL) {
        fprintf(stderr, "Erreur\n");
        return NULL;
    }

    // On nettoie cette benne à ordures : tous les champs à zéro.
    memset(button, 0, sizeof(ei_impl_button_t));

    // Retourne ça comme un ei_widget_t parce que visiblement, tout est déguisé ici.
    return (ei_widget_t)button;
}

void button_release(ei_widget_t widget){
    ei_impl_button_t* button = (ei_impl_button_t* ) widget;
    
    // desallocation de ei_impl_widget_t
    free(button->widget.wclass);
    free(button->widget.user_data);
    free(button->widget.content_rect);
     
    // désallocation des autres structures contenues dans button: 
    free(button->requested_size);
    //free(button->color);
    free(button->border_width);
    free(button->corner_radius);
    free(button->relief);
    free(button->text_font);
    free(button->text_color);
    free(button->text_anchor);
    free(button->img);
    free(button->img_anchor);
    free(button->callback);
    free(button->user_param);
    
    // on desaloue finaleemnt la button:
    free(button);
}


ei_widgetclass_t* create_button_widgetclass(){

    ei_widgetclass_t* button_widgetclass = malloc(sizeof(ei_widgetclass_t));
    strcpy(button_widgetclass->name, "button");
    button_widgetclass->allocfunc = button_alloc;
    button_widgetclass->releasefunc = button_release;
    button_widgetclass->drawfunc = button_draw;
    button_widgetclass->setdefaultsfunc = button_setdefaults;
    button_widgetclass->geomnotifyfunc = button_geonotify;
    button_widgetclass->handlefunc = button_handle_intern;
    button_widgetclass->next=NULL;
    return button_widgetclass;
}



void button_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){

    // on revient en type frame :
    ei_impl_button_t* button = (ei_impl_button_t*) widget;

    // on gernere une couleur unique pour cette frame pour la pick surface :
    ei_color_t couleur_pick = genere_couleur_suivante();
    ajouter(get_dicco_app(), *(uint32_t *)&couleur_pick, widget);
    widget->pick_color = couleur_pick;
    widget->pick_id = *(uint32_t *)&couleur_pick;


    // gestion des couleurs 
    ei_color_t* couleur = button->color;
    ei_rect_t* cadre = (ei_rect_t*) ei_widget_get_screen_location(widget);

    // on dessine d'abord  dans l'offscreen de picking
    hw_surface_lock(get_offscreen_picking());
    draw_button(get_offscreen_picking(), cadre, *(button->corner_radius), couleur_pick, &couleur_pick, &couleur_pick, clipper);
    hw_surface_unlock(get_offscreen_picking());
    
    // on lock avant tous la surface 
    hw_surface_lock(surface);

    // on dessine la frame en profondeur en fonction du relief
    switch(*(button->relief)){
        case ei_relief_raised:
            draw_button(surface,cadre,*(button->corner_radius),*couleur, change_color(couleur, true), change_color(couleur, false),clipper);
        break;

        case ei_relief_sunken:
        draw_button(surface,cadre,*(button->corner_radius),*couleur, change_color(couleur, false), change_color(couleur, true), clipper);

        break;

        default:
        draw_button(surface,cadre,*(button->corner_radius),*couleur, change_color(couleur, true), change_color(couleur, false),clipper);
    }
    
    // écriture du texte
    
    ei_const_string_t texte =  button->text;
    if (texte){
        int width_z = 0, height_z = 0;
        hw_text_compute_size(texte, *(button->text_font), &width_z, &height_z);
        ei_draw_text(surface, surface_localistion(*(button->widget.content_rect), width_z, height_z, button->text_anchor, *(button->border_width)), texte, *(button->text_font), reorder_color_channels(*(button->text_color), surface), widget->content_rect);
    }
    
    // affichage des images 

    else{
        // ei_const_string_t* filename=(ei_const_string_t*)widget->user_data;
        // ei_surface_t image=hw_image_load(*filename, surface);
        // ei_rect_ptr_t* rect_img=button->img_rect;
        // ei_anchor_t* img_anchor=button->img_anchor;
    }

    // parcours en largeur des fils et dessins respectifs 

    // on delock la surface : 
    hw_surface_unlock(surface);

    
    // parcours en largeur des fils et dessins respectifs 
    ei_widget_t fils_cour = widget->children_head;
    while (fils_cour!=NULL){
        ei_impl_widget_draw_children(fils_cour, surface, pick_surface, widget->content_rect);
        fils_cour=fils_cour->next_sibling;
    }
    
    hw_surface_update_rects(surface, get_invalidate_rect_list());

}

void ei_button_configure(ei_widget_t		widget,
                        ei_size_t*		requested_size,
                        const ei_color_t*	color,
                        int*			border_width,
                        int*			corner_radius,
                        ei_relief_t*		relief,
                        ei_string_t*		text,
                        ei_font_t*		text_font,
                        ei_color_t*		text_color,
                        ei_anchor_t*		text_anchor,
                        ei_surface_t*		img,
                        ei_rect_ptr_t*		img_rect,
                        ei_anchor_t*		img_anchor,
                        ei_callback_t*		callback,
                        ei_user_param_t*	user_param){
    
    ei_impl_button_t* button = (ei_impl_button_t*) widget;
    if (requested_size != NULL) {
        if (button->requested_size == NULL)
            button->requested_size = malloc(sizeof(ei_size_t));
        widget->requested_size=*requested_size;
        *(button->requested_size) = *requested_size;
    }

    if (color != NULL) {
        if (button->color == NULL)
            button->color = malloc(sizeof(ei_color_t));
        *(button->color) = *color;
    }

    if (text != NULL && *text != NULL) {
        if (button->text != NULL)
            free(button->text);
        button->text = strdup(*text);  // on copie la chaîne
    }

    if (border_width != NULL) {
        if (button->border_width == NULL)
            button->border_width = malloc(sizeof(int));
        *(button->border_width) = *border_width;
    }

    if (corner_radius != NULL){
        if (button->corner_radius == NULL)
            button->corner_radius = malloc(sizeof(int));
        *(button->corner_radius) = *corner_radius;
    }

    if (relief != NULL) {
        if (button->relief == NULL)
            button->relief = malloc(sizeof(ei_relief_t));
        *(button->relief) = *relief;
    }

    if (text_font != NULL) {
        if (button->text_font == NULL)
            button->text_font = malloc(sizeof(ei_font_t));
        *(button->text_font) = *text_font;
    }

    if (text_color != NULL) {
        if (button->text_color == NULL)
            button->text_color = malloc(sizeof(ei_color_t));
        *(button->text_color) = *text_color;
    }

    if (text_anchor != NULL) {
        if (button->text_anchor == NULL)
            button->text_anchor = malloc(sizeof(ei_anchor_t));
        *(button->text_anchor) = *text_anchor;
    }

    if (img != NULL) {
        if (button->img == NULL)
            button->img = malloc(sizeof(ei_surface_t));
        *(button->img) = *img;
    }

    if (img_rect != NULL && *img_rect != NULL) {
        button->img_rect = malloc(sizeof(ei_rect_t));
        *(button->img_rect) = **img_rect;
    }

    if (img_anchor != NULL) {
        if (button->img_anchor == NULL)
            button->img_anchor = malloc(sizeof(ei_anchor_t));
        *(button->img_anchor) = *img_anchor;
    }

    if (callback != NULL) {
        if (button->callback == NULL)
            button->callback = malloc(sizeof(ei_callback_t));
        (button->callback) = *callback;
    }

    if (user_param != NULL) {
        if (button->user_param == NULL)
            button->user_param = malloc(sizeof(ei_callback_t));
        *(button->user_param) = *user_param;
    }

    // // on initialise content reect à screen location
    // widget->content_rect = &(widget->screen_location);
}

void button_geonotify(ei_widget_t widget){}


void ei_event_set_default_handle_func(ei_default_handle_func_t func){}

void ei_app_quit_request(void){}

//cette fonction prend en parametre un widget et pas un ei_impl_frame_t car elle doit etre "commune" à toutes les classes cou


/*****************************************************************************************************************************************/

ei_widgetclass_t* ei_widget_get_class(ei_widget_t widget){
    return widget->wclass;
}

const ei_color_t* ei_widget_get_pick_color(ei_widget_t widget){
    return &(widget->pick_color);
}

ei_widget_t ei_widget_get_parent(ei_widget_t widget){
    return widget->parent;
}

ei_widget_t ei_widget_get_first_child(ei_widget_t widget){
    return widget->children_head;
}

ei_widget_t ei_widget_get_last_child(ei_widget_t widget){
    return widget->children_tail;
}

ei_widget_t ei_widget_get_next_sibling(ei_widget_t widget){
    return widget->next_sibling;
}

void* ei_widget_get_user_data(ei_widget_t widget){
    return widget->user_data;
}

const ei_size_t* ei_widget_get_requested_size(ei_widget_t widget){
    return &(widget->requested_size);
}

/*****************************************************************************************************************************************/

ei_relief_t inverse_relief(ei_relief_t relief){


    if (relief == ei_relief_raised){
        return ei_relief_sunken;
    }
    
    else{
        return ei_relief_raised;
    }
}



bool button_handle_intern(ei_widget_t widget, struct ei_event_t* event){
    
    ei_impl_button_t* button = (ei_impl_button_t*) widget;
    // bool etait_dessus = false;

    // if (*(button->relief) != ei_relief_none){
        
    //     // tanque le bouton est maintenue en click bas : 
    //     while (event->type != ei_ev_mouse_buttonup  && event->type != ei_ev_close)
    //     {
    //         ei_rect_t rect_invalidate = {event->param.mouse.where,{1,1}};
    //         if(est_dans_rect(event->param.mouse.where, widget->screen_location)){
                
    //             if(etait_dessus==false){
    //                 ei_button_set_relief(widget, inverse_relief(*(button->relief)));
    //                 ei_app_invalidate_rect(&rect_invalidate);
    //                 etait_dessus = true;
    //             } 
    //         }
    //         else{
    //             if(etait_dessus == true){
    //                 ei_button_set_relief(widget, inverse_relief(*(button->relief)));
    //                 ei_app_invalidate_rect(&rect_invalidate);                
    //             }
    //             etait_dessus = false;
    //         }
    //         draw_invalidate_rect();
    //         hw_event_wait_next(event);
    //     }
        
    // }

    bool update_if = true, update_else = true;
    printf("%d %d %d %d \n", widget->screen_location.top_left.x, widget->screen_location.top_left.y, widget->screen_location.size.width, widget->screen_location.size.height);
    while (event->type != ei_ev_mouse_buttonup && event->type != ei_ev_close)
    {
        if(*(button->relief) != ei_relief_none){
            bool dans_button = true;
                        //button_draw(widget, ei_app_root_surface(), ei_app_root_surface(), NULL);      a supprimer --------------------------------------
            widget->wclass->drawfunc(widget, ei_app_root_surface(), ei_app_root_surface(), NULL);
            if(est_dans_rect(event->param.mouse.where, widget->screen_location)){
                
                if(update_if){
                    ei_button_set_relief(widget, inverse_relief(*(button->relief)));
                }
                update_if = false;
                update_else = true;
            }
            else{
                if(update_else){
                    ei_button_set_relief(widget, inverse_relief(*(button->relief)));
                }
                update_else = false;
                update_if = true;
            }
            

        }
        hw_event_wait_next(event);
    }
    
    

    // appel du call back externe
    button->callback(widget, event ,button->user_param);
}

bool est_dans_rect(ei_point_t point, ei_rect_t rect){
    if(point.x >= rect.top_left.x && point.x <= rect.top_left.x + rect.size.width && point.y >= rect.top_left.y && point.y <= rect.top_left.y + rect.size.height){
        return true;
    }
    return false;
}


// typedef struct ei_linked_rect_t{
//     ei_rect_t rect;
//     struct ei_linked_rect_t* next;
// }ei_linked_rect_t;



void ei_app_invalidate_rect(const ei_rect_t* rect){
    ei_linked_rect_t* a_traiter = malloc(sizeof(ei_linked_rect_t));
    if (a_traiter==NULL){
        fprintf(stderr,"memoire finito, va t'acheter de la ram\n");
        exit(EXIT_FAILURE);
    }
    a_traiter->rect = *rect;
    a_traiter->next = ei_linked_rect_t_list;
    ei_linked_rect_t_list = a_traiter;
}

ei_linked_rect_t* get_invalidate_rect_list(void){
    return ei_linked_rect_t_list;
}

ei_surface_t ei_app_root_surface(void){
    return root_surface;
}

void draw_invalidate_rect(void){

    // on lock la surface avant de tracer
    // hw_surface_lock(ei_app_root_surface());
    // hw_surface_lock(get_offscreen_picking());
     
    //on récupere le 1er rectangle a redessiner
    ei_linked_rect_t* cour = get_invalidate_rect_list();
    if(cour != NULL)
    {
        ei_widget_t widget;
        ei_widgetclass_t* class;
        while(cour!=NULL){
            widget = get_widget_by_pt(cour->rect.top_left.x,cour->rect.top_left.y);
            printf(" invalidate rect pour -> %s\n",widget->wclass->name);

            if(widget->wclass->drawfunc)  
                widget->wclass->drawfunc(widget,ei_app_root_surface(),NULL,&(cour->rect));
            printf(" fin inv rect pour -> %s\n\n\n",widget->wclass->name);
            cour=cour->next;
    }
    }
    // on delock la surface root:
    // hw_surface_unlock(ei_app_root_surface());
    // hw_surface_unlock(get_offscreen_picking());
    // on update les rects : 
    hw_surface_update_rects(ei_app_root_surface(), get_invalidate_rect_list());
    ei_linked_rect_t_list = NULL;
}


ei_widget_t get_widget_by_pt(int x0, int y0){
    ei_size_t dimension = hw_surface_get_size(ei_app_root_surface());
    int indice_surface = y0 * dimension.width + x0;
    hw_surface_lock(get_offscreen_picking());
    uint32_t* pixel_ptr_offscreen = (uint32_t*)hw_surface_get_buffer(get_offscreen_picking());
    hw_surface_unlock(get_offscreen_picking());
    return obtenir(get_dicco_app(), pixel_ptr_offscreen[indice_surface]);
}

