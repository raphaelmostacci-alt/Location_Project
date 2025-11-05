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

    if(file!=NULL)
    {
        fclose(file);
        file=NULL;
    }

    return ok;
}

// Function to open the file
void open_file(FILE** clientFile)
{
    fprintf(stdout, "open_file: Begin\n");

    *clientFile=fopen("client_list.txt", "r+b");
    if(*clientFile==NULL)
    {
        fprintf(stdout, "Error opening file r+b\n");

        *clientFile=fopen("client_list.txt", "w+b");
        if(*clientFile==NULL)
        {
            fprintf(stdout, "Error opening file w+b. Exiting program\n");
            exit(-1);
        }
    }
    fprintf(stdout, "open_file: End\n");
}

// Function to add a client
void add_client(FILE* file)
{
    client new_client;
    int nb_elements=0, read_items=0, i=0;

    if(file==NULL)
        printf("Error creating file\n");
    else
    {
        // Move to the end of the file to append new client
        fseek(file,0,SEEK_SET);
        do
        {
            memset(&new_client,0,sizeof(client));
            read_items=fread(&new_client,sizeof(client),1,file);
            i++;
        }while(!feof(file) && read_items!=0);

        printf("\nLast Name: ");
        scanf("%s", new_client.last_name);
        printf("\nFirst Name: ");
        scanf("%s", new_client.first_name);
        printf("\nPhone Number: ");
        scanf("%ld", &new_client.phone_number);

        nb_elements=fwrite(&new_client,sizeof(client),1,file);
        // Check if write was successful
        if(nb_elements!=1)
            printf("Error writing to file\n");
        else
        {
            printf("Client added successfully\n");
        }
    }
} 

// Function to show the number of clients
void show_number_client(FILE* file)
{
    client current_client;
    int read_items=0;

    if(file==NULL)
        printf("Error opening file\n");
    else
    {
        fseek(file,0,SEEK_SET);
        do
        {
            memset(&current_client,0,sizeof(client));
            read_items=fread(&current_client,sizeof(client),1,file);

            if(read_items!=0)
                printf("Last Name: |%s| \tFirst Name: |%s| \tPhone Number: |%ld|\n", current_client.last_name, current_client.first_name, current_client.phone_number);
        }while(!feof(file) && read_items!=0);
    }
}

// Function to search for a client
void search_client(FILE* file, client* who_client)
{
    printf("\nEnter the last name of the client to search: ");
    scanf("%s", who_client->last_name);
    printf("\nEnter the first name of the client to search: ");
    scanf("%s", who_client->first_name);
}

// Function to find a client's number in function of their last and first name
void find_number_client(FILE* file, client who_client)
{
    int read_items=0, found=0;
    client current_client;

    if(file==NULL)
        printf("Error opening file\n");
    else
    {
        fseek(file,0,SEEK_SET);
        do
        {
            memset(&current_client,0,sizeof(client));
            read_items=fread(&current_client,sizeof(client),1,file);

            if(read_items!=0) 
                found++;
        }while(!feof(file) && read_items!=0 && (strcmp(current_client.last_name, who_client.last_name)!=0 || strcmp(current_client.first_name, who_client.first_name)!=0));

        if(read_items==0 || feof(file))
            printf("\nThe searched client was not found in the file\n");
        else
        {
            printf("Last Name: |%s| \tFirst Name: |%s| \tPhone Number: |%ld|\n", current_client.last_name, current_client.first_name, current_client.phone_number);
        }
    }
}

// Function to change a client's number
void change_number_client(FILE* file, client who_client)
{
    int nb_elements=0;

    find_number_client(file, who_client);

    printf("\nEnter the new phone number: ");
    scanf("%ld", &who_client.phone_number);

    // Move the file pointer to the position of the found client
    fseek(file, (nb_elements-1)*sizeof(client), SEEK_CUR);

    // Update the totality of the client's phone number in the file
    nb_elements=fwrite(&who_client, sizeof(client), 1, file);
    if(nb_elements!=1)
        printf("Error updating phone number\n");
    else
    {
        printf("Phone number updated successfully\n");
        printf("New details - Last Name: |%s| \tFirst Name: |%s| \tPhone Number: |%ld|\n", who_client.last_name, who_client.first_name, who_client.phone_number);
    }
}

// Menu function
void menu(FILE* file)
{
    int choice=0;
    client current_client;

    // Menu loop
    do
    {
        printf("\nMenu:\n");
        printf("1. Add a client\n");
        printf("2. Show all clients\n");
        printf("3. Search for a client\n");
        printf("4. Change a client's number\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        // User choice
        switch(choice)
        {
            case 1:
                add_client(file); break;
            case 2:
                show_number_client(file); break;
            case 3:
                search_client(file, &current_client); find_number_client(file, current_client); break;
            case 4:
                search_client(file, &current_client); change_number_client(file, current_client); break;
            case 0:
                printf("Exiting program.\n"); break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }while(choice!=0);
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