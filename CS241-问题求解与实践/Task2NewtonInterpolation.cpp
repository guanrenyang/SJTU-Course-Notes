#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;
double interval=0.1;
/*
0.0 0.1 0.2 0.3 0.4 0.5 
1.0 0.995 0.98 0.955 0.921 0.878
0.7
Output: 0.748
*/

// You can add additional standard libraries if necessary.
// Implement the Newton interpolation!
class Newton {
public:
    Newton(vector<double> x, vector<double> y): X(x), Y(y) {}
    double computeDifference(int k,int i);
    double predict(double x);
     double Factorial(double k)
    {   double sum=1;
        for(double i=1;i<=k;i++)
            sum*=i;
        return sum;
    }

private:
    vector<double> X, Y;
};
double Newton::predict(double x)
{
    double h=X[1]-X[0];
    double t=(x-X[0])/h;
    double sum=0;
    for(int k=1;k<X.size();k++)
    {
        double PI=1;
        for(int i=0;i<k;i++)
            PI*=(t-(double)i);
        sum+=((computeDifference(k,0)*PI)/(double)Factorial(k));
    }
    return (Y[0]+sum);
}
double Newton::computeDifference(int k,int i)//k为次数，i为差分序号
{
    if(k!=1)
        return computeDifference(k-1,i+1)-computeDifference(k-1,i);
    else
        return Y[i+1]-Y[i];
}

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
   // double interp_x;
   // cin >> interp_x;

    // Do Newton interpolation for interp_x using X and Y, and print your results
    // Note: The result retains three decimal places (rounded)!
    
    Newton nt(X,Y);
    cout<<'[';
    for(double i=-5;i<=5;i+=interval)
    {
        cout<<fixed<<setprecision(3)<<nt.predict(i)<<',';
    }
    cout<<']';
    // End
    return 0;
}
