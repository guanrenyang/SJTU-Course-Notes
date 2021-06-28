#include<bits/stdc++.h>
#define INFINITY 100000
/*
The process to calculate the dynamic can be divided into four steps:
1.Create an empty cost matrix DTW with X and Y labels as amplitudes of the two series to be compared.
2.Use the given state transition function to fill in the cost matrix.
3.Identify the warping path starting from top right corner of the matrix and traversing to bottom left. The traversal path is identified based on the neighbor with minimum value.
i.e., When we reach the point (i, j) in the matrix, the next position is to choose the point with the smallest cost among (i-1,j-1), (i,j-1), and (i-1,j),
For the sake of simplicity, when the cost is equal, the priority of the selection is (i-1,j-1), (i,j-1), and (i-1,j) in order.
4.Calculate th time normalized distance. We define it as the average cost of the selected points.
*/
using namespace std;

double distance(vector<int> x, vector<int> y,int window_constraint=INFINITY ) {
    int n = x.size();
    int m = y.size();
    vector<vector<int>> DTW(n,vector<int>(m,0));
    //Use the given state transition function to fill in the cost matrix.
    /*
    Please write your code here.
    */
    for(int i=0;i<DTW.size();++i)
    {
        for(int j=0;j<DTW[0].size();++j)
        {
    
    /*
    Modication 1:
    If you input the window_constraint, the comparison will be operated. Otherwise, things will go on and we have no need to deal with the confusing modification. 
    */
            if(abs(i-j)>window_constraint)
            {
                DTW[i][j]=INFINITY;
                continue;
            }
            if(i==0&&j==0)
                DTW[i][j]=abs(x[i]-y[j]);
            else if(i==0)
                DTW[i][j]=abs(x[i]-y[j])+DTW[i][j-1];
            else if(j==0)
                DTW[i][j]=abs(x[i]-y[j])+DTW[i-1][j];
            else 
                DTW[i][j]=abs(x[i]-y[j])+min(min(DTW[i-1][j],DTW[i-1][j-1]),DTW[i][j-1]);
        }
    }

    vector<int> d;
    //Identify the warping path.
    /*
    Please write your code here.
    */
    int index=n-1;
    int column=m-1;
    /*
    Modification 2
    Since that if the window constraint is too small, D[n,m] will no longer exist if |n-m|>window_constrainst.
    I don't think that I had a deep understanding of the rule of modication. Whatever, I assume that D[n,m] can't be computed if |n-m|>window_constraint>
    As a result, I do the "tracking back" operation beginning with D[n,n+window_constraint] or D[m+window_constraint,m] if |n-m|>window_constraint.
    */
    if(window_constraint!=INFINITY)
    {
        if(n<=m)
        {
            index=n-1;
            column=min(m-1,n-1+window_constraint);
        }
        else
        {
            column=m-1;
            index=min(n-1,m-1+window_constraint);
        }
    }
        
    d.push_back(DTW[index][column]);
    while(true)
    {
        if(index==0&&column==0)
        {
            break;
        }
        else if(index==0)
        {
            d.push_back(DTW[index][column-1]);
            column--;
        }
        else if (column==0)
        {
            d.push_back(DTW[index-1][column]);
            index--;
        }
        else
        {
            int min=DTW[index-1][column-1];
            int min_index=index-1;
            int min_column=column-1;
            if(DTW[index][column-1]<min)
            {
                min=DTW[index][column-1];
                min_index=index;
                min_column=column-1;
            }
            if(DTW[index-1][column]<min)
            {
                min=DTW[index-1][column];
                min_index=index-1;
                min_column=column;
            }
            d.push_back(min);
            index=min_index;
            column=min_column;
        }
    }

    double ans = 0;
    //Calculate th time normalized distance
    /*
    Please write your code here.
    */
    
    ans=double(accumulate(d.begin(),d.end(),0))/double(d.size());

    return ans;
}

int main(){
	vector<int> X,Y;
	//test case 1
	X = {37,37,38,42,25,21,22,33,27,19,31,21,44,46,28};
	Y = {37,38,42,25,21,22,33,27,19,31,21,44,46,28,28};
	cout<<"Case 1, no window constraint: "<<distance(X,Y)<<endl;
    cout<<"Case 1, window constraint=0 : "<<distance(X,Y,0)<<endl;
    cout<<"Case 1, window constraint=1 : "<<distance(X,Y,1)<<endl<<endl;
	//test case 2
	X = {11,14,15,20,19,13,12,16,18,14};
	Y = {11,17,13,14,11,20,15,14,17,14};
	cout<<"Case 2, no window constraint: "<<distance(X,Y)<<endl;
    cout<<"Case 2, window constraint=0 : "<<distance(X,Y,0)<<endl;
    cout<<"Case 2, window constraint=1 : "<<distance(X,Y,1)<<endl;
    
	//Remark: when you modify the code to add the window constraint, the distance function has thus three inputs: X, Y, and the size of window w.

    system("pause");
}
