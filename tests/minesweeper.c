/*
 *	minesweeper.c
 *	ig
 *
 *	Created by Denis Becker on 12.02.16.
 *	Copyright 2016 Ensimag. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for random seed
#include <string.h> // memset function

#include "ei_application.h"
#include "ei_widget_configure.h"
#include "ei_widget_attributes.h"
#include "ei_utils.h"
#include "ei_event.h"
#include "ei_placer.h"

/* constants */

ei_size_t			g_map_button_size	= {28, 28};
ei_color_t			g_button_bg_color	= {0x88, 0x88, 0x88, 0xff};
ei_color_t			g_toplevel_bg_color	= {0xbb, 0xbb, 0xbb, 0xff};

/* global resources */

static ei_surface_t		g_flag_img;
static ei_surface_t		g_bomb_img;
static ei_surface_t		g_reset_img;
static ei_widget_t 		g_game_window		= NULL;

/* structs & typedefs */

struct map_t;

typedef struct map_pos_t {

	struct map_t*		map;
	ei_point_t		coord;
	bool			has_mine;
	bool			has_flag;
	bool			is_revealed;
	int			nb_mines_around;
	ei_widget_t		widget;
	
} map_cell_t;

typedef enum {
	e_toplevel,
	e_flag_count,
	e_timer,
	e_message,
	e_widget_name_last
} widget_names_t;

typedef struct map_t {

	ei_size_t 		size;
	ei_point_t		offset;
	int			nb_mines;
	map_cell_t*		cells;
	bool			game_over;
	int			nb_revealed;
	int			flag_count;
	double 			start_time;

	ei_widget_t 		widgets[e_widget_name_last];

} map_t;

/* utility functions */

static void create_cell_buttons(map_t* map);
void set_time(map_t *p_map, int time);

static inline map_cell_t* get_cell_at(map_t* map, int x, int y)
{
	return &map->cells[y * map->size.width + x];
}

static inline bool check_bounds(map_t* map, int x, int y)
{
	return x >= 0 && y >= 0 && x < map->size.width && y < map->size.height;
}

static inline bool can_play(map_cell_t* map_pos)
{
	return ((!map_pos->has_flag) && (!map_pos->is_revealed));
}

/* core functions */

/*
 * check_victory: configures the victory message if victory.
 */
static bool check_victory(map_t* map)
{
	bool		victory = (map->nb_revealed + map->nb_mines == map->size.width * map->size.height);

	if (victory) {
		map->game_over	= true;
		map->start_time	= -1.0;
		ei_frame_configure(map->widgets[e_message], NULL,
				   &(ei_color_t){0x33, 0x33, 0x33, 0xff}, NULL, NULL,
				   &(ei_string_t){"You won!"}, NULL,
				   &(ei_color_t){0x55, 0xbb, 0x55, 0xff}, NULL, NULL, NULL, NULL);
	}
	return victory;
}

void update_flag_count(map_t* map)
{
	char		txt[5];
	snprintf(txt, sizeof(txt), "%d", map->flag_count);
	ei_frame_set_text(map->widgets[e_flag_count], txt);
}

void switch_flag(map_cell_t* map_pos)
{
	map_pos->has_flag = !map_pos->has_flag;

	if (map_pos->has_flag) {
		ei_button_set_image(map_pos->widget, g_flag_img);
		map_pos->map->flag_count--;
	} else {
		ei_button_set_image(map_pos->widget, g_reset_img);
		map_pos->map->flag_count++;
	}
	update_flag_count(map_pos->map);
}

/*
 * Reveal one position on the map, don't explore neighbors, don't check
 * if already revealed (must be done by the caller), don't end game if it's a mine.
 */
