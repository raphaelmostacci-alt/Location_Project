
#ifndef ROOM_MANAGEMENT_H
#define ROOM_MANAGEMENT_H

#define ROOM_NAME_SIZE 100

typedef struct room_user {
	int id;
	char name[ROOM_NAME_SIZE];
	int capacity;
	int available; // 1 = available, 0 = not available
} room_user;


// Prototypes des fonctions de gestion des salles
void show_all_rooms();
void search_room();
void add_room();
void edit_room();
void delete_room();
void room_management_menu();

#endif // ROOM_MANAGEMENT_H
