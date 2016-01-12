#include "PageTable.h"

//Default constructor
PageTable::PageTable(size_t numPageFrames) {
    //Initialize an invalid page table entry to return if needed
	this->numPageFrames = numPageFrames;
	this->pgFrameIndex = 0;
    invalid.dirtyBit = false;
    invalid.pageFrame = 0;
    invalid.pageCounter = 0;
    invalid.validBit = false;
    invalid.pageNum = 0;
    invalid.isModified = false;
}

//Print the page table
void PageTable::Print(void) {
    std::cout << std::endl << "Page Table Contents" <<endl;
    map<unsigned int, pageTableEntry>::iterator it;
    for(it  = table.begin(); it != table.end(); it++)
    {
         cout << "Page Num " << it->first <<endl;
         cout << "Page Frame " << it->second.pageFrame <<endl;
         cout << "Page Counter " << it->second.pageCounter <<endl;
         cout << "Valid Bit " << it->second.validBit <<endl;
         cout << "Dirty Bit " << it->second.dirtyBit <<endl;
    }
}

//Checks if Page Entry exists
bool PageTable::EntryExists(const unsigned int page,pageTableEntry & entry) {
    //Return true if the page entry exists AND is valid
    if(table.count(page)) {
    	entry = table[page];
        if(entry.validBit) {
        	return true;
        }
    }
    return false;
}

//Add a new entry in Page Table
void PageTable::AddEntry(const unsigned int page, pageTableEntry & entry) {
    //Set the valid bit
    entry.validBit = true;
    entry.pageFrame = pgFrameIndex++;
    entry.pageNum = page;
    entry.isModified = false;
    //Store the entry in the page table
    table[page] = entry;
}

//Update new Page
void PageTable::UpdateEntry(const unsigned int newPageNum,const unsigned int oldPageNum,input_t opType,pageTableEntry &entry)
{
	entry.pageFrame = table[oldPageNum].pageFrame;
	table.erase(oldPageNum);
	entry.pageCounter = 0;
	entry.validBit = true;
	entry.isModified = false;
	if(opType == IN_TYPE_WRITE)
		entry.dirtyBit = true;
	else
		entry.dirtyBit = false;
	entry.pageNum = newPageNum;
	//intShiftAndInsert(&entry.pageCounter,1);
	table[newPageNum] = entry;
}

//Removing a Page from Table
void PageTable::RemoveEntry(const unsigned int page) {
    table.erase(page);
}

//Find the Entry in Page Table
pageTableEntry & PageTable::GetEntry(const unsigned int page) {
    //Returns the entry associated with the page, if it exists
    if(table.count(page)) {
        return table[page];
    } else {
        return invalid;
    }
}

//Checks if Page Table is empty
bool PageTable::isEmpty()
{
	if(table.empty() == true)
		return true;
	else
		return false;
}

//Gets free slot in Page Table
bool PageTable::getFreeFrame(unsigned long &pageFrame)
{
	if(pgFrameIndex != numPageFrames)
	{
		pageFrame = pgFrameIndex;
		return true;
	}
	else
		return false;
}

//Prints contents of Table
vector<unsigned int> PageTable::GetTableContent()
{
	vector<unsigned int> page;
	std::map<unsigned int, pageTableEntry>::iterator it;

	for(it = table.begin(); it != table.end(); it++)
	{
		page.push_back(it->first);
	}
	return page;
}

//checks if the page is dirty
bool PageTable::isDirty(unsigned int page)
{
	pageTableEntry entry;
	entry = table[page];
	if(entry.dirtyBit == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//checks if page is modified
bool PageTable::isModified(unsigned int page)
{
	pageTableEntry entry;
	entry = table[page];
	if(entry.isModified == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Update LRU counter
void PageTable::UpdateCounter(const unsigned int page,char insert_bit)
{
    
	pageTableEntry entry;
	if(table.count(page))
	{
		entry = table[page];
                unsigned int counter = entry.pageCounter;
		intShiftAndInsert(&counter,insert_bit);
                entry.pageCounter = counter;
                table[page] = entry;
	}
}

