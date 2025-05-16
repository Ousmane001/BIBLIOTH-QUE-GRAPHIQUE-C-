ei_widgetclass_t* ei_widget_get_class(ei_widget_t widget){
    return widget->wclass;
}

const ei_color_t* ei_widget_get_pick_color(ei_widget_t widget){
    return widget->pick_color;
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
    return widget->requested_size;
}



