#include <stdio.h>
#include "client_management.h"

void administrator_menu() 
{
	int quit = 0;
	while (!quit) 
    {
		printf("\n==== Administrator Menu ====\n");
		printf("1. Room Management\n");
		printf("2. Client Management\n");
		printf("3. Reservation Management\n");
		printf("4. Exit\n");
		printf("Choose an option: ");
		int choice = 0;
		scanf("%d", &choice);
		getchar(); // consume newline
		switch (choice) 
        {
			case 1:
				// TODO: call room_management_menu();
				printf("Room management selected.\n");
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
				quit = 1;
				printf("Exiting administrator menu.\n");
				break;
			default:
				printf("Invalid option. Try again.\n");
		}
	}
}
// Fonctions pour le menu administrateur
