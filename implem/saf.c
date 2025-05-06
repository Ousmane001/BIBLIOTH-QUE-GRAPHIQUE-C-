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

table_de_cotes* creer_TC(ei_point_t* point_array, size_t point_array_size){
    int* indices_triées = construit_tab_y_min(point_array, point_array_size);
    liste_de_table_de_cotes liste= malloc()*sizeof(table_de_cotes))
    while (indices_triées!=null)
    {
        ind
    }

}