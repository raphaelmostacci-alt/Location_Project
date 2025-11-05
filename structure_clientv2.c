
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE1 20
#define TAILLE2 50
#define ok 0


typedef struct {
    int numero;
    char nom[TAILLE1];
    char prenom[TAILLE1];
    char adresse[TAILLE2];
    int age;
} Etudiant;

typedef struct {
    int numero;
    int cotisation;
} Cotise;


// Global file pointers for members and fees
FILE *F_Etud = NULL;
    FILE *F_Cot = NULL;

// Prototypes for member management
void open_member_files();
void input_member(Etudiant*);
void display_member(const Etudiant*);
void save_member(const Etudiant*);
void save_fee(const Cotise*);
int get_last_member_number();
void insert_new_member();
void show_unpaid_members();
void member_menu();

// Main function

int main(int argc, char** argv)
{
    member_menu();
    return ok;
}

// Function to open member files
void open_member_files() {
    F_Etud = fopen("Etudiants.dat", "r+b");
    if (F_Etud == NULL) {
        F_Etud = fopen("Etudiants.dat", "w+b");
        if (F_Etud == NULL) {
            printf("Error opening Etudiants.dat\n");
            exit(-1);
        }
    }
    F_Cot = fopen("Cotise.dat", "r+b");
    if (F_Cot == NULL) {
        F_Cot = fopen("Cotise.dat", "w+b");
        if (F_Cot == NULL) {
            printf("Error opening Cotise.dat\n");
            exit(-1);
        }
    }
}

// Function to input a member
void input_member(Etudiant* member) {
    printf("Entrez le nom : ");
    scanf("%s", member->nom);
    printf("Entrez le prénom : ");
    scanf("%s", member->prenom);
    printf("Entrez l'adresse : ");
    scanf("%s", member->adresse);
    printf("Entrez l'âge : ");
    scanf("%d", &member->age);
}

// Function to display a member
void display_member(const Etudiant* member) {
    printf("Numéro: %d\tNom: %s\tPrénom: %s\tAdresse: %s\tÂge: %d\n",
        member->numero, member->nom, member->prenom, member->adresse, member->age);
}

// Function to save a member to file
void save_member(const Etudiant* member) {
    if (F_Etud == NULL) {
        printf("Member file not open\n");
        return;
    }
    fseek(F_Etud, 0, SEEK_END);
    fwrite(member, sizeof(Etudiant), 1, F_Etud);
}

// Function to save fee to file
void save_fee(const Cotise* fee) {
    if (F_Cot == NULL) {
        printf("Fee file not open\n");
        return;
    }
    fseek(F_Cot, 0, SEEK_END);
    fwrite(fee, sizeof(Cotise), 1, F_Cot);
}

// Function to get last member number
int get_last_member_number() {
    Etudiant temp;
    int last_number = 0;
    if (F_Etud == NULL) return 0;
    fseek(F_Etud, 0, SEEK_SET);
    while (fread(&temp, sizeof(Etudiant), 1, F_Etud) == 1) {
        last_number = temp.numero;
    }
    return last_number;
}

// Function to insert a new member
void insert_new_member() {
    Etudiant new_member;
    Cotise new_fee;
    int last_number;

    last_number = get_last_member_number();
    new_member.numero = last_number + 1;
    input_member(&new_member);
    display_member(&new_member);
    save_member(&new_member);

    new_fee.numero = new_member.numero;
    new_fee.cotisation = 0; // Non réglée
    save_fee(&new_fee);
    printf("Nouvel adhérent inséré avec le numéro %d\n", new_member.numero);
}

// Function to show unpaid members
void show_unpaid_members() {
    Cotise fee;
    Etudiant member;
    if (F_Cot == NULL || F_Etud == NULL) {
        printf("Files not open\n");
        return;
    }
    fseek(F_Cot, 0, SEEK_SET);
    while (fread(&fee, sizeof(Cotise), 1, F_Cot) == 1) {
        if (fee.cotisation == 0) {
            // Recherche de l'étudiant dans Etudiants.dat
            fseek(F_Etud, 0, SEEK_SET);
            while (fread(&member, sizeof(Etudiant), 1, F_Etud) == 1) {
                if (member.numero == fee.numero) {
                    display_member(&member);
                    break;
                }
            }
        }
    }
}

// Member menu function
void member_menu() {
    int choice = 0;
    open_member_files();
    do {
        printf("\nMember Menu:\n");
        printf("1. Insert new member\n");
        printf("2. Show unpaid members\n");
        printf("0. Exit member menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                insert_new_member();
                break;
            case 2:
                show_unpaid_members();
                break;
            case 0:
                printf("Exiting member menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
    if (F_Etud != NULL) { fclose(F_Etud); F_Etud = NULL; }
    if (F_Cot != NULL) { fclose(F_Cot); F_Cot = NULL; }
}