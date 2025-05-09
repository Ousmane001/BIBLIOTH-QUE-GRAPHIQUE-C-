#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"



/* test_line --
 *
 *	Draws a simple line in the canonical octant, that is, x1>x0 and y1>y0, with
 *	dx > dy. This can be used to test a first implementation of Bresenham
 *	algorithm, for instance.
 */
void test_line(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 255, 255 };
	ei_point_t		pts[2]		= { {200, 200}, {600, 400} };

	ei_draw_polyline(surface, pts, sizeof(pts) / sizeof(ei_point_t), color, clipper);
}

/**
 * @brief fonction qui afficher le clipper du main histoire de bien
 * 			voir les bors de celuici 
 */
void test_rectangle(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 0, 0, 255 };
	ei_point_t		pts[5]		= { {200, 150}, {600, 150}, {600, 450}, {200, 450},  {200, 150}};

	ei_draw_polyline(surface, pts, sizeof(pts) / sizeof(ei_point_t), color, clipper);
}



/* test_octogone --
 *
 *	Draws an octogone in the middle of the screen. This is meant to test the
 *	algorithm that draws a polyline in each of the possible octants, that is,
 *	in each quadrant with dx>dy (canonical) and dy>dx (steep).
 */
void test_octogone(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_point_t		pts[9];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].x		= 400;
	pts[0].y		= 90;

	/* Draw the octogone */
	for(i = 1; i <= 8; i++) {
		 /*	Add or remove 70/140 pixels for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 2 or 1, according to which coordinate grows faster
			The third term is simply the amount of pixels to skip */
		xdiff = (int)(pow(-1, (i + 1) / 4) * pow(2, (i / 2) % 2 == 0) * 70);
		ydiff = (int)(pow(-1, (i - 1) / 4) * pow(2, (i / 2) % 2) * 70);

		pts[i].x	= pts[i-1].x + xdiff;
		pts[i].y	= pts[i-1].y + ydiff;
	}

	/* Draw the shape with a polygon */
	//ei_draw_polygon(surface, pts, sizeof(pts) / sizeof(ei_point_t), (ei_color_t){0, 0, 255, 10}, clipper);

	/* Draw the shape with polylines */
	ei_draw_polyline(surface, pts, sizeof(pts) / sizeof(ei_point_t), (ei_color_t){0, 255, 0, 255}, clipper);
	ei_draw_polygon(surface, pts, sizeof(pts) / sizeof(ei_point_t), (ei_color_t){0, 255, 0, 255}, clipper);
}



/* test_square --
 *
 *	Draws a square in the middle of the screen. This is meant to test the
 *	algorithm for the special cases of horizontal and vertical lines, where
 *	dx or dy are zero
 */
void test_square(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 0, 255 };
	ei_point_t		pts[5];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].x		= 300;
	pts[0].y		= 400;

	/* Draw the square */
	for(i = 1; i <= 4; i++) {
		/*	Add or remove 200 pixels or 0 for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 0 or 1, according to which coordinate grows
			The third term is simply the side of the square */
		xdiff = (int)(pow(-1, i / 2) * (i % 2) * 200);
		ydiff = (int)(pow(-1, i / 2) * (i % 2 == 0) * 200);

		pts[i].x	= pts[i-1].x + xdiff;
		pts[i].y	= pts[i-1].y + ydiff;
	}

	/* Draw the shape with polylines */
	ei_draw_polyline(surface, pts, sizeof(pts) / sizeof(ei_point_t), color, clipper);
	printf("renteemnt\n");
	ei_draw_polygon(surface, pts,sizeof(pts) / sizeof(ei_point_t), color, clipper);
}



/* test_dot --
 *
 *	Draws a dot in the middle of the screen. This is meant to test the special 
 *	case when dx = dy = 0
 */
void test_dot(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 0, 255, 255 };
	ei_point_t		pts[2]		= { {400, 300}, {400, 300} };

	/* Draw the shape with polylines */
	ei_draw_polyline(surface, pts, sizeof(pts) / sizeof(ei_point_t), color, clipper);
}


