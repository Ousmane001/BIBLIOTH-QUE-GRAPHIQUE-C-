/**
 *  @file	ei_placer.h
 *  @brief	Manages the positioning and sizing of widgets on the screen.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#ifndef EI_PLACER_H
#define EI_PLACER_H

#include "ei_types.h"
#include "ei_widget.h"



/**
 * \brief	Configures the geometry of a widget using the "placer" geometry manager.
 *
 *		The placer computes a widget's geometry relative to its parent *content_rect*.
 *
 * 		If the widget was already managed by the "placer", then this calls simply updates
 *		the placer parameters: arguments that are not NULL replace previous values.
 *		Arguments that are NULL don't change the current value of the parameter.
 *
 * 		When the arguments are passed as NULL, the placer uses default values (detailed in
 *		the argument descriptions below). If no size is provided (either absolute or
 *		relative), then either the requested size of the widget is used if one was provided,
 *		or the default size is used.
 *
 * @param	widget		The widget to place.
 * @param	anchor		How to anchor the widget to the position defined by the placer
 *				(defaults to ei_anc_northwest).
 * @param	x		The absolute x position of the widget (defaults to 0).
 * @param	y		The absolute y position of the widget (defaults to 0).
 * @param	width		The absolute width for the widget (defaults to the requested width or
 * 				the default width of the widget if rel_width is NULL, or 0 otherwise).
 * @param	height		The absolute height for the widget (defaults to the requested height or
 *				the default height of the widget if rel_height is NULL, or 0 otherwise).
 * @param	rel_x		The relative x position of the widget: 0.0 corresponds to the left
 *				side of the parent, 1.0 to the right side (defaults to 0.0).
 * @param	rel_y		The relative y position of the widget: 0.0 corresponds to the top
 *				side of the parent, 1.0 to the bottom side (defaults to 0.0).
 * @param	rel_width	The relative width of the widget: 0.0 corresponds to a width of 0,
 *				1.0 to the width of the parent (defaults to 0.0).
 * @param	rel_height	The relative height of the widget: 0.0 corresponds to a height of 0,
 *				1.0 to the height of the parent (defaults to 0.0).
 */
void		ei_place	(ei_widget_t		widget,
				 ei_anchor_t*		anchor,
				 int*			x,
				 int*			y,
				 int*			width,
				 int*			height,
				 float*			rel_x,
				 float*			rel_y,
				 float*			rel_width,
				 float*			rel_height);



/**
 * \brief	Tells the placer to remove a widget from the screen and forget about it.
 *		Note: the widget is not destroyed and still exists in memory.
 *
 * @param	widget		The widget to remove from screen.
 */
void		ei_placer_forget(ei_widget_t widget);


static inline void ei_place_xy		(ei_widget_t widget, int x, int y)			{ ei_place(widget, NULL, &x, &y, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_place_anchored_xy	(ei_widget_t widget, ei_anchor_t anchor, int x, int y)	{ ei_place(widget, &anchor, &x, &y, NULL, NULL, NULL, NULL, NULL, NULL); }

#endif
