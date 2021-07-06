task81: main.o mbf16.o TFastList.o FunctionMaker.o
	g++ main.o mbf16.o TFastList.o FunctionMaker.o -march=native -O3 -o task81
clean:
	rm -f *.o task81
main.o:
	g++ -c -O3 -march=native -O3 -o main.o -I/mnt/c/github/random123/include  main.cpp
mbf16.o:
	g++ -c -O3 -march=native -O3 -o mbf16.o  mbf16.cpp
TFastList.o:
	g++ -c -O3 -march=native -O3 -o TFastList.o TFastList.cpp
FunctionMaker.o:
	g++ -c -O3 -march=native -O3 -o FunctionMaker.o -I/mnt/c/github/random123/include FunctionMaker.cpp
