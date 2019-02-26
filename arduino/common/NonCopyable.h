#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

/**
 * @brief Classe a hériter pour empécher la copie de cette dernière.
 * 
 */
class NonCopyable
{
protected:

	NonCopyable(){}

private:
	/**
	 * @brief Constructeur par recopie défini en temps que constructeur privé.
	 * 
	 * @param other Objet à copier.
	 */
	NonCopyable(const NonCopyable& other);

	/**
	 * @brief Surcharge de l'opérateur d'égalité inutile pour des variables sans copie (donc unique).	 *  
	 * @param other Objet à compérer.
	 */
	NonCopyable& operator=(const NonCopyable& other);
};

#endif // __NONCOPYABLE_H__
