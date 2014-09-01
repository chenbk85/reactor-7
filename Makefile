FLAGS = -g3 -Wall -I.
OBJFLAGS = -c -fPIC
SOFLAGS = -fPIC -shared

all : libreactor.so tests/timer_test

event_handler.o : event_handler.c event_handler.h reactor.h
	gcc $(FLAGS) $(OBJFLAGS) event_handler.c

simple_reactor.o: event_handler.o reactor.h simple_reactor.h
	gcc $(OBJFLAGS) $(FLAGS) simple_reactor.c -ldstructs -o simple_reactor.o

libreactor.so: simple_reactor.o event_handler.o
	gcc $(FLAGS) $(SOFLAGS) simple_reactor.o event_handler.o -o libreactor.so

tests/timer_test: libreactor.so
	#gcc $(FLAGS) ../data_structures/heap/heap.o ./event_handler.o tests/timer_test.c simple_reactor.o -o ./tests/timer_test && ./tests/timer_test
	gcc $(FLAGS) -ldstructs -lreactor tests/timer_test.c -o ./tests/timer_test

install: libreactor.so
	cp libreactor.so /usr/lib/libreactor.so

.PHONY: clean

clean : 
	rm -f *.o *.so *.lo
	rm -f tests/*.o
	rm -f timer_test
