#include "includes.h"
#ifndef RESERVATION_ROOM_H
#define RESERVATION_ROOM_H

typedef struct {
	int day;
	int month;
	int year;
	int hour;
	int minute;
} Date;

typedef struct {
	char client_mail[MAX_SIZE];
	int room_id;
	int reservation_id;
	Date date;
} ReservationRoom;

// Dynamic reservation array and count
extern ReservationRoom *reservations;
extern int reservation_count;
extern char current_user_email[MAX_SIZE];

// Reservation management function prototypes
void show_available_room();
void show_room_locked_by_client();
void book_reservation();
void reservation_room_menu();
void invalidate_reservations_by_client(const char *client_mail);
void invalidate_reservations_by_room(int room_id);
void edit_reservation_id(int reservation_id);
void delete_reservation_id(int reservation_id);
void save_reservations();
void load_reservations();
void free_reservations();

#endif // RESERVATION_ROOM_H