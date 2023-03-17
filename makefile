DEBUG_FLAGS := -O0 -g

LIBS := -lSDL2

sdl_% : sdl_%.cpp
	g++ -c $@.cpp $(LIBS)

comp: sdl_main.o sdl_functions.o sdl_linalg.o
	g++ -o prog sdl_main.o sdl_functions.o sdl_linalg.o $(LIBS)

debug: sdl_main.cpp sdl_functions.cpp sdl_linalg.cpp
	g++ -c sdl_functions.cpp $(LIBS) $(DEBUG_FLAGS)
	g++ -c sdl_main.cpp $(LIBS) $(DEBUG_FLAGS)
	g++ -o prog sdl_main.o sdl_functions.o sdl_linalg.o $(LIBS) $(DEBUG_FLAGS)

run: comp
	./prog

drun: debug
	gdbtui ./prog

clean:
	rm -f prog; rm -f *.o
