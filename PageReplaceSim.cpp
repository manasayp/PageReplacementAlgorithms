/*
 * PageReplaceSim.cpp
 *
 *  Created on: Oct 17, 2015
 *      Author: Manasa
 */
#include "PageTable.h"
#include "Utils.h"
using namespace std;

//Optimal Page Replace Algorithm
void optimal(AlgoData &optimalDataParams,PageTable &pageEntry,optimalPageData *reference_string,int length,unsigned int &refer_index)
{
    size_t startPageNum = optimalDataParams.operationParams.start_pgNum;
    size_t endPageNum = optimalDataParams.operationParams.end_pgNum;
    pageTableEntry entry;
    bool errFlag = false;
    int futureReference = 0;

    cout << "******** Optimal Page Replacement ********" <<endl;
    if(optimalDataParams.operationParams.inType == IN_TYPE_WRITE)
    {
	cout << "Operation : Write " <<endl;
	if(startPageNum == endPageNum)
		cout << "[ Page : " << dec << startPageNum;
	else
		cout << "[ Pages : " << dec << startPageNum << " to  " << endPageNum;
	cout << " , Start Address : 0x" << hex << optimalDataParams.operationParams.start_addr;
	cout << " , Ending Address : 0x" << optimalDataParams.operationParams.end_addr << "]" <<endl;
   }
   else
   {
	cout << "Operation : Read "<<endl;
	if(startPageNum == endPageNum)
	    cout << "[ Page : " << dec << startPageNum;
	else
	    cout << "[ Pages : " << dec << startPageNum << " to  " << endPageNum;
	cout << " , Start Address : 0x" << hex << optimalDataParams.operationParams.start_addr;
	cout << " , Ending Address : 0x" << optimalDataParams.operationParams.end_addr << "]" <<endl;
   }

   cout << endl;

   for(unsigned int i = startPageNum; i <= endPageNum; i++)
   {
	if(pageEntry.isEmpty()) //If table has no entry
	{
	    if(optimalDataParams.operationParams.inType == 3)
	    {
	    	entry.dirtyBit = false;
	    }
	    else
	    {
	    	entry.dirtyBit = true;
	    }
	    pageEntry.AddEntry(i,entry);
	    cout << "Page Num " << i << " is mapped to Page Frame " << entry.pageFrame << " Dirty Bit : " << entry.dirtyBit << endl;
	}
	else if(pageEntry.EntryExists(i,entry)) //If Entry already exists in table
	{
	    if(optimalDataParams.operationParams.inType == IN_TYPE_WRITE)
	    {
		entry.dirtyBit = true;
		entry.isModified = true;
	    }
	    else
	    {
		entry.isModified = false;
	    }
	    cout << "Entry exists at Page Frame " << entry.pageFrame << " Dirty Bit " << entry.dirtyBit <<endl;
	}
	else if(pageEntry.getFreeFrame(entry.pageFrame) == false) //If maximum available physical page frames is reached
	{
	    pageRefInfo pages[pageEntry.numPageFrames];
	    vector<unsigned int> content;
	    //size_t pos[pageEntry.numPageFrames];
	    int i = 0, /*k = 0*/max = -1;

	    content = pageEntry.GetTableContent();

	    for(vector<unsigned int>::iterator it = content.begin(); it != content.end(); it++)
	    {
	        pages[i].pageNum = *it;
	        pages[i].page_ref = pageEntry.numPageFrames+1;
	        i++;
	        // cout << pages[i].pageNum << " : " << pages[i].page_ref <<endl;
	    }
	    for(unsigned int i = 0; i < pageEntry.numPageFrames; i++)
	    {
             for(int j = 0; j < reference_count; j++)
             {
                if(pages[i].pageNum == reference_string[j].pgNo)
                {
                	pages[i].page_ref = j;
                }
             }
        }

	    /* for(unsigned int i = 0; i < pageEntry.numPageFrames; i++)
	    {
	        cout << pages[i].pageNum << " : " << pages[i].page_ref <<endl;
	    }*/

	    for(unsigned int j = 0; j < pageEntry.numPageFrames; j++)
	    {
             if(pages[j].used)
             {
            	 futureReference++;
             }
             else
             {
                	 if(pages[j].page_ref > max)
                	 {
                		 max = pages[j].page_ref;
                		 entry = pageEntry.GetEntry(pages[j].pageNum);
                	 }
                	 else if(pages[j].page_ref == max)
                	 {
                		 if(!pageEntry.isDirty(pages[j].pageNum))
                		 {
                			 max = pages[j].page_ref;
                			 entry = pageEntry.GetEntry(pages[j].pageNum);
                		 }
                	 }
                 }
	         }
	         if(futureReference != 0)
	         {
	        	 for(unsigned int j = 0; j < pageEntry.numPageFrames; j++)
	        	 {
	        		 entry = pageEntry.GetEntry(pages[j].pageNum);
	        		 if((pages[j].used == true) && (entry.dirtyBit == false))
	        		 {
	        			 break;
	        		 }
	        	 }
	         }

	         if(entry.isModified == true)
	         {
	        	 cout << "Page Frame " << entry.pageFrame << " is written to Disk at Page " << entry.pageNum << "address " <<endl;
	         }

	         unsigned int oldPageNum = entry.pageNum;
	         pageEntry.UpdateEntry(i,oldPageNum,optimalDataParams.operationParams.inType,entry);

	         cout << "Replacing Page Frame " << entry.pageFrame << " with new Page " << i << " Dirty Bit " << entry.dirtyBit <<endl;
		}
		else // Adding as next entry in page table
		{
			entry.validBit = true;
			entry.pageNum = i;
			pageEntry.getFreeFrame(entry.pageFrame);
	    	       if(optimalDataParams.operationParams.inType == 3)
	    	       {
	    		   entry.dirtyBit = false;
	    	       }
	    	       else
	    	       {
	    		   entry.dirtyBit = true;
	    	       }
	    	       //entry.bit_array_length
	    	       pageEntry.AddEntry(i,entry);
	    	       cout <<  " Page " << i << " mapped to Page Frame " << entry.pageFrame << " Dirty Bit : " << entry.dirtyBit <<endl;
	         }
		if(errFlag == false)
		{
		    reference_string[refer_index].used = true;
		    reference_string[refer_index].isDirty = entry.dirtyBit;
		    refer_index++;
		}
	}
    cout << "*************************************************" <<endl;
	cout << endl;
}

