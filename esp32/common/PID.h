#ifndef __PID_H__
#define __PID_H__

#include <math.h>

/**
 * @brief Classe d'asservissement
 * \author Ulysse Darmet
 * PID est une classe d'asservissement composée des 3 types d'asservissements. Elle permet à partir de l'erreur et de la constante désiré de retourner une commande asservie.
 *
 */
class PID
{
public:
	/**
	 * @brief Constructeur de PID
	 * Constructeur de PID qui initialise toutes les valeurs à des valeurs neutres (Kp=1,Ki=0,Kd=0).
	 */
	PID() : m_Kp(1), m_Ki(0), m_Kd(0), m_minOutput(-INFINITY), m_maxOutput(INFINITY){}
	/**
	 * @brief Calcul l'asservissement.
	 *
	 * A partir de l'erreur et du temps depuis le dernier appel et de la constante demandé, rend une consigne asservie.
	 *
	 * @param setpoint Constante désiré.
	 * @param input Constante actuel.
	 * @param timestep Temps depuis le dernier appel en secondes.
	 *
	 * @return float La valeur asservie.
	 */
	float compute(float setpoint, float input, float timestep);
	/**
	 * @brief Réinitialise les accumulateurs
	 */
	void reset();
	/**
	 * @brief Charge de nouvelles constantes d'asservissements.

	 *
	 * @param Kp Coefficient proportionnel.
	 * @param Ki Coefficient intégrateur.
	 * @param Kd Coefficient dérivateur.
	 */
	void setTunings(float Kp, float Ki, float Kd);
	/**
	 * @brief Charge les limites de sorties.
	 *
	 *
	 * @param minOutput Minimun de sortie (peux être négatif).
	 * @param maxOutput Maximum de sortie (peux être).
	 */
	void setOutputLimits(float minOutput, float maxOutput);
	/**
	 * @brief Retourne le coefficient proportionnel.
	 * @return Coefficient proportionnel.
	 */
	float getKp() const {return m_Kp;}
	/**
	 * @brief Retourne le coefficient intégrateur.
	 * @return Coefficient intégrateur.
	 */
	float getKi() const {return m_Ki;}
	/**
	 * @brief Retourne le coefficient dérivateur.
	 * @return Coefficient dérivateur.
	 */
	float getKd() const {return m_Kd;}
	/**
	 * @brief Retourne la sortie minimal.
	 * @return Sortie minimal.
	 */
	float getMinOutput() const {return m_minOutput;}
	/**
	 * @brief Retourne la sortie maximal.
	 * @return Sortie maximal.
	 */
	float getMaxOutput() const {return m_maxOutput;}
	/**
	 * @brief Charge les paramètres de la mémoire.
	 * @param address Adresse à utiliser.
	 */
	void load(int address);
	/**
	 * @brief Sauvegarde les paramètres dans la mémoire.
	 * @param address Adresse à utiliser.
	 */
	 void save(int address) const;

private:

	float m_errorIntegral; /*!< Accumulateur integral. */
	float m_previousError; /*!< Erreur précédent. */

	float m_Kp;/*!< Coefficient proportionnel.*/
	float m_Ki;/*!< Coefficient intégrateur.*/
	float m_Kd;/*!< Coefficient dérivateur.*/
	float m_minOutput; /*!< Sortie minimal.*/
	float m_maxOutput;/*!< Sortie maximal.*/
};

#endif // __PID_H__
