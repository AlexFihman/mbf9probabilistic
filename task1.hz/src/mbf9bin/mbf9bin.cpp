// mbf9bin.cpp : Defines the entry point for the console application.
//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

extern "C" {
	#include "SFMT.h"
}

#include "time_ms.h"
#include "mbf16random.h"

#include <emmintrin.h>

#include <boost/thread.hpp>
#include <boost/atomic.hpp>

using namespace std;
using namespace mbf16;

boost::atomic<uint64_t> seed;
boost::atomic<int> loop_count;
boost::mutex mtx_;


const int ARRAY_SIZE = 10000;

void thread()
{		
	sfmt_t mt;
	uint64_t working_seed;
	{
		mtx_.lock();			
		working_seed = seed++;
		mtx_.unlock();
	}
	rand_init(working_seed, mt);

	
	int i=0;
	int j=0;

	int cnt;
	mbf8* f8l = (mbf8*)calloc(ARRAY_SIZE, sizeof(mbf8));
	mbf8* f8r = (mbf8*)calloc(ARRAY_SIZE, sizeof(mbf8));

	for (cnt=0;cnt<loop_count;cnt++)
	{
		double t1 = TimeMillis();
		for (i=0;i<ARRAY_SIZE;i++)
		{
			f8l[i] = makef8(mt);
			f8r[i] = makef8(mt);
		}

		unsigned int sumcnt = 0;
		double t2 = TimeMillis();
		
		for (i=0;i<ARRAY_SIZE;i++)
		{
			for (j=0;j<ARRAY_SIZE;j++)
			{
				if (cmp8(f8l[i],f8r[j]))
				{
					sumcnt++;
				}
			}
		}				
		mtx_.lock();
		cout << working_seed << "\t" << cnt << "\t" << sumcnt << "\t" << t2-t1 << "\t" << TimeMillis()-t2 << endl;
		mtx_.unlock();
		
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "too few parameters";
		return -1;
	}

	std::string param1(argv[1]);
	seed = std::stoull(param1);

	std::string param2(argv[2]);
	int threadcount = std::stoi(param2);

	std::string param3(argv[3]);
	loop_count = std::stoi(param3);

	if (seed==0)
	{
		cout << "seed should not be 0";
		return -1;
	}
	init();

    
	boost::thread** threads = new boost::thread*[threadcount];
	for (int i = 0; i < threadcount; i++)
	{
		threads[i] = new boost::thread( thread );
	}

	for (int i = 0; i < threadcount; i++)
	{
		threads[i]->join();	
	}

    
    return EXIT_SUCCESS;
}

