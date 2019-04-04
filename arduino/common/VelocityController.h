#ifndef __VELOCITYCONTROLLER_H__
#define __VELOCITYCONTROLLER_H__

#include "DifferentialController.h"

#include <math.h>

#define ENABLE_VELOCITYCONTROLLER_LOGS 0 // for debug purposes
#define VELOCITYCONTROLLER_LOGS_TIMESTEP 50e-3 // mm

/**
 * @brief Objet de controle de la vitesse.
 * 
 * VelocityController est une version améliorée de DifferentialController car il rajoute la gestion de Spin et les accélérations capées.
 * 
 * 
 */
class VelocityController : public DifferentialController
{
public:
	/**
	 * @brief Constructeur de VelocityController
	 *
	 * Construteur de VelocityController qui initialise ces vairables sur des valeurs neutres.
	 *
	 */
	VelocityController() : m_rampLinVelSetpoint(0), m_rampAngVelSetpoint(0), m_maxLinAcc(INFINITY), m_maxLinDec(INFINITY), m_maxAngAcc(INFINITY), m_maxAngDec(INFINITY), m_linSpinGoal(0.0), m_angSpinGoal(0.0),m_spinShutdown(true){}
	/**
	 * @brief Paramètre les accélérations max.
	 *
	 * @param maxLinAcc Accélération linéaire en mm/s².
	 * @param maxAngAcc Accélération angulaire en rad/s².
	 */

	void setMaxAngAcc   (float maxAngAcc);

	void setMaxLinAcc   (float maxLinAcc);

	/**
	 * @brief Paramètre les décéleration max.
	 *
	 * @param maxLinDec Décélération linéaire en mm/s².
	 * @param maxAngDec Décélération angulaire en rad/s².
	 */

	void setMaxLinDec   (float maxLinDec);

	void setMaxAngDec   (float maxAngDec);

	/**
	 * @brief Change l'état de l'arret d'urgence.
	 * @param spinShutdown Etat à appliquer à la variable spinShutdown.
	 */
	void setSpinShutdown(bool spinShutdown);
	/**
	 * @brief Retourne l'accélération max linéaire.
	 * 
	 * @return Accélération en mm/s².
	 */
	float getMaxLinAcc() const {return m_maxLinAcc;}
	/**
	 * @brief Retourne l'accélération max angulaire.
	 * 
	 * @return Accélération en rad/s².
	 */
	float getMaxAngAcc() const {return m_maxAngAcc;}
	/**
	 * @brief Retourne la décélération max linéaire.
	 * 
	 * @return Décélération en mm/s².
	 */	
	float getMaxLinDec() const {return m_maxLinDec;}
	/**
	 * @brief Retourne la décélération max angulaire.
	 * 
	 * @return Décélération en rad/s².
	 */
	float getMaxAngDec() const {return m_maxAngDec;}
	float getLinSpinGoal() const {return m_linSpinGoal;}
	float getAngSpinGoal() const {return m_angSpinGoal;}
	/**
	 * @brief Retourne l'état de spinShutDown
	 * 
	 * @return true Si le robot est bloqué par un obstacle.
	 * @return false Si le robot n'est pas bloqué.
	 */
	bool getSpinShutdown() const {return m_spinShutdown;}
	/**
	 * @brief Charge les paramètres.
	 * 
	 * Charge les derniers paramètres sauvegarder (les acc et dec) dans l'Arduino.
	 * 
	 * @param address Adresse à utiliser.
	 */
	void load(int address);
	/**
	 * @brief Sauvegarde les paramètres.
	 * 
	 * Sauvegarde les paramètres actuellement chargés.
	 * 
	 * @param address Adresse à utiliser.
	 */
	void save(int address) const;

protected:
	/**
	 * @brief Calcul la vitesse à atteindre.
	 * 
	 * Calcul les nouvelles vitesse à atteindre pour respecter les contraites d'accélérations.
	 * 
	 * @param stepSetpoint Vitesse demandé.
	 * @param input Vitesse actuel
	 * @param rampSetpoint Ancienne vitesse intermédiaire calculée.
	 * @param maxAcc Accélération max.
	 * @param maxDec Accélération min.
	 * @param timestep Temps depuis le dernier appel.
	 * 
	 * @return float Nouvelle vitesse intermédiaire.
	 */
	float genRampSetpoint(float stepSetpoint, float input, float rampSetpoint, float maxAcc, float maxDec, float timestep);
	/**
	 * @brief Calcul l'asservissement.
	 * @param timestep temps depuis le dernier appel.
	 */
	virtual void process(float timestep);
	/**
	 * @brief Initialisation de l'asservissement.
	 */
	virtual void onProcessEnabling();

	float m_rampLinVelSetpoint;/*!< Vitesse linéaire intermédiaire en mm/s.  */  // in mm/s (no longer w/e unit)
	float m_rampAngVelSetpoint; /*!< Vitesse angulaire intermédiaire en rad/s.  */// in rad/s (no longer w/e unit)
	float m_maxLinAcc; /*!< Accélération max linéaire en mm/s². Toujours positif. */   // always positive, in mm/s^2
	float m_maxLinDec; /*!< Accélération max angulaire en rad/s². Toujours positif.*/ // always positive, in mm/s^2
	float m_maxAngAcc; /*!< Décélération max linéaire en mm/s². Toujours positif. */   // always positive, in mm/s^2
	float m_maxAngDec; /*!< Décélération max angulaire en rad/s². Toujours positif.*/ // always positive, in mm/s^2
	bool  m_spinShutdown; /*!< Etat de la sécurité de patinage.*/
	float m_linSpinGoal; // Velocity wanted before the spin alarm
	float m_angSpinGoal; // Velocity wanted before the spin alarm

#if ENABLE_VELOCITYCONTROLLER_LOGS
	friend class VelocityControllerLogs;
#endif // ENABLE_VELOCITYCONTROLLER_LOGS
};

#if ENABLE_VELOCITYCONTROLLER_LOGS
/**
 * @brief Classe d'enregistrement de vitesse.
 */
class VelocityControllerLogs : public PeriodicProcess
{
public:

	void setController(const VelocityController& controller){m_controller = &controller;}

protected:

	virtual void process(float timestep);

	const VelocityController* m_controller;
};
#endif // ENABLE_VELOCITYCONTROLLER_LOGS

#endif // __VELOCITYCONTROLLER_H__
