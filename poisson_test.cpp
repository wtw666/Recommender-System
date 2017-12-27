#include <bits/stdc++.h>
using namespace std;
const double e = exp(1);
inline double Poisson(double lambda,int k){
	long double ret = exp(-lambda);
	for(int i =1;i<=k;i++) ret*=lambda;
	for(int i =1;i<=k;i++) ret/=i;
	return ret;
}
int main(){
	double lambda = 0.5;
	int k;
	k = 0;
	for(int i = 0;i<20;i++,lambda +=0.1){
		cout<<"lambda = "<<lambda<<endl;
		for(int k = 0;k<=5;k++) cout<<Poisson(lambda,k)<<' '; 
		cout<<endl;
	}
	return 0;
}
