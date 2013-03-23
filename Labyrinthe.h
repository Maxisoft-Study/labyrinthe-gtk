/** @file
 */
#include <stdint.h>
#include "bool.h" //permet d'avoir les booleans comme type

#ifndef LABYRINTHE_H
#define	LABYRINTHE_H


/**La hauteur du plateau de Jeu*/
#define JEU_HAUTEUR 4

/**La Largeur du plateau de Jeu*/
#define JEU_LARGEUR 5
/**
 * Alias de l'enumeration Direction.
 */
typedef enum Direction Direction_e;
/**
 * Alias de la structure piece.
 */
typedef struct piece piece_t;

/**
 * Alias de la structure pos.
 */
typedef struct pos pos_t;

/**
 * Alias de la structure Jeu.
 */
typedef struct Jeu Jeu_t;



//
//
//
//				PIECE
//
//
//




/**
 * @brief Representation de l'element atomique du labyrinthe.
 * 
 * 
 * LA Structure principale.
 * 
 * On utilise ici des champs de bits.
 * voir http://cpp.developpez.com/cours/cpp/?page=page_5#LV-A-4 pour plus d'info.
 * 
 * De plus comme dans la pluspars des systemes / compilateurs, par defaut,
 * les struct prenne au minimum 8 bits (1 octet), 
 * on defini d'autre booleens utiles.
 * 
 * 
 * Ainsi la taille de cette structure est de 1 octet
 */
struct piece {
	/** 1 si deplacement vers le haut possible ; 0 sinon . */
	bool up : 1;

	/** 1 si deplacement vers la droite possible. */
	bool rigth : 1;

	/** 1 si deplacement vers le bas possible.*/
	bool down : 1;

	/** 1 si deplacement vers la gauche possible. */
	bool left : 1;

	/** 1 si la case doit être eclairé en jaune ( ie si le chemin est possible). */
	bool higthligth : 1;

	/** 1 si la case doit être eclairé en jaune seulement verticalement (pour la piece croisement). */
	bool higthligth_vertical : 1;

	/** 1 si la case doit être eclairé en jaune seulement horizontalement (pour la piece croisement). */
	bool higthligth_horizontal : 1;

	unsigned char unusedflags : 1;
};


// PIECE RELATED METHODS


/*
 * Notes Generales :
 * - le premier arguments (self) represente la piece.
 */

/**
 * Savoir si la piece est un croisement (ie {true,true,true,true} ) .
 * @param self la piece.
 * @return true si la piece est un croisement.
 */
inline bool piece_est_croisement(piece_t self);

/**
 * Savoir si la piece est la case noire (ie {false,false,false,false} )
 * @param self la piece.
 * @return true (1) si la piece est la case noire.
 */
inline bool piece_est_noire(piece_t self);

/**
 * Permet d'enlever une direction a une piece.
 * On obtient ainsi une piece qui ne contient qu'un booleen de direction*
 * Note : * Ne fonctionne pas correctement avec les pieces croisements.
 * @param self la piece 
 * @param dir la direction a supprimer
 * @return une copie de la piece modifier
 */
piece_t piece_remove_dir(piece_t self, Direction_e dir);

/**
 * Permet de 'convertir' une piece en tant que direction.
 * La piece doit être construit avec la fonction piece_remove_dir.
 * Note : Ne fonctionne pas correctement avec les pieces croisements.
 * @param p la piece a convertir
 * @return la direction correspondant a la piece.
 */
Direction_e piece_as_Dir(piece_t p);


/**
 * Verification de la compatibilite de deux pieces.
 * @param p1 une piece
 * @param p2 une autre piece
 * @param dir la direction qu'a p1 par rapport a p2
 * @return true si les deux pieces sont compatible. false sinon
 */
bool pieces_compatibles(piece_t p1, piece_t p2, Direction_e dir);


/**
 * Calcule un string represantant une piece (a l'aide d'un buffer).
 * @param dir La piece
 * @return le string represantant une piece
 */
char* piece_str(piece_t self, char *buff);








//
//
//
//				Position
//
//
//






/**
 * @brief Position dans une matrice.
 * 
 * 
 * Note : le type utilisé est signée.
 * 
 * Limite : l'indice de la  ligne / colone doit être compris entre [-128;127] (largement suffisant normalement)
 */
struct pos {
	int8_t ligne;
	int8_t colone;
};




/**
 * Comparateur de position.
 * Priorité lors de la comparaison sur la colonne.
 * Renvoie 0 si positions egal, -1 si pos2>pos1, 1 si pos1>pos2
 * @param pos1 
 * @param pos2
 * @return 0 si positions egal, -1 si pos2>pos1, 1 si pos1>pos2
 */
