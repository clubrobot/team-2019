#ifndef TRAITEMENTS_h
#define TRAITEMENTS_h

#include <Arduino.h>
#define OB 8190

class MoyenneGlissante
{

  public:
    MoyenneGlissante() : _size(0), _echantillon(NULL){};
    MoyenneGlissante(uint8_t size, uint16_t* echantillon) : _size(size), _echantillon(echantillon){};
    void reset(uint8_t size, uint16_t* echantillon){delete [] _echantillon; clear(); _size = size; _echantillon = echantillon;}; // appelle du 'destructeur' puis du constructeur
    void clear(void);
    void AddElement(uint16_t elt);
    float getAverage(void) const;

  private:
    uint16_t* _echantillon = NULL;
    uint8_t _size = 0;
    uint8_t _cnt = 0;
    uint8_t _idx = 0;
    uint8_t _nop = 0;
};


#endif
