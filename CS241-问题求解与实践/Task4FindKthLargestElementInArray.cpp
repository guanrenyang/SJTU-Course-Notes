#include <iostream>
#include <vector>
using namespace std;

// Implement your selection algorithm. You can add any standard library if needed.
//





int main() {
    // Input processing.
    vector<int> a;
    //input data
    int temp;
    while (cin>>temp)
    {
        a.push_back(temp);
    }
    int k=a.back();
    a.pop_back();
    //find the biggest and the smallest one in vector a
    int max=0,min=0;
    for(int i=0;i<a.size();i++)
    {
        if(a[i]>max) max=a[i];
        if(a[i]<min) min=a[i];
    }
    
    //declare vector c
    int sizeOfC=max-min+1;
    vector<int> c(sizeOfC,0);
    
    
    // select the target and print the results.

    //counting
    for(int i=0;i<a.size();i++)
    {
        c[a[i]-min]++;
    }

    //select the kth biggest element
    int count=0;//
    int kthBIgggest;
    for(int i=c.size()-1;i>=0;i--)
    {
        count+=c[i];
        if(count>=k)
        {
            kthBIgggest=i;
            break;
        }
    }
    
    cout<<kthBIgggest;
    

    return 0;
}
