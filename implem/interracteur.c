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
}
	

// #include "ei_draw.h"
// #include <stdio.h>
// #include "ei_implementation.h"
// #include "ei_widget.h"
// #include "ei_widgetclass.h"
// #include "ei_widget_attributes.h"
// #include "ei_widget_configure.h"
// #include "ei_application.h"


ei_widgetclass_t* create_frame_widgetclass(){
    ei_widgetclass_t* frame_widgetclass=malloc(sizeof(ei_widgetclass_t));
    strncpy(frame_widgetclass->name, "frame", sizeof(frame_widgetclass->name) - 1);
    frame_widgetclass->allocfunc=frame_alloc;
    frame_widgetclass->releasefunc=frame_release;
    frame_widgetclass->drawfunc=frame_draw;
    frame_widgetclass->setdefaultsfunc=frame_setdefaults;
    frame_widgetclass->geomnotifyfunc=frame_geonotify;
    frame_widgetclass->handlefunc=frame_handle;
    frame_widgetclass->next=NULL;
    return frame_widgetclass;
}
static widet root_widget;
void ei_app_create(ei_size_t main_window_size,bool fullscreen){
    // Crée une fenêtre principale avec la taille spécifiée et le mode plein écran.
    hw_init();
    ei_widgetclass_register(create_frame_widgetclass());
    ei_widget_create("frame",NULL,NULL,NULL);
    ei_surface_t main_window = hw_create_window(main_window_size, fullscreen);
    ei_size_t offscreen_size = hw_surface_get_size(main_window);
    ei_surface_t off_screen=hw_surface_create(main_window, offscreen_size, false);
    
}

void ei_app_run(){
    getchar();
}


ei_widget_t ei_widget_create(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data,  ei_widget_destructor_t destructor){}

void ei_frame_configure(ei_widget_t widget, ei_size_t* requested_size, const ei_color_t* color, int* border_width, ei_relief_t* relief, ei_string_t* text, ei_font_t* text_font,ei_color_t* text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_ptr_t* img_rect, ei_anchor_t* img_anchor){}

static inline void ei_place_xy(ei_widget_t widget, int x, int y){}

void ei_app_free(void){}

ei_widget_t ei_app_root_widget(){}

void frame_setdefaults(ei_widget_t widget){}

void frame_geonotify(ei_widget_t		widget){}

bool	frame_handle(ei_widget_t		widget,
    struct ei_event_t*	event){}

void			ei_widgetclass_register(ei_widgetclass_t* widgetclass){}

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


ei_widget_t frame_alloc(){
    ei_widget_t frame = malloc(sizeof(ei_impl_frame_t));
    return frame;
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

void frame_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){

    // on revient en type frame :
    ei_impl_frame_t* frame= (ei_impl_frame_t*) widget;


    // gestion des couleurs 
    ei_color_t couleur = widget->pick_color;
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
    while (fils_cour!=NULL){
        ei_impl_widget_draw_children(fils_cour, surface, pick_surface, clipper);
        fils_cour=fils_cour->next_sibling;
    }

    // on delock la surface : 
    hw_surface_unlock(surface);

}

void ei_impl_widget_draw_children(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
    ei_widgetclass_t* wclass = widget->wclass;
    
    

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