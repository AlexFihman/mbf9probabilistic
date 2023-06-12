// mbf9_count_symmhash.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdint.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <set>
#include <string>

#include "mbf16.h"
#include "mbf16random.h"
#include "time_ms.h"
#include "mbf_permutation7.h"
#include "func_hash.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>
#include <boost/exception/info.hpp>

using namespace std;
using namespace mbf16;
using namespace boost::multiprecision;

#pragma pack(1)
struct file_supershort_record
{
	uint8_t hash[3];
	uint8_t count[6];

	uint32_t get_hash()
	{
		uint32_t result = 0;
		for (int i = 0; i < 3; i++)
		{
			result = (result << 8) + hash[i];
		}
		return result;
	}

	uint64_t get_count()
	{
		uint64_t result = 0;
		for (int i = 0; i < 6; i++)
		{
			result = (result << 8) + count[i];
		}
		return result;
	}
};

struct file_badhash_record 
{
	unsigned int f1;
	unsigned int f2;
	uint8_t count[6];

	void set(uint64_t t_count)
	{		
		uint64_t h1 = t_count;
		for (int i = 5; i >= 0; i--)
		{
			count[i] = (uint8_t)h1;
			h1 = h1 >> 8;
		}
		if (h1 != 0)
			std::cout << "error, hash is too big!" << endl;
	}

	uint64_t get_count()
	{
		uint64_t result = 0;
		for (int i = 0; i < 6; i++)
		{
			result = (result << 8) + count[i];
		}
		return result;
	}
};

file_supershort_record* hashdata;
uint32_t* goodhash_startpos;
file_badhash_record* badhash;

uint64_t goodhash_fff(uint64_t hash)
{
	uint32_t first3bytes = (uint32_t)(hash >> 24);

	uint32_t last3bytes = (uint32_t)(hash & 0xFFFFFF);

	int pos_min;
	if (first3bytes == 0)
		pos_min = 0;
	else
		pos_min = goodhash_startpos[first3bytes - 1] + 1;
	int pos_max = goodhash_startpos[first3bytes];

	if (hashdata[pos_min].get_hash() == last3bytes)
		return hashdata[pos_min].get_count();

	if (hashdata[pos_max].get_hash() == last3bytes)
		return hashdata[pos_max].get_count();

	int loop = 0;
	while (loop < 100)
	{
		loop++;
		int pos_mid = (pos_min + pos_max) >> 1;

		if (hashdata[pos_mid].get_hash() == last3bytes)
			return hashdata[pos_mid].get_count();

		if (hashdata[pos_mid].get_hash() < last3bytes)
			pos_min = pos_mid;
		else
			pos_max = pos_mid;
	}

	cout << "fail goodhash_fff, hash: " << hash << endl;
	return 0;	
}

uint64_t badhash_fff(uint32_t f1, uint32_t f2)
{
	int pos_min = 0;
	int pos_max = 15787-1;

	if ((badhash[pos_min].f1 == f1) && (badhash[pos_min].f2 == f2))
		return badhash[pos_min].get_count();

	if ((badhash[pos_max].f1 == f1) && (badhash[pos_max].f2 == f2))
		return badhash[pos_max].get_count();

	int loop = 0;
	while (loop < 100)
	{
		loop++;
		int pos_mid = (pos_min + pos_max) >> 1;

		if ((badhash[pos_mid].f1 == f1) && (badhash[pos_mid].f2 == f2))
			return badhash[pos_mid].get_count();

		if ((badhash[pos_mid].f1 < f1))
			pos_min = pos_mid;
		else
			if ((badhash[pos_mid].f1 == f1) && (badhash[pos_mid].f2 < f2))
				pos_min = pos_mid;
			else
				pos_max = pos_mid;
	}
	cout << "fail badhash_fff, f1: " << f1 << " f2: " << f2 << endl;
	return 0;	
}

set<uint64_t> badhash_set;

uint64_t get_count(uint64_t hash, mbf7 mbf_func)
{
	if (badhash_set.find(hash) != badhash_set.end())
	{
		mbf_permutation7 mbpm;
		mbf7 mbf_symm = mbpm.min_symmetry(mbf_func);
		int f1 = find_f6(mbf_symm.f1);
		int f2 = find_f6(mbf_symm.f2);
		//cout << "f1 " << f1 << " f2 " << f2 << endl;
		return badhash_fff(f1, f2);
	}
	else
		return goodhash_fff(hash);
}

