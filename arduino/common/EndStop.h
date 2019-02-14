#ifndef __ENDSTOP_H__
#define __ENDSTOP_H__

/** class EndStop
 *  \brief Capteur fin de course
 *  est une classe permettant d'utiliser les capteurs fins de courses (clic de souris/bouton poussoir)
 *  Pour utiliser cette classe le bouton doit être d'un côté relié à la masse et de l'autre à l'arduino. 
 */
class EndStop {

private:

    int m_pin;

public: 
    //! Permet de connaitre l'état courant du bouton.
		/*!
			\return etat (1 si enfoncé, 0 sinon).
	*/
    bool getState();
    //! Indique le pin de l'arduino utilisé.

    void attach(int pin);
    //! Réciproque de attach.
    void detach();
};

#endif // __ENDSTOP_H__
