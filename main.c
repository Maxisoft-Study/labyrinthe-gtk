
#include <gtk/gtk.h> 
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "Labyrinthe.h"
#include "Controler.h"

/** A mettre a jour si on change le fichier glad*/
#define PIECE_HAUTEUR_EN_NOMBRE_DE_PIXEL 120
/** A mettre a jour si on change le fichier glad*/
#define PIECE_LARGEUR_EN_NOMBRE_DE_PIXEL 120

/**
 * GLOBAL
 */
/**
 * Pointeur sur le jeu.
 */
Jeu_t *JP = NULL;
/**
 * True si on veut empercher les controls.
 */
bool LOCK_WINDOW = false;

GtkBuilder *builder = NULL;

GtkImage* ECRAN[JEU_HAUTEUR][JEU_LARGEUR];

gboolean on_MainWindow_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
gboolean on_MainWindow_button_press_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
const gchar * piece_to_imgfilepath(piece_t);


void ECRAN_load_from_Jeu(Jeu_t *jp);
void quick_message(gchar *message, GtkWindow* parent);
gchar* quick_filechoose_dialog(GtkWindow* parent);

/**
 * Fonction d'initialisation de la global ECRAN.
 * A appeler 1 seul fois (normalment)
 * @param builder l'object qui contient le glade parsée.
 */
void inline ECRAN_init(GtkBuilder *builder, Jeu_t *jp) {
    size_t i, j;
    size_t k = 1;

    char* curr_img_name = calloc(strlen("image99") + 1, sizeof (char));
    //1st reset
    for (i = 0; i < JEU_LARGEUR; ++i) {
        sprintf(curr_img_name, "imaget%u", i + 1);
        gtk_image_set_from_file(GTK_IMAGE(gtk_builder_get_object(builder, curr_img_name)), "./ress/blank.png");
        sprintf(curr_img_name, "imageb%u", i + 1);
        gtk_image_set_from_file(GTK_IMAGE(gtk_builder_get_object(builder, curr_img_name)), "./ress/blank.png");
    }

    for (i = 0; i < JEU_HAUTEUR; ++i) {
        for (j = 0; j < JEU_LARGEUR; ++j) {
            sprintf(curr_img_name, "image%u", k);
            ++k;
            ECRAN[i][j] = GTK_IMAGE(gtk_builder_get_object(builder, curr_img_name));
        }
    }
    //rond
    sprintf(curr_img_name, "imaget%u", jp->start_pos.colone + 1);
    gtk_image_set_from_file(GTK_IMAGE(gtk_builder_get_object(builder, curr_img_name)), "./ress/top.png");

    //carre
    sprintf(curr_img_name, "imageb%u", jp->end_pos.colone + 1);
    gtk_image_set_from_file(GTK_IMAGE(gtk_builder_get_object(builder, curr_img_name)), "./ress/bot.png");

    free(curr_img_name);
    ECRAN_load_from_Jeu(jp);
}

void ECRAN_load_from_Jeu(Jeu_t *jp) {
    size_t i, j;
    GtkImage* curr_img;
    piece_t curr_piece;
    for (i = 0; i < JEU_HAUTEUR; ++i) {
        for (j = 0; j < JEU_LARGEUR; ++j) {
            curr_piece = jp->mat[i][j];
            curr_img = ECRAN[i][j];
            gtk_image_set_from_file(curr_img, piece_to_imgfilepath(curr_piece));
        }
    }
}

/**
 * 
 * @param self
 * @return 
 */
const gchar * piece_to_imgfilepath(piece_t self) {
    char* curr_img_name = malloc((strlen("./ress/1111_1.png") + 1) * sizeof (char));
    sprintf(curr_img_name, "./ress/%i%i%i%i_%i.png", self.up, self.rigth, self.down, self.left, self.higthligth);
    //printf(curr_img_name);
    return curr_img_name; //TODO
}

