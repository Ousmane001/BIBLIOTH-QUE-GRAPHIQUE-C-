/**
 * @file	ei_implementation.h
 *
 * @brief 	Private definitions.
 * 
 */

 #ifndef EI_IMPLEMENTATION_H
 #define EI_IMPLEMENTATION_H
 
 #include "hw_interface.h"
 #include "ei_types.h"
 #include "ei_widget.h"
 
 
typedef struct segment
{
	float ymax;
	float x_ymin;
	float pente;
	struct segment* next;
}segment;

typedef  struct  TC_line_table
{
	segment* head;
	segment* tail;
}TC_line_table;

typedef  struct TC_t
{
	uint32_t scan_line;
	TC_line_table *head_of_TC;
} TC_t;


void draw_point(uint32_t* pixel_ptr, ei_size_t dimension, ei_point_t point, ei_color_t* color);

bool est_dans_clipper(ei_point_t* point, const ei_rect_t* clipper);

TC_t* construire_TC(ei_point_t* point_array, uint32_t point_array_size, int y_min, int y_max);

void supprime_cote_ymax_atteint(TC_line_table* TCA, uint32_t scanline);

TC_line_table* tri_line_table(TC_line_table* table);

void affiche_pixel_scanline(ei_surface_t surface, TC_line_table* TCA, uint32_t scanline, ei_color_t* color, const ei_rect_t* clipper);

void maj_en_x_ymin(TC_line_table* TCA);

void ajouter_segment(TC_t *TC, ei_point_t point1, ei_point_t point2, int y_min);

segment* creer_segment(ei_point_t point1, ei_point_t point2);

/*******************************************************************************************************************************************/



ei_widget_t frame_alloc();

void	frame_release(ei_widget_t	widget);

void	frame_draw(ei_widget_t		widget,
    ei_surface_t		surface,
    ei_surface_t		pick_surface,
    ei_rect_t*		clipper);

void	frame_setdefaults(ei_widget_t		widget);

void	frame_geonotify(ei_widget_t		widget);

bool	frame_handle(ei_widget_t		widget,
    struct ei_event_t*	event);

ei_widgetclass_t* create_frame_widgetclass();

void ei_app_create(ei_size_t main_window_size,bool fullscreen);

ei_widget_t ei_widget_create(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data,  ei_widget_destructor_t destructor);



/*******************************************************************************************************************************************/


  /**
   * @brief Colorie les pixels entre origine et extrémité en respectant l'algorithme de Bresenham
   *  
   * @param origine Point de départ à colorier /origine de la ligne
   * @param extremite Point suivant à colorier / extrémité de la ligne
   * @param color Pointeur vers la couleur avec laquelle on va colorier
   * @param
   *  
   */
 void algo_Bresenham(ei_point_t origine, ei_point_t extremite, ei_color_t* color, uint32_t* pixel_ptr, ei_size_t dimension, const ei_rect_t* clipper);
 
 
 
 
 /**
  * \brief	A structure storing the placement parameters of a widget.
  *		You have to define this structure: no suggestion provided.
  */
 struct ei_impl_placer_params_t;
 
 
 
 /**
  * \brief	Tells the placer to recompute the geometry of a widget.
  *		The widget must have been previsouly placed by a call to \ref ei_place.
  *		Geometry re-computation is necessary for example when the text label of
  *		a widget has changed, and thus the widget "natural" size has changed.
  *
  * @param	widget		The widget which geometry must be re-computed.
  */
 void ei_impl_placer_run(ei_widget_t widget);
 
 
 
 /**
  * \brief	Fields common to all types of widget. Every widget classes specializes this base
  *		class by adding its own fields.
  */
 typedef struct ei_impl_widget_t {
	 ei_widgetclass_t*	wclass;		///< The class of widget of this widget. Avoids the field name "class" which is a keyword in C++.
	 uint32_t		pick_id;	///< Id of this widget in the picking offscreen.
	 ei_color_t		pick_color;	///< pick_id encoded as a color.
	 void*			user_data;	///< Pointer provided by the programmer for private use. May be NULL.
	 ei_widget_destructor_t	destructor;	///< Pointer to the programmer's function to call before destroying this widget. May be NULL.
 
	 /* Widget Hierachy Management */
	 ei_widget_t		parent;		///< Pointer to the parent of this widget.
	 ei_widget_t		children_head;	///< Pointer to the first child of this widget.	Children are chained with the "next_sibling" field.
	 ei_widget_t		children_tail;	///< Pointer to the last child of this widget.
	 ei_widget_t		next_sibling;	///< Pointer to the next child of this widget's parent widget.
 
	 /* Geometry Management */
	 //ei_impl_placer_params_t* placer_params;	///< Pointer to the placer parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
	 ei_size_t		requested_size;	///< See \ref ei_widget_get_requested_size.
	 ei_rect_t		screen_location;///< See \ref ei_widget_get_screen_location.
	 ei_rect_t*		content_rect;	///< See ei_widget_get_content_rect. By defaults, points to the screen_location.
 } ei_impl_widget_t;
 
 
/**
  * \brief	Fields specific to widget frame.
  */
typedef struct ei_impl_frame_t {
  ei_impl_widget_t widget;
  /*spécificités*/
  ei_size_t* requested_size;
  const ei_color_t*	color;
  int*	border_width;
  ei_relief_t* relief;
  ei_string_t* text;
  ei_font_t* text_font;
  ei_color_t*	text_color;
  ei_anchor_t* text_anchor;
  ei_surface_t* img;
  ei_rect_ptr_t* img_rect;
  ei_anchor_t*	img_anchor;
  
  } ei_impl_frame_t;




 
 /**
  * @brief	Draws the children of a widget.
  * 		The children are draw withing the limits of the clipper and
  * 		the widget's content_rect.
  *
  * @param	widget		The widget which children are drawn.
  * @param	surface		A locked surface where to draw the widget's children.
  * @param	pick_surface	The picking offscreen.
  * @param	clipper		If not NULL, the drawing is restricted within this rectangle
  *				(expressed in the surface reference frame).
  */
 void		ei_impl_widget_draw_children	(ei_widget_t		widget,
						  ei_surface_t		surface,
						  ei_surface_t		pick_surface,
						  ei_rect_t*		clipper);
 
 
 
 /**
  * \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
  * 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
  * 		The surface parameter provides the channel order.
  *
  * @param	surface		The surface where to store this pixel, provides the channels order.
  * @param	color		The color to convert.
  *
  * @return 			The 32 bit integer corresponding to the color. The alpha component
  *				of the color is ignored in the case of surfaces that don't have an
  *				alpha channel.
  */
 uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color);
 
 #endif
 