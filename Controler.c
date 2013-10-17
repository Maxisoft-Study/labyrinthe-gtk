#include <stdlib.h>
#include <stdio.h>
#include "bool.h"
#include "Labyrinthe.h"
#include "Controler.h"
#include "utils.h"

/**
 * Retourne une postion suivant une position et une direction donnée.
 * Exemple : compute_next_position( {1,3}, droite) -> {1,4}
 * Note : Fait pour l'utilisation matricielle donc haut et bas sont inversés automatiquement.
 * @param startpos la position initiale
 * @param dir la direction
 * @return position calcule
 */
pos_t compute_next_position(pos_t startpos, Direction_e dir) {
    pos_t ret;
    if (dir == haut) {
        ret.ligne = startpos.ligne - 1;
        ret.colone = startpos.colone;
    } else if (dir == droite) {
        ret.ligne = startpos.ligne;
        ret.colone = startpos.colone + 1;
    } else if (dir == bas) {
        ret.ligne = startpos.ligne + 1;
        ret.colone = startpos.colone;
    } else if (dir == gauche) {
        ret.ligne = startpos.ligne;
        ret.colone = startpos.colone - 1;
    }
    return ret;
}

/**
 * Calcul recursif de la validite d'un chemin.
 * @param startpos position initiale
 * @param dir direction du deplacement
 * @param jp pointeur sur le jeu
 * @return true si le chemin arrive a la position finale du jeu.
 */
bool calcul_du_chemin_rec(pos_t startpos, Direction_e dir, Jeu_t* jp) {
    pos_t next_pos = compute_next_position(startpos, dir);
    /*printf("nextpos %s\n", pos_str(next_pos, buff));*/
    /*printf("dir : %s\n", direction_str(dir));*/
    piece_t curr_piece;
    if (estcompris(startpos.ligne, 0, JEU_HAUTEUR - 1)) {
        curr_piece = Jeu_get_piece(jp, startpos);
        curr_piece.higthligth = true;
        Jeu_set_piece(jp, startpos, curr_piece);
    }


    if (position_compare(next_pos, jp->end_pos) == 0) { // Le jeu est finit
        /*printf("GG\n");*/
        return true;
    }
    if (next_pos.ligne < 0) { // on essaye de se deplacer hors du tableau
        /*printf("limitpos_min\n");*/
        return false;
    }
    if (next_pos.colone < 0) { // on essaye de se deplacer hors du tableau
        /*printf("limitpos_min\n");*/
        return false;
    }

    if (next_pos.ligne >= JEU_LARGEUR) {// on essaye de se deplacer hors du tableau
        /*printf("limitpos_max\n");*/
        return false;
    }
    if (next_pos.colone >= JEU_HAUTEUR) {// on essaye de se deplacer hors du tableau
        /*printf("limitpos_max\n");*/
        return false;
    }


    piece_t new_piece = Jeu_get_piece(jp, next_pos);
    /*printf("new piece : %s\n", piece_str(new_piece, buff));*/
    Direction_e next_dir;
    if (piece_est_croisement(new_piece)) { //on fait a la main le cas d'une piece croisement

        if (dir == haut) {
            new_piece.higthligth_vertical = true;
            next_dir = haut;
        } else if (dir == droite) {
            new_piece.higthligth_horizontal = true;
            next_dir = droite;
        } else if (dir == bas) {
            new_piece.higthligth_vertical = true;
            next_dir = bas;
        } else if (dir == gauche) {
            new_piece.higthligth_horizontal = true;
            next_dir = gauche;
        }

        Jeu_set_piece(jp, next_pos, new_piece);

    } else {

        Direction_e reverse_dir = direction_reverse(dir);

        piece_t onedir_piece = piece_remove_dir(new_piece, reverse_dir);
        /*printf("onedir piece : %s\n", piece_str(onedir_piece, buff));*/
        next_dir = piece_as_Dir(onedir_piece);
    }
    if (estcompris(startpos.ligne, 0, JEU_HAUTEUR - 1) && !pieces_compatibles(curr_piece, new_piece, dir)) {
        /*printf("incompatible : estcompris : %i, pieces_compatibles : %i\n", estcompris(startpos.ligne, 0, JEU_HAUTEUR - 1), pieces_compatibles(curr_piece, new_piece, dir));*/
        return false;
    }
    /*printf("next dir : %s \n", direction_str(next_dir));*/
    /*printf("------------\n\n");*/
    return calcul_du_chemin_rec(next_pos, next_dir, jp);
}

/**
 * Lance le calcul recursif de la validite d'un chemin sur un jeu.
 * @param jp le pointeur sur le jeu
 * @return 
 */
bool inline calcul_du_chemin(Jeu_t* jp) {
    Jeu_reset_higthligth(jp);
    return calcul_du_chemin_rec(jp->start_pos, bas, jp);
}

/**
 * Lit stdin et effectue les actions necessaire.
 * @param jp le pointeur sur le jeu
 * @return une Direction . 0 si aucune dirrection a ete parsée.
 */
Direction_e parse_stdin(Jeu_t *jp) {
    char buffer[1];
    scanf("%c", buffer);
    scanf("%*[^\n]s");
    getchar();
    switch (*buffer) {
        case '8':
            return haut;
        case '6':
            return droite;
        case'2':
            return bas;
        case '4':
            return gauche;
        case '0':
            printf("A bientot :)\n");
            fflush(stdout); //force print avant
            exit(EXIT_SUCCESS);
        case '1':
            Jeu_init(jp, ALL_PIECE, START_POS, END_POS);
            return -1;

    }
    return 0;
}

/**
 * Verification si deplacement possible.
 * @param dir la direction
 * @param jp le pointeur sur le jeu
 * @return true si le deplacement est possible
 */
inline bool deplacement_possible(Direction_e dir, Jeu_t* jp) {
    pos_t next_pos = compute_next_position(jp->piece_noire_pos, dir);
    return estcompris(next_pos.ligne, 0, JEU_HAUTEUR - 1) && estcompris(next_pos.colone, 0, JEU_LARGEUR - 1);
}

