
#ifndef ROOM_MANAGEMENT_H
#define ROOM_MANAGEMENT_H

#define ROOM_NAME_SIZE 100

typedef struct room_user {
	int id;
	char name[ROOM_NAME_SIZE];
	int capacity;
	int available; // 1 = available, 0 = not available
} room_user;


// Room management function prototypes
void show_all_rooms();
void search_room();
void add_room();
void edit_room(room_user *room); // Prototype matches implementation
void delete_room(room_user *room); // Prototype matches implementation
void room_management_menu();

// TODO: When integrating with reservation_room, add prototypes for cross-module checks (e.g., invalidate reservations when a room is deleted).

#endif // ROOM_MANAGEMENT_H
