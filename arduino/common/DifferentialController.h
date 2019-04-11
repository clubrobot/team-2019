#ifndef __DIFFERENTIALCONTROLLER_H__
#define __DIFFERENTIALCONTROLLER_H__

#include "PeriodicProcess.h"
#include "PID.h"


/**
 * @brief Instance de moteur.
 * \author Ulysse Darmet
 * Instance de Moteur permettant une parfaite compatibilitée entre les classes Motor et le DifferentialController.
 */
class AbstractMotor
{
public:
	/**
	 * @brief Constructeur d'AbstractMotor.
	 * 
	 * Méthode à implémenter.
	 */
	virtual ~AbstractMotor(){}
	/**
	 * @brief Charge une nouvelle vitesse.
	 * 
	 * Change la vitesse du moteur par celle passée en parametre. Méthode à implémenter.
	 * 
	 * @param velocity Nouvelle vitesse.
	 */
	virtual void setVelocity(float velocity) = 0;
	/**
	 * @brief Retourne vitesse max
	 * 
	 * Retourne la vitesse max du moteur pour son intégration dans les calculs de DifferentialController.
	 * 
	 * @return virtual float
	 */
	virtual float getMaxVelocity() const = 0;
};

/**
 * @brief Controle les moteurs.
 * 
 * DifferentialController permet de controler les deux moteurs du robot à partir de l'odométrie et d'un PID.
 * 
 */
class DifferentialController : public PeriodicProcess
{
public:
	/**
	 * @brief Constructeur de DifferentialController
	 * Constructeur de DifferentialController qui initialise les variables à des valeurs neutres. 
	 */
	DifferentialController() : m_linSetpoint(0), m_angSetpoint(0), m_axleTrack(1){}

	/**
	 * @brief Charge les vitesses actuel.
	 * 
	 * Charge les vitesses instantanées du robot pour l'asservissemeent. 
	 * 
	 * @param linInput Vitesse linéaire en mm/s.
	 * @param angInput Vitesse angulaire en rad/s.
	 */
	void setInputs   (float linInput,    float angInput)   {m_linInput    = linInput;    m_angInput    = angInput;}
	/**
	 * @brief Charge les vitesses désirées.
	 * 
	 * Charge la vitesse désirée par l'utilisation.
	 * 
	 * @param linSetpoint Vitesse linéaire en mm/s.
	 * @param angSetpoint Vitesse angulaire en rad/s.
	 */
	void setSetpoints(float linSetpoint, float angSetpoint){m_linSetpoint = linSetpoint; m_angSetpoint = angSetpoint;}
	/**
	 * @brief Charge l'entraxe.
	 * 
	 * Charge l'entraxe entre les deux roues du robot. Attention: cette entraxe est différente que celle du de l'odométrie.
	 * 
	 * @param axleTrack
	 */
	void setAxleTrack(float axleTrack);

	/**
	 * @brief Charge les moteurs
	 * 
	 * Charge les pointeurs de AbstractMotor du DifferentialController.
	 * 
	 * @param leftWheel Roue gauche (AbstractMotor).
	 * @param rightWheel Roue droite (AbstractMotor).
	 */
	void setWheels(AbstractMotor& leftWheel, AbstractMotor& rightWheel){m_leftWheel = &leftWheel; m_rightWheel = &rightWheel;}
	/**
	 * @brief Charge l'asservissement.
	 * 
	 * Charge les pointeurs PID pour l'asservissement de DifferentialController.
	 * 
	 * @param linPID Asservissement linéaire.
	 * @param angPID Asservissement angulaire.
	 */
	void setPID(PID& linPID, PID& angPID){m_linPID = &linPID; m_angPID = &angPID;}
	/**
	 * @brief Retourne la vitesse demandée.
	 * 	
	 * @return float Vitesse linéaire en mm/s.
	 * 
	 */
	float getLinSetpoint() const {return m_linSetpoint;}
	/**
	 * @brief Retourne la vitesse demandée.
	 * 	
	 * 
	 * @return float Vitesse angulaire en rad/s.
	 */
	float getAngSetpoint() const {return m_angSetpoint;}

	/**
	 * @brief Retour la commande linéaire actuel.
	 * 
	 * @return float  Commande linéaire en mm/s
	 */
	float getLinOutput() const {return m_linVelOutput;}
	/**
	 * @brief Retour la commande angulaire actuel.
	 * 
	 * @return float  Commande angulaire en rad/s
	 */
	float getAngOutput() const {return m_angVelOutput;}

	/**
	 * @brief Retourne l'entraxe.
	 * @return Entraxe en mm.
	 */
	float getAxleTrack() const {return m_axleTrack;}
	/**
	 * @brief Charge les paramètres
	 * 
	 * Charge les paramètres depuis la mémoire de l'Arduino.
	 *  
	 * @param address Adresse à utiliser.
	 */
	void load(int address);
	/**
	 * @brief Sauvegarde les paramètres
	 * 
	 * Sauvegarde les paramètres dans la mémoire de l'Arduino.
	 *  
	 * @param address Adresse à utiliser.
	 */
	void save(int address) const;

protected:
	/**
	 * @brief Calcul l'asservissement	
	 * @param timestep Temps depuis le dernier appel.
	 */
	virtual void process(float timestep);
	/**
	 * @brief Reset les accumulateurs des asserv.
	 */
	virtual void onProcessEnabling();

	float m_linInput; /*!< Vitesse linéaire actuel du robot.  */
	float m_angInput; /*!< Vitesse angulaire actuel du robot. */
	float m_linSetpoint; /*!< Vitesse linéaire demandée (en mm/s). */
	float m_angSetpoint; /*!< Vitesse angulaire demandée (en rad/s).*/
	float m_axleTrack; /*!< Entraxe entre les deux roues motrices du robot (en mm).*/

	float m_linVelOutput;/*!< Vitesse linéaire asservie.*/
	float m_angVelOutput;/*!< Vitesse angiulaire asservie.*/

	AbstractMotor* m_leftWheel;/*!< Pointeur du moteur gauche (AbstractMotor).*/
	AbstractMotor* m_rightWheel;/*!< Pointeur du moteur droit (AbstractMotor).*/
	PID* m_linPID;/*!< Pointeur de l'asservissement linéaire. */
	PID* m_angPID;/*!< Pointeur de l'asservissement angulaire.*/
};

#endif // __DIFFERENTIALCONTROLLER_H__
