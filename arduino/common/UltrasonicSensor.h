#ifndef __ULTRASONICSENSOR_H__
#define __ULTRASONICSENSOR_H__
/** class TrigHandler
 *  \brief Gère le pin d'émission du capteur
 *  
 *  Cette classe est uniquement utilisée par la classe UltrasonicSensor et sert à gérer les émissions d'impulsion
 *  Par conséquent, il n'y a pas d'objet TrigHandler dans le .ino
 *  
 *  Il existe un objet TrigHandler pour chacun des pins utilisables par l'arduino nano (Numérique de 1 à 9)
 *  L'attribution s'effectue au moment dans la fonction attach() d'UltrasonicSensor.
 *
 *  Cette classe contrôle que le capteur soit disponible avant d'emettre une nouvelle impulsion
 * 
 *  Si plusieurs capteurs sont connectés sur le même pin de trig, ils ont alors le même trighandler.
 *  Pour que le trig handler soit de nouveau prêt, tous les capteurs doivent avoir fini leur mesure.
 *  
 */
class TrigHandler {

  private :
   
   int m_enable;
   int m_nReady;
   int m_trigP;
   bool m_ready;
   

  public : 
    
  TrigHandler(int Trig);
  //! Incrémente le nombre de capteur sur un trigHandler et met les pins de l'arduino dans en Output
  void enabler();
  
  //! Incrémente le nombre de capteur prêt, si ce nombre est égal au nombre de capteur le trighandler devient prêt
  void Ready();

  //! Renvoie l'état du trigHandler
  /*!
    \return 1 si Ready 0 sinon
  */
  bool getReady();

  //! Envoie la commande de l'impulsion de l'emetteur du capteur
  void trig();

  //! Enlève un capteur compteur
  void disabler();

};
/** class EchoHandler
 *  \brief Gère le pin de reception du capteur
 *  
 *  Cette classe est uniquement utilisée par la classe UltrasonicSensor et sert à gérer les émissions d'impulsion
 *  Par conséquent, il n'y a pas d'objet EchoHandler dans le .ino
 *  
 *  Il existe un objet EchoHandler pour chacun des pins utilisables par l'arduino nano (Numérique 2 et 3)
 *  L'attribution s'effectue au moment dans la fonction attach() d'UltrasonicSensor.
 *  
 *  Il est déconseillé d'utiliser deux capteurs sur le même EchoHandler
 */
class EchoHandler {

  private :

    int m_echoP;
    unsigned long m_startTime;
    unsigned long m_endTime;
    bool m_flag;
    static void echoInterrupt_2();
    static void echoInterrupt_3(); 
    int m_enable; 

  public :

    EchoHandler(int echo);
    //! Renvoie l'heure (depuis le démarrage du programme) de reception de l'impulsion en microsecondes 
    /*!
      \return heure microsecondes
    */
    unsigned long getEndTime();
    //! Renvoie l'heure (depuis le démarrage du programme) d'emission de l'impulsion en microsecondes 
     /*!
      \return heure microsecondes
    */
    unsigned long getStartTime();
    //! Renvoie l'état du flag qui indique si une mesure est en cours ou non
    /*!
      \return 0 si le capteur attend une mesure 1 sinon
    */
    bool getflag();
    //! Renvoie la pin de l'arduino du EchoHandler
    /*!
      \return pin 
    */
    int getPin();
    //! Incrémente le nombre de capteur sur un trigHandler et met les pins de l'arduino dans en Input attache aussi les interruptions
    void enabler();
    //!Change l'état du flag
    /*!
      \param b etat
    */ 
    void setflag(bool b);
};


/** class UltrasonicSensor
 *  \brief Permet d'utiliser des capteurs de distance ulrasons
 *
 *  est un outil gérant l'émission/reception d'un signal ultrason(non codé) 
 *  depuis un capteur ultrason (ceux utilisés étaient des HC-SR04). 
 *  Il calcule également la distance qui sépare le capteur de l'objet en mm. 
 * 
 * Fonctionnement : le capteur emet une impulsion ultrasonore puis passe en mode reception.
 *                  L'impulsion "rebondit" sur l'obstacle et revient vers le capteur. 
 *                  On reçoit alors un signal haut sur la pin echo de du capteur.
 *                  Il suffit de mesurer le temps l'aller-retour pour connaitre la distance parcourue (2x distance de l'objet).
 * 
 * ATTENTION : - Seuls les pins 2-3 de l'arduino nano peuvent être utilisés pour la réception(ECHO) du signal
 *             car ils doivent supporter les interruptions     
 *             - Il est déconseillé d'utiliser deux capteurs ultrasons sur le même pin de reception de l'arduino
 * 
 * Il est tout à fait possible d'utiliser le pin de Trig pour plusieurs capteurs qui émettront alors en même temps.
 * 
 * Les pins 1 à 9 peuvent être utilisés pour le pin de TRIG
 * 
 * Remarque : La classe n'utilise pas Clock.h mais aurait très bien pu très bien :)
 */

class UltrasonicSensor {
private:
    unsigned int m_mesure;
    EchoHandler* echohandler;
    TrigHandler* trighandler;
    bool m_dejaTest;
    //! Permet un d'apporter un filtrage aux valeurs obtenues. (Fonction à modifier si besoin, le filtrage actuel améliore les erreurs liées aux très petites distnace) 
    void filtrage();
    bool m_inf2;

public:
    //! Definit les pins de TRIG et ECHO, (et attribue alors un TrigHandler et un EchoHandler)
		/*!
			\param trig pin de l'arduino correspondant à TRIG
      \param echo pin de l'arduino correspondant à ECHO
		*/
    void attach(int trig, int echo);
    //! Permet le calcul de distance ou de TIMEOUT fonction à mettre dans la loop() arduino
    void update();
    //! Renvoie la dernière mesure effectuée par le capteur en mm
    /*!
      \return distance en mm 
    */
    unsigned int getMesure();
    //! Commande au capteur d'émettre une impulsion (ne réémet que si le capteur est prêt
    void trig();
    //! Permet de savoir si le capteur est prêt à réémettre une nouvelle impulsion ou s'il est en attente d'une mesure
      /*!
      \return 1 si pret 0 sinon
      */
    bool getReady();
};

extern EchoHandler echohandler2;
extern EchoHandler echohandler3;

extern TrigHandler trighandler1;
extern TrigHandler trighandler2;
extern TrigHandler trighandler3;
extern TrigHandler trighandler4;
extern TrigHandler trighandler5; 
extern TrigHandler trighandler6;
extern TrigHandler trighandler7;
extern TrigHandler trighandler8;
extern TrigHandler trighandler9;

#endif