//LRU Page Replace Algorithm
void LRU(AlgoData &LruDataParams,PageTable &pageEntry)
{
    size_t startPageNum = LruDataParams.operationParams.start_pgNum;
    size_t endPageNum = LruDataParams.operationParams.end_pgNum;
    pageTableEntry entry;

    cout << "******** LRU Page Replacement ********" <<endl;
    if(LruDataParams.operationParams.inType == IN_TYPE_WRITE)
    {
    	cout << "Operation : Write " <<endl;
    	if(startPageNum == endPageNum)
    		cout << "[ Page : " << dec << startPageNum;
    	else
    		cout << "[ Pages : " << dec << startPageNum << " to  " << endPageNum;
    	cout << " , Start Address : 0x" << hex << LruDataParams.operationParams.start_addr;
    	cout << " , Ending Address : 0x" << LruDataParams.operationParams.end_addr << "]" <<endl;

    }
    else
    {
    	cout << "Operation : Read " <<endl;
    	if(startPageNum == endPageNum)
    		cout << "[ Page : " << dec << startPageNum;
    	else
    		cout << "[ Pages : " << dec << startPageNum << " to  " << endPageNum;
    	cout << " , Start Address : 0x" << hex << LruDataParams.operationParams.start_addr;
    	cout << " , Ending Address : 0x" << LruDataParams.operationParams.end_addr << "]" <<endl;
    }
    cout <<endl;

    for(unsigned int i = startPageNum; i <= endPageNum; i++)
    {
    	if(pageEntry.isEmpty()) //If page table is empty
    	{
    		if(LruDataParams.operationParams.inType == 3)
    		{
    		    entry.dirtyBit = false;
    		}
    		else
    		{
    			entry.dirtyBit = true;
    		}
    		entry.pageCounter = 0;
    		//intShiftAndInsert(&entry.pageCounter,1);
    		//cout << "Page Counter " << entry.pageCounter <<endl;
			pageEntry.AddEntry(i,entry);
			cout << "Page Num " << i << " is mapped to Page Frame " << entry.pageFrame << " Dirty Bit : " << entry.dirtyBit << endl;
    	}
    	else if(pageEntry.EntryExists(i,entry)) //entry exists
    	{
    		if(LruDataParams.operationParams.inType == IN_TYPE_WRITE)
    		{
    			entry.dirtyBit = true;
    			entry.isModified = true;
    		}
    		else
    		{
    			entry.isModified = false;
    		}
    		//intShiftAndInsert(&entry.pageCounter,1);
    		cout << "Page Entry  "<< i << " exists at Page Frame " << entry.pageFrame << " Dirty Bit " << entry.dirtyBit <<endl;
    	}
    	else if(pageEntry.getFreeFrame(entry.pageFrame) == false) // if no frames available
        {
    	   unsigned int pgNum;
    	   unsigned int counter_min;
    	   vector<unsigned int> contents;
    	   contents = pageEntry.GetTableContent();
    	   pgNum = contents[0];
    	   entry = pageEntry.GetEntry(pgNum);
    	   counter_min = entry.pageCounter;

    	//   cout << "Counter Values in Page Table " <<endl;
    	   for(unsigned int pNo = 1; pNo < pageEntry.numPageFrames; pNo++)
    	   {
    	    	entry = pageEntry.GetEntry(contents[pNo]);
    	    	//cout << "Page Frame : " << entry.pageFrame <<endl;
    	    	//cout << "Page Counter : " << entry.pageCounter <<endl;
    	    	//cout << "Page Number :  " << entry.pageNum <<endl;
    	    	if(entry.pageCounter == counter_min)
    	    	{
    	    		if(entry.dirtyBit == false)
    	    		{
    	    			counter_min = entry.pageCounter;
    	    			pgNum = entry.pageNum;
    	    			//cout << pgNum << endl;
    	    		}
    	    	}
    	    	else if(entry.pageCounter < counter_min)
    	    	{
    	    		counter_min = entry.pageCounter;
    	    		pgNum = entry.pageNum;
    	    		//cout << pgNum << endl;
    	    	}
    	   }
    	   if(entry.isModified == true)
    	   {
    	    	cout << "Page Frame " << entry.pageFrame << " is written to Disk at Page " << entry.pageNum << "address " <<endl;
    	   }
    	   pageEntry.UpdateEntry(i,pgNum,LruDataParams.operationParams.inType,entry);
    	   cout << "Replacing Page Frame " << entry.pageFrame << " with new Page " << i << " Dirty Bit " << entry.dirtyBit <<endl;
    	}
    	else //add to next position
    	{
    		if(LruDataParams.operationParams.inType == IN_TYPE_READ)
    		{
    		     entry.dirtyBit = false;
    		}
    		else
    		{
    		     entry.dirtyBit = true;
    		}
    		//entry.bit_array_length
    		entry.pageCounter = 0;
    		//intShiftAndInsert(&entry.pageCounter,1);
    		pageEntry.AddEntry(i,entry);
    		//cout << "Counter === " << entry.pageCounter <<endl;
    		cout <<  " Page " << i << " mapped to Page Frame " << entry.pageFrame << " Dirty Bit : " << entry.dirtyBit <<endl;
    	}
       // pageEntry.Print();
    }
    vector<unsigned int> contents;
    unsigned int temp[endPageNum-startPageNum+1], k = 0;
    //pageEntry.Print();
    for(unsigned int j = startPageNum; j <= endPageNum; j++)
    {
 	temp[k++] = j;
    }
    k = 0;
    contents = pageEntry.GetTableContent();
    for(vector<unsigned int>::iterator it = contents.begin(); it != contents.end(); it++)
    {
        if((k < (endPageNum-startPageNum+1)) && (*it == temp[k]))
        {
            pageEntry.UpdateCounter(*it,1);
            k++;
        }
        else
        {
            pageEntry.UpdateCounter(*it,0);
        }

    }
    cout << "**************************************" <<endl;
    cout << endl;
}


