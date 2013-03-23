#include "Labyrinthe.h"
#include "stdlib.h"
/*LIST DES PIECES 
 *basée sur la figure 1 */
piece_t ALL_PIECE[20] = {
	{1, 0, 0, 1},
	{1, 1, 0, 0},
	{1, 0, 1, 0},
	{0, 1, 0, 1},
	{1, 1, 0, 0},
	{1, 0, 1, 0},
	{1, 1, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 1, 1},
	{0, 1, 0, 1},
	{1, 1, 0, 0},
	{0, 1, 0, 1},
	{1, 1, 1, 1},
	{0, 1, 1, 0},
	{1, 0, 1, 0},
	{0, 1, 0, 1},
	{1, 0, 1, 0},
	{1, 0, 1, 0},
	{0, 1, 1, 0},
	{0, 1, 0, 1}
};
/**
 * Globale contenant un backup de la position du cerle (le debut du parcours).
 */
pos_t START_POS = {-1, 2};

/**
 * Globale contenant un backup de la position du carré (la fin du parcours).
 */
pos_t END_POS = {JEU_HAUTEUR, 2};


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

/**
 * Permet d'enlever une direction a une piece.
 * On obtient ainsi une piece qui ne contient qu'un booleen de direction*
 * Note : * Ne fonctionne pas correctement avec les pieces croisements.
 * @param self la piece 
 * @param dir la direction a supprimer
 * @return une copie de la piece modifier
 */
piece_t piece_remove_dir(piece_t self, Direction_e dir) {
	piece_t ret = self;
	if (dir == haut) {
		ret.up = false;
	}
	if (dir == droite) {
		ret.rigth = false;
	}
	if (dir == bas) {
		ret.down = false;
	}
	if (dir == gauche) {
		ret.left = false;
	}
	return ret;
}

/**
 * Permet de 'convertir' une piece en tant que direction.
 * La piece doit être construit avec la fonction piece_remove_dir.
 * Note : Ne fonctionne pas correctement avec les pieces croisements.
 * @param p la piece a convertir
 * @return la direction correspondant a la piece.
 */
Direction_e piece_as_Dir(piece_t p) {
	if (p.up) {
		return haut;
	}
	if (p.rigth) {
		return droite;
	}
	if (p.down) {
		return bas;
	}
	if (p.left) {
		return gauche;
	}
}

/**
 * Verification de la compatibilite de deux pieces.
 * @param p1 une piece
 * @param p2 une autre piece
 * @param dir la direction qu'a p1 par rapport a p2
 * @return true si les deux pieces sont compatible. false sinon
 */
bool pieces_compatibles(piece_t p1, piece_t p2, Direction_e dir) {
	if (piece_est_croisement(p1)) {
		if (p2.down && dir == haut && p1.higthligth_vertical) {
			return true;
		}
		if (p2.up && dir == bas && p1.higthligth_vertical) {
			return true;
		}
		if (p2.rigth && dir == gauche && p1.higthligth_horizontal) {
			return true;
		}
		if (p2.left && dir == droite && p1.higthligth_horizontal) {
			return true;
		}
		return false;
	}
	//else
	if (p1.up && p2.down && dir == haut) {
		return true;
	}
	if (p1.down && p2.up && dir == bas) {
		return true;
	}
	if (p1.left && p2.rigth && dir == gauche) {
		return true;
	}
	if (p1.rigth && p2.left && dir == droite) {
		return true;
	}
	//else
	return false;
}

/**
 * Calcule un string represantant une piece (a l'aide d'un buffer).
 * @param dir La piece
 * @return le string represantant une piece
 */
