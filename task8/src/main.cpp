#include <cstdio>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include "mbf16.h"
#include "FunctionMaker.h"
#include <Random123/threefry.h>

double TimeMillis()
{
    static struct timeval _tstart;
    static struct timezone tz;
    gettimeofday(&_tstart, &tz);
    return (double)_tstart.tv_sec + (double)_tstart.tv_usec / (1000 * 1000);
};

int inverse(int item) 
{
	return (d1 - 1) ^ item;
}

double experiment(Mbf16* m, threefry4x32_ctr_t& ctr, threefry4x32_key_t& key)
{
	double s = 0.0;
	int midLevel = d3 >> 1;	
	int midLevelDepth = m->LevelDepth[midLevel];	

	TFastList* in = new TFastList();
	TFastList* out = new TFastList();

	for (int i = 0;i < midLevelDepth;i++)
		in->Add(m->levellist[midLevel][i]);



	for (int mid_fill = 0; mid_fill <= midLevelDepth; mid_fill++) {
		while (out->Count < mid_fill) {
			ctr.v[0]++;
			threefry4x32_ctr_t rand = threefry4x32(ctr, key);
			int randItemPos = rand.v[0] % in->Count;
			int randItem = in->Items[randItemPos];
			in->Delete(randItem);
			out->Add(randItem);
		}


		TFastList* revList = new TFastList();
		for (int i = 0;i < in->Count;i++)
			revList->Add(inverse(in->Items[i]));

		FunctionMaker* upper = new FunctionMaker(m, midLevel, out);

		FunctionMaker* lower = new FunctionMaker(m, d3 - midLevel, revList);
		double cmb = m->comb(midLevelDepth, mid_fill);
		double upper1 = upper->getS(ctr, key);
		double lower1 = lower->getS(ctr, key);
		//std::cout << "mid_fill = " << mid_fill << std::endl;
		//std::cout << "cmb = " << cmb << std::endl;
		//std::cout << "upper1 = " << upper1 << std::endl;
		//std::cout << "lower1 = " << lower1 << std::endl;
		delete upper;
		delete lower;
		
		s += cmb * upper1 * lower1;
		delete revList;

	}
	delete in;
	delete out;	
	return s;
}

int main(int argc, char** argv)
{    

	//int i;
	threefry4x32_ctr_t  ctr = {{0,0,0,0}};
	/* we illustrate one user-specified seed and one constant as the key */
	threefry4x32_key_t key = {{17,15,0,0}};

	/*
	printf( "The first few randoms with key 0x%llx 0x%llx\n",
		   (unsigned long long)key.v[0], (unsigned long long)key.v[1]);
	    for(i=0; i<10; ++i){
	        ctr.v[0] = i;
		{
	          threefry4x32_ctr_t rand = threefry4x32(ctr, key);
	          printf("ctr: %llx %llx threefry4x32(20, ctr, key): %llx %llx\n",
	                 (unsigned long long)ctr.v[0], (unsigned long long)ctr.v[1],
	                 (unsigned long long)rand.v[0], (unsigned long long)rand.v[1]);
		}
	}
	*/


	int init_value = atol(argv[1]);
    //rand_init(state, init_value);
	ctr.v[1] = init_value;

	std::cout.precision(17);

	double t_start = TimeMillis();
    Mbf16* m = new Mbf16();
	double sums = 0.0;
	double sums_sq = 0.0;
	int iter = 10000000;
	for (int i = 0;i < iter;i++) {
		double exp = experiment(m, ctr, key);
		//sums += exp;
		//sums_sq += exp * exp;
		std::cout << i << "\t" << exp << "\t" << exp * exp << "\t" << (TimeMillis() - t_start) << std::endl;
	}
	delete m;
	double t = TimeMillis() - t_start;
	//std::cout << "iter = " << iter << std::endl;
	//std::cout << "sums_avg = " << sums / iter << std::endl;
	//std::cout << "sums_sq_avg = " << sums_sq / iter << std::endl;

	//std::cout << "run time: = " << t << std::endl;
    return 0;
}
