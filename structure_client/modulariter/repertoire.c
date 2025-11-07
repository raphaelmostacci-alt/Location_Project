#include "librairies_rep.h"
#include "prototypes_rep.h"
#include "prototypes_utiles.h"
#include "prototypes_principales.h"

int main(int argc, char** argv)
{
    FILE* file=NULL;

    open_file(&file);
    menu(file);

    if(file!=NULL)
    {
        fclose(file);
        file=NULL;
    }

    return ok;
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