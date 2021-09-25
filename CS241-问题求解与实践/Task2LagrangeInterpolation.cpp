#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <bits/stdc++.h>
using namespace std;
double interval=0.1;
/*
0.0 0.1 0.2 0.3 0.4 0.5 
1.0 0.995 0.98 0.955 0.921 0.878
0.7
*/


// You can add additional standard libraries if necessary.
// Implement the Lagrange interpolation!
class Lagrange {
public:
    Lagrange(vector<double> x, vector<double> y): X(x), Y(y){}
    double predict(double x);
    double l(double x,int i);

    

private:
    vector<double> X, Y;
};


// Test your implementation.
int main(int argc, const char * argv[]) {
    //  Input preprocessing.
    string str;
    getline(cin, str);
    stringstream xstr(str);
    getline(cin, str);
    stringstream ystr(str);

    // X and Y are two vectors of equal length to be traversed.
    vector<double> X, Y;
    double a;
    while (xstr >> a)
        X.push_back(a);
    while (ystr >> a)
        Y.push_back(a);
    
    // interp_x is the point to be interpolated.

//    double interp_x;
//    cin >> interp_x;

    // Do Lagrange interpolation for interp_x using X and Y, and print your results
    // Note: The result retains three decimal places (rounded)!
    Lagrange lg(X,Y);
    cout<<'[';
    for(double i=-5;i<=5;i+=interval)
    {
        cout<<fixed<<setprecision(3)<<lg.predict(i)<<',';
    }
    cout<<']';
    
    
    
    // End
    return 0;
}
double Lagrange::predict(double x)
{
    double result=0;
    for(int j=0;j<X.size();++j)
    {
        result+=Y[j]*l(x,j);
    }
    return result;
}
double Lagrange::l(double x,int j)
{
    double up=1,down=1;
    for(int i=0;i<X.size();++i)
    {
        if(i==j) continue;
        up*=(x-X[i]);
    }
    for(int i=0;i<X.size();++i)
    {
        if(i==j) continue;
        down*=(X[j]-X[i]);
    }
    return up/down;
}
