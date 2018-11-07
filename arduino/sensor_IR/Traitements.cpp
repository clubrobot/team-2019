#include "Traitements.h"
#define OOB    8190
#define NB_NOP 3

void MoyenneGlissante::AddElement(uint16_t elt){
  /**
    Ajoute des éléments dans un tableau circulaire (hors OOB)
  */
  if (elt!=OOB){
    _nop = 0;
    if (_cnt<_size) _cnt++;
    _echantillon[_idx] = elt;
    _idx = (_idx + 1)%_size;
  }
  else{
    _nop ++;
  }
}

float MoyenneGlissante::getAverage() const{
  /**
    Calcul la moyenne sur le nombre d'échantillons entré en paramètre
    Les OOB ne sont pas pris en compte dans la moyenne, mais sont indiqués au bout de NB_NOP OOB
  */
  int sum = 0;
  if (_nop >= NB_NOP) return -1.0; // Pour la sensibilité aux "bugs"
  if (_cnt>0){ // Permet d'éviter le délai d'attente pour le _size premiers échantillons
    for (int i=0; i<_size; i++) sum += _echantillon[i];
    return (float)sum/(float)_cnt;
  }
  return (float)sum;
}
