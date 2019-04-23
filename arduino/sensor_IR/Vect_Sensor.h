#ifndef VECT_SENSOR_h
#define VECT_SENSOR_h

#include <Arduino.h>
#include "../common/initializer_list.h"

using namespace std;


template <class T>
class Vect_Sensor
{
  private:
    int _size;
    T* _list;

    /**
      Fonction utilitaire : Fonction "spéciale" Arduino permettant de faire l'équivalant du split en python
    */

    String getValue(String data, char separator, int index)
    {
        int found = 0;
        int strIndex[] = { 0, -1 };
        int maxIndex = data.length();

        for (int i = 0; i <= maxIndex && found <= index; i++) {
            if (data.charAt(i) == separator || i == maxIndex) {
                found++;
                strIndex[0] = strIndex[1] + 1;
                strIndex[1] = (i == maxIndex) ? i+1 : i;
            }
        }
        return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
    };

    /**
      Fonction utilitaire : Permet de couper des chaines de caractères à partir d'un délimiteur et d'une sring principale
      De plus convertit en entier le résutat de chaque chaine
      @return : new int[32] # 32 étant le max de paramètres!! Ne pas oublier le DELETE !!
    */

    int * split_int(String data, char separator){
        int* token = new int[32]; // max de paramètres = 32
        int i = 0;
        while(getValue(data, separator, i)!=""){
          String a = getValue(data, separator, i);
          token[i] = a.toInt();
          i++;
        }
        return token;
    };

  public:

    /**
      Le destructeur : s'exécute automatiquement à la fin de l'utilisation de l'objet
    */

    ~Vect_Sensor(){delete(_list);};

    /**
      Exemple d'utilisation :
        Vect_Sensor<int> A;
    */

    Vect_Sensor():_list(NULL),_size(0){};

    /**
      Exemple d'utilisation :
        Vect_Sensor<int> A = {3,1,4,2,5};
        Vect_Sensor<int> B = A;
    */

    Vect_Sensor(const Vect_Sensor<T>& A){
        _size = A._size;
        _list = new T[_size];
        for(int i=0; i<_size; i++){
            _list[i] = A._list[i];
        }
    };

    /**
      Exemple d'utilisation :
        int list = {1,2,3,4}
        Vect_Sensor<int> A = Vect_Sensor(list);
    */

    Vect_Sensor(T* list):_list(list),_size((int)(sizeof(list)/sizeof(list[0]))){};

    /**
      Exemple d'utilisation :
        Vect_Sensor<int> A = {3,1,4,2,5};
    */

    Vect_Sensor( initializer_list<T> list ){
        _size = list.size();
        _list = new T[list.size()];
        for(int i = 0; i<_size; i++) _list[i]=*(list.begin()+i);
    };

    /**
      Exemple d'utilisation :
        Vect_Sensor<int> A = {3,1,4,2,5};
        cout<<A[3];
        ==> (int)(2)
    */

    T operator[](int indice){
        return _list[indice];
    };

    /**
      Exemple d'utilisation :
        Vect_Sensor<int> A = {3,1,4,2,5};
        string nbs = "1,3"
        Vect_Sensor<int> B = A[nbs];
        ==> B = {1,2} et A = {3,1,4,2,5}
        Rq : On peut aussi utiliser l'objet A[nbs]
          A[nbs].init(); # Seul le Sensor 1 et 2 seront initialisé (En supposant que Vect_Sensor<MyCapteur> A;)
    */

    Vect_Sensor operator[](String s){
        int * nbs = split_int(s,","); // !! new int[];
        Vect_Sensor Vect_res = operator[](nbs);
        delete(nbs);
        return Vect_res;
    };

    /**
      Exemple d'utilisation :
        Vect_Sensor<int> A = {3,1,4,2,5};
        int nbs = {1,3}
        Vect_Sensor<int> B = A[nbs];
        ==> B = {1,2} et A = {3,1,4,2,5}
    */

    Vect_Sensor operator[](int * nbs){
        int sizeNbs = sizeof(nbs)/sizeof(nbs[0]);
        T* Nlist = new T[sizeNbs];
        for(int i = 0; i < sizeNbs; i++){
            Nlist[i] = _list[nbs[i]];
        }
        return Vect_Sensor(Nlist);
    };

    /**
      Exemple d'utilisation :
        Vect_Sensor<int> A = {3,1,4,2,5};
        Vect_Sensor<int> B = A[{1,3}];
        ==> B = {1,2} et A = {3,1,4,2,5}
    */

    Vect_Sensor operator[](initializer_list<int> ints ){
        T* Nlist = new T[ints.size()];
        for(int i = 0; i < ints.size(); i++){
            Nlist[i] = _list[*(ints.begin()+i)];
        }
        return Vect_Sensor(Nlist);
    };

    /**
      Exemple d'utilisation :
        Vect_Sensor<int> A = {1,2,3};
        Vect_Sensor<int> B = {2,3,4};
        B=A;
        ==> B = {1,2,3} et A = {1,2,3}
      Fait en sorte que B soit égal à A (mais distinct en terme d'emplacement mémoire)
    */

    Vect_Sensor& operator=(const Vect_Sensor<T>& A) {
        this->_size = A._size;
        this->_list = new T[this->_size];
        for(int i=0; i<this->_size; i++){
            this->_list[i] = A._list[i];
        }
        return *this;
    };

    /**
      __________________________________________________________________________________________________________
     | Sensor_IR.h : Applique les fonctions de Sensor_IR.h à tous les Sensors appartenant à l'objet Vect_Sensor |
     ------------------------------------------------------------------------------------------------------------
    */
    void begin(void){for (int i=0;i<_size;i++) _list[i]->begin();};
    void bind(void){for (int i=0;i<_size;i++) _list[i]->bind();};
    void init(void){for (int i=0;i<_size;i++) _list[i]->init();};
    void configureDefault(void){for (int i=0;i<_size;i++) _list[i]->configureDefault();};
    void startContinuous(uint32_t period_ms =0){for (int i=0;i<_size;i++) _list[i]->startContinuous(period_ms);};
    void update(void){for (int i=0;i<_size;i++) _list[i]->update();};
    void setNbEchantillonsMG(int nb){for (int i=0;i<_size;i++) _list[i]->setNbEchantillonsMG(nb);};
    String readRangeContinuousMillimeters(void){ // On ne peut pas revoyer de tab à la rasp (mais on peut un String = char * ?)
      String a = "";
      for (int i=0;i<_size;i++){
        if (_list[i]->getNbEchantillonsMG()>0) a+=String(_list[i]->getAverage());
        else a+=String(_list[i]->readRangeContinuousMillimeters());
        a+=";";
      }
      return a;
    };
    void stopContinuous(void){for (int i=0;i<_size;i++) _list[i]->stopContinuous();};
    String readRangeSingleMillimeters(void){
      String a = "";
      for (int i=0;i<_size;i++){a+=String(_list[i]->readRangeSingleMillimeters()); a+=";";}
      return a;
    };
    void setTimeout(uint16_t timeout){for (int i=0;i<_size;i++) _list[i]->setTimeout(timeout);};
    bool timeoutOccurred(void){
      bool a;
      for (int i=0;i<_size;i++) a = a || _list[i]->timeoutOccurred();
      return a;
    };
    String getAverage(void){
      String a = "";
      for (int i=0;i<_size;i++){a+=String(_list[i]->getAverage()); a+=";";}
      return a;
    };
};

#endif
