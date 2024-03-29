FLAGS = -g3 -Wall 
OBJFLAGS = -fPIC -c
SOFLAGS = -fPIC -shared
all : libreactor.so tests/timer_test

event_handler.o : event_handler.c event_handler.h reactor.h
	gcc $(FLAGS) $(OBJFLAGS) event_handler.c

simple_reactor.o: event_handler.o reactor.h simple_reactor.h
	gcc $(OBJFLAGS) $(FLAGS) simple_reactor.c -ldstructs -o simple_reactor.o

libreactor.so: simple_reactor.o event_handler.o simple_reactor.h
	gcc $(FLAGS) $(SOFLAGS) simple_reactor.o event_handler.o -o libreactor.so

tests/timer_test: libreactor.so
	gcc $(FLAGS) ./tests/timer_test.c -o ./tests/timer_test -lreactor -ldstructs

install: libreactor.so
	cp libreactor.so /usr/local/lib/libreactor.so
	cp event_handler.h reactor.h reactor_event.h simple_reactor.h /usr/local/include

uninstall:
	rm -f /usr/local/lib/libreactor.so /usr/local/include/event_handler.h /usr/local/include/reactor.h /usr/local/include/reactor_event.h /usr/local/include/simple_reactor.h
.PHONY: clean

clean : 
	rm -f *.o *.so *.lo
	rm -f tests/*.o
	rm -f timer_test
