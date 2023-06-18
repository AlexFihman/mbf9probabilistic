#define SFMT_MEXP 19937

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#include "time_ms.h"
#include "SFMT.h"

using namespace std;

typedef uint64_t mbf6;
struct mbf7 {mbf6 f1; mbf6 f2;};
struct mbf8 {mbf7 f1; mbf7 f2;};

mbf6** funclists;
int* funcsize;
int N6 = 0;
sfmt_t rng;

const mbf6 F6_MAX = (mbf6)(-1);


bool cmp6(mbf6* fl, int i, int j) {
    return ((~fl[i]) | fl[j]) ==  F6_MAX;
}

bool cmp6f(mbf6 f1, mbf6 f2) {
    return ((~f1) | f2) ==  F6_MAX;
}

bool cmp7(mbf7 f1, mbf7 f2) {
    return cmp6f(f1.f1,f2.f1) && cmp6f(f1.f2,f2.f2);
}

bool cmp8(mbf8 f1, mbf8 f2) {
    return cmp7(f1.f1,f2.f1) && cmp7(f1.f2,f2.f2);
}


void uplevel(int init_level){
    int i,j;
    int lsize = funcsize[init_level];
    int newsize = 0;
    for (i=0;i<lsize;i++){
        for (j=0;j<lsize;j++) {
            if (cmp6(funclists[init_level], i, j))
                newsize++;
        }
    }

    funcsize[init_level+1] = newsize;
    funclists[init_level+1] = new mbf6[newsize];

    newsize = 0;
    int shift = 1 << init_level;
    for (i=0;i<lsize;i++) {
        for (j=0;j<lsize;j++) {
            if (cmp6(funclists[init_level], i, j)) {
                funclists[init_level+1][newsize] = ((funclists[init_level][i]) << shift) | funclists[init_level][j];
                newsize++;
            }
        }
    }
    cout << newsize << endl;
}

mbf7 make7() {
	mbf6 f1, f2;
	do {
		int i1 = sfmt_genrand_uint32(&rng) % N6;
		int i2 = sfmt_genrand_uint32(&rng) % N6;
		f1 = funclists[6][i1];
		f2 = funclists[6][i2];
	} while (!cmp6f(f1,f2));
	mbf7 result;
	result.f1 = f1;
	result.f2 = f2;
	return result;
}

mbf8 make8() {
	mbf7 f1, f2;
	do {
		f1 = make7();
		f2 = make7();
	} while (!cmp7(f1,f2));
	mbf8 result;
        result.f1 = f1;
        result.f2 = f2;
	return result;
}


int main(int argc, char* argv[]) {
    int seed0 = atoi(argv[1]);
    sfmt_init_gen_rand(&rng, seed0);

    std::string filename = "data/" + std::to_string(seed0) + ".bin";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
       cout << "failed to open file: " << filename << endl;
       return 1;
    }

    funclists = new mbf6*[7];
    funclists[0] = new mbf6[2];
    funclists[0][0] = 0;
    funclists[0][1] = 1;
    funcsize = new int[7];
    funcsize[0] = 2;

    uplevel(0);
    uplevel(1);
    uplevel(2);
    uplevel(3);
    uplevel(4);
    uplevel(5);
    N6 = funcsize[6];

    int N = 10000;
    mbf8* array1 = static_cast<mbf8*>(malloc(N * sizeof(mbf8)));

    for (int loop=0; loop<1000; loop++) {
        double t0 = TimeMillis();
        for (int i=0; i<N; i++) {
            array1[i] = make8();
        }
        double t1 = TimeMillis();
        file.write(reinterpret_cast<const char*>(array1), N * sizeof(mbf8));
        cout << loop << "\t" << (t1-t0) << endl;
    }
    file.close();
}