int main(int argc, char *argv []) {
    Jeu_t j;
    Jeu_t *jp = &j;

    Jeu_init(jp, ALL_PIECE, START_POS, END_POS);
    GtkWidget *fenetre_principale = NULL;
    
    GError *error = NULL;
    gchar *filename = NULL;
    /* Initialisation de la librairie Gtk. */
    gtk_init(&argc, &argv);

    /* Ouverture du fichier Glade de la fenêtre principale */
    builder = gtk_builder_new();

    /* Création du chemin complet pour accéder au fichier main.glade. */
    /* g_build_filename(); construit le chemin complet en fonction du système */
    /* d'exploitation. ( / pour Linux et \ pour Windows) */
    filename = g_build_filename("main.glade", NULL);

    /* Chargement du fichier main.glade. */
    gtk_builder_add_from_file(builder, filename, &error);
    g_free(filename);
    if (error) {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free(error);
        return code;
    }

    /* Récupération du pointeur de la fenêtre principale */
    fenetre_principale = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));

    //initialisation de la matrice global ECRAN
    ECRAN_init(builder, jp);
    bool fin = calcul_du_chemin(jp);
    ECRAN_load_from_Jeu(jp);
    JP = jp;
    /* Affectation des signaux de l'interface aux différents CallBacks. (Glade)*/
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_add_events(fenetre_principale, GDK_BUTTON_PRESS_MASK); //pour accepter les click.
    g_signal_connect(fenetre_principale, "key_press_event", G_CALLBACK(on_MainWindow_key_press_event), NULL);
    g_signal_connect(fenetre_principale, "button-press-event", G_CALLBACK(on_MainWindow_button_press_event), NULL);

    /* Affichage de la fenêtre principale. */
    gtk_widget_show_all(fenetre_principale);
    //quick_message("Cool story bro !", GTK_WINDOW(fenetre_principale));
    gtk_main();

    return EXIT_SUCCESS;
}

/**
 * Fonction appeler pour la gestion des click.
 * @param widget
 * @param event
 * @param user_data
 * @return 
 */
gboolean on_MainWindow_button_press_event(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    //printf("Called !\n");
    size_t i, j, k;
    i = (int) (event->motion.x / PIECE_LARGEUR_EN_NOMBRE_DE_PIXEL);
    j = (int) (event->motion.y / PIECE_HAUTEUR_EN_NOMBRE_DE_PIXEL) - 1;
    pos_t clicked = {j, i};
    pos_t next;

    //bruteforce 
    Direction_e all_dir[4] = {haut, droite, bas, gauche};
    Direction_e curr_dir;
    for (k = 0; k < 4; k++) {
        curr_dir = all_dir[k];
        next = compute_next_position(clicked, curr_dir);
        if (!LOCK_WINDOW && position_compare(JP->piece_noire_pos, clicked) && Jeu_deplace(JP, next,clicked )) {
            Jeu_increment_compteur(JP); //on incremmente le compteur
            pos_t next = compute_next_position(JP->piece_noire_pos, haut);
            bool fin = calcul_du_chemin(JP);
            if (fin) {
                char*msg = malloc(strlen("Bravo, vous avez gagnez en 10000") + 1);
                sprintf(msg, "Bravo, vous avez gagnez en %u coup%s", JP->count, (JP->count) > 1 ? "s" : "");
                quick_message(msg, GTK_WINDOW(widget));
                free(msg);
                LOCK_WINDOW = true;
            }
            ECRAN_load_from_Jeu(JP);
            return FALSE;
        }

    }

    return FALSE;
}

/**
 * Fonction appeler pour la gestion du clavier
 * @param widget
 * @param event
 * @param user_data
 * @return 
 */
