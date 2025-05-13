#include "ei_draw.h"
#include "ei_implementation.h"

void supprimer_un_cote(table_de_cotes* TCA, cote* cote)
{
    if (TCA!=NULL && TCA->tete!=NULL)
    {
        cote* cour = TCA->tete;
        if (cour==cote)
        {
            TCA->tete = cour->suivant;
            free(cour);
        }
        while (cour!=NULL && cour->suivant != cote)
        {
            cour = cour->suivant;
        }
        if (cour->suivant->suivant==NULL)
        {
            TCA->queue = cour;
        }
        cour->suivant =cour-> suivant->suivant;
        free(cote);
    }
}

typedef struct{

    uint32_t scan_line;
    table_de_cotes* TC;
    
}liste_de_table_de_cotes;


liste_de_table_de_cotes* creer_TC(ei_point_t* point_array, size_t point_array_size){
    int* indices_tries = construit_tab_y_min(point_array, point_array_size);
    liste_de_table_de_cotes liste= malloc(point_array[indices_tries[point_array_size-2]]-point_array[[]])*sizeof(table_de_cotes))
    while (indices_tries[point_array_size-2]!=null)
    {
        ind
    }

}

table_de_cotes* tri_TCA_en_xymin(table_de_cotes* TCA){

}


void maj_x_ymin(table_de_cotes* TCA){
    
    if (TCA!=null){
        cote* cour=TCA->tete;
        int num = 0, den = 0;
        float frac = 0;
        
        while(cour!=null){
            num = cour->dx;
            den=cour->dy;
            frac=((float)num/den);
            cour->xymin=cour->xymin + frac;
            cour=cour->suivant;
        }
    }
}
#include <math.h>
ei_point_t* arc(float cx, float cy, float rayon, float angle_debut_arc, float angle_fin_arc){
    uint32_t nombre_de_points = (angle_fin_arc-angle_debut_arc)*rayon
    ei_point_t* points = malloc(nombre_de_points * sizeof(ei_point_t));
    float angle_saut = (angle_fin_arc - angle_debut_arc) / (nombre_de_points - 1);
    for (uint32_t i = 0; i < nombre_de_points; i++) {
        float angle = start_angle + i * angle_saut;
        points[i].x = (uint32_t)(cx + rayon * cosf(angle));
        points[i].y = (uint32_t)(cy + rayon * sinf(angle));
    }
    return points;
}
