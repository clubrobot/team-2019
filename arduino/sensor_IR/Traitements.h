#ifndef TRAITEMENTS_h
#define TRAITEMENTS_h

#include <Arduino.h>

/*
  Class permettant la mise en œuvre de la moyenne glissante
*/

class MoyenneGlissante
{

  public:
    MoyenneGlissante() : _size(0), _echantillon(NULL){};
    ~MoyenneGlissante(){delete [] _echantillon;};
    MoyenneGlissante(uint8_t size) : _size(size){_echantillon = new uint16_t[ size ];};
    void reset(uint8_t size){delete [] _echantillon; clear(); _size = size; _echantillon = new uint16_t[ size ];}; // appelle du 'destructeur' puis du constructeur
    void clear(void){_cnt = 0; _idx = 0;};
    void AddElement(uint16_t elt);
    float getAverage(void) const;
    int getNbEchantillonsMG(void){return _size;}

  private:
    uint16_t* _echantillon = NULL;
    uint8_t _size = 0;
    uint8_t _cnt = 0;
    uint8_t _idx = 0;
    uint8_t _nop = 0;
};


#endif
