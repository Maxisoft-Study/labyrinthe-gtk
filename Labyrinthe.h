/* 
 * File:   Labyrinthe.h
 *
 * Created on 16 mars 2013, 18:57
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
 * LA Structure principale.
 * 
 * On utilise ici des champs de bits.
 * voir http://cpp.developpez.com/cours/cpp/?page=page_5#LV-A-4 pour plus d'info.
 * 
 * De plus comme dans la plusparts des systems / compilateur, par defaut,
 * les struct prenne au minimum 8 bit (1 octet), 
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
	
	/** 1 si la case doit être eclairé en jaune (si le chemin est possible). */
	bool higthligth : 1;

	unsigned char flags : 3; // on peut mettre ce que l'on veut dans ce flags
};

/**
 * Alias de la structure piece.
 */
typedef struct piece piece_t;

/**
 * Permet de serialiser une piece.
 */
union piece_to_int {
	struct piece p;
	unsigned int c; //on peut mettre char a la place de int.
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
 * Structure qui definit une Position dans une matrice.
 * Note :	le type utilisé doit etre signée.
 *			On a choisi ici un 'int8_t'
 * Limite : l'indice de la  ligne / colone doit être compris entre [-128;127] (largement suffisant normalement)
 */
struct pos {
	int8_t ligne;
	int8_t colone;
};

/**
 * Alias de la structure pos.
 */
typedef struct pos pos_t;

/**
 * 
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
 * Alias de la structure Jeu.
 */
typedef struct Jeu Jeu_t;



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





#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* LABYRINTHE_H */