void reveal_position_no_check(map_cell_t* cell)
{
	char		nb_txt[12]	= " ";	// large size so that the compiler does not complain that we may lose precision.

	cell->is_revealed = true;
	cell->map->nb_revealed++;

	ei_widget_destroy(cell->widget);
	cell->widget = ei_widget_create("frame", cell->map->widgets[e_toplevel], (void*)cell, NULL);

	ei_place_xy(cell->widget, cell->map->offset.x + cell->coord.x * g_map_button_size.width,
		    		  cell->map->offset.y + cell->coord.y * g_map_button_size.height);

	if (cell->has_mine) {
		ei_frame_configure(cell->widget, &g_map_button_size, NULL, NULL,
				   &(ei_relief_t){ei_relief_none}, NULL, NULL, NULL, NULL,
				   &g_bomb_img, NULL, NULL);

	} else if (cell->nb_mines_around > 0) {
		snprintf(nb_txt, sizeof(nb_txt), "%d", cell->nb_mines_around);
		ei_frame_configure(cell->widget, &g_map_button_size, NULL, NULL, NULL,
				   &(ei_string_t){nb_txt}, NULL, NULL, NULL, NULL, NULL, NULL);
	} else
		ei_widget_set_requested_size(cell->widget, g_map_button_size);
}

void defeat(map_t* map)
{
	map->game_over	= true;
	map->start_time	= -1.0;

	for (int i = 0; i < map->size.width * map->size.height; i++)
		if (map->cells[i].has_mine)
			reveal_position_no_check(&map->cells[i]);
	
	ei_frame_configure(map->widgets[e_message], NULL,
			   &(ei_color_t){0x33, 0x33, 0x33, 0xff}, NULL, NULL,
			   &(ei_string_t){"You lost!"}, NULL,
			   &(ei_color_t){0xbb, 0x55, 0x55, 0xff}, NULL, NULL, NULL, NULL);
}

/*
 * Caller must ensure that map_cell isn't already revealed.
 */
void reveal_and_explore(map_cell_t* map_cell)
{
	int			x;
	int			y;
	int			cx	= map_cell->coord.x;
	int			cy	= map_cell->coord.y;
	map_t*			map	= map_cell->map;
	map_cell_t*		next;

	reveal_position_no_check(map_cell);
	if (map_cell->has_mine) {
		defeat(map);
		return;

	} else if (map_cell->nb_mines_around > 0)
		// explosion risk around, stop exploration
		return;

	for (y = cy - 1; y <= cy + 1; y++)
		for (x = cx - 1; x <= cx + 1; x++) {
			next = get_cell_at(map, x, y);
			if ((x != cx || y != cy) && check_bounds(map, x, y)
					&& (!next->is_revealed) && (!next->has_flag))
				reveal_and_explore(next);
		}
}

/*
 * set_mine_in_cell: places a mine in the given cell.
 */
void set_mine_in_cell(map_cell_t* map_cell)
{
	int	x;
	int	y;
	int	cx	= map_cell->coord.x;
	int	cy	= map_cell->coord.y;
	map_t*	map	= map_cell->map;

	map_cell->has_mine = true;

	// update neighbors mine count
	for (y = cy - 1; y <= cy + 1; y++)
		for (x = cx - 1; x <= cx + 1; x++)
			if ((x != cx || y != cy) && check_bounds(map, x, y))
				get_cell_at(map, x, y)->nb_mines_around++;
}

/*
 * place_mines: places the mines on an empty map.
 */
void place_mines(map_t* map)
{
	int i, n;

	for (n = 0; n < map->nb_mines; n++) {
		do {
			i = rand() % (map->size.width * map->size.height);
		} while (map->cells[i].has_mine);

		set_mine_in_cell(&map->cells[i]);
	}
}

void create_mine_map(map_t* map, ei_size_t size, int nb_mines)
{
	int		x, y;
	map_cell_t*	cell;

	memset(map, 0, sizeof(*map));

	map->size		= size;
	map->nb_mines		= nb_mines;
	map->cells		= (map_cell_t*)calloc((size_t)size.width * (size_t)size.height, sizeof(map_cell_t));
	map->game_over		= false;
	map->nb_revealed	= 0;
	map->flag_count		= nb_mines;
	map->start_time		= -1.0;

	for (y = 0, cell = map->cells; y < map->size.height; y++)
		for (x = 0; x < map->size.width; x++, cell++) {
			cell->map	= map;
			cell->coord	= ei_point(x, y);
		}

	place_mines(map);
}

