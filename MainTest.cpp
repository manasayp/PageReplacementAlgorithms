#include"Utils.h"
#include"PageTable.h"
using namespace std;

#define BIT_ARRAY_SIZE 8


int main()
{
    string temp;
    unsigned int wordSize = 0;
    unsigned int phyMemSize = 0;
    unsigned int pageSize = 0;
    unsigned int virtualMemSize = 0;
    unsigned int refer_index = 0;
    size_t numVirtualPages = 0;
    size_t numPhysicalPages = 0;
    struct optimalPageData *reference_string;

    inpParams input_parameters;
    AlgoData algorithmData;
    vector<PageAccessInfo> operations;
    PageAccessInfo pageInfo;

    std::ifstream in("test.dat");
    std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!

    while(std::getline(cin,temp))
    {
        input_parameters = extract_parameters(temp);
    	if(input_parameters.inType == 0)
    	{
    		wordSize = input_parameters.inp1;
    		virtualMemSize = pow(2,wordSize);
    		cout << "Virtual Memory Size : " << virtualMemSize <<endl;
    	}
    	else if(input_parameters.inType == 1)
    	{
    		phyMemSize = input_parameters.inp1;
    		if(!isPowerOfTwo(phyMemSize))
    		{
    		    cerr << "Invalid Input : Memory Size must be a power of 2\n";
    		    exit(-1);
    		}
    	}
    	else if(input_parameters.inType == 2)
    	{
    	    pageSize = input_parameters.inp1;
    	    if(!isPowerOfTwo(pageSize))
    	    {
    	        cerr << "Invalid Input : Page Size must be a power of 2\n";
    	        exit(-1);
    	    }
    	}
    	else if((input_parameters.inType == 3) || (input_parameters.inType == 4))
    	{
    		//cout << input_parameters.inType <<  "\t" << input_parameters.inp1 <<"\t" << input_parameters.inp2 <<endl;
            readPageAccessInfo(input_parameters,pageInfo,pageSize);
    	    operations.push_back(pageInfo);
    	}
    }

    if((pageSize == 0) || (wordSize == 0) || (phyMemSize == 0) || operations.empty() == true)
    {
    	cerr << "Input Missing..Provide the input in the following format\n";

    	cerr << "wordSize\nmemorySize\npageSize\noperations\n";
    	exit(-1);
    }

    if(phyMemSize > virtualMemSize)
    {
    	cerr << "Invalid Input : Memory Size should be within word boundary\n";
    	exit(-1);
    }

    if(pageSize > virtualMemSize)
    {
        cerr << "Invalid Input : Page Size should be within word boundary\n";
        exit(-1);
    }

    numVirtualPages = virtualMemSize/pageSize;
    cout << " Number of Virtual Pages : " << numVirtualPages << endl;
    numPhysicalPages = phyMemSize/pageSize;
    cout << " Number of Phyical Pages : " << numPhysicalPages << endl;

    PageTable pageEntry1(numPhysicalPages); //Page Table for Optimal Replacement
    PageTable pageEntry2(numPhysicalPages);//Page Table for LRU
    algorithmData.pageSize = pageSize;
    algorithmData.numPageFrames = numPhysicalPages;
    algorithmData.numPages = numVirtualPages;

    reference_string = new struct optimalPageData[reference_count+1];
    if(reference_string == NULL)
    {
    	cerr << " Unable to allocate memory\n";
        exit(-1);
    }

    std::vector<PageAccessInfo>::iterator it;

    int i = 0;

    for(it = operations.begin(); it != operations.end(); it++)
    {
    	size_t start = it->start_pgNum;
    	size_t end = it->end_pgNum;
    	for(size_t j = start; j <= end; j++)
    	{
    	    reference_string[i].pgNo = j;
    	}
    }

    for(it = operations.begin(); it != operations.end(); it++)
    {
    	algorithmData.operationParams.end_addr = it->end_addr ;
    	algorithmData.operationParams.start_addr = it->start_addr ;
    	algorithmData.operationParams.end_pgNum = it->end_pgNum ;
    	algorithmData.operationParams.start_pgNum = it->start_pgNum ;
    	algorithmData.operationParams.numBytes = it->numBytes ;
    	algorithmData.operationParams.offset = it->offset ;
    	algorithmData.operationParams.inType = it->inType ;
    	LRU(algorithmData,pageEntry1);
    	optimal(algorithmData,pageEntry2,reference_string,reference_count,refer_index);
    }

    std::cin.rdbuf(cinbuf);   //reset to standard input again

    getchar();
    return 0;
}
