#include "includes.h"
#include "client_management.h"
#include "room_management.h"
#include "useful_fonction.h"

void administrator_menu() 
{
	int quit = 0;
	while (!quit) 
    {
		printf("\n==== Administrator Menu ====\n");
		printf("1. Room Management\n");
		printf("2. Client Management\n");
		printf("3. Reservation Management\n");
		printf("4. Exit Application\n");
		printf("Choose an option: ");
		int choice = 0;
		scanf("%d", &choice);
		getchar(); // consume newline
		switch (choice) 
        {
			case 1:
				room_management_menu();
				break;
			case 2:
				printf("Client management selected.\n");
				client_management_menu();
				break;
			case 3:
				// TODO: call reservation_room_menu();
				printf("Reservation management selected.\n");
				break;
			case 4:
				exit_application();
				break;
			default:
				printf("Invalid option. Try again.\n");
		}
	}
}
