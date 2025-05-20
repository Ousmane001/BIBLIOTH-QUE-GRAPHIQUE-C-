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

#include <math.h>

#define PI 3.14159265358979323846

#define NB_PTS_CADRE_ARRONDI 364
#define NB_PTS_CADRAN 90

#define DEG_TO_RAD(angle_deg) ((angle_deg) * M_PI / 180.0)
#define PI_SUR_4 0.785398163397448
#define PI_SUR_2 1.570796326794897
#define PI_PLUS_QUART 3.926990816987241
#define MOINS_PI_SUR 4.712388980384690

#define FONCEE 0.7
#define CLAIRE 1.3


// Constantes pour les bits du code cohen
#define CODE_DESSOUS 1   // bit 0
#define CODE_DESSUS  2   // bit 1
#define CODE_DROITE  4   // bit 2
#define CODE_GAUCHE  8   // bit 3





 typedef struct cote
 {
	 float ymax;
	 float x_ymin;
	 float dx;
	 float dy;
 
	 struct cote* suivant;
 }cote;
 
 typedef struct 
 {
	 cote* tete;
	 cote* queue;
 }table_de_cotes;
 
 
 
 typedef struct{
 
	 uint32_t scan_line;
	 table_de_cotes* tete_ligne_tc;
	 
 }liste_de_table_de_cotes;
 /** @brief colorie le point avec la couleur color
  * @param pixel_ptr pointeur vers les pixels de l'Ã©cran
  * @param dimension dimension de la fenÃªtre ,pour calculer la position du point
  * @param point point Ã  colorier
  * @param color couleur choisie
  */
 void draw_point(uint32_t* pixel_ptr, ei_size_t dimension, ei_point_t point, ei_color_t* color);

/** @brief retoune un boolÃ©en selon si point est Ã  l'intÃ©rieur du clipper ou pas
 * @param point point pour lequel on veut vÃ©rifier la position
 * @param clipper rectangle de clipping
 */
 bool est_dans_clipper(ei_point_t* point, const ei_rect_t* clipper);
 /*FONCTION NON UTILISEE*/
 uint32_t cherche_min(ei_point_t** addr_coorddonnee_y, size_t point_array_size);

 /*FONCTION NON UTILISEE*/
 bool est_alignee_horizontal(ei_point_t* point_min, ei_point_t* adjacent);
  /*FONCTION NON UTILISEE*/
 int cherche_xymin(ei_point_t* origine, ei_point_t* extremite);
  /*FONCTION NON UTILISEE*/
 void ajouter_un_cote(table_de_cotes* table, ei_point_t* point1, ei_point_t* point2);
  /*FONCTION NON UTILISEE*/
 void supprimer_un_cote(table_de_cotes* table, cote* cote);
  /*FONCTION NON UTILISEE*/
 table_de_cotes* creer_ligne_TC(ei_point_t** point_array, size_t point_array_size, uint32_t indice_elem);
  /*FONCTION NON UTILISEE*/
 uint32_t* construit_tab_y_min(ei_point_t* point_array, size_t point_array_size);
  /*FONCTION NON UTILISEE*/
 liste_de_table_de_cotes* creer_TC(ei_point_t* point_array, size_t point_array_size, uint32_t* taille_tc);
  /*FONCTION NON UTILISEE*/
 cote* couper_le_min(table_de_cotes* TCA, cote* cellule_min_prec, bool debut);
  /*FONCTION NON UTILISEE*/
 table_de_cotes* tri_TCA_en_xymin(table_de_cotes* TCA);
  /*FONCTION NON UTILISEE*/
 void maj_x_ymin(table_de_cotes* TCA);
  /*FONCTION NON UTILISEE*/
 void colorie_segment(uint32_t* pixel_ptr, ei_color_t* color, uint32_t debut, uint32_t fin);
  /*FONCTION NON UTILISEE*/
 void afficher_x_ymin(const table_de_cotes* table);
 //void afficher_TC(liste_de_table_de_cotes* TC, uint32_t taille_tc);
  /*FONCTION NON UTILISEE*/
 void ajouter_un_cote_deb(table_de_cotes* table, ei_point_t* point1, ei_point_t* point2);
  /*FONCTION NON UTILISEE*/
 void supprimer_cotes_ymax_sup_ou_egal(table_de_cotes* table, int max);
 /// @brief dessine un bouton
 /// @param surface surface sur laquelle il faut dessiner le bouton
 /// @param cadre le cadre du bouton
 /// @param rayon rayon de courbure du bouton pour donner un effet de relief au bouton
 /// @param couleur couleur principale du bouton
 /// @param clipper le bouton est dans ce rectangle ou pas
 /// @param cliquee bool qui intervertit les couleurs du bouton pour lui donner cet effet de cliquÃ© ou pas
 void draw_button(ei_surface_t surface, ei_rect_t* cadre, uint32_t rayon, uint32_t bordure, ei_color_t couleur, ei_color_t* couleur_fonce, ei_color_t* couleur_claire, const ei_rect_t* clipper);

