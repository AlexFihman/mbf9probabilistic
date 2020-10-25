// mbf9bin.cpp : Defines the entry point for the console application.
//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <thread>
#include <mutex>

#include <emmintrin.h>
#include "time_ms.h"
#include "mbf16random.h"
#include "mbf16.h"

using namespace std;
using namespace mbf16;


const int ARRAY_SIZE = 10000;
int loop_count;
std::mutex mtx;

void thread_routine(int ctr_no)
{		
	squares_state state;
	rand_init(state, ctr_no);

	
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
			f8l[i] = makef8(state);
			f8r[i] = makef8(state);
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
		
		std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
		lck.lock();
		cout << ctr_no << "\t" << cnt << "\t" << sumcnt << "\t" << t2-t1 << "\t" << TimeMillis()-t2 << endl;
		lck.unlock();
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
	int seed = std::stoull(param1);

	std::string param2(argv[2]);
	int threadcount = std::stoi(param2);

	std::string param3(argv[3]);
	loop_count = std::stoi(param3);

	init();

    
	std::thread** threads = new std::thread*[threadcount];
	for (int i = 0; i < threadcount; i++)
	{
		threads[i] = new std::thread(thread_routine, seed+i);
	}

	for (int i = 0; i < threadcount; i++)
	{
		threads[i]->join();
	}

    
    return EXIT_SUCCESS;
}

