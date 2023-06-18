clang++ -std=c++17 -DHAVE_SSE2 -DSFMT_MEXP=19937 -march=native mbf8deb.cpp time_ms.cpp SFMT.c -O3 -o mbf8deb
# mac m1
#clang++ -std=c++17 -DHAVE_NEON -DHAVE_SHA3 -DSFMT_MEXP=19937 -march=armv8.3-a+sha3 mbf8deb.cpp time_ms.cpp SFMT.c -O3 -o mbf8deb