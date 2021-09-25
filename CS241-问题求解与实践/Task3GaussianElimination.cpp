#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

/*
10 -1 2 0 6
-1 11 -1 3 25
2 -1 10 -1 -11
0 3 -1 8 15
Output: 1.000 2.000 -1.000 1.000

1 1 1 4
2 2 1 6
1 1 2 6
No unique solution!

2 3 10
2 3 12
Output: No solution!
*/
// Implement your Gaussian Elimination algorithm.
// You can add any standard library if needed.
//
double accuracy=0.00000000001;
double z=0;
void compute(vector<vector<double>> m,vector<double> p,int N);

bool equal(double a)
{
    if(abs(a)<=accuracy)
        return 1;
    else
        return 0;
}
bool equal(double a,double b)
{
    if(abs(a-b)<=accuracy)
        return 1;
    else
        return 0;
}
// Test your implementation.
int main() {

    vector<double> stream;
    double temp;
    while (cin>>temp)
    {
        stream.push_back(temp);

    }
    int N=1;
    while (N<stream.size())
    {
        if(N*(N+1)==stream.size())
            break;
        N++;
    }

    vector<vector<double>> a(N,vector<double>(N,0));
    vector<double>b;
    int t=0;
    for(int i=0;i<N;i++)
    {  
       for(int j=0;j<N;j++)
       {
           a[i][j]=stream[t];
           t++;
       } 
       b.push_back(stream[t]);
       t++;
    }
    /*for(int i=0;i<a.size();i++)
    {
        for(int j=0;j<a[0].size();j++)
            cout<<a[i][j]<<' ';
        cout<<b[i]<<endl;
    }
    */
    // Input processing.
    /*
    vector<double> c;
    vector<double> b;
    
    int row=0, N;
    while(true)
    {
        double db;
        cin>>db;
        char ch=cin.get();
        if(ch!='\n')
        {
            c.push_back(db);
        }
        else
        {
            b.push_back(db);
            break;
        }
    }
    N=c.size();
    vector<vector<double>> a(N,vector<double>(N,0));
    vector<double> x(N,0);

    a[0].clear();
    for(int i=0;i<N;i++)
    {
        a[0].push_back(c[i]);
    }
    row=1;
    
    
    for(int i=1;i<N;i++)
        a[i].clear();

    int j=1;    
    do
    {
        double db;
        cin>>db;
        char ch=cin.get();
        
        if(ch!='\n')
        {
            a[j].push_back(db);
        }
        else
        {
            b.push_back(db);
            row++;
            j++;
        }
        
    } while (row<N);
    */
    // Solve the linear system and print the results.

    compute(a,b,N);
    system("pause");
    return 0;
}
int findPrincipleCpn(vector<vector<double>> obj,int col)
{
    int row=col;
    int pCpn=abs(obj[row][col]);
    for(int i=row+1;i<obj.size();i++)
    {
        if(abs(obj[i][col])>abs(pCpn))
        {
            row=i;
            pCpn=abs(obj[i][col]);
        }
    }
    return row;
}
void Swap(vector<vector<double>> & obj,int rowOfPcpn,int currentRow)
{
    int currentCol=currentRow;
    for(int j=currentRow;j<obj[0].size();j++)
    {
        swap(obj[rowOfPcpn][j],obj[currentRow][j]);
    }
}
void Elimination(vector<vector<double>> &obj,int row)
{
    int col=row;

    //主元位置为row，col
    for(int r=row+1;r<obj.size();r++)
        {
            double l=obj[r][col]/obj[row][col];
            for(int c=col;c<obj[0].size();c++)
            {
                obj[r][c]=obj[r][c]-obj[row][c]*l;
                if(equal(obj[r][c])) 
                    obj[r][c]=0;
            }
        }
}
void Solve(vector<vector<double>> & obj,vector<double> &X)
{
    int n=obj.size();
    X[n-1]=obj[n-1][n]/obj[n-1][n-1];
    for(int i=n-2;i>=0;i--)
    {
        double sum=0;
        for(int j=i+1;j<n;j++)
            sum+=obj[i][j]*X[j];
        X[i]=(obj[i][n]-sum)/obj[i][i];    
    }
}
void compute(vector<vector<double>> a,vector<double> b,int N)
{
    vector<vector<double>> A(a);
    vector<double> X(a.size(),0);
    for(int i=0;i<A.size();i++)
    {
        A[i].push_back(b[i]);
    }
    for(int i=0;i<A.size();i++)//i代表当前主行
    {
        int rowOfPcpn=findPrincipleCpn(A,i);
        
        if(!equal(A[rowOfPcpn][i]))//如果主元不等于0，就将其换到ii位置
        {
            Swap(A,rowOfPcpn,i);

            Elimination(A,i);//用第i行高斯消元

        }
        else//如果主元等于0
        {
            continue;
        }
    }

    int solutionSituations=0;//0代表有唯一解，1代表无穷多解，2代表无解

    for(int i=0;i<A.size();i++)
    {
        bool stay=0;
        if(!equal(A[i][i]))
            continue;
        else//ii位置的元素为0
        {   solutionSituations=1;
            int k;
            for(k=i+1;k<A[0].size();k++)
            {
                if(!equal(A[i][k]))//找到第i行第一位不为0的数，就在第k列找不为0的数
                {
                    for(int r=i+1;r<A.size();r++)//找第k列哪一个元素不等于0
                    {
                        if(!equal(A[r][k]))//第r行第k列的元素不为0
                        {
                           double l=A[i][k]/A[r][k];
                           for(int t=k;t<A[0].size();t++)
                           {
                               A[i][t]=A[i][t]-l*A[r][t];
                               if(equal(A[i][t]))
                                    A[i][t]=0;
                           }
                           i--;
                           stay=1;
                           break;
                        }
                    }//如果都是0则不考虑
                }
                if(stay==1)
                    break;
            }
        }
    }

//下面检查矩阵的秩
    int rank=0;
    for(int i=0;i<A.size();i++)
    {
        bool allCoeZero=1;
        for(int j=0;j<A[0].size()-1;j++)
        {
            if(!equal(A[i][j]))//第i行只要有一个系数不等于0，那么就不全为0
            {
                allCoeZero=0;
                break;
            }
        }
        if(allCoeZero==1&&A[i].back()!=0)//第i行元素全都是0，检查常数
        {
            solutionSituations=2;//方程无解
            break;
        }
        else if(allCoeZero==0)//第i行元素不全为0，秩加1
        {
            rank++;
        }
        //第i行全为0，常数也为0，相当于没有这个方程，故不考虑，仍为无穷多解
    }
    if(rank==A.size()) 
        solutionSituations=0;
    switch(solutionSituations)
    {
        case 0:
        {
            solve(A,X);
            for(int i=0;i<X.size();i++)
            {
                cout<<fixed<<setprecision(3)<<X[i]<<' ';
            }
            break;
        }
        case 1:cout<<"No unique solution!";break;
        case 2:cout<<"No solution!";break;
    }
}
bool checkRatio(vector<double> obj1,vector<double> obj2,int col)//成比例则返回1
{
    double l=obj1[col]/obj2[col];
    int flag=1;
    for(int j=col;j<obj1.size();j++)
    {
        if(!equal(obj1[j],l*obj2[j]))//如果两行只要有一个不成比例，那么两行就不成比例
        {
            flag=0;
            break;
        }
    }
    return flag;
}
void setZero(vector<double> & obj)
{
    for(int i=0;i<obj.size();i++)
    {
        obj[i]=0;
    }
}