void destroy_mine_map(map_t* map)
{
	free(map->cells);
}

/* event handlers */

void handle_time(map_t* map)
{
	static int	prev_int_time		= -1;
	int 		int_time;

	if (map->start_time == -1.0)
		return;

	int_time = (int)floor(hw_now() - map->start_time);
	if (int_time != prev_int_time) {
		set_time(map, int_time);
		prev_int_time	= int_time;
	}
}

void set_time(map_t* map, int time)
{
	char 		time_str[20];

	snprintf(time_str, sizeof(time_str), "%d", time);
	ei_frame_set_text(map->widgets[e_timer], time_str);
}

void default_handler(ei_event_t* event)
{
	if ( (event->type == ei_ev_close) ||
	    ((event->type == ei_ev_keydown) && (event->param.key_code == SDLK_ESCAPE)))
		ei_app_quit_request();

	else if ((event->type == ei_ev_app) && (event->param.application.user_param == &g_game_window)) {
		if (g_game_window != NULL)
			handle_time((map_t*)ei_widget_get_user_data(g_game_window));
		hw_event_schedule_app(250, &g_game_window);
	}
}

ei_widget_t create_game_window(ei_size_t map_size, int nb_mine);

void restart_button_cb(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
	// set the map size and nb mines from entries

	ei_widget_t	toplevel	= (ei_widget_t)user_param;
	map_t* map	= (map_t*)ei_widget_get_user_data(toplevel);
	ei_size_t 	size		= map->size;
	int 		nb_mines	= map->nb_mines;

	ei_point_t twhere = ei_widget_get_screen_location(toplevel)->top_left;
	ei_widget_destroy(toplevel);

	g_game_window = create_game_window(size, nb_mines);
	ei_place_xy(g_game_window, twhere.x, twhere.y);
}

void cell_button_handler(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
	map_cell_t*		cell = (map_cell_t*)(ei_widget_get_user_data(widget));

	if (cell->map->game_over)
		return;

	if (cell->map->start_time == -1.0)
		cell->map->start_time	= hw_now();

	if (event->param.mouse.button == ei_mouse_button_left) {
		if (can_play(cell)) {
			reveal_and_explore(cell);
			check_victory(cell->map);
		}

	} else if (event->param.mouse.button == ei_mouse_button_right) {
		if (!cell->is_revealed)
			switch_flag(cell);
	}
}

void create_cell_buttons(map_t* map)
{
	int 		x, y;
	map_cell_t*	cell;
	ei_widget_t	button;

	for (y = 0, cell = map->cells; y < map->size.height; y++) {
		for (x = 0; x < map->size.width; x++, cell++) {

			button = ei_widget_create	("button", map->widgets[e_toplevel], (void*)cell, NULL);
			cell->widget = button;

			ei_button_configure		(button, &g_map_button_size,
							    	 &g_button_bg_color,
							     	 &(int){2},
							      	 &(int){0},
							       	 &(ei_relief_t){ei_relief_raised}, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
								 &(ei_callback_t){cell_button_handler}, NULL);
			ei_place_xy			(button, map->offset.x + x * g_map_button_size.width, map->offset.y + y * g_map_button_size.height);
		}
	}
}

void handle_game_window_destruction(ei_widget_t widget)
{
	map_t* map = (map_t*)ei_widget_get_user_data(widget);
	destroy_mine_map(map);
	free(map);
	g_game_window = NULL;
}

/*
 * Create the game window, 'map' must have been created before (with 'create_mine_map').
 */
