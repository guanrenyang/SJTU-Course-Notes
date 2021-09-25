#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int nthUglyNumber(int n)
    {
        vector<int> uglyNumber;
        uglyNumber.push_back(1);
        if(n==1) return 1;
        int big=0,mid=0,sma=0;
        for(;uglyNumber.size()<n;)
        {
            int b=2*uglyNumber[big];
            int m=3*uglyNumber[mid];
            int s=5*uglyNumber[sma];

            if(find(uglyNumber.begin(),uglyNumber.end(),b)!=uglyNumber.end())
                {++big;continue;}
            if(find(uglyNumber.begin(),uglyNumber.end(),m)!=uglyNumber.end())
                {++mid;continue;}
            if(find(uglyNumber.begin(),uglyNumber.end(),s)!=uglyNumber.end())
                {++sma;continue;}

            int temp=min(min(b,m),min(b,s));
            if(temp==b) ++big;
            else if(temp==m) ++mid;
            else if(temp==s) ++sma;
            uglyNumber.push_back(temp);
        }
        return uglyNumber.back();
        
    }
};

int main(){
    while(true)
	{Solution s;
	int n;
	cin >> n;
	cout << s.nthUglyNumber(n) << endl;}
	return 0;
}
