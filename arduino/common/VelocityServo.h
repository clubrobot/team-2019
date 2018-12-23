#ifndef __VELOCITYSERVO_H__
#define __VELOCITYSERVO_H__

#include "Clock.h"
#include <Servo.h>
#include "PeriodicProcess.h"
/** class VelocityServo
 *  \brief Objet permettant de controller la vitesse d'un servomoteur.
 *
 *  est un outil permettant de déplacer un servomoteur vers une position à une vitesse donnée.
 *  Il utilise la classe Servo.h et PeriodicProcess	
 *  Par défaut la vitesse du servomoteur est de 360 degrés/secondes.
 */
class VelocityServo : public PeriodicProcess{

private: 

	Servo m_servo;
	int m_setpoint;
	float m_velocity;
	int m_startpoint;
	int m_dir;
	bool m_arrived; 
	Clock m_clock;

public: 
	
	VelocityServo(){m_velocity = 100;}
	//! Permet de changer la vitesse du servomoteur
		/*!
			\param vel vitesse en degrés/s
		*/
	void setVelocity(float vel){m_velocity = vel;}

	//! Déplace le servomoteur à la vitesse réglée (defaut = 360°/s)
		/*!
			\param setpoint position angulaire voulue 
		*/
	void Velocitywrite(int setpoint);
	
	//! Définit le pin sur lequel est connecté le serovmoteur
		/*!
			\param pin de l'arduino
		*/	void attach(int pin);
	//! Fonction réciproque de attach() (permet par exemple d'avoir un servomoteur en roue libre)

	void detach();
	void write(int setpoint){m_arrived = true; m_servo.write(setpoint); }
	//! Permet de déplacer le servomoteur à vitesse normale (~Vitesse max)
		/*!
			\param setpoint position angulaire voulue 
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