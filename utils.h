#ifndef UTILS_H
#define	UTILS_H

#define DEBUG 0
/**
 * Permet de Savoir si n appartien a l'intervalle [borneinf;bornesup]
 * @param n le chiffre
 * @param borneinf la borne inferieur
 * @param bornesup la borne superieur
 * @return true si n appartien a l'intervalle.
 */
bool inline estcompris(int n, int borneinf, int bornesup);

/**
 * Retourne le maximum de deux entier
 * @param a
 * @param b
 * @return le maximum
 */
bool inline max(int a, int b);

/**
 * Retourne la valeur absolue d'un entier.
 * @param n
 * @return la valeur absolue
 */
int inline abs(int n);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

