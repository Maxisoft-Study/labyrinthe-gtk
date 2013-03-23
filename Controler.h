/*! \file */ 
#include "bool.h"
#include "Labyrinthe.h"
#ifndef CONTROLER_H
#define	CONTROLER_H

/**
 * Retourne une postion suivant une position et une direction donnée.
 * Exemple : compute_next_position( {1,3}, droite) -> {1,4}
 * Note : Fait pour l'utilisation matricielle donc haut et bas sont inversés automatiquement.
 * @param startpos la position initiale
 * @param dir la direction
 * @return position calcule
 */
pos_t compute_next_position(pos_t startpos, Direction_e dir);

/**
 * Calcul recursif de la validite d'un chemin.
 * @param startpos position initiale
 * @param dir direction du deplacement
 * @param jp pointeur sur le jeu
 * @return true si le chemin arrive a la position finale du jeu.
 */
bool calcul_du_chemin_rec(pos_t startpos, Direction_e dir, Jeu_t* jp);


/**
 * Lance le calcul recursif de la validite d'un chemin sur un jeu.
 * @param jp le pointeur sur le jeu
 * @return 
 */
bool inline calcul_du_chemin(Jeu_t* jp);


/**
 * Lit stdin et effectue les actions necessaire.
 * @param jp le pointeur sur le jeu
 * @return une Direction . 0 si aucune dirrection a ete parsée.
 */
Direction_e parse_stdin(Jeu_t *jp);


/**
 * Verification si deplacement possible.
 * @param dir la direction
 * @param jp le pointeur sur le jeu
 * @return true si le deplacement est possible
 */
inline bool deplacement_possible(Direction_e dir, Jeu_t* jp);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* CONTROLER_H */

