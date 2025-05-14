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
    frame_widgetclass->allocfunc=frame_alloc;
    frame_widgetclass->releasefunc=frame_release;
    frame_widgetclass->drawfunc=frame_draw;
    frame_widgetclass->setdefaultsfunc=frame_setdefaults;
    frame_widgetclass->geomnotifyfunc=frame_geonotify;
    frame_widgetclass->handlefunc=frame_handle;
    frame_widgetclass->next=NULL;
    return frame_widgetclass;
}

void ei_app_create(ei_size_t main_window_size, bool fullscreen) {

    hw_init();
    root_surface = hw_create_window(main_window_size, fullscreen);
    hw_surface_lock(root_surface);
    


    // on cree une surface offscreen pour dessiner en mémoire
    ei_size_t surface_size = hw_surface_get_size(root_surface);
    offscreen = hw_surface_create(root_surface, surface_size, false);

    // on enregistre les classes de widgets (ici, "frame" pour le root) 
    ei_widgetclass_register(create_frame_widgetclass());

    // Crée le widget racine (de type "frame"), sans parent
    root_widget = ei_widget_create("frame", NULL, NULL, NULL);
    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface,NULL);
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
}

void ei_app_run(){
    ei_size_t surface_size = hw_surface_get_size(root_surface);
    ei_frame_configure(root_widget,&surface_size,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    frame_draw(root_widget, root_surface, offscreen, NULL);
	printf("tout est affichee\n");
    getchar();
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

void ei_app_free(void){}

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
}


void frame_geonotify(ei_widget_t		widget){}

bool	frame_handle(ei_widget_t		widget,
    struct ei_event_t*	event){
        return false;
    }

void		ei_place	(ei_widget_t		widget,
    ei_anchor_t*		anchor,
    int*			x,
    int*			y,
    int*			width,
    int*			height,
    float*			rel_x,
    float*			rel_y,
    float*			rel_width,
    float*			rel_height){}
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


void frame_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){

    // on revient en type frame :
    ei_impl_frame_t* frame= (ei_impl_frame_t*) widget;


    // gestion des couleurs 
    ei_color_t couleur = *(frame->color);
    ei_color_t *claire = change_color(&couleur, false), *foncee = change_color(&couleur, true);

    // creation des points poour les figures a dessineer
    ei_rect_t zone = {{0,0},{600,600}};
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
        ei_draw_text(surface, surface_localistion(frame->widget.screen_location, width_z, height_z, frame->text_anchor, bordure), texte, *(frame->text_font), *(frame->text_color), clipper);
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
        ei_impl_widget_draw_children(fils_cour, surface, pick_surface, clipper);
        fils_cour=fils_cour->next_sibling;
    }
    
    hw_surface_update_rects(surface,NULL);

}



void ei_impl_widget_draw_children(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
	if (widget != NULL && widget->wclass->drawfunc != NULL) {
		widget->wclass->drawfunc(widget, surface, pick_surface, clipper);
	}
}


// void frame_setdefaults(ei_widget_t widget){
//     //à voir si on fait un static ou un malloc, si on fait un malloc, il faut free
//     //si on fait un static, les valeurs seront les mêmes pour tous les frames
//     ei_impl_frame_t* frame= (ei_impl_frame_t*) widget;
//     static ei_color_t couleur = ei_default_background_color;
//     static int border_width = 0;
//     static relief = ei_relief_none;
//     static ei_font_t font = ei_default_font;
//     static ei_color_t text_color = ei_font_default_color;
//     static ei_anchor_t text_anchor = ei_anc_center;
//     static ei_anchor_t img_anchor = ei_anc_center;

//     frame->color=&couleur;
//     frame->border_width=&border_width;
//     frame->relief=&relief;
//     frame->text=NULL;
//     frame->text_font=&font;
//     frame->text_color=&text_color;
//     frame->text_anchor=&text_anchor;
//     frame->img=NULL;
//     frame->img_rect=NULL;
//     frame->img_anchor=&img_anchor;

// } 
// ei_impl_widget_t widget;// lien vers ei_impl_widget_t
// /*spécificités*/
// ei_size_t* requested_size;//----- 
// const ei_color_t*	color;//----- la couleur de la frame
// int*	border_width;//----- l'écart pour créer le relief
// ei_relief_t* relief;//----- le type de relief 
// ei_string_t text;//----- le texte à ecrire 
// ei_font_t* text_font;//----- le font qui est donné par le programmeur ou hw_text_font_create 
// ei_color_t*	text_color;//
// ei_anchor_t* text_anchor;
// ei_surface_t* img;
// ei_rect_ptr_t img_rect;
// ei_anchor_t*	img_anchor;



