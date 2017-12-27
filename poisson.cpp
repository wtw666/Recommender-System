#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>
#include <cstring>
#include <cstdlib>
#include <queue>
#include <cmath>
using namespace std;

namespace Random{
	const int max_int  = 1LL<<31-1;
	//srand(time(0));
	double random_real(){
		return 1.0*rand()/max_int;
	}
	static int Poisson(double mean){
		double limit = exp(-mean);
		double product = random_real();
		int count = 0;
		while(product>limit){
			count++;
			product*=random_real();
		}
		return count;
	}
}
int main(){
	int *p ;
	if(p==NULL) cout<<"YES"<<endl; else cout<<"NO"<<endl;
	p = new int ;
	if(p==NULL) cout<<"YES"<<endl; else cout<<"NO"<<endl;
	delete p;
	//delete p;
	if(p==NULL) cout<<"YES"<<endl; else cout<<"NO"<<endl;
	return 0;
}
