#include<iostream>
using namespace std;

double fastExponentiation(double x,int n){
    double ans=1.0;
    if(n<0){
        x=1.0/x;
        n=-n;
    }
    while(n>0){
        if(n%2!=0){
            ans*=x;
            n--;
        }
        else{
            n/=2;
            x*=x;
        }
    }
    return ans;
}

int main() {
    double base;
    int exponent;
    cout << "Enter base (x): ";
    cin >> base;
    cout << "Enter exponent (n): ";
    cin >> exponent;
    double result = fastExponentiation(base, exponent);
    cout << base << "^" << exponent << "=" << result;
    return 0;
}