all: main

main: main.o algorithms.o display.o appGraphic.o appConsole.o app.o
	g++ main.o algorithms.o display.o appGraphic.o appConsole.o app.o -o main -lraylib

main.o: app.hpp
	g++ -c main.cpp

algorithms.o: algorithms.hpp
	g++ -c algorithms.cpp

display.o: algorithms.hpp
	g++ -c display.cpp

appGraphic.o: app.hpp
	g++ -c appGraphic.cpp

appConsole.o: app.hpp
	g++ -c appConsole.cpp

app.o: app.hpp
	g++ -c app.cpp

clean:
	rm -f main *.o

