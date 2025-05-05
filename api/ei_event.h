/**
 *  @file	ei_event.h
 *  @brief	Allows the binding and unbinding of callbacks to events.
 *
 *  \author 
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */
#ifndef EI_EVENT_H
#define EI_EVENT_H

#include "ei_types.h"
#include "ei_widget.h"

/**
 * @brief	The types of events.
 */
typedef enum {
	ei_ev_none		= 0,	///< No event, used to initialize the structure.
	ei_ev_app,			///< An application event, created by \ref hw_event_post_app.
	ei_ev_close,			///< The user requested to close the application's window.
	ei_ev_exposed,			///< The application's window has been exposed and should be redrawn.
	ei_ev_keydown,			///< A keyboard key has been pressed.
	ei_ev_keyup,			///< A keyboard key has been released.
	ei_ev_text_input,		///< Some text was input with the keyboard.

	/* event types at or above "ei_ev_mouse_buttondown" are mouse events that require picking. */
	ei_ev_mouse_buttondown,		///< A mouse button has been pressed.
	ei_ev_mouse_buttonup,		///< A mouse button has been released.
	ei_ev_mouse_move,		///< The mouse has moved.
	ei_ev_mouse_wheel,		///< The mouse wheel was rolled.

	ei_ev_last			///< Last event type, its value is the number of event types.
} ei_eventtype_t;

/**
 * @brief	The modifier keys (shift, alt, etc.)
 */
typedef enum {
	ei_mod_shift_left	= 0,	///< The "shift" key at the left of the space bar.
	ei_mod_alt_left,		///< The "alternate" key at the left of the space bar.
	ei_mod_meta_left,		///< The "meta" (command) key at the left of the space bar.
	ei_mod_ctrl_left,		///< The "control" key at the left of the space bar.
	ei_mod_shift_right,		///< The "shift" key at the right of the space bar.
	ei_mod_alt_right,		///< The "alternate" key at the right of the space bar.
	ei_mod_meta_right,		///< The "meta" (command) key at the right of the space bar.
	ei_mod_ctrl_right		///< The "control" key at the right of the space bar.

} ei_modifier_key_t;

/**
 * @brief	A bitfield indicating which of the modifier keys are currently pressed.
 * 		e.g., if the 4th bit is 1, it means that the "control" key on the left side of the keyboard
 * 		was pressed at the time of the event (\ref ei_mod_ctrl_left is the 4th identifier in the
 * 		enumerate \ref ei_modifier_key_t).
 */
typedef uint32_t			ei_modifier_mask_t;

/**
 * @brief	The various mouse buttons.
 */
typedef enum {
	ei_mouse_button_left,		///< The left mouse button.
	ei_mouse_button_middle,		///< The middle mouse button.
	ei_mouse_button_right		///< The right mouse button.
} ei_mouse_button_t;

/**
 * @brief	The event parameter for mouse-related event types.
 */
typedef struct {
	ei_point_t			where;		///< Where the mouse pointer was at the time of the event.
	ei_mouse_button_t 		button;		///< The mouse button that was pressed or released. Only valid for \ref ei_ev_mouse_buttondown or \ref ei_ev_mouse_buttonup event types.
	float 				wheel;		///< The amount of wheel rotation. Only valid for \ref ei_ev_mouse_wheel event types.
} ei_mouse_event_t;

/**
 * @brief	The event parameter for application defined event types.
 */
typedef struct {
	void*				user_param;
} ei_app_event_t;

/**
 * @brief	Describes an event.
 */
typedef struct ei_event_t {
	ei_eventtype_t			type;		///< The type of the event.
	ei_modifier_mask_t 		modifier_mask;	///< which of the modifier keys where pressed at the time of the event.
	union {
		char 			text;		///< Event parameter for \ref ei_ev_text_input events.
		SDL_Keycode		key_code;	///< Event parameters for \ref ei_ev_keydown and \ref ei_ev_keyup event.
		ei_mouse_event_t	mouse;		///< Event parameters for mouse-related	events (see \ref ei_mouse_event_t).
		ei_app_event_t		application;	///< Event parameters for application-related events (see \ref ei_app_event_t).
	} param;
} ei_event_t;

/**
 * @brief	Tests is a modifier key is noted as "pressed" in a modifier mask.
 *
 * @param	modifier_mask	The modifier mask (see \ref ei_modifier_mask_t).
 * @param	modifier	The modifier key (see \ref ei_modifier_key_t).
 *
 * @return			true if this modifier key was pressed, false otherwise.
 */
static inline bool		ei_mask_has_modifier(ei_modifier_mask_t	modifier_mask,
						     ei_modifier_key_t	modifier)
							{ return (bool)(modifier_mask & (1 << modifier)); }

static inline bool		ei_event_has_shift(const ei_event_t* event)
						{ return ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) ||
							 ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right); }
static inline bool		ei_event_has_alt(const ei_event_t* event)
						{ return ei_mask_has_modifier(event->modifier_mask, ei_mod_alt_left) ||
							 ei_mask_has_modifier(event->modifier_mask, ei_mod_alt_right); }
static inline bool		ei_event_has_ctrl(const ei_event_t* event)
						{ return ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left) ||
							 ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_right); }
static inline bool		ei_event_has_meta(const ei_event_t* event)
						{ return ei_mask_has_modifier(event->modifier_mask, ei_mod_meta_left) ||
							 ei_mask_has_modifier(event->modifier_mask, ei_mod_meta_right); }

/**
 * Sets the widget which is currently being manipulated by the user.
 *
 * @param	widget		The widget to declare as active, or NULL to declare
 *				that a widget is no more being manipulated.
 */
void				ei_event_set_active_widget(ei_widget_t widget);

/**
 * Returns the widget currently being manipulated by the user.
 *
 * @return			The widget currently being manipulated, or NULL.
 */
ei_widget_t			ei_event_get_active_widget(void);



/**
 * @brief	A function that is called in response to an event that has not been processed
 *		by any widget.
 *
 * @param	event		The event containing all its parameters (type, etc.)
 */
typedef void			(*ei_default_handle_func_t)(ei_event_t* event);

/**
 * Sets the function that must be called when an event has been received but no processed
 *	by any widget.
 *
 * @param	func		The event handling function.
 */
void				ei_event_set_default_handle_func(ei_default_handle_func_t func);

/**
 * Returns the function that must be called when an event has been received but no processed
 *	by any widget.
 *
 * @return			The address of the event handling function.
 */
ei_default_handle_func_t	ei_event_get_default_handle_func(void);



#endif
