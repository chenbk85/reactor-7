FLAGS = -g 
OPTFLAGS= -c -shared -fPIC

all :event_handler.o simple_reactor.o tests/timer_test

event_handler.o : event_handler.c event_handler.h reactor.h
	gcc $(FLAGS) $(OPTFLAGS) event_handler.c

tests/timer_test: simple_reactor.o
	gcc $(FLAGS) ../data_structures/heap/heap.o ./event_handler.o tests/timer_test.c simple_reactor.o -o ./tests/timer_test && ./tests/timer_test

simple_reactor.o: event_handler.o reactor.h simple_reactor.h
	gcc $(FLAGS) $(OPTFLAGS) simple_reactor.c -L ../data_structures/heap -o simple_reactor.o

.PHONY: clean

clean : 
	rm -f *.o *.so *.lo
	rm -f tests/*.o
	rm -f timer_test
