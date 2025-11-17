
#ifndef ROOM_MANAGEMENT_H
#define ROOM_MANAGEMENT_H

#define MAX_SIZE 100

typedef struct {
	char name[MAX_SIZE];
	int size;
	int id;
	int available; // 1 = YES, 0 = NO
} room;

// Prototypes des fonctions de gestion des salles
void show_all_room();
void add_room();
void edit_room();
void delete_room(); // rend la salle indisponible
void room_management_menu();

#endif // ROOM_MANAGEMENT_H
