connect: main.o MyClient.o
	g++ -o connect.exe main.o MyClient.o -lws2_32
main.o: main.cpp
	g++ -g -O0 -c main.cpp
MyClient.o: MyClient.cpp
	g++ -g -O0 -c MyClient.cpp -lws2_32