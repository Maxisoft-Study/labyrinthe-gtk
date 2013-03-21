
#include <gtk/gtk.h> 
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "Labyrinthe.h"

/** A mettre a jour si on change le fichier glad*/
#define PIECE_HAUTEUR_EN_NOMBRE_DE_PIXEL 120
/** A mettre a jour si on change le fichier glad*/
#define PIECE_LARGEUR_EN_NOMBRE_DE_PIXEL 120



GtkImage* ECRAN[JEU_HAUTEUR][JEU_LARGEUR];

gboolean on_MainWindow_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
const gchar * piece_to_imgfilepath(piece_t);
/**
 * Fonction d'initialisation de la global ECRAN.
 * A appeler 1 seul fois (normalment)
 * @param builder l'object qui contient le glade parsée.
 */
void inline ECRAN_init(GtkBuilder *builder);
void ECRAN_load_from_Jeu(Jeu_t *jp);
void quick_message(gchar *message, GtkWindow* parent);
gchar* quick_filechoose_dialog(GtkWindow* parent);

void inline ECRAN_init(GtkBuilder *builder) {
    size_t i, j;
    size_t k = 1;
    char* curr_img_name = calloc(strlen("image99") + 1, sizeof (char));
    for (i = 0; i < JEU_HAUTEUR; ++i) {
        for (j = 0; j < JEU_LARGEUR; ++j) {
            sprintf(curr_img_name, "image%u", k);
            ++k;
            ECRAN[i][j] = GTK_IMAGE(gtk_builder_get_object(builder, curr_img_name));
        }
    }
    free(curr_img_name);
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
    return "test.png"; //TODO
}

int main(int argc, char *argv []) {
    Jeu_t j;
    Jeu_t *jp = &j;
    GtkWidget *fenetre_principale = NULL;
    GtkBuilder *builder = NULL;
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
    ECRAN_init(builder);

    ECRAN_load_from_Jeu(jp);
    /* Affectation des signaux de l'interface aux différents CallBacks. (Glade)*/
    gtk_builder_connect_signals(builder, NULL);

    g_signal_connect(fenetre_principale, "key_press_event", G_CALLBACK(on_MainWindow_key_press_event), NULL);

    /* Affichage de la fenêtre principale. */
    gtk_widget_show_all(fenetre_principale);
    quick_message("Cool story bro !", GTK_WINDOW(fenetre_principale));
    gtk_main();

    return EXIT_SUCCESS;
}

gboolean on_MainWindow_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    switch (event->keyval) {
        case GDK_KP_8:
        case GDK_z:
        case GDK_w:
        case GDK_Up:
            printf("key pressed: %s\n", "Up");
            break;
            
            
            
        case GDK_KP_2:
        case GDK_s:
        case GDK_Down:
            printf("key pressed: %s\n", "Down");
            break;
            
            
            
        case GDK_KP_4:
        case GDK_q:
        case GDK_a:
        case GDK_Left:
            printf("key pressed: %s\n", "Left");
            break;
            
            
        case GDK_KP_6:
        case GDK_d:
        case GDK_Right:
            printf("key pressed: %s\n", "Rigth");
            break;

        case GDK_o: //open a file
            if (event->state & GDK_CONTROL_MASK) { //ctrl + o
                printf("key pressed: %s\n", "ctrl + o");
                printf("%s\n", quick_filechoose_dialog(GTK_WINDOW(widget)));
            } else {
                printf("key pressed: %s\n", "o");
            }
            break;
            
            
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