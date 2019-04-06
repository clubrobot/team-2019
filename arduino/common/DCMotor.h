#ifndef __DCMOTOR_H__
#define __DCMOTOR_H__

#include "NonCopyable.h"
#include "DifferentialController.h"

#include <math.h>

/** class DCMotor
 *  \brief Pilotage de moteur continu
 *  \author Ulysse Darmet
 *	Cette classe permet de contrôler un moteur à courant continu par PWM via un driver Moteur
 * 	
 *
 *
 *	 Remarque : Pour les moteurs qui ne sont reliées à des roues seulement régler la constante du moteur à 1/tension_PWM.
 * 				Ainsi, on peut simplement controler le moteur via setVelocity() en envoyant comme commande la tension de PWM souhaitée.
 *	 
 */
class DCMotor : private NonCopyable, public AbstractMotor
{
public:
	DCMotor() : m_enabled(false), m_velocity(0), m_wheelRadius(1 / (2 * M_PI)), m_constant(1), m_maxPWM(1){}

	//!Indique quels pins de l'arduino son utilisé pour ce moteur (actuellement correspond à moteur 1 ou 2)
	void attach(int EN, int PWM, int DIR);

	//!Envoie une commande de vitesse au moteur
	/*!
		\param velocity vitesse de commande en mm/s
	*/
	void setVelocity(float velocity){m_velocity = velocity; update();}

	//!Paramètre la constante du moteur 
	/*!
		\param constant constante en rad/s/Volt
	*/
	void setConstant   (float constant);

	//!Paramètre le rayon de la roue liée au moteur
	/*!
		\param wheelRadius rayon en mm
	*/
	void setWheelRadius(float wheelRadius);

	//!Paramètre une valeur limite de PWN à ne pas dépasser
	/*!
		\param maxPWM valeur limite entre 0 et 1
	*/
	void setMaxPWM     (float maxPWM);

	void enable (){m_enabled = true;  update();}
	void disable(){m_enabled = false; update();}

	//!Renvoie la vitesse de commande actuelle du moteur
	/*!
		\return vitesse en mm/s
	*/
	float getVelocity   () const {return m_velocity;}

	//!Renvoie la constante du moteur paramétrée
	/*!
		\return constante : (60 * reduction_ratio / velocity_constant_in_RPM) / supplied_voltage_in_V
	*/
	float getConstant   () const {return m_constant;}

	//!Renvoie rayon de la roue du moteur
	/*
		\return rayon en mm 
	*/
	float getWheelRadius() const {return m_wheelRadius;}

	//!Renvoie la valeur max de PWM
	/*!
		\return valeur entre 0 et 1
	*/
	float getMaxPWM     () const {return m_maxPWM;}
	bool  isEnabled     () const {return m_enabled;}

	//!Renvoie la vitesse maximale avec les constantes actuelles
	/*!
		\return vitesse max en mm/s
	*/
	float getMaxVelocity() const;

	void load(int address);
	void save(int address) const;
	
protected:

	void update();

	bool  m_enabled;
	float m_velocity; //!<  in mm/s (millimeters per second)
	float m_wheelRadius; //!<  in mm
	float m_constant; //!<  (60 * reduction_ratio / velocity_constant_in_RPM) / supplied_voltage_in_V
	float m_maxPWM; //!<  in range ]0, 1]

	int	m_EN;
	int	m_PWM;
	int	m_DIR;
};

/** class DCMotorsDriver
 *  \brief Utilisation des drivers moteurs
 *  \author Ulysse Darmet
 * 	est une classe permettant d'utiliser les fonctions du driver
 *	 
 */
class DCMotorsDriver
{
public:
	//!Définit les pins utiles au driver
	/*!
		\param RESET pin de reset
		\param FAULT pin de Fault
	*/
	void attach(int RESET, int FAULT);

	void reset();

	bool isFaulty();

private:

	int m_RESET;
	int m_FAULT;
};

#endif // __DCMOTOR_H__
