#ifndef __POSITIONCONTROLLER_H__
#define __POSITIONCONTROLLER_H__

#include "PeriodicProcess.h"
#include "Odometry.h"


class AbstractMoveStrategy;
/**
 * @brief Classe support des objets AbstractMoveStrategy.
 *
 * PositionController est le support des AbstractMoveStrategy. C'est à dire qu'il permet de charger ou supprimer une stratégie.
 * Quand PositionController execute une stratégie de mouvement, il va l'executé tous les time_steps pour y obtenir de nouvelles vitesses à suivre.
 * PositionConstroller va égalemenr renseigné la position du robot à AbstractMoveStrategy chargée.
 * 
 * @return class PositionController : public PeriodicProcess { public: 
 */
class PositionController : public PeriodicProcess
{
public:
	/**
	 * @brief Constructeur de PositionController
	 * Initialise les variables de PositionController à des valeurs neutre.
	 */
	PositionController() : m_linVelKp(1), m_angVelKp(1), m_linVelMax(1000), m_angVelMax(2 * M_PI){}
	/**
	 * @brief Charge les nouvelles positions du robot.
	 * Charge les nouvelles positions du robot pour les donner à une potentiel AbstractMoveStrategy chargée.
	 * @param posInput Nouvelle objet Position représentant la position du robot.
	 */
	void setPosInput   (const Position& posInput)   {m_posInput    = posInput;}
	/**
	 * @brief Charge la position à atteindre.
	 * 
	 * Charge la position à atteindre avec une AbstractMoveStrategy.
	 * Cette variable est potentiellement utilisé par l'AbstractMoveStrategy.
	 * 
	 * @param posSetpoint Position à atteindre
	 */
	void setPosSetpoint(const Position& posSetpoint){m_posSetpoint = posSetpoint;}
	/**
	 * @brief Charge l'angle à atteindre.
	 * 
	 * Change l'angle objectif de l'objet Position. 
	 * Remarque : les coordonnées objectif reste les mêmes. 
	 * 
	 * @param theta Nouvelle angle objectif.
	 */
	void setThetaSetpoint(float theta){m_posSetpoint.theta = theta;}
	/**
	 * @brief Retourne la vitesse linéaire à atteindre.
	 * 
	 * Cette méthode retourne la vitesse linéaire que l'AbstractMoveStrategy souhaite atteindre.
	 * 
	 * @return Vitesse à atteindre en mm/s.
	 */	
	float getLinVelSetpoint() const {return m_linVelSetpoint;}
	/**
	 * @brief Retourne la vitesse angulaire à atteindre.
	 * 
	 * Cette méthode retourne la vitesse angulaire que l'AbstractMoveStrategy souhaite atteindre.
	 * 
	 * @return Vitesse angulaire à atteindre en rad/s.
	 */	
	float getAngVelSetpoint() const {return m_angVelSetpoint;}
	/**
	 * @brief Paramètre les coéfficients linéaire.
	 * 
	 * Paramètre de nouvelles valeurs pour les coefficients linéaire de vitesse et vitesse de rotation.
	 * Ces coefficients seront appliqués lors du calcul de vitesses à atteindre.
	 * 
	 * @param linVelKp Coefficient proportionnel de vitesse linéaire.
	 * @param angVelKp Coefficient proportionnel de vitesse angulaire.
	 */
	void setVelTunings(float linVelKp, float angVelKp);
	/**
	 * @brief Paramètre les vitesses max.
	 * 
	 * Paramètre des vitesses maximals qui dois être appliquer à l'AbstractMoveStrategy.
	 * 
	 * @param linVelMax Vitesse linéaire max.
	 * @param angVelMax Vitesse angulaire max.
	 */
	void setVelLimits(float linVelMax, float angVelMax);
	/**
	 * @brief Paramètre les précisions en position.
	 * 
	 * Paramètre les valeurs de précision pour l'AbstractMoveStrategy. Si l'erreur de position est inférieur, l' AbstractMoveStrategy est arrêté.
	 * 
	 * @param linPosThreshold Précision en coordonnés cartésiens (en mm).
	 * @param angPosThreshold Précision d'angle (en rad).
	 */
	void setPosThresholds(float linPosThreshold, float angPosThreshold);
	/**
	 * @brief Charge une stratégie de mouvement.
	 * 
	 * Charge la stratégie de mouvement (AbstractMoveStrategy) passée en paramètre.
	 * Elle sera active après l'activation de PositionController ( PositionController::enable).
	 * 
	 * @param moveStrategy AbstractMoveStrategy à utiliser.
	 */
	void setMoveStrategy(AbstractMoveStrategy& moveStrategy);

