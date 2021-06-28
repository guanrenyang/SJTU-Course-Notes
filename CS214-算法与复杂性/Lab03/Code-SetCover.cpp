#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

void quickSort(int s[], int l, int r)
{
    if (l< r)
    {
        int i = l, j = r, x = s[l];
        while (i < j)
        {
            while(i < j && s[j]>= x)
                j--;
            if(i < j)
                s[i++] = s[j];
            while(i < j && s[i]< x)
                i++;
            if(i < j)
                s[j--] = s[i];
        }
        s[i] = x;
        quickSort(s, l, i - 1);
        quickSort(s, i + 1, r);
    }
}

int Greedy(vector<int> A, int k, int n)
{
    
    int length=A.size();//size of the array
    int size=length;//size of the set
    sort(A.begin(),A.end());
    int num_interval;

    for(vector<int>::iterator it=A.begin();it<A.end();++it)
    {
        for(int i=0;i<k;i++)
        {
            int base=(*it);
            if(find(A.begin(),A.end(),base+i)!=A.end())
            {
                it=find(A.begin(),A.end(),base+i);
            }
        }
        num_interval++;
    }
    return num_interval;
}

int main()
{
    //x is the point set P with n=7 nodes in total, and the length of intervals is k=3.
    vector<int> x={1,2,3,4,5,6,-2};
    int k=3;
    int n=x.size();
    int num_interval=Greedy(x,k,n);
    cout << num_interval << endl;
    return 0;
}
