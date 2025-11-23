#ifndef USEFUL_FONCTION_H
#define USEFUL_FONCTION_H

#include "room_management.h"
#include "client_management.h"
#include "reservation_room.h"

// Exits the application with a message
void exit_application();
// Returns to the previous menu
void return_menu();

// Wrappers for generic entity management
room_user* load_rooms_util(int* out_count);
void save_rooms_util(room_user* rooms, int count);
void free_rooms_util(room_user* rooms);
client_user* load_clients_util(int* out_count);
void save_clients_util(client_user* clients, int count);
void free_clients_util(client_user* clients);
ReservationRoom* load_reservations_util(int* out_count);
void save_reservations_util(ReservationRoom* reservations, int count);
void free_reservations_util(ReservationRoom* reservations);

// Input utility functions
void read_string(const char *prompt, char *buffer, int size);
int read_int(const char *prompt);
void print_error(const char *msg);
void print_success(const char *msg);
int is_empty(const char *str);

#endif // USEFUL_FONCTION_H