	/**
	 * @brief Indique si la position est atteinte.
	 * 
	 * Permet de savoir si la position objectif est atteinte avec la précision souhaitée.
	 * 
	 * @return true Position atteinte.
	 * @return false Position non atteinte.
	 */
	bool getPositionReached();
	/**
	 * @brief Retourne le coef proportionnel de vitesse linéaire.
	 * @return Coefficient proportionnel (sans unité).
	 */
	float getLinVelKp() const {return m_linVelKp;}
	/**
	 * @brief Retourne le coef proportionnel de vitesse angulaire.
	 * @return Coefficient proportionnel (sans unité).
	 */
	float getAngVelKp() const {return m_angVelKp;}
	/**
	 * @brief Retourne la vitesse max linéaire.
	 * @return Vitesse max en mm/s.
	 */
	float getLinVelMax() const {return m_linVelMax;}
	/**
	 * @brief Retourne la vitesse max angulaire.
	 * @return Vitesse  angulaire max en rad/s.
	 */
	float getAngVelMax() const {return m_angVelMax;}
	/**
	 * @brief Retourne la précision cartésienne.
	 * 
	 * @return Précision cartésienne en mm.
	 */
	float getLinPosThreshold() const {return m_linPosThreshold;}
	/**
	 * @brief Retourne la précision angulaire.
	 * 
	 * @return Précision angulaire en rad.
	 */
	float getAngPosThreshold() const {return m_angPosThreshold;}
	/**
	 * @brief Charge les configs
	 * 
	 * Charge les configurations de la mémoire de l'Arduino avec l'adresse indiqué en paramètre.
	 * 
	 * @param address Adresse à utiliser.
	 */
	void load(int address);
	/**
	 * @brief Sauvegarde la configuration actuel.
	 * @param address Adresse à utiliser.
	 */
	void save(int address) const;

private:
	/**
	 * @brief Execute la AbstractMoveStrategy chargé. 
	 * 
	 * Méthode implémenté de PeriodicProcess qui lance le calcul des nouveaux ordres de vitesse de l'AbstractMoveStrategy.
	 * 
	 * @param timestep Temps depuis le dernier appel.
	 */
	virtual void process(float timestep);
	virtual void onProcessEnabling();

	// IO
	Position m_posInput;/*!< Position du robot.*/
	Position m_posSetpoint;/*!< Position à atteindre.*/

	float m_linVelSetpoint;/*!< Vitesse linéaire à atteindre*/
	float m_angVelSetpoint;/*!< Vitesse angulaire à atteindre*/

	// Engineering control tunings
	float m_linVelKp; /*!< Coefficient proportionnel de vitesse linéaire. */
	float m_angVelKp; /*!< Coefficient proportionnel de vitesse angulaire. */
	float m_linVelMax;/*!< Vitesse linéaire max.*/
	float m_angVelMax;/*!< Vitesse angulaire max.*/
	float m_linPosThreshold;/*!< Précision cartésienne. */
	float m_angPosThreshold;/*!< Précision angulaire.*/

	// Strategy Design Pattern
	AbstractMoveStrategy* m_moveStrategy;/*!< Pointeur de stratégie utilisé.*/

	friend class AbstractMoveStrategy;
};

/**
 * @brief Interface de Stratégie de mouvement.
 * 
 * Interface à implémenter pour réaliser une classe de strategie de mouvement. 
 */
class AbstractMoveStrategy
{
protected:
	/**
	 * @brief Calcul les nouvelles vitesses désirer.
	 * 
	 * Méthode à implémenter pour réaliser une AbstractMoveStrategy. Cette méthode calcul à partir de la position du robot des vitesses à suivre pour le robot.
	 * 
	 * @param timestep Temps depuis le dernier appel en secondes.
	 * 
	 */
	virtual void computeVelSetpoints(float timestep) = 0;
	/**
	 * @brief Indique si la position désirée est atteinte.
	 * 
	 * Calcul la distance entre la position du robot et la position désirée selon le mode de calcul de l'AbstractMoveStrategy.
	 * 
	 * @return true Si la position est atteinte.
	 * @return false Si la position n'est pas atteinte.
	 */
	virtual bool getPositionReached() = 0;
	/**
	 * @brief Retourne la position du robot.
	 * 
	 * Retourne la position du robot stocker dans le PositionController.
	 * 
	 * @return La position du robot sous la struct Position.
	 */
	const Position& getPosInput()    const {return m_context->m_posInput;}
	/**
	 * @brief  Retourne la position à atteindre.
	 * @return Position à atteindre.
	 */
	const Position& getPosSetpoint() const {return m_context->m_posSetpoint;}
	/**
	 * @brief Charge une nouvelle vitesse pour le robot.
	 * 
	 * @param linVelSetpoint Vitesse linéaire en mm/s.
	 * @param angVelSetpoint Vitesse angulaire en rad/s.
	 */
	void setVelSetpoints(float linVelSetpoint, float angVelSetpoint){m_context->m_linVelSetpoint = linVelSetpoint; m_context->m_angVelSetpoint = angVelSetpoint;}
	/**
	 * @brief Retourne le coef proportionnel de vitesse linéaire.
	 * @return Coefficient proportionnel (sans unité).
	 */
	float getLinVelKp() const {return m_context->m_linVelKp;}
	/**
	 * @brief Retourne le coef proportionnel de vitesse angulaire.
	 * 
	 * @return Coefficient proportionnel (sans unité).
	 */
	float getAngVelKp() const {return m_context->m_angVelKp;}
	/**
	 * @brief Retourne vitesse linéaire max.
	 * 
	 * @return Vitesse en mm/s.
	 */
	float getLinVelMax() const {return m_context->m_linVelMax;}
	/**
	 * @brief Retourne vitesse angulaire max.
	 * 
	 * @return Vitesse en rad/s.
	 */
	float getAngVelMax() const {return m_context->m_angVelMax;}
	/**
	 * @brief Retourne la précision cartésienne à atteindre.
	 * 
	 * 
	 * @return Précision en mm.
	 */
	float getLinPosThreshold() const {return m_context->m_linPosThreshold;}
	/**
	 * @brief Retourne la précision angulaire à atteindre.
	 * 
	 * 
	 * @return Précision en rad.
	 */
	float getAngPosThreshold() const {return m_context->m_angPosThreshold;}

protected:

	PositionController* m_context;/*!< Pointeur du PositionControlleur associé.*/

	friend class PositionController;
};

#endif // __POSITIONCONTROLLER_H__
