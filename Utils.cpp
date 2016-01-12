#include <iostream>
#include "Utils.h"
using namespace std;

int reference_count = 0;

/*read page information*/
void readPageAccessInfo(inpParams &inpParams, PageAccessInfo &pageInfo,size_t pageSize)
{
	pageInfo.inType = inpParams.inType;
	pageInfo.start_addr = inpParams.inp1;
	pageInfo.numBytes = inpParams.inp2;
	pageInfo.end_addr = pageInfo.start_addr+pageInfo.numBytes-1;
	pageInfo.start_pgNum = floor(pageInfo.start_addr/pageSize);
	pageInfo.end_pgNum = floor(pageInfo.end_addr/pageSize);
	pageInfo.offset = pageInfo.start_addr % pageSize;
	reference_count += (pageInfo.end_pgNum - pageInfo.start_pgNum+1);
}

/*counter - right shift and set the MSB*/
void intShiftAndInsert(unsigned int *num, char insert_bit )
{
	*num >>= 1;
	if(insert_bit)
	{
		*num |= 0x80000000;
	}
}

/* Function to check if x is power of 2*/
bool isPowerOfTwo (int x)
{
/* First x in the below expression is for the case when x is 0 */
return x && (!(x&(x-1)));
}

static char Main_input_string_op[IN_TYPE_TOTAL_COUNT][12] = {"wordSize", "memorySize","pageSize","read","write"};

/*extracting input parameters*/
inpParams extract_parameters(string input)
{
	inpParams result;
	size_t tb = 1099511627776;
	size_t gb = 1073741824;
	size_t mb = 1048576;
	size_t kb = 1024;
    string inpStr;

	/*ignore comments in the input*/
	while((input[0] == '#') || ((input[0] == '\\')&& ( input[1] == '*'))
		     || ((input[0] == '\\') && (input[1] == '*')))
	{
		//cout << "here \n";
        std::getline(std::cin,input);
	}
	/*remove all the spaces in the string*/
	input.erase(std::remove(input.begin(),input.end(),' '),input.end());

	inpStr = input.substr(0,input.find('#'));
	char * cstr = new char [inpStr.length()+1];
	std::strcpy (cstr, inpStr.c_str());
	char *p = std::strtok(cstr,"(,)");

	result.inType = IN_TYPE_INVALID;
	for(int i = 0; i < IN_TYPE_TOTAL_COUNT; i++)
	{
		if(p != NULL && strcmp(p, Main_input_string_op[i]) == 0)
		{
			result.inType = (input_t)i;
			break;
		}
	}

	if(result.inType == IN_TYPE_INVALID )
	{
		cerr << "Invalid Input..Provide the input in the following format\n";
		cerr << "wordSize\nmemorySize\npageSize\noperations\n";
		getchar();
		exit(-1);
	}

	p = std::strtok(NULL,"(,)");

	if(p != NULL)
	{
		result.inp1 = (size_t)strtol(p,NULL,0);

		if(strstr(p,"KB"))
		{
			result.inp1 = result.inp1 * kb;
		}
		else if(strstr(p,"MB"))
		{
			result.inp1 *= mb;
		}
		else if(strstr(p,"GB"))
		{
			result.inp1 *= gb;
		}
		else if(strstr(p,"TB"))
		{
			result.inp1 *= tb;
		}

	}

	p = std::strtok(NULL,"(,)");

	if(p != NULL)
	{
		result.inp2 = (size_t)strtol(p,NULL,0);

		if(strstr(p,"KB"))
		{
			result.inp2 *= kb;
		}
		else if(strstr(p,"MB"))
		{
			result.inp2 *= mb;
		}
		else if(strstr(p,"GB"))
		{
			result.inp2 *= gb;
		}
		else if(strstr(p,"TB"))
		{
			result.inp2 *= tb;
		}
	}
    return result;
}
