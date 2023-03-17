all:	main.cpp linalg.o functions.o
	g++ -o raytracer main.cpp linalg.o functions.o -lSDL2

debug:	main.cpp linalg.h linalg.cpp functions.h functions.cpp
	g++ -c functions.cpp -O0 -g
	g++ -c linalg.cpp -O0 -g
	g++ -o deb_raytracer main.cpp linalg.o functions.o -lSDL2 -O0 -g

run:	all
	./raytracer

drun:	debug
	./deb_raytracer

clean:
	rm -f *.o
	rm -f raytracer
	rm -f deb_raytracer
