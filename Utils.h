/*
 * Utils.h
 *
 *  Created on: Oct 10, 2015
 *      Author: Manasa
 */

#ifndef UTILS_H_
#define UTILS_H_
#include<iostream>
#include<limits.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<cstring>
#include<algorithm>
#include<fstream>
#include<map>

using namespace std;
typedef enum input_t
{
	IN_TYPE_WSIZE = 0,
	IN_TYPE_MSIZE = 1,
	IN_TYPE_PSIZE = 2,
	IN_TYPE_READ =  3,
	IN_TYPE_WRITE = 4,

	//Last 2 parameters
    IN_TYPE_TOTAL_COUNT,
	IN_TYPE_INVALID = -1
} input_t;

typedef struct input_params
{
	input_t inType;
    std::size_t inp1;
	std::size_t inp2;
}inpParams;

typedef struct page_access
{
	input_t inType;
	std::size_t start_addr;
	std::size_t end_addr;
	int numBytes;
	unsigned int offset;
	std::size_t start_pgNum;
	std::size_t end_pgNum;
}PageAccessInfo;

typedef struct page_table
{
	unsigned long pageFrame;
	unsigned long pageNum;
	unsigned int pageCounter;
	bool validBit;
	bool dirtyBit;
	bool isModified;
}pageTableEntry;

struct pageRefInfo
{
	unsigned long pageNum;
	int page_ref;
	bool used;
};
// struct to hold Algorithm data
typedef struct Algorithm_data
{
    size_t pageSize;
    size_t numPageFrames;
    size_t numPages;
    PageAccessInfo operationParams;
}AlgoData;
extern ofstream fp;
extern int reference_count;
void readPageAccessInfo(inpParams &, PageAccessInfo &,size_t);
void intShiftAndInsert(unsigned int *array, char insert_bit );
bool isPowerOfTwo(int);
inpParams extract_parameters(string inpStr);
int findRecentlyUsed(unsigned char *array, unsigned int size);

#endif /* UTILS_H_ */
