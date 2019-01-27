#ifndef __FULLSPEEDSERVO_H__
#define __FULLSPEEDSERVO_H__

#include "Clock.h"
#include <Servo.h>
#include "PeriodicProcess.h"

/** class FullSpeedServo
 *  \brief Pilotage de Servomoteur particulier
 *
 *	Cette classe permet de controler un Servomoteur comme un moteur continu avec une butée
 *	Cela veut dire faire tourner un servomoteur dans une direction pendant un certain temps. 
 */
class FullSpeedServo : public PeriodicProcess{

private: 

	Servo m_servo;
	float m_time;
	bool m_moving;
	Clock m_clock;

public:
	//! Fait tourner le servomoteur pendant le temps donné ou jusqu'à la position demandée
	/*!
		\param setpoint position de commande
		\param time durée en secondes
	*/
	void SpeedWrite(int setpoint, float time);
	//! Fait tourner le servomoteur jusqu'à la position demandée (vitesse normale)
	/*!
		\param setpoint position de commande
	*/
	void write(int setpoint);
	//! Définit le pin sur lequel est connecté le serovmoteur
		/*!
			\param pin de l'arduino
	*/
	void attach(int pin);
	//! Fonction réciproque de attach() (permet par exemple d'avoir un servomoteur en roue libre)
	void detach();
	//! Permet de savoir si le servo est attaché ou pas (respectivement maintient sa position / roue libre)
		/*!
			\return renvoie 1 si attached et 0 si detached
	*/
	bool attached(){return m_servo.attached();}
	//! renvoie la dernière position envoyée au servo (seulement si le servo est "attached")
		/*!
			\return position du Servo entier
	*/
	int read(){return m_servo.read();}

protected:
	
	virtual void process(float timestep);
	
};

#endif