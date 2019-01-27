#ifndef __PERIODICPROCESS_H__
#define __PERIODICPROCESS_H__

#include "Clock.h"

/** class PeriodicProcess
 *  \brief Classe à implémenter pour gérer les appels dans la loop.
 *
 *  est un outil permettant à l'Arduino de pouvoir appeler l'objet PeriodicProcess tout les X s. 
 *  Cela permet de ne pas saturer le microcontroleur pour des tâches qui ne nécessitent pas un très gros rafraichissement.
 *  En général, on l'utilise pour toutes les tâches dans la loop.
 *
 * 	Par exemple : faire tourner un moteur pendant un certain temps puis l'arreter en autonomie sans delay()
 */
class PeriodicProcess
{
public:
	//! Constructeur de  PeriodicProcess
	/*!
	* Le constructeur est totalement vide.
	*/
	virtual ~PeriodicProcess(){}
	//! Active le PeriodicProcess
	/*!
	* Passe la variable m_enable à Vrai et execute onProcessEnabling.
	*/
	void enable();
	//! Désactive le PeriodicProcess
	/*!
	* Passe la variable m_enable à Faux et execute onProcessDisabling.
	*/
	void disable();
	//! Sélectionne une nouvelle valeur pour timestep.
	/*!
	* Change le timestep par celui donné en paramètre.
		\param timestep Temps en secondes du taux de rafraichissement. 	
	
	*/
	void setTimestep(float timestep){m_timestep = timestep;} 

	//! Execute la méthode process.
	/*!
	* Execute la méthode process si le temps passé est supérieur à timestep. Envoie à process le temps depuis le dernier appel. 

		\return Vrai si process a été lancé et Faux sinon.
	*/
	bool update();

	//! Vérifie si le PeriodicProcess est activé.
	/*! 
		\return La valeur de m_enabled.
	*/
	bool isEnabled() const {return m_enabled;}
	//! Retourne la valeur de m_timestep.
	/*! 
		\return La valeur de m_timestep.
	*/
	float getTimestep() const {return m_timestep;}

protected:
	//! Méthode à implémenter obligatoirement pour hériter de PeriodicProcess. 
	/*!
	* Process est la méthode qui s'exécutera toutes les m_timestep. Il doit donc définir l'action répétitive voulue dans la loop de l'Arduino. 
		\param timestep Temps écoulé depuis le dernier appel en seconde.
	*/
	virtual void process(float timestep) = 0;
	//! Méthode exécutée à l'activation du PeriodicProcess. 
	/*!
	* Méthode à implémenter si votre class nécessite des actions à son activation. 
	*/

	virtual void onProcessEnabling(){}
	//! Méthode exécutée à la désactivation du PeriodicProcess. 
	/*!
	* Méthode à implémenter si votre class nécessite des actions à sa déactivation. 
	*/
	virtual void onProcessDisabling(){}

private:

	bool  m_enabled;
	/*! \var bool m_enabled
	\brief Boolean qui stocke l'état de l'activation du PeriodicProcess. 
	*/
	float m_timestep;
	/*! \var float m_timestep
    \brief Temps défini en secondes entre chaque appel de PeriodicProcess::process.
	*/
	Clock m_clock;//!< Objet Clock requis pour l'utilisation de PeriodicProcess.
};

#endif // __PERIODICPROCESS_H__
