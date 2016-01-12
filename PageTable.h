/*
 * PageTable.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Manasa
 */

#ifndef PAGETABLE_H_
#define PAGETABLE_H_
#include"Utils.h"
#include<map>

using namespace std;

typedef std::map<unsigned int, pageTableEntry> PageTableMap_t;

class PageTable {
public:
    PageTable(size_t numPageFrames);

    void Print(void);
    bool EntryExists(const unsigned int,pageTableEntry &);
    void AddEntry(const unsigned int, pageTableEntry &);
    void RemoveEntry(const unsigned int);
    bool getFreeFrame(unsigned long &pageFrame);
    bool isEmpty();
    pageTableEntry & GetEntry(const unsigned int);
    vector<unsigned int> GetTableContent();
    bool isDirty(unsigned int page);
    bool isModified(unsigned int page);
    void UpdateCounter(const unsigned int pageNum,char insert_bit);
    void UpdateEntry(const unsigned int newPageNum,const unsigned int oldPageNum,input_t,pageTableEntry &entry);
    size_t numPageFrames;

private:
    size_t pgFrameIndex;
    PageTableMap_t table;
    pageTableEntry invalid;
};

struct optimalPageData
{
	unsigned int pgNo;
	bool used;
	bool isDirty;
};

void LRU(AlgoData &, PageTable &);
void optimal(AlgoData &optimalDataParams,PageTable &pageEntry,optimalPageData *,int,unsigned int &);

#endif /* PAGETABLE_H_ */
