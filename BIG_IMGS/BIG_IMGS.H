#include <big2.h>

/* Fonction de chargement et dispatching */
int load_image (char *fichier, MFDB *imag);

/* Fonctions par formats d'image */
int load_img (char *fichier, MFDB *imag);
int load_pac (char *fichier, MFDB *imag);
int load_pix (char *fichier, MFDB *imag);
int load_pcx (char *fichier, MFDB *imag);

/* Fonction de d‚compactage et de lien avec un objet B_PICT */
int img_attach (t_image_dial *bimg, char *fichier, int tree, int obj, int fcenterw, int fcenterh);

