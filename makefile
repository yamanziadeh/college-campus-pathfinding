build:
	rm -f program.exe
	g++ -O2 -std=c++11 -Wall main.cpp dist.cpp osm.cpp tinyxml2.cpp -o program.exe

test:
	rm -f program.exe
	g++ -g -std=c++11 -Wall maincatch.cpp test01.cpp -o program.exe

testall:
	rm -f program.exe
	g++ -g -std=c++11 -Wall maincatch.cpp test*.cpp -o program.exe
	
run:
	./program.exe

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./program.exe
