#ifndef __ODOMETRY_H__
#define __ODOMETRY_H__

#include "PeriodicProcess.h"

/**
 * @brief Structure de position.
 * \author Ulysse Darmet
 * Position est une structure de Odometry.h qui permet en une variable d'obtenir la totalité des informations à propos du positionnement du robot.
 * 
 * @return struct Position  {
 */
struct Position
{
	/**
	 * @brief Constructeur de Position.
	 * Constructeur de Position qui initialise la position au coordonnées (0,0) et à l'angle 0.
	 */
	Position() : x(0), y(0), theta(0){}
	/**
	 * @brief  Constructeur de Position.
	 * 
	 * Constructeur de Position qui initialise la position au coordonnées indiqués.
	 * 
	 * @param x coordoonée en x initial.
	 * @param y coordoonée en y initial.
	 * @param theta angle initial.
	 */
	Position(float x, float y, float theta) : x(x), y(y), theta(theta){}

	float x;/*!< Coordonnée en x.*/
	float y;/*!< Coordonnée en y.*/
	float theta;/*!< Angle.*/
};

/**
 * @brief Classe abstraite d'une roue codeuse.
 * 
 * Cette classe est à implémenter pour être compatible avec la classe Odometry.
 * \author Ulysse Darmet
 */
class AbstractCodewheel
{
public:

	virtual ~AbstractCodewheel(){}

	/**
	 * @brief Calcul la distance parcourue.
	 * 
	 * Méthode à implémenter, dont le rôle est de retourner la distance parcourue depuis le dernier appel de cette méthode (ou depuis l'initialisation de l'objet).
	 * 
	 * @return Distance parcourue depuis le dernier appel.
	 */
	virtual float getTraveledDistance() = 0;
	/**
	 * @brief Réinitialise l'objet.
	 * 
	 * Réinitialise l'objet. C'est à dire, repasse tous les paramètres à leur état inital. De plus cette méthode dois retourner la distance parcourue depuis le dernier appel de AbstractCodewheel::getTraveledDistance.
	 * 
	 * @return Distance parcourue depuis le dernier getter de distance.
	 */
	virtual float restart() = 0;
};
/**
 * @brief Calcule la position en temps réel du robot.
 * \author Ulysse Darmet
 * Odometry est un PeriodicProcess qui calcule la position du robot à partir des roues codeuses ( AbstractCodewheel ) .
 */
class Odometry : public PeriodicProcess
{
public:
	/**
	 * @brief Attribut une nouvelle position.
	 * 
	 * A partir des coordonnées passer en paramètre, attribut les nouvelles coordonnées à sa Position.
	 * 
	 * @param x Nouvelle coordonnée en x.
	 * @param y Nouvelle coordonnée en y.
	 * @param theta Nouvelle angle.
	 */
	void setPosition(float x, float y, float theta){m_pos.x = x; m_pos.y = y; m_pos.theta = theta;}
	/**
	 * @brief Defini une nouvelle entraxe pour les roues codeuses.
	 * 
	 * Change l'entraxe actuel par celui indiqué en paramètre.
	 * 
	 * @param axleTrack Nouvelle entraxe en mm.
	 */
	void setAxleTrack(float axleTrack);
	/**
	 * @brief Defini la nouvelle dérive orthogonal.
	 * 
	 * Change la dérive orthogonal par celle indiquée en paramètre.
	 *
	 * @param slippage Nouvelle dérive orthogonal sans unité et signé.
	 */
	void setSlippage (float slippage);
	/**
	 * @brief Defini les roues codeuses de Odometry.
	 * 
	 * Paramètre les pointeurs sur les deux AbstractCodewheel à utiliser pour le calcul d'odométrie.
	 * 
	 * @param leftCodewheel AbstractCodewheel de la roue codeuse gauche.
	 * @param rightCodewheel AbstractCodewheel de la roue codeuse droite.
	 */
	void setCodewheels(AbstractCodewheel& leftCodewheel, AbstractCodewheel& rightCodewheel){m_leftCodewheel = &leftCodewheel, m_rightCodewheel = &rightCodewheel;}
	/**
	 * @brief Retourne la position
	 * 
	 * Retourne sa struc Position avec les dernières positions calculés.
	 * 
	 * @return La structure Position.
	 */
	const Position&	getPosition() const {return m_pos;}
	/**
	 * @brief Retourne la vitesse linéaire.
	 * 
	 * Rend la dernière vitesse linéaire calculé.
	 * 
	 * @return Vitesse lineaire en mm/s.
	 */
	float getLinVel() const {return m_linVel;}
	/**
	 * @brief Retourne la vitesse angulaire.
	 * 
	 * Rend la dernière vitesse angulaire calculé.
	 * 
	 * @return Vitesse angulaire en rad/s.
	 */
	float getAngVel() const {return m_angVel;}
	/**
	 * @brief Retourne l'entraxe utilisée.
	 * @return Entraxe en mm.
	 */
	float getAxleTrack() const {return m_axleTrack;}
	/**
	 * @brief Retourne la dérive utilisée.
	 * @return Dérive sans unité.
	 */
	float getSlippage () const {return m_slippage;}
	/**
	 * @brief Charge les paramètres.
	 * 
	 * Charge les paramètres depuis la mémoire de l'arduino.
	 * 
	 * @param address Adresse à utilisé pour charger les données.
	 */
	void load(int address);
	/**
	 * @brief Sauvegarde les paramètres
	 * 
	 * Sauvegarde les paramètres actuelement utilisés.
	 * 
	 * @param address Adresse à utilisé pour la sauvegarde.
	 */
	void save(int address) const;

protected:
	/**
	 * @brief Calcule la nouvelle position et la nouvelle vitesse.
	 * A partir de ses AbstractCodewheel, détermine la nouvelle vitesse instantanée et la nouvelle position.
	 * @param timestep Temps depuis le dernier appel de cette méthode en secondes.
	 */
	virtual void process(float timestep);

	Position m_pos; /*!< Structure de position de Odometry. */
	float m_linVel; /*!< Vitesse lineaire en mm/s. */
	float m_angVel; /*!< Vitesse angulaire en rad/s.*/
	float m_axleTrack;/*!< Entraxe entre les deux roues codeuses. */
	float m_slippage;/*!< Constante de dérivation othogonal. */

	AbstractCodewheel* m_leftCodewheel;/*!< Pointeur de l'AbstractCodewheel gauche. */
	AbstractCodewheel* m_rightCodewheel;/*!< Pointeur de l'AbstractCodewheel droite.  */
};

#endif // __ODOMETRY_H__
