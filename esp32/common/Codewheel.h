#ifndef __CODEWHEEL_H__
#define __CODEWHEEL_H__

#include "NonCopyable.h"
#include "Odometry.h"

#include <math.h>

/** class Codewheel
 *  \brief Fait la passerelle entre les roues codeuses et le compteur.
 *  \author Ulysse Darmet
 *	Cette classe permet de récupérer les tics des roues codeuses à travers la puce compteuse.
 *
 */
class Codewheel : private NonCopyable, public AbstractCodewheel
{
public:

	Codewheel() : m_currentCounter(0), m_startCounter(0), m_wheelRadius(1 / (2 * M_PI)), m_countsPerRev(1000){}

	/**
	 * @brief Set les pins pour le compteur.
	 *
	 * @param XY Voir doc elec
	 * @param AXIS Voir doc elec
	 * @param SEL1 Voir doc elec
	 * @param SEL2 Voir doc elec
	 * @param OE Voir doc elec
	 * @param RST Voir doc elec
	 */
	void attachCounter(int XY, int AXIS, int SEL1, int SEL2, int OE, int RST);

	/**
	 * @brief Set les pins pour les registres du compteur.
	 *
	 * @param DATA
	 * @param LATCH
	 * @param CLOCK
	 */
	void attachRegister(int DATA, int LATCH, int CLOCK);

	/**
	 * @brief Donne le nombre de tic absolue courant.
	 *
	 * @return long Le nombre de tics depuis le dernier reset.
	 */
	long getCounter(){update(); return m_currentCounter;}

	/**
	 * @brief Donne le nombre de tics par tour courant.
	 *
	 * @return long Nombre de tics par tour.
	 */
	long getCountsPerRev(){return m_countsPerRev;}

	/**
	 * @brief Donne le rayon de la roue en mm
	 *
	 * @return float rayon en mm.
	 */
	float getWheelRadius(){return m_wheelRadius;}
	
	/**
	 * @brief Set le nombre de tics par tour.
	 *
	 * @param countsPerRev nb de tics par tour.
	 */
	void setCountsPerRev(long countsPerRev);

	/**
	 * @brief Set le rayon en mm de la roue.
	 *
	 * @param wheelRadius rayon en mm.
	 */
	void setWheelRadius (float wheelRadius);

	/**
	 * @brief Réinitialise le compteur à 0.
	 *
	 * Cette méthode peut rendre caduque le prochain Codewheel::getTraveledDistance.
	 */
	void reset();

	/**
	 * @brief Donne la distance parcouru.
	 *
	 * Cette méthode donne la distance parcouru par la roue depuis le dernier Codewheel::reset ou Codewheel::restart.
	 * @return float
	 */
	float getTraveledDistance();

	/**
	 * @brief Donne la distance parcouru et reset le compteur.
	 *
	 * Cette méthode appel la méthode Codewheel::getTravemedDistance avant de mettre à jour le compteur de l'arduino (pas du compteur (puce elec)).
	 *
	 * @return float
	 */
	float restart();

	/**
	 * @brief Charge les données de l'EEPROM avec l'offset.
	 *
	 * @param address Offset à utiliser pour lire dans l'EEPROM.
	 */
	void load(int address);

	/**
	 * @brief Sauvegarde les constantes actuelles dans l'EEPROM
	 *
	 * @param address Offset à utiliser pour ecrire dans l'EEPROM.
	 */
	void save(int address) const;

protected:

	/**
	 * @brief Récupère le nombre de tics stoqués dans le compteur.
	 *
	 */
	void update();

	long m_currentCounter; //!< Tic courant.
	long m_startCounter;//!< Tic depuis le dernier reset

	float m_wheelRadius; //!< Rayon de la roue codeuse en  mm
	long m_countsPerRev;   //!<  Nombre de tics par tour de roue.

	int m_COUNTER_XY;   //!<  Select one of the two quad counters. See below.
	int m_COUNTER_AXIS; //!<  Not a pin: X = 0, Y = 0
	int m_COUNTER_SEL1; //!<  MSB = 0, 2ND = 1, 3RD = 0, LSB = 1
	int m_COUNTER_SEL2; //!<  MSB = 0, 2ND = 0, 3RD = 1, LSB = 1
	int m_COUNTER_OE;   //!<  Active LOW. Enable the tri-states output buffers.
	int m_COUNTER_RST;  //!<  Active LOW. Clear the internal position counter and the position latch.

	int m_REGISTER_DATA;  //!<  Serial data input from the 74HC165 register.
	int m_REGISTER_LATCH; //!<  Active LOW. Latch signal for the 74HC165 register.
	int m_REGISTER_CLOCK; //!<  LOW-to-HIGH edge-triggered. Clock signal for the 74HC165 register.
};

#endif // __ROTARYENCODER_H__