gboolean on_MainWindow_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    switch (event->keyval) {
        case GDK_KP_8:
        case GDK_z:
        case GDK_w:
        case GDK_Up:
            printf("key pressed: %s\n", "Up");
            if (!LOCK_WINDOW && deplacement_possible(haut, JP)) {
                Jeu_increment_compteur(JP); //on incremmente le compteur
                pos_t next = compute_next_position(JP->piece_noire_pos, haut);
                Jeu_deplace(JP, JP->piece_noire_pos, next);
                bool fin = calcul_du_chemin(JP);
                if (fin) {
                    char*msg = malloc(strlen("Bravo, vous avez gagnez en 10000") + 1);
                    sprintf(msg, "Bravo, vous avez gagnez en %u coup%s", JP->count, (JP->count) > 1 ? "s" : "");
                    quick_message(msg, GTK_WINDOW(widget));
                    free(msg);
                    LOCK_WINDOW = true;
                }
                ECRAN_load_from_Jeu(JP);
            }
            break;



        case GDK_KP_2:
        case GDK_s:
        case GDK_Down:
            printf("key pressed: %s\n", "Down");
            if (!LOCK_WINDOW && deplacement_possible(bas, JP)) {
                Jeu_increment_compteur(JP); //on incremmente le compteur
                pos_t next = compute_next_position(JP->piece_noire_pos, bas);
                Jeu_deplace(JP, JP->piece_noire_pos, next);
                bool fin = calcul_du_chemin(JP);
                if (fin) {
                    char*msg = malloc(strlen("Bravo, vous avez gagnez en 10000") + 1);
                    sprintf(msg, "Bravo, vous avez gagnez en %u coup%s", JP->count, (JP->count) > 1 ? "s" : "");
                    quick_message(msg, GTK_WINDOW(widget));
                    free(msg);
                    LOCK_WINDOW = true;
                }
                ECRAN_load_from_Jeu(JP);
            }
            break;



        case GDK_KP_4:
        case GDK_q:
        case GDK_a:
        case GDK_Left:
            printf("key pressed: %s\n", "Left");
            if (!LOCK_WINDOW && deplacement_possible(gauche, JP)) {
                Jeu_increment_compteur(JP); //on incremmente le compteur
                pos_t next = compute_next_position(JP->piece_noire_pos, gauche);
                Jeu_deplace(JP, JP->piece_noire_pos, next);
                bool fin = calcul_du_chemin(JP);
                if (fin) {
                    char*msg = malloc(strlen("Bravo, vous avez gagnez en 10000 coups") + 1);
                    sprintf(msg, "Bravo, vous avez gagnez en %u coup%s", JP->count, (JP->count) > 1 ? "s" : "");
                    quick_message(msg, GTK_WINDOW(widget));
                    free(msg);
                    LOCK_WINDOW = true;
                }
                ECRAN_load_from_Jeu(JP);
            }
            break;


        case GDK_KP_6:
        case GDK_d:
        case GDK_Right:
            printf("key pressed: %s\n", "Rigth");
            if (!LOCK_WINDOW && deplacement_possible(droite, JP)) {
                Jeu_increment_compteur(JP); //on incremmente le compteur
                pos_t next = compute_next_position(JP->piece_noire_pos, droite);
                Jeu_deplace(JP, JP->piece_noire_pos, next);
                bool fin = calcul_du_chemin(JP);
                if (fin) {
                    char*msg = malloc(strlen("Bravo, vous avez gagnez en 10000") + 1);
                    sprintf(msg, "Bravo, vous avez gagnez en %u coup%s", JP->count, (JP->count) > 1 ? "s" : "");
                    quick_message(msg, GTK_WINDOW(widget));
                    free(msg);
                    LOCK_WINDOW = true;
                }
                ECRAN_load_from_Jeu(JP);
            }
            break;

        case GDK_o: //open a file
            if (event->state & GDK_CONTROL_MASK) { //ctrl + o
                //printf("key pressed: %s\n", "ctrl + o");
                gchar *file_str = quick_filechoose_dialog(GTK_WINDOW(widget)); // ! Malloced string !!!
                
                printf("%s\n", file_str); //TODO Faire quelque chose avec ce file_str :)
                LOCK_WINDOW = false;

                g_free(file_str);
            } else {
                printf("key pressed: %s\n", "o");
            }
            break;

        case GDK_KP_1://RESET
            Jeu_init(JP,ALL_PIECE,START_POS,END_POS);
            bool fin = calcul_du_chemin(JP);
            ECRAN_init(builder,JP);
            LOCK_WINDOW = false;
            break;
            
        case GDK_KP_0:
        case GDK_Escape:
            gtk_main_quit();
            break;
        default:
            return FALSE;
    }
}

/*Fonction pour ouvrir une boîte de dialogue qui affiche un message  */
void quick_message(gchar *message, GtkWindow* parent) {
    GtkWidget *dialog, *label;

    /* Créer les widgets */
    dialog = gtk_dialog_new_with_buttons("Message",
            parent,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_STOCK_OK,
            GTK_RESPONSE_NONE,
            NULL);
    label = gtk_label_new(message);

    /* S'assurer que la boîte de dialogue est détruite quand l'utilisateur a répondu. */
    g_signal_connect_swapped(dialog,
            "response", G_CALLBACK(gtk_widget_destroy),
            dialog);

    /*Ajouter le label, et afficher tout ce qui a été ajouté à la boîte de dialogue. */
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
            label);
    gtk_widget_show_all(dialog);
}

/*Fonction pour ouvrir une boîte de dialogue qui demande un fichier  */
gchar* quick_filechoose_dialog(GtkWindow* parent) {
    GtkWidget *dialog;
    /* Créer les widgets */
    dialog = gtk_file_chooser_dialog_new("Open File",
            parent,
            GTK_FILE_CHOOSER_ACTION_OPEN,
            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
            NULL);
    char *ret = "";
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        ret = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }
    /* S'assurer que la boîte de dialogue est détruite quand l'utilisateur a répondu. */
    gtk_widget_destroy(dialog);
    return ret;
}