char* piece_str(piece_t self, char *buff) {
	if (piece_est_croisement(self)) {
		sprintf(buff, "Piece [up : %i, rigth : %i,down : %i, left : %i, higthligth : %i, vertical : %i]", self.up, self.rigth, self.down, self.left, self.higthligth, self.higthligth_vertical);
	} else {
		sprintf(buff, "Piece [up : %i, rigth : %i,down : %i, left : %i, higthligth : %i]", self.up, self.rigth, self.down, self.left, self.higthligth);
	}
	return buff;
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
	if (start_pos.ligne != -1) {
		return false;
	}
	if (end_pos.ligne != JEU_HAUTEUR) {
		return false;
	}

	bool piece_noir_found = false;
	this->count = 0;
	this->start_pos = start_pos;
	this->end_pos = end_pos;

	int i, j, k;
	k = 0;
	for (i = 0; i < JEU_HAUTEUR; ++i) {
		for (j = 0; j < JEU_LARGEUR; ++j) {
			this->mat[i][j] = array[k];
			k++;
			this->mat[i][j].higthligth = 0;
			this->mat[i][j].higthligth_vertical = 0;
			this->mat[i][j].higthligth_horizontal = 0;
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
void inline Jeu_set_piece(Jeu_t* this, pos_t _pos, piece_t p) {
	this->mat[_pos.ligne][_pos.colone] = p;
}

/**
 * Permet de connaitre la piece a la postion donnée.
 * @param this pointeur sur le jeu
 * @param _pos la position
 * @return la piece a la postion donnée
 */
piece_t inline Jeu_get_piece(Jeu_t* this, pos_t _pos) {
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
bool Jeu_deplace(Jeu_t* this, pos_t src_pos, pos_t dest_pos) {
        if (src_pos.colone < 0 || src_pos.ligne < 0 || dest_pos.colone < 0 || dest_pos.ligne < 0) {
            return false;
        }
        if (src_pos.colone >= JEU_LARGEUR || src_pos.ligne >= JEU_HAUTEUR || dest_pos.colone >= JEU_LARGEUR || dest_pos.ligne >= JEU_HAUTEUR) {
            return false;
        }
	piece_t src_piece = Jeu_get_piece(this, src_pos);
	piece_t dest_piece = Jeu_get_piece(this, dest_pos);
	if (!(piece_est_noire(src_piece) || piece_est_noire(dest_piece))) {
		return false;
	}
	Jeu_set_piece(this, src_pos, dest_piece);
	Jeu_set_piece(this, dest_pos, src_piece);
	if (piece_est_noire(src_piece)) {
		this->piece_noire_pos = dest_pos;
	}
	if (piece_est_noire(dest_piece)) {
		this->piece_noire_pos = src_pos;
	}
	return true;
}

/**
 * Mise a zero des boolean de surlignement (higthlight) de toutes les cases d'un Jeu.
 * @param this le pointeur sur le Jeu.
 */
void Jeu_reset_higthligth(Jeu_t *this) {

	size_t i, j;
	for (i = 0; i < JEU_HAUTEUR; ++i) {
		for (j = 0; j < JEU_LARGEUR; ++j) {
			this->mat[i][j].higthligth = 0;
			this->mat[i][j].higthligth_horizontal = 0;
			this->mat[i][j].higthligth_vertical = 0;
		}
	}
}

/**
 * Incremente le compteur.
 * @param this le pointeur sur le jeu.
 */
inline void Jeu_increment_compteur(Jeu_t *this) {
	this->count += 1;
}



// POSITION RELATED METHODS

/**
 * Comparateur de position.
 * Priorité lors de la comparaison sur la colonne.
 * Renvoie 0 si positions egal, -1 si pos2>pos1, 1 si pos1>pos2
 * @param pos1 
 * @param pos2
 * @return 0 si positions egal, -1 si pos2>pos1, 1 si pos1>pos2
 */
int position_compare(pos_t pos1, pos_t pos2) {
	if (pos1.colone > pos2.colone) {
		return 1;
	}
	if (pos1.colone < pos2.colone) {
		return -1;
	}
	//else pos1.colone == pos2.colone
	if (pos1.ligne < pos2.ligne) {
		return -1;
	}
	if (pos1.ligne > pos2.ligne) {
		return 1;
	}
	//else equals
	return 0;
}

/**
 * Calcule un string represantant une position (a l'aide d'un buffer).
 * @param dir La position
 * @return le string represantant une position
 */
char* pos_str(pos_t self, char *buff) {
	sprintf(buff, "Pos [ligne : %i, colone : %i]", self.ligne, self.colone);
	return buff;
}


// DIRECTION RELATED METHODS


/**
 * Liste des Strings correspondant aux Directions
 */
static const char *DIRSTR[] = {"erreur", "Haut", "Droit", "Bas", "Gauche"};

/**
 * Retourne un string represantant une direction.
 * @param dir La direction
 * @return le string represantant une direction
 */
const char * direction_str(Direction_e dir) {
	return DIRSTR[dir];
}

/**
 * Inversion de la direction.
 * exemple : direction_reverse(haut) -> bas
 * @param dir la direction a inversée
 * @return la direction inversée . ou 0 si erreur
 */
Direction_e direction_reverse(Direction_e dir) {
	if (dir == haut) {
		return bas;
	}
	if (dir == droite) {
		return gauche;
	}
	if (dir == bas) {
		return haut;
	}
	if (dir == gauche) {
		return droite;
	}
	return 0;
}