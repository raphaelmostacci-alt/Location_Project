#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20
#define ok 0

typedef struct client
{
    char last_name[MAX];
    char first_name[MAX];
    long phone_number;
} client;

#define SIZE_NAME 20
#define SIZE_ADDRESS 50

typedef struct {
    int number; // member number
    char last_name[SIZE_NAME];
    char first_name[SIZE_NAME];
    char address[SIZE_ADDRESS];
    int age;
} Etudiant;

typedef struct {
    int number; // member number
    int fee;
} Cotise;

// Global file pointers for members and fees
FILE *F_Etud = NULL;
FILE *F_Cot = NULL;

// Function prototypes
void open_file(FILE**);
void add_client(FILE*);
void show_number_client(FILE*);
void search_client(FILE*, client*);
void find_number_client(FILE*, client);
void change_number_client(FILE*, client);
void menu(FILE*);

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
    FILE* file=NULL;

    open_file(&file);
    menu(file);

    // Member management menu
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
    printf("Enter last name: ");
    scanf("%s", member->last_name);
    printf("Enter first name: ");
    scanf("%s", member->first_name);
    printf("Enter address: ");
    scanf("%s", member->address);
    printf("Enter age: ");
    scanf("%d", &member->age);
}

// Function to display a member
void display_member(const Etudiant* member) {
    printf("Number: %d\tLast Name: %s\tFirst Name: %s\tAddress: %s\tAge: %d\n",
        member->number, member->last_name, member->first_name, member->address, member->age);
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
        last_number = temp.number;
    }
    return last_number;
}

// Function to insert a new member
void insert_new_member() {
    Etudiant new_member;
    Cotise new_fee;
    int last_number;

    last_number = get_last_member_number();
    new_member.number = last_number + 1;
    input_member(&new_member);
    display_member(&new_member);
    save_member(&new_member);

    new_fee.number = new_member.number;
    new_fee.fee = 0; // Not paid yet
    save_fee(&new_fee);
    printf("New member inserted with number %d\n", new_member.number);
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
        if (fee.fee == 0) {
            // Find member in Etudiants.dat
            fseek(F_Etud, 0, SEEK_SET);
            while (fread(&member, sizeof(Etudiant), 1, F_Etud) == 1) {
                if (member.number == fee.number) {
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