#ifndef ROOM_MANAGEMENT_H
#define ROOM_MANAGEMENT_H

#define ROOM_NAME_SIZE 100

typedef struct room_user {
	int id;
	char name[ROOM_NAME_SIZE];
	int capacity;
	int available; // 1 = available, 0 = not available
} room_user;

// Dynamic room array and count
extern room_user *rooms;
extern int room_count;

// Dynamic room management function prototypes
void load_rooms();
void save_rooms();
void add_room_dynamic(room_user new_room);
void free_rooms();


// Room management function prototypes
void show_all_rooms();
void search_room();
void add_room();
void edit_room(room_user *room); // Prototype matches implementation
void delete_room(room_user *room); // Prototype matches implementation
void room_management_menu();



#endif // ROOM_MANAGEMENT_H
