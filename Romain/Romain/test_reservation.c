// Test program for reservation_room module
#include "includes.h"
#include "reservation_room.h"
#include "room_management.h"
#include "client_management.h"

int main() {
    printf("===========================================\n");
    printf("  TEST - Reservation Room Module\n");
    printf("===========================================\n\n");
    
    // Launch the reservation menu directly
    reservation_room_menu();
    
    printf("\nTest completed. Exiting...\n");
    return 0;
}
