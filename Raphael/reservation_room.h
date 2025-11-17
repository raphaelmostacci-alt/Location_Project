
#ifndef RESERVATION_ROOM_H
#define RESERVATION_ROOM_H

#define TAILLE_MAX 100

typedef struct {
	int day;
	int month;
	int year;
} Date;

typedef struct {
	char client_mail[TAILLE_MAX];
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
