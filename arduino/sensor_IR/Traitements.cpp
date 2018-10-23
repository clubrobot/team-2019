#include "Traitements.h"

void MoyenneGlissante::clear(void){
  _cnt = 0;
  _idx = 0;
}

void MoyenneGlissante::AddElement(uint16_t elt){
  // Ajoute les éléments hors Out of Bound
  if (elt!=8190){
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
  int sum = 0;
  if (_nop >= 3) return -1.0; // 3 pour la sensibilité aux "bugs"
  if (_cnt>0){
    for (int i=0; i<_size; i++) sum += _echantillon[i];
    return (float)sum/(float)_cnt;
  }
  return (float)sum;
}
