#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    float hMedian(vector<int>& citations);
    void Sort(vector<int> & citations);
    int searchHindex(vector<int>& citations);
    float computeHmedian(vector<int> citations,int Hindex);

};

int main(int argc, char** argv) {
    std::string line;
    int number;
    std::vector<int> numbers;
	
    std::getline(std::cin, line);
    std::istringstream stream(line);
    while (stream >> number)
        numbers.push_back(number);
	
	Solution s;
	float res= s.hMedian(numbers);

	std::cout << std::fixed;
	std::cout << std::setprecision(1) << res << endl;

	return 0;
}

float Solution::hMedian(vector<int>& citations)
{
    Sort(citations);

    int Hindex=searchHindex(citations);

    return computeHmedian(citations,Hindex);
}
void Solution::Sort(vector<int> &citations)
{
    int i,j;
    int tmp;
    bool flag=true;//用于记录一趟冒泡中有没有发生过交换

    for(i=1;i<citations.size()&&flag;++i)//size-1次冒泡
    {
        flag=false;
        for(j=0;j<citations.size()-i;++j)//第i次冒泡
        {
            if(citations[j+1]<citations[j])
            {
                tmp=citations[j];
                citations[j]=citations[j+1];
                citations[j+1]=tmp;
                flag=true;
            }
        }
    }                 
}
int Solution::searchHindex(vector<int>& citations)
{
    int tempIndex=0;
    for(int index=1;index<citations.size();index++)
    {
        int gtIndex1=0;//大于index的元素个数（包含Index）
        int gtIndex2=0;//大于index的元素个数（不包含Index）
        for(int k=0;k<citations.size();k++)
        {
            if(citations[k]>=index)
            {
                gtIndex1=citations.size()-k;
                break;
            }
        }
        for(int k=0;k<citations.size();k++)
        {
            if(citations[k]>index)
            {
                gtIndex2=citations.size()-k;
                break;
            }
        }
        if(gtIndex2<=index&&index<=gtIndex1)
            tempIndex=max(tempIndex,index);
    }
    return tempIndex;
}
float Solution::computeHmedian(vector<int> citations,int Hindex)
{
    if(Hindex%2==1)
    {
        return float(*(citations.end()-(Hindex/2+1)));
    }
    else
    {
        return (float)((*(citations.end()-(Hindex/2+1)))+(*(citations.end()-(Hindex/2))))/2;
    }
}

