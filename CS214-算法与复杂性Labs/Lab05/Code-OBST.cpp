#include <iostream>

using namespace std;

#define MAX 10000

const int n = 7;
double p[n + 1] = {0,0.04,0.06,0.08,0.02,0.10,0.12,0.14};
double q[n + 1] = {0.06,0.06,0.06,0.06,0.05,0.05,0.05,0.05};

int root[n + 1][n + 1];//Record the root node of the optimal subtree
double e[n + 2][n + 2];//Record the expected cost of the subtree
double w[n + 2][n + 2];//Record the probability sum of the subtree

void optimal_binary_search_tree(double *p,double *q,int n)
{
//The result is stored in e.
/*
Please write your code here.
*/
    for (int i=1;i<=n+1;i++)
    {
        e[i][i-1]=q[i-1];
        w[i][i-1]=q[i-1];
    }
    for(int l=1;l<=n;l++)
    {
        for(int i=1;i<=n-l+1;i++)
        {
            int j=i+l-1;
            e[i][j]=INT_MAX;
            w[i][j]=w[i][j-1]+p[j]+q[j];
            for(int r=i;r<=j;r++)
            {
                double temp=e[i][r-1]+e[r+1][j]+w[i][j];
                if(temp<e[i][j])
                {
                    e[i][j]=temp;
                    root[i][j]=r;
                }
            }
        }
    }
}

/*
You can print the structure of the optimal binary search tree based on root[][]
The format of printing is as follows:
k2 is the root
k1 is the left child of k2
d0 is the left child of k1
d1 is the right child of k1
k5 is the right child of k2
k4 is the left child of k5
k3 is the left child of k4
d2 is the left child of k3
d3 is the right child of k3
d4 is the right child of k4
d5 is the right child of k5
*/
void construct_optimal_bst(int i,int j,int p,int d)
{
//You can adjust the number of input parameters
/*
Please write your code here.
*/
    if(p==0)
        cout<<"k"+to_string(root[i][j])+" is the root"<<endl;
    else if(i<=j)
    {
        if(d==0)
            cout<<"k"+to_string(root[i][j])+" is the left child of k"+to_string(p)<<endl;
        else
            cout<<"k"+to_string(root[i][j])+" is the right child of k"+to_string(p)<<endl;
    }
    
    if (i<root[i][j])
    {
        construct_optimal_bst(i,root[i][j]-1,root[i][j],0);
    }
    if(root[i][j]==j)
    {
        cout<<"d"+to_string(root[i][j])+" is the right child of k"+to_string(root[i][j])<<endl;
    }
    if(root[i][j]==i)
    {
        cout<<"d"+to_string(root[i][j]-1)+" is the left child of k"+to_string(root[i][j])<<endl;
    }
    if(root[i][j]<j)
    {
        construct_optimal_bst(root[i][j]+1,j,root[i][j],1);
    }
}

int main()
{
    optimal_binary_search_tree(p,q,n);
    cout<<"The cost of the optimal binary search tree is: "<<e[1][n]<<endl;
    cout << "The structure of the optimal binary search tree is: " << endl;
    construct_optimal_bst(1,n,0,0);
    return 0;
}