ei_widget_t create_game_window(ei_size_t map_size, int nb_mine)
{
	int 		margin		= 4;
	const int	k_top_height	= 50;
	ei_size_t	size		= ei_size(0, k_top_height - 2 * margin);
	ei_color_t	light		= {0xcc, 0xcc, 0xcc, 0xcc};
	ei_widget_t*	widget_ptr;
	char		string_chars[10];
	char*		string		= string_chars;

	map_t* map = malloc(sizeof(map_t));
	create_mine_map(map, map_size, nb_mine);

	// toplevel

	ei_widget_t*	toplevel_ptr	= &(map->widgets[e_toplevel]);
	*toplevel_ptr			= ei_widget_create("toplevel", ei_app_root_widget(), map, handle_game_window_destruction);
	ei_toplevel_configure(*toplevel_ptr, &(ei_size_t){1600, 1200}, &g_toplevel_bg_color, &(int){0}, &(ei_string_t){"Minesweeper"}, NULL, &(ei_axis_set_t){ei_axis_none}, NULL);
	ei_place_xy(*toplevel_ptr, 40, 40);

	// flag count label, timer, and message labels

	size.width			= 50;
	int 		cur_x		= margin;
	for (int i = 0; i < 3; i++) {
		if (i < 2)
			snprintf(string_chars, sizeof(string_chars), "%d", i == 0 ? map->nb_mines : 0);
		else
			size.width	= 120;

		widget_ptr		= &(map->widgets[e_flag_count + i]);
		*widget_ptr		= ei_widget_create("frame", *toplevel_ptr, NULL, NULL);
		ei_frame_configure(*widget_ptr, &size, i < 2 ? &light : &g_toplevel_bg_color, &(int){0}, NULL, (i < 2) ? &string : NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		ei_place_xy(*widget_ptr, cur_x, margin);
		cur_x			+= size.width + margin;
	}

	// restart button

	size.width			= 80;
	ei_widget_t button 		= ei_widget_create("button", *toplevel_ptr, NULL, NULL);
	ei_button_configure(button, &size, &g_button_bg_color, &(int){2}, &(int){0}, NULL, &(ei_string_t){"Restart"}, NULL, NULL,
			    NULL, NULL, NULL, NULL, &(ei_callback_t){restart_button_cb}, &(ei_user_param_t){*toplevel_ptr});
	ei_place_xy(button, cur_x, margin);

	// Compute toplevel size

	const ei_rect_t* trect		= ei_widget_get_screen_location(*toplevel_ptr);
	const ei_rect_t* brect		= ei_widget_get_screen_location(button);
	size.width			= brect->top_left.x + brect->size.width + margin - trect->top_left.x + 2*margin;
	int cells_width			= map->size.width * g_map_button_size.width + 4*margin;
	if (cells_width > size.width)
		size.width		= cells_width;
	map->offset			= ei_point((size.width - cells_width)/2 + 2*margin, k_top_height + margin);
	size.height			= map->size.height * g_map_button_size.height + k_top_height + 3*margin;

	ei_toplevel_configure(*toplevel_ptr, &size, NULL, NULL, NULL, NULL, NULL, NULL);

	create_cell_buttons(map);

	return *toplevel_ptr;
}

int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL)); // random seed

	ei_app_create((ei_size_t){ 1200, 800 }, false);
	ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){ 0x52, 0x7f, 0xb4, 0xff });

	if ((g_flag_img = hw_image_load("misc/flag.png", ei_app_root_surface())) == NULL) {
		printf("ERROR: could not load image \"misc/flag.png\"");
		return 1;
	}
	if ((g_bomb_img = hw_image_load("misc/bomb.png", ei_app_root_surface())) == NULL) {
		printf("ERROR: could not load image \"misc/bomb.png\"");
		return 1;
	}
	g_reset_img = NULL;

	g_game_window = create_game_window((ei_size_t){22, 16}, 40);

	ei_event_set_default_handle_func(default_handler);

	hw_event_schedule_app(250, &g_game_window);

	ei_app_run();

	hw_surface_free(g_flag_img);
	hw_surface_free(g_bomb_img);

	ei_app_free();

	return (EXIT_SUCCESS);
}