// buttons et arcs :
typedef enum
{
	HAUT,
	BAS,
	COMPLET
}partie_button;


/// @brief CrÃ©e un pointeur qui pointe vers un ensemble de points qui permettent de dessiner un arc
/// @param arc On donne directement le pointeur vers le tableau contenant les points de l'arc
/// @param nb_points npmbre de points qu'on veut colorier ,moins on en donne moins l'arc sera smooth
/// @param x0 abscisse du centre de l'arc
/// @param y0 ordonnÃ©e du centre de l'arc
/// @param rayon rayon de l'arc
/// @param angle_debut angle de dÃ©but "sens trigo"
/// @param angle_fin angle de fin "sens trigo"
void draw_arc(ei_point_t* arc, uint32_t nb_points, int x0, int y0, uint32_t rayon, double angle_debut, double angle_fin);

/// @brief CrÃ©e un pointeur qui pointe vers un ensemble de points qui permettent de dessiner le frame du bouton "un rectangle avec les coins arrondis"
/// @param pts Le pointeur en question
/// @param cadre le rectangle qu'on veut arrondir les coins
/// @param rayon rayon des arc aux coins(il y' a certainemennt un rayon qui soit parfait et qui donne la meilleure allure au bouton
/// @param partie structure qui dÃ©cide si on veut que le bas, le haut ou les deux "plus beau que faire des requÃªtes conditionnelles avec des ou et des and"
void rounded_frame(ei_point_t* pts , ei_rect_t* cadre, uint32_t rayon, partie_button partie);
/// @brief fonction qui retourne la couleur des reliefs d'un bouton soit plus foncee soit plus claire
/// @param base couleur de base
/// @param foncee bool qui dÃ©cide si on veut une couleur foncee ou pas
/// @return retourne la couleur
ei_color_t* change_color(ei_color_t* base, bool foncee);


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

/*FONCTION NON UTILSEE*/
void affiche_TCA(TC_line_table* TCA, uint32_t scanline_courante);
/*FONCTION NON UTILSEE*/
void affiche_TC(TC_t* TC, uint32_t taille_TC);
// prototypes des fonctions utilisÃ©s pour le dessin d'un polygone

/// @brief  Construit la table de cÃ´tÃ©s
/// @param point_array pointeur vers un tableau des sommets de notre cher polygone
/// @param point_array_size le nombre de sommets
/// @param y_min c'est la plus petite ordonnÃ©e des sommets du polygone "on veut travailler qu'entre y_min et y_max"
/// @param y_max c'est la plus grande ordonnÃ©e des sommets du polygone
/// @return la table de cÃ´tÃ©s
TC_t* construire_TC(ei_point_t* point_array, uint32_t point_array_size, int y_min, int y_max);

/// @brief supprime le cote dÃ¨s qu'on atteint son y_max
/// @param TCA table de cÃ´tÃ©s actifs
/// @param scanline ligne surlaquelle on travaille
void supprime_cote_ymax_atteint(TC_line_table* TCA, uint32_t scanline);

/// @brief Trie une ligne de TC selon x_ymin
/// @param table ligne TC
/// @return retourne la ligne de TC triÃ©e
TC_line_table* tri_line_table(TC_line_table* table);

/// @brief Colorie les pixels qui se trouvent dans nos scanlines entre 2 cÃ´tÃ©s adjacents
/// @param surface surface sur laquelle on dessine
/// @param TCA table de cÃ´tÃ©s actifs
/// @param scanline scanline qu'on va colorier
/// @param color couleur choisier---
/// @param clipper colorie uniquement quand le pixel appartient au clipper
void affiche_pixel_scanline(ei_surface_t surface, TC_line_table* TCA, uint32_t scanline, ei_color_t* color, const ei_rect_t* clipper);

/// @brief met Ã  jour les x__ymin dans TCA
/// @param TCA TCA qu'on veut mettre Ã  jour
void maj_en_x_ymin(TC_line_table* TCA);
/*FONCTION NON UTILSEE*/
uint32_t* tri_point_en_y(ei_point_t* point_array, uint32_t taille);
/*FONCTION NON UTILSEE*/
int compare_coord_y(const void* a, const void* b, void* point_array);

/// @brief Ajoute un segment Ã  une ligne dans une TC
/// @param TC la TC en question
/// @param point1 extremitÃ© du segment
/// @param point2 extremitÃ© du segment
/// @param y_min ordonnÃ©e minimale du segment
void ajouter_segment(TC_t *TC, ei_point_t point1, ei_point_t point2, int y_min);

/*FONCTION NON UTILSEE*/
TC_t cree_scanline_TC(TC_t* TC, ei_point_t* point_array, uint32_t taille_TC, uint32_t indice_permut);

