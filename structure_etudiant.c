/* -------------------------------------------------------
   Gestion simple des adhérents BDE (version débutant)
   - Fichiers binaires : Etudiants.dat, Cotise.dat
   - Tout est écrit et lu séquentiellement (pas de trucs compliqués)
   ------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/* Tailles pour les chaînes */
#define TAILLE1 20
#define TAILLE2 50
 
/* ========= STRUCTURES demandées ========= */
typedef struct {
    int numero;                 /* numero d'adhérent */
    char nom[TAILLE1];
    char prenom[TAILLE1];
    char adresse[TAILLE2];
    int age;
} Etudiant;
 
typedef struct {
    int numero;                 /* numero d'adhérent */
    int cotisation;             /* 0 si non réglée, >0 si payée */
} Cotise;
 
/* ========= Variables globales : deux pointeurs de fichiers ========= */
FILE *F_Etud = NULL;
FILE *F_Cot  = NULL;
 
void saisirAdherent (int num, Etudiant *e, Cotise *c);
void afficheAdherent (Etudiant *e, Cotise *c);
void sauvegarde (Etudiant *e, Cotise *c);
int getNumero ();
void ajoutAdherent (Etudiant *e, Cotise *c);
void adhrentNonCotisant ();
int menu ();
 
int main () {
    int num = 0;
    Etudiant e;
    Cotise c;
    int choix;
 
    do {
        choix = menu();
        switch (choix){
        case 0:
            printf("vous avez quité le programme");
            break;
        case 1:
            saisirAdherent(num, &e, &c);
            sauvegarde (&e, &c);
            break;
        case 2:
            printf("%-10s | %-15s | %-15s | %-30s | %-5s | %-10s\n", "Numero", "Nom", "Prenom", "Adresse", "Age", "Cotisation");
            afficheAdherent(&e, &c);
            break;
        case 3:
            ajoutAdherent (&e, &c);
            break;
        case 4:
            adhrentNonCotisant ();
            break;
        default:
            break;
        }
    } while (choix != 0);
    return 0;
}
 
int menu () {
    int choix;
    printf("1. Saisir un Adherent\n");
    printf("2. Afficher les Adherents\n");
    printf("3. Ajouter un adherent\n");
    printf("4. Afficher les non cotisant\n");
    printf("Choix : ");
    scanf("%d", &choix);
 
    return choix;
 
 
}
 
void saisirAdherent (int num, Etudiant *e, Cotise *c) {
    /* saisie du nom */
    printf ("Saisir nom : ");
    scanf ("%s", e->nom);
    /* saisie du prenom */
    printf("Saisir prenom : ");
    scanf ("%s", e->prenom);
    getchar();
    /* saisie de l'adresse */
    printf("Saisir adresse : ");
    scanf ("%[^\n]", e->adresse);
 
    /* saisie de l'age */
    printf("Saisir l'age : ");
    scanf ("%d", &e->age);
 
    /* saisie de la cotisation */
    printf ("Cotisation : ");
    scanf ("%d", &c->cotisation);
 
    /* numéro adéhrent cotisant */
    e->numero = num +1;
    c->numero = num +1;
 
}
 
void afficheAdherent (Etudiant *e, Cotise *c) {
    printf("%-10d | %-15s | %-15s | %-30s | %-5d | %-10d\n", e->numero, e->nom, e->prenom, e->adresse, e->age, c->cotisation);
}
 
void sauvegarde (Etudiant *e, Cotise *c) {
    /* ouverture creation des fichiers */
    F_Etud = fopen("Etudiants.dat", "ab+");
    F_Cot = fopen("Cotise.dat", "ab+");
 
    /* verifie si les fichiers sont bien ouvert */
    if (F_Etud == NULL || F_Cot == NULL) {
        exit(0);
    } else {
        /* écrit dans les fichiers */
        fwrite(e, sizeof(Etudiant), 1, F_Etud);
        fwrite(c, sizeof(Cotise), 1, F_Cot);
 
        /* ferme les fichiers */
        fclose(F_Etud);
        fclose(F_Cot);
    }
}
 
int getNumero () {
    Etudiant e;
    Cotise c;
    int last_etud;
    int last_cot;
    /* ouverture creation des fichiers sans écriture*/
    F_Etud = fopen("Etudiants.dat", "rb");
    F_Cot = fopen("Cotise.dat", "rb");
 
    /* verifie si les fichiers sont bien ouvert */
    if (F_Etud == NULL || F_Cot == NULL) {
        printf("les fichiers n'existe pas il faut jouter saisir un adhérent");
        exit(0);
    } else {
        /* Parcours le fichier pour aller chercher le denier numéro*/
        while ((fread(&e, sizeof(Etudiant), 1, F_Etud)!=0) && (fread(&c, sizeof(Cotise), 1, F_Cot)!=0)){
            last_etud = e.numero;
            last_cot = c.numero;
        }
    }
    if (last_cot == last_etud){
        printf ("%d\n", last_etud);
        return last_etud;
    } else {
        printf ("%d\n", last_etud);
        printf ("%d\n", last_cot);
        printf ("le numero d'adherent est different du numero de cotisation\n");
    }
}
 
void ajoutAdherent (Etudiant *e, Cotise *c) {
    int num = getNumero ();
    saisirAdherent(num, e, c);
    afficheAdherent(e, c);
    sauvegarde (e, c);
}
 
void adhrentNonCotisant (){
    Etudiant e;
    Cotise c;
 
    /* ouverture creation des fichiers sans écriture*/
    F_Etud = fopen("Etudiants.dat", "rb");
    F_Cot = fopen("Cotise.dat", "rb");
 
    /* verifie si les fichiers sont bien ouvert */
    if (F_Etud == NULL || F_Cot == NULL) {
        printf("les fichiers n'existe pas il faut jouter saisir un adhérent");
        exit(0);
    } else {
        /* Parcours le fichier pour aller chercher le denier numéro*/
        printf("%-10s | %-15s | %-15s | %-30s | %-5s | %-10s\n", "Numero", "Nom", "Prenom", "Adresse", "Age", "Cotisation");
        while ((fread(&e, sizeof(Etudiant), 1, F_Etud)!=0) && (fread(&c, sizeof(Cotise), 1, F_Cot)!=0)){
            if (c.cotisation == 0){
                afficheAdherent (&e, &c);
            }
        }
    }
}
 