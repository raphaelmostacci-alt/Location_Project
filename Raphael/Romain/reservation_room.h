
#ifndef RESERVATION_ROOM_H
#define RESERVATION_ROOM_H

#include "includes.h"

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

// Prototypes des fonctions de gestion des réservations
void show_available_room();
void show_room_locked_by_client();
void book_reservation();
void edit_reservation();
void delete_reservation();
void reservation_room_menu();

#endif // RESERVATION_ROOM_H