int position_compare(pos_t pos1, pos_t pos2);


/**
 * Calcule un string represantant une position (a l'aide d'un buffer).
 * @param dir La position
 * @return le string represantant une position
 */
char* pos_str(pos_t self, char *buff);






//
//
//
//				JEU
//
//
//


/**
 * @brief L'object representant le Jeu.
 * Il contient une matrice de piece.
 */
struct Jeu {
	/** 
	 * La matrice du jeu.
	 * Element principal de cette structure.
	 */
	piece_t mat[JEU_HAUTEUR][JEU_LARGEUR];

	/**Nombre de coup jouer.*/
	unsigned int count;

	/**
	 * La position de depart. 
	 * note : normalement a {-1, X} (ou X represente la colone)
	 */
	pos_t start_pos;

	/**
	 * La position de fin.
	 * note : normalement a {JEU_HAUTEUR, X} (ou X represente la colone)
	 */
	pos_t end_pos;

	/**
	 * La position de la case noir.
	 */
	pos_t piece_noire_pos;
};





/**
 * Initialise un Jeu.
 * @param this pointeur sur le jeu
 * @param array l'array a convertir en matrice
 * @param start_pos la position de depart (le rond bleu sur les figures)
 * @param end_pos la position de fin (le carre sur les figures)
 * @return true si l'initialisation s'est bien passer.
 */
bool Jeu_init(Jeu_t * this, piece_t *array, pos_t start_pos, pos_t end_pos);

/**
 * Permet de definir une piece a une certaine position dans un Jeu.
 * @param this pointeur sur le jeu
 * @param _pos la position
 * @param p la piece
 */
void inline Jeu_set_piece(Jeu_t * this, pos_t _pos, piece_t p);

/**
 * Permet de connaitre la piece a la postion donnée.
 * @param this pointeur sur le jeu
 * @param _pos la position
 * @return la piece a la postion donnée
 */
piece_t inline Jeu_get_piece(Jeu_t * this, pos_t _pos);


/**
 * Effectue un deplacement.
 * Renvoie false si deplacement impossible ( aucune des postions données reprensente une case noire).
 * Note : l'ordre des deux derniers parametres n'est PAS important.
 * @param this pointeur sur le jeu
 * @param src_pos une position
 * @param dest_pos une autre position
 * @return true si le deplacement s'est effectuer.
 */
bool Jeu_deplace(Jeu_t * this, pos_t src_pos, pos_t dest_pos);


/**
 * Mise a zero des boolean de surlignement (higthlight) de toutes les cases d'un Jeu.
 * @param this le pointeur sur le Jeu.
 */
void Jeu_reset_higthligth(Jeu_t *this);


/**
 * Incremente le compteur.
 * @param this le pointeur sur le jeu.
 */
inline void Jeu_increment_compteur(Jeu_t *this);
//
//
//
//				DIRECTION
//
//
//

/**
 * \enum Direction
 * @brief l'enumeration des directions possible.
 * 
 */
enum Direction {
	haut = 1, droite, bas, gauche
};



/**
 * Inversion de la direction.
 * exemple : direction_reverse(haut) -> bas
 * @param dir la direction a inversée
 * @return la direction inversée . ou 0 si erreur
 */
Direction_e direction_reverse(Direction_e dir);

/**
 * Retourne un string represantant une direction.
 * @param dir La direction
 * @return le string represantant une direction
 */
const char * direction_str(Direction_e dir);







//
//
//
//				GLOBALS
//
//
//

/**
 * Globale contenant un backup du dernier chargement de piece.
 * La valeur par defaut est definit dans Labyrinthe.c.
 * Cette globals est a modifier lorsque l'on chargera un fichier.
 * \see la structure piece | la fonction 'Jeu_init'
 */
extern piece_t ALL_PIECE[20];
/**
 * Globale contenant un backup de la position du cerle (le debut du parcours).
 * La valeur par defaut est definit dans Labyrinthe.c.
 * Cette globals est a modifier lorsque l'on chargera un fichier.
 * \remark la ligne doit etre egal a -1
 * \see la structure piece | la fonction 'Jeu_init'
 */
extern pos_t START_POS;
/**
 * Globale contenant un backup de la position du carré (la fin du parcours).
 * La valeur par defaut est definit dans Labyrinthe.c.
 * Cette globals est a modifier lorsque l'on chargera un fichier.
 * \remark la ligne doit etre egal a JEU_HAUTEUR
 * \see la structure piece | la fonction 'Jeu_init'
 */
extern pos_t END_POS;

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* LABYRINTHE_H */