boost::atomic<uint64_t> seed;

boost::mutex mtx_; // explicit mutex declaration 

const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

void thread()
{
	mbf7* m1 = (mbf7*)calloc(1000000, sizeof(mbf7));
	mbf7* m2 = (mbf7*)calloc(1000000, sizeof(mbf7));

	while (true)
	{
		uint128_t sum_result = 0;
		double sum_sq_result = 0;

		double t0 = TimeMillis();
		sfmt_t mt;

		uint64_t working_seed =  1;
		{
			//boost::lock_guard<boost::mutex> guard(mtx_);
			mtx_.lock();			
			working_seed = seed++;
			mtx_.unlock();
		}
		rand_init(working_seed, mt);


		for (int count = 0; count < 1000000; count++)
		{
			m1[count] = makef7(mt);
			m2[count] = makef7(mt);
		}
		double m_time = TimeMillis() - t0;		

		t0 = TimeMillis();

		for (int count = 0; count < 1000000; count++)
		{
			func_data func_dt;


			mbf7 m_or = (m1[count] | m2[count]);

			func_dt.func = m_or;
			func_hash fh_or(func_dt);
			uint64_t hash_or = fh_or.hash(func_dt) >> 16;

			//cout << "hash1: " << hash_or << endl;

			uint64_t or_count = get_count(hash_or, m_or);


			mbf7 m_and = (m1[count] & m2[count]).bitneg();
			func_dt.func = m_and;
			func_hash fh_and(func_dt);
			uint64_t hash_and = fh_and.hash(func_dt) >> 16;

			//cout << "hash2: " << hash_and << endl;

			uint64_t and_count = get_count(hash_and, m_and);

			
			uint128_t sum = (uint128_t)or_count * (uint128_t)and_count;
			double sum_sq = static_cast<double>(or_count) * static_cast<double>(and_count);
			sum_sq *= sum_sq;
			sum_result += sum;
			sum_sq_result += sum_sq;			
		}

		double t1 = TimeMillis();
		{
			//boost::lock_guard<boost::mutex> guard(mtx_);
			mtx_.lock();
			std::cout << "seed:\t" << working_seed << "\tsumresult:\t" << sum_result << "\tsum_sq_result\t" << sum_sq_result 
				<< "\trt:\t" << (t1 - t0) << " m_tm:\t" << m_time << " dt:\t" << currentDateTime() << endl;
			mtx_.unlock();
		}
		
	}
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "too few parameters";
		return -1;
	}

	std::string param1(argv[1]);
	seed = std::stoull(param1);

	std::string param2(argv[2]);
	int threadcount = (int)std::stoull(param2);

	if (seed==0)
	{
		cout << "seed should not be 0";
		return -1;
	}
	init();

	
	mbf_permutation7 mbpm;
	mbpm.init_symm7();
	
	hashdata = (file_supershort_record*)calloc(489997361, sizeof(file_supershort_record));
	ifstream file1("../data/goodhash.bin", std::ifstream::binary);
	file1.read((char*)hashdata, 489997361LL * sizeof(file_supershort_record));
	file1.close();
	
	goodhash_startpos = (uint32_t*)calloc(256*256*256, sizeof(uint32_t));
	ifstream file2("../data/goodhash_startpos.bin", std::ifstream::binary);
	file2.read((char*)goodhash_startpos, 256*256*256 * sizeof(uint32_t));
	file2.close();	

	badhash = (file_badhash_record*)calloc(15787, sizeof(file_badhash_record));
	ifstream file3("../data/badhash.bin", std::ifstream::binary);
	file3.read((char*)badhash, 15787 * sizeof(file_badhash_record));
	file3.close();

	
	ifstream file4("../data/hashes.bin", std::ifstream::binary);
	while(file4)
	{
		uint64_t badhash_val;
		file4.read((char*)&badhash_val,sizeof(uint64_t));
		if (file4)
		{
			badhash_set.insert(badhash_val);
		}
	}
	file4.close();

	file3.read((char*)badhash, 15787 * sizeof(file_badhash_record));
	file3.close();


	boost::thread** threads = new boost::thread*[threadcount];
	for (int i = 0; i < threadcount; i++)
	{
		threads[i] = new boost::thread( thread );
	}
	
	threads[0]->join();	
	
    return 0;
}

