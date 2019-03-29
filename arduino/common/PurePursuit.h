#ifndef __PUREPURSUIT_H__
#define __PUREPURSUIT_H__

#include "PositionController.h"
#include "Odometry.h"

#include <math.h>

#ifndef PUREPURSUIT_MAX_WAYPOINTS
#define PUREPURSUIT_MAX_WAYPOINTS 16
#endif

/** class PurePursuit
 *  \brief Trajectoire courbe le long d'une ligne brisée.
 *
 * est un AbstractMoveStrategy.
 */

class PurePursuit : public AbstractMoveStrategy
{
public:
	/** struct Waypoint
	 *  \brief Structure d'un point de passage de Purpursuit.
	 * 
	 */
	struct Waypoint
	{
		Waypoint() : x(0), y(0){}
		Waypoint(float x, float y) : x(x), y(y){}
		Waypoint(const Position& pos) : x(pos.x), y(pos.y){}

		float x, y;
	};
	/** enum Direction
	 *  \brief Sens de déplacement pour le robot.
	 */
	enum Direction {FORWARD=1 ///< Le robot avance en marche avant.
	, BACKWARD=-1 ///< Le robot avance en marche arrière.
	};

	PurePursuit() : m_numWaypoints(0), m_direction(FORWARD){}
	/**
	 * @brief Setter du sens de marche du robot.
	 * @param direction Sens à utiliser pour le déplacement du robot. 
	 */
	void setDirection(Direction direction);
	/**
	 * @brief Setter de l'angle à atteindre en fin de trajectoire.
	 * @param finalAngle  Angle à atteindre (entre l'axe du robot et l'axe des x)
	 */
	void setFinalAngle(float finalAngle);
	/**
	 * @brief Ajout un point en fin de ligne brisée.
	 * 
	 * @param waypoint Nouveau point à ajouter.
	 * @return true Ajout réussi. 
	 * @return false L'ajout a rencontré un problème.
	 */
	bool addWaypoint(const Waypoint& waypoint);
	/**
	 * @brief Initialise le Purpuisuit.
	 * Supprime les points de la ligne brisée et les paramètres temporaires.
	 */
	void reset();
	/**
	 * @brief  Setter du lookAhead.
	 * 
	 * Met à jour la distance entre le point intermediaire et le robot.
	 * 
	 * @param lookAhead Distance en mm.
	 */
	void setLookAhead   (float lookAhead) ;
	/**
	 * @brief Setter du lookAheadBis.
	 * 
	 * Met à jour la distance entre le point intermediaire et le robot en fin de trajectoire.
	 * 
	 * @param lookAheadBis Distance en mm.
	 */
	void setLookAheadBis(float lookAheadBis);
	/**
	 * @brief Getter de la direction courant.
	 * 
	 * @return Direction Sens utilisé.
	 */
	Direction getDirection() const {return m_direction;}
	/**
	 * @brief Getter de l'angle final.
	 * 
	 * Retourne l'angle final que le robot va atteindre en fin de trajectoire.
	 * 
	 * 
	 * @return float Angle final en rad.
	 */
	float getFinalAngle() const {return m_finalAngle;}
	/**
	 * @brief Getter d'un point de passage.
	 * 
	 * Retourne le point de passage sous la forme d'un Waypoint. L'index permet d'identifier le point à retourner.
	 * 
	 * @param index Numéro du point à retourner.
	 * @return Waypoint& Point de passage demandé.
	 */
	const Waypoint& getWaypoint(int index) const {return m_waypoints[index];}
	/**
	 * @brief Getter du nombre de point de passage.
	 * 
	 * @return int Nombre de points de la ligne brisée.
	 */
	int getNumWaypoints() const {return m_numWaypoints;}
	/**
	 * @brief Getter du lookahead.
	 * 
	 * @return float LookaHead en mm.
	 */
	float getLookAhead()    const {return m_lookAhead;}
	/**
	 * @brief Getter du LookAHead de fin de trajectoire.
	 * 
	 * @return float LookaHeadbis en mm.
	 */
	float getLookAheadBis() const {return m_lookAheadBis;}
	/**
	 * @brief Charge les paramètres sauvegardés.
	 * 
	 * @param address Adresse à utiliser.
	 */
	void load(int address);
	/**
	 * @brief Sauvegarde les paramètres actuels.
	 * 
	 * @param address Adresse à utiliser.
	 */
	void save(int address) const;

protected:

	virtual void computeVelSetpoints(float timestep);
	virtual bool getPositionReached();

	/**
	 * @brief Calcul le point intermediaire sur la ligne brisée.
	 * 
	 * Calcul le point intermediaire sur la courbe et met à jour le segment courant.
	 * 
	 * @param x Coordonnées x du robot (mm).
	 * @param y Coordonnées y du robot (mm).
	 * @return true Un point a été trouvé.
	 * @return false Aucun point n'a été trouvé.
	 */
	bool checkLookAheadGoal(const float x, const float y);
	/**
	 * @brief Calcule le point intermediaire sur la ligne brisée.
	 * 
	 * Calcule le point le plus près du robot sur la ligne brisée non parcouru.
	 * 
	 * @param x Coordonnées x du robot (mm).
	 * @param y Coordonnées y du robot (mm).
	 */
	void checkProjectionGoal(const float x, const float y);

	/**
	 * @brief Retourne la distance restante à parcourir.
	 * 
	 * @return float Distance en mm à parcourir.
	 */
	float getDistAfterGoal();

	// Trajectory specifications
	Waypoint m_waypoints[PUREPURSUIT_MAX_WAYPOINTS];/*!< Liste des points de la ligne brisée à suivre.*/
	int m_numWaypoints; /*!< Nombre de points constituant la ligne brisée.*/
	Direction m_direction;/*!< Sens du robot pendant la trajectoire.*/
	float m_finalAngle;/*!< Angle à atteindre en fin de trajectoire.*/

	// Computation variables
	int m_goalIndex; /*!< Index courant.*/
	float m_goalParam;/*!< Position relative de la projection du robot sur le segment courant.*/
	bool m_goalReached;/*!< Arrivé ou non.*/

	// Path following tunings
	float m_lookAhead;/*!< Distance entre le point intermediaire et le robot.*/
	float m_lookAheadBis;/*!< Distance entre le point intermediaire et le robot en fin de trajectoire.*/
};

#endif // __PUREPURSUIT_H__
