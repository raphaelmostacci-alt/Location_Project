meeting_room: meeting_room.o client_management.o administrator.o room_management.o reservation_room.o useful_fonction.o useful_mutual.o
	gcc meeting_room.o client_management.o administrator.o room_management.o reservation_room.o useful_fonction.o useful_mutual.o -o meeting_room -Wall

meeting_room.o: meeting_room.c includes.h client_management.h administrator.h room_management.h reservation_room.h useful_fonction.h useful_mutual.h
	gcc -c meeting_room.c -Wall

client_management.o: client_management.c includes.h client_management.h useful_fonction.h reservation_room.h useful_mutual.h
	gcc -c client_management.c -Wall

administrator.o: administrator.c includes.h administrator.h client_management.h room_management.h reservation_room.h useful_fonction.h useful_mutual.h
	gcc -c administrator.c -Wall

room_management.o: room_management.c includes.h room_management.h useful_fonction.h reservation_room.h useful_mutual.h
	gcc -c room_management.c -Wall

reservation_room.o: reservation_room.c includes.h reservation_room.h useful_fonction.h room_management.h useful_mutual.h
	gcc -c reservation_room.c -Wall

useful_fonction.o: useful_fonction.c useful_fonction.h
	gcc -c useful_fonction.c -Wall

useful_mutual.o: useful_mutual.c useful_mutual.h
	gcc -c useful_mutual.c -Wall

clean:
	rm -f *.o meeting_room
