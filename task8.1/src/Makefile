all: mbfup.o mbf16c.o fastlist.o uMbfObj.o time_ms.o
	g++ mbfup.o mbf16c.o fastlist.o uMbfObj.o time_ms.o -march=native -lpthread -g -O3 -o mbfup
clean:
	rm -f *.o mbfup
mbfup.o: mbfup.cpp mbf16c.h uMbfObj.h time_ms.h
	g++ -c -march=native -g -O3 -o mbfup.o -I../../../random123/include mbfup.cpp
mbf16c.o: mbf16c.cpp mbf16c.h
	g++ -c -march=native -g -O3 -o mbf16c.o  mbf16c.cpp
fastlist.o:
	g++ -c -march=native -g -O3 -o fastlist.o -I../../../random123/include fastlist.cpp
uMbfObj.o:
	g++ -c -march=native -g -O3 -o uMbfObj.o -I../../../random123/include uMbfObj.cpp
time_ms.o:
	g++ -c -march=native -g -O3 -o time_ms.o  time_ms.cpp