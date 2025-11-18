#include "administrator.h"
#include "client_management.h"
#include "includes.h"
#include "useful_fonction.h"

void account_authentification();

int main()
{
    load_clients();
    account_authentification();
    exit_application();
}


void account_authentification()
{
    int authenticated = 0;
    int quit = 0;
    while (!authenticated && !quit)
    {
    char last_name[MAX_SIZE];
    char first_name[MAX_SIZE];

        printf("==== Meeting Room Login ====\n");
            printf("Enter your last name (or type EXIT to quit): ");
        fgets(last_name, 50, stdin);
        last_name[strcspn(last_name, "\n")] = 0;
            if (strcmp(last_name, "EXIT") == 0)
            {
                exit_application();
            }

            printf("Enter your first name (or type EXIT to quit): ");
        fgets(first_name, 50, stdin);
        first_name[strcspn(first_name, "\n")] = 0;
            if (strcmp(first_name, "EXIT") == 0)
            {
                exit_application();
            }

        int user_exists = search_client(last_name, first_name);
        int is_admin = ((strcmp(last_name, "BLACHERE") == 0 && strcmp(first_name, "Romain") == 0) ||
                        (strcmp(last_name, "MOSTACCI") == 0 && strcmp(first_name, "Raphael") == 0));

        if (user_exists)
        {
            printf("Welcome, %s %s!\n", first_name, last_name);
            if (is_admin) 
            {
                printf("You are an administrator.\n");
                administrator_menu();
            } 
            else 
            {
                printf("You are a regular user.\n");
                // Set current user email for reservation module
                for (int i = 0; i < client_count; i++) {
                    if (strcmp(clients[i].last_name, last_name) == 0 && strcmp(clients[i].first_name, first_name) == 0) {
                        extern char current_user_email[MAX_SIZE];
                        strncpy(current_user_email, clients[i].mail, MAX_SIZE);
                        break;
                    }
                }
                reservation_room_menu();
            }
        }
        else
        {
            printf("Account not found.\n");
            printf("1. Create a new account\n2. Try again\n3. Exit Application\nChoose an option: ");
            int choice = 0;
            scanf("%d", &choice);
            getchar(); // consume newline
            if (choice == 1)
            {
                int before = search_client(last_name, first_name);
                add_client();
                int after = search_client(last_name, first_name);
                if (!before && after)
                {
                    printf("Account created. Please login again.\n");
                }
            }
            else if (choice == 3)
            {
                exit_application();
            }
        }
    }
}