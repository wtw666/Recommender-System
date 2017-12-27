#include <bits/stdc++.h>
using namespace std;
const int N = 50;
int Dist[N][N],x;
double P[N];
int remain[N];
inline double Poisson(double lambda,int k){
	long double ret = exp(-lambda);
	for(int i =1;i<=k;i++) ret*=lambda;
	for(int i =1;i<=k;i++) ret/=i;
	return ret;
}
int main(){
	srand(time(0));
	freopen("gr21.tsp","r",stdin);
	for(int i =1;i<=21;i++){
		for(int j =1;j<i;j++) scanf("%d",&Dist[i][j]),Dist[j][i] = Dist[i][j];
		scanf("%d",&x);
	}
	
	freopen("in.txt","w",stdout);
	int C = 15,M = 20,L = 5;
	printf("%d %d\n",C,L);
	for(int i =1;i<=C;i++){
		double tmp = rand()%10000/10000.0;
		//P[i] = tmp=1-tmp*tmp;
		P[i] = tmp = sqrt(tmp);
		printf("%.5lf ",tmp);
	}
	puts("");
	for(int i =1;i<=C;i++) {
		for(int j =1;j<=C;j++) printf("%d ",Dist[i][j]);
		puts("");
	}
	printf("%d\n",M);
	for(int i =1;i<=M;i++){
		int tmp = rand()%6;
		for(int j = 1;j<=C;j++) printf("%d ",Dist[16+tmp][j]);
		puts("");
	}
	double c = 0;
	for(int i = 1;i<=C;i++){
		double lambda = -log(1.0-P[i]);
		c+=lambda;
		//cout<<lambda<<endl;
		double pro = rand()%10000/10000.0;
		//cout<<pro<<endl;
		for(int k = 0;;k++){
			pro -= Poisson(lambda,k);
			remain[i] = k;
			if(pro<0) break;
		}
		//cout<<remain[i]<<endl;
		printf("%d ",remain[i]);
	}
	puts("");
	cerr<<c/C<<endl;
	return 0;
}