/// @brief crÃ©e un segment qu'on veut ajouter dans la ligne de TC
/// @param point1 extremitÃ© du segment
/// @param point2 extremitÃ© du segment
/// @return retourne une variable de type segment qu'on veut ajouter Ã  notre ligne de TC "voir struc segment"
segment* creer_segment(ei_point_t point1, ei_point_t point2);

/*******************************************************************************************************************************************/


void	ei_draw_img(ei_surface_t surface, ei_surface_t img_surf, const ei_rect_t* rect_img, ei_point_t* where, ei_rect_t* clipper);


/*******************************************************************************************************************************************/


  /**
   * @brief Colorie les pixels entre origine et extrÃ©mitÃ© en respectant l'algorithme de Bresenham
   *
   * @param origine Point de dÃ©part Ã  colorier /origine de la ligne
   * @param extremite Point suivant Ã  colorier / extrÃ©mitÃ© de la ligne
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

typedef struct ei_impl_placer_params_t {
  ei_anchor_t   anchor;
  int           x;
  int           y;
  int           width;
  int           height;
  float         rel_x;
  float         rel_y;
  float         rel_width;
  float         rel_height;
} ei_impl_placer_params_t;

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
	 ei_impl_placer_params_t* placer_params;	///< Pointer to the placer parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
	 ei_size_t		requested_size;	///< See \ref ei_widget_get_requested_size.
	 ei_rect_t		screen_location;///< See \ref ei_widget_get_screen_location.
	 ei_rect_t*		content_rect;	///< See ei_widget_get_content_rect. By defaults, points to the screen_location.
 } ei_impl_widget_t;


/**
  * \brief	Fields specific to widget frame.
  */
typedef struct ei_impl_frame_t {
  ei_impl_widget_t widget;// lien vers ei_impl_widget_t

  /*spÃ©cificitÃ©s communs au button et Ã  la frame */
  ei_size_t* requested_size;//-----
  ei_color_t*	color;//----- la couleur de la frame
  int*	border_width;//----- l'Ã©cart pour crÃ©er le relief
  ei_relief_t* relief;//----- le type de relief
  ei_string_t text;//----- le texte Ã  ecrire
  ei_font_t* text_font;//----- le font qui est donnÃ© par le programmeur ou hw_text_font_create
  ei_color_t*	text_color;//
  ei_anchor_t* text_anchor;
  ei_surface_t* img;
  ei_rect_ptr_t img_rect;
  ei_anchor_t*	img_anchor;

}ei_impl_frame_t ;


/**
  * \brief	Fields specific to widget button.
  */
typedef struct ei_impl_button_t {
  ei_impl_widget_t widget;// lien vers ei_impl_widget_t

  /*spÃ©cificitÃ©s communs au button et Ã  la frame */
  ei_size_t* requested_size;//-----
  ei_color_t*	color;//----- la couleur de la frame
  int*	border_width;//----- l'Ã©cart pour crÃ©er le relief
  int* corner_radius;
  ei_relief_t* relief;//----- le type de relief
  ei_string_t text;//----- le texte Ã  ecrire
  ei_font_t* text_font;//----- le font qui est donnÃ© par le programmeur ou hw_text_font_create
  ei_color_t*	text_color;//
  ei_anchor_t* text_anchor;
  ei_surface_t* img;
  ei_rect_ptr_t img_rect;
  ei_anchor_t*	img_anchor;
	ei_callback_t		callback;
	ei_user_param_t*	user_param;
	bool is_pressed;
}ei_impl_button_t ;

typedef struct ei_impl_toplevel_t {
  ei_impl_widget_t widget;// lien vers ei_impl_widget_t

  /*spÃ©cificitÃ©s communs au button et Ã  la frame */
  ei_size_t* requested_size;
  ei_color_t*	color;//Ã  revoir !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  int* border_width;
  ei_string_t title;
  bool* closable;
  ei_axis_set_t* resizable;
  ei_size_ptr_t min_size;
  ei_font_t title_font;
  ei_color_t title_color;
} ei_impl_toplevel_t;

 
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
  *				of the color _is ignored in the case of surfaces that don't have an
  *				alpha channel.
  */
 uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color);

 void ei_fill_optim(ei_surface_t surface, ei_color_t* couleur, ei_rect_t* clipper1, ei_rect_t* clipper2);

 ei_rect_t* ei_rect_intersection(ei_rect_t* rect1, ei_rect_t* rect2);

int cohen_code(ei_point_t point, const ei_rect_t* clipper);
bool clip_segment(ei_point_t* origine, ei_point_t* extremite, const ei_rect_t* clipper);


void algo_Bresenham_analytique(ei_point_t origine, ei_point_t extremite, ei_color_t* color, uint32_t* pixel_ptr, ei_size_t dimension, const ei_rect_t* clipper);
void	ei_draw_polyline_analytique	(ei_surface_t	surface, ei_point_t*	point_array, size_t point_array_size, ei_color_t color, const ei_rect_t*	clipper);










ei_relief_t inverse_relief(ei_relief_t relief);

 #endif
 

 