/*test_polygone 
 * On dessine un polygone et puis on essaye de le colorier
*/
void test_polygone(ei_surface_t surface, ei_rect_t* clipper){
	printf("debut polygone\n");
    ei_color_t color = {255,0,255,255};
	ei_color_t color1 = {0,0,0,255};
    ei_point_t pts[5] ={{10,10},{700,100},{700,500},{10,400},{10,10}};
    // On dessine maintenant le polygone
	printf("avant poline\n");
    ei_draw_polyline(surface,pts,sizeof(pts)/sizeof(ei_point_t),color1,clipper);
	printf("poliline fait\n");
	ei_draw_polygon(surface,pts,sizeof(pts)/sizeof(ei_point_t), color,clipper);
}

/*test_arc
 * j'essaie de dessiner un arc avec polyline
 * 
*/
// void test_arc(ei_surface_t surface, ei_rect_t* clipper){
// 	ei_color_t color = {0,250,0,255};
// 	ei_point_t* point_arc=arc(0,0,0,1.57)
// 	ei_draw_polyline(surface,point_arc,sizeof(pts)/sizeof(ei_point_t),color,NULL);
// }

/*test_drapeau_maroc 
 * On essaye de dessiner le drapeau du maroc
*/

void drapeau_maroc(ei_surface_t surface, ei_rect_t* clipper) {
    // 1. Dessiner le fond rouge
    ei_color_t rouge = { 17, 38, 206, 255};  // Rouge du drapeau marocain
    ei_rect_t drapeau_rect = {{200, 150}, {400, 300}}; // Rectangle du drapeau
    ei_fill(surface, &rouge, &drapeau_rect);

    // 2. Dessiner l'étoile verte
    ei_color_t vert = {0, 128, 0, 255};  // Vert du drapeau marocain
    
    // Points de l'étoile ordonnés (ABCDA)
    ei_point_t etoile[6] = {
        {400, 250},      // A (point de départ)
        {450, 300},      // B
        {400, 350},      // C
        {350, 300},      // D
        {400, 250}       // A (retour au point de départ)
    };

    // Dessiner le contour de l'étoile (ABCDA)
    //ei_draw_polyline(surface, etoile, 5, vert, clipper);

    // Dessiner le remplissage de l'étoile
    //ei_draw_polygon(surface, etoile, 5, vert, clipper);

    // Dessiner les lignes intérieures (diagonales)
    ei_point_t diagonales[6] = {
        {400, 200},      // A vers C
        {300, 400},      // C
        {500, 266},      // B vers D
        {300, 266},      // D
        {500, 400},      // A (pour fermer)
        {400, 200}       // C (dernière diagonale)
    };
	ei_point_t diagonale2[6] = {
    {390, 190},      // A vers C
    {290, 390},      // C
    {490, 256},      // B vers D
    {290, 256},      // D
    {490, 390},      // A (pour fermer)
    {390, 190}       // C (dernière diagonale)
};


    ei_draw_polyline(surface, diagonales, 6, vert, clipper);
    //ei_draw_polyline(surface, diagonale2, 6, vert, clipper);

}
// ei_draw_polygon(surface,pentagram,sizeof(pentagram)/sizeof(ei_point),vert,clipper);
	
// 	};
/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		main_window	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_rect_t*		clipper_ptr	= NULL;
	ei_rect_t		clipper		= ei_rect(ei_point(200, 150), ei_size(400, 300));
	clipper_ptr		= &clipper;
	ei_event_t		event;

	hw_init();
		
	main_window = hw_create_window(win_size, false);
	
	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	ei_fill		(main_window, &white, clipper_ptr);

	/* Draw polylines. */
	//test_line	(main_window, clipper_ptr);
	//test_octogone	(main_window, clipper_ptr);
	 //test_square	(main_window, clipper_ptr);
	//test_dot	(main_window, clipper_ptr);
	// on affiche le clipper pour voir si le clipping est bon : 
	//test_rectangle(main_window, NULL);
	printf("avant de rentrer\n");
	//test_polygone(main_window,clipper_ptr);
	drapeau_maroc(main_window,NULL);

	
	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);
	
	/* Wait for a character on command line. */
	event.type = ei_ev_none;
	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown))
		hw_event_wait_next(&event);

	hw_quit();
	return (EXIT_SUCCESS);
}
