#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <Arduino.h>

/** class Clock
 *  \brief Utilitaire pour gérer le temps dans vos programmes Arduino.
 *	\author Ulysse Darmet
 *  est un outil permettant à vos programmes d'utiliser plus simplement la méthode micros() de <Arduino.h>.
 * Cette objet vous permettera de mesurer le temps écoulé depuis le dernier appel de la méthode Clock::restart .
 * 
 */
class Clock
{
public:
	//! Constructeur de  Clock
	/*!
	* Le constructeur de Clock en plus de construire l'objet fait un premier marqueur qui vous permettra d'utiliser Clock::getElapsedTime pour avoir le temps écoulé depuis la création de l'objet.
	*/
	Clock() : m_startTime(micros()){}

	//! Récupère le temps depuis le dernier reset.
	/*!
	* Récupère le temps écoulé en secondes depuis la construction de l'objet ou depuis le dernier Clock::restart.
	* \return Temps écoulé en secondes.
	*/
	float getElapsedTime() const
	{
		unsigned long currentTime = micros();
		float elapsedTimeInSeconds = (currentTime - m_startTime) / float(1e6);
		return elapsedTimeInSeconds;
	}

	//! Reset le temps.
	/*!
	* Réinitialise le temps à 0s.
	* \return Temps écoulé en secondes depuis le dernier reset.
	*/
	float restart()
	{
		unsigned long currentTime = micros();
		float elapsedTimeInSeconds = (currentTime - m_startTime) / float(1e6);
		m_startTime = currentTime;
		return elapsedTimeInSeconds;
	}

private:

	unsigned long m_startTime; //!< temps en microsecondes du microcontroleur utilisé comme repère.
};

#endif // __CLOCK_H__
