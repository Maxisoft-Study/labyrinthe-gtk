#include "Labyrinthe.h"

// PIECE RELATED METHODS


/*
 * Notes Generale :
 * - le premier arguments (self) represente la piece.
 */

/**
 * Savoir si la piece est un croisement (ie {true,true,true,true} )
 * @param self la piece.
 * @return true (1) si la piece est un croisement.
 */
inline bool piece_est_croisement(piece_t self) {
	return self.up && self.rigth && self.left && self.down;
}

/**
 * Savoir si la piece est la case noire (ie {false,false,false,false} )
 * @param self la piece.
 * @return true (1) si la piece est la case noire.
 */
inline bool piece_est_noire(piece_t self) {
	return !self.up && !self.rigth && !self.left && !self.down;
}


// JEU RELATED METHODS


/*
 * Notes Generale :
 * - le premier arguments (this) represente le un pointeur sur le jeu.
 */

/**
 * Initialise un Jeu.
 * @param this pointeur sur le jeu
 * @param array l'array a convertir en matrice
 * @param start_pos la position de depart (le rond bleu sur les figures)
 * @param end_pos la position de fin (le carre sur les figures)
 * @return true si l'initialisation s'est bien passer.
 */
bool Jeu_init(Jeu_t* this, piece_t *array, pos_t start_pos, pos_t end_pos) {
	bool piece_noir_found = false;

	this->count = 0;
	this->start_pos = start_pos;
	this->end_pos = end_pos;

	int i, j, k;
	k = 0;
	for (i = 0; i < JEU_HAUTEUR; ++i) {
		for (j = 0; j < JEU_LARGEUR; ++j) {
			this->mat[i][j] = array[k++];
			this->mat[i][j].higthligth = 0;
			this->mat[i][j].flags = 0;
			if ((!piece_noir_found) && piece_est_noire(this->mat[i][j])) {
				this->piece_noire_pos.ligne = i;
				this->piece_noire_pos.colone = j;
				piece_noir_found = true;
			}
		}
	}
	return piece_noir_found;
}

/**
 * Permet de definir une piece a une certaine position dans un Jeu.
 * @param this pointeur sur le jeu
 * @param _pos la position
 * @param p la piece
 */
void inline Jeu_set_piece(Jeu_t* this, pos_t _pos, piece_t p){
	this->mat[_pos.ligne][_pos.colone] = p;
}

/**
 * Permet de connaitre la piece a la postion donnée.
 * @param this pointeur sur le jeu
 * @param _pos la position
 * @return la piece a la postion donnée
 */
piece_t inline Jeu_get_piece(Jeu_t* this, pos_t _pos){
	return this->mat[_pos.ligne][_pos.colone];
}

/**
 * Effectue un deplacement.
 * Renvoie false si deplacement impossible ( aucune des postions données reprensente une case noire).
 * Note : l'ordre des deux derniers parametres n'est PAS important.
 * @param this pointeur sur le jeu
 * @param src_pos une position
 * @param dest_pos une autre position
 * @return true si le deplacement s'est effectuer.
 */
bool Jeu_deplace(Jeu_t* this, pos_t src_pos, pos_t dest_pos){
	piece_t src_piece = Jeu_get_piece(this, src_pos);
	piece_t dest_piece = Jeu_get_piece(this, dest_pos);
	if ( !(piece_est_noire(src_piece) || piece_est_noire(dest_piece))){
		return false;
	}
	Jeu_set_piece(this, src_pos, dest_piece);
	Jeu_set_piece(this, dest_pos, src_piece);
	return true;
}