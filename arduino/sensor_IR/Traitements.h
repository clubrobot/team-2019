#ifndef TRAITEMENTS_h
#define TRAITEMENTS_h

#include <Arduino.h>
#define OB 8192

class MoyenneGlissante
{

  public:

    MoyenneGlissante(uint8_t size, uint16_t* echantillon) : _size(size), _echantillon(echantillon){};
    void clear(void);
    void AddElement(uint16_t elt);
    float getAverage(void) const;

  private:
    uint16_t* _echantillon;
    uint8_t _size;
    uint8_t _cnt = 0;
    uint8_t _idx = 0;
    uint8_t _nop = 0;
};


#endif
