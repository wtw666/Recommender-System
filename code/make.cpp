#include <bits/stdc++.h>
using namespace std;

int main(){
	srand(time(0));
	freopen("in.txt","w",stdout);
	int T = 32;
	printf("%d\n",T);
	while(T--){
		int n = 800,m = 7;
		printf("%d %d\n",n,m);
		for(int i =1;i<=n;i++){
			for(int j =1;j<m;j++) 
				printf("%d ",rand()%2+1);
			puts("");
		}
		for(int i =1;i<n;i++){
			for(int j =1;j<=m;j++) 
				printf("%d ",rand()%2+1);
			puts("");
		}
	}
	//puts("YYY");
	return 0;
}
