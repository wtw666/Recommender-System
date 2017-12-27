#include <bits/stdc++.h>
using namespace std;
const int N = 30;
const int M = 10000;
const double INF = 1e2;
const double eps = 1e-8;
inline double Poisson(double lambda,int k){
	long double ret = exp(-lambda);
	for(int i =1;i<=k;i++) ret*=lambda;
	for(int i =1;i<=k;i++) ret/=i;
	return ret;
}
struct Route{
	double dis,pro,ptd;
	int last;
};
const int M2 = 10000000;
class SkyRoute{
	public:
	#define X first
	#define Y second
	#define mp make_pair
	typedef vector<int> VI;
	typedef pair<int,int> PII;
	inline int dcmp(const double& a,const double &b){
		if(fabs(a-b)<eps) return 0;
		else if(a>b) return 1;
		return -1;
	}
	Route R[M2];
	int f[N][10],next[M2],last[M2];
	int tot;
	double P[N],P2[N];
	double Dist[N][N];
	int count[N];
	int C ;// set of cluster node with central positions
	int L;// the length of suggested drive route
	int PoCab; // the position of one empty cab
	//vector<pair<Entity,VI> > skyline[N][N][N];
	inline void  add(int &x,Route r){
		tot++;
		next[tot] = last[tot] = 0;
		next[tot] = x;
		R[tot] = r;
		if(x!=0) {
			last[tot] = last[x];
			last[x] = tot;
			if(last[tot]!=0) next[last[tot]] = tot;
		}
		x = tot;
	}
	void Init(){
		memset(count,0,sizeof(count));
		scanf("%d%d",&C,&L);
		for(int i = 1;i<=C;i++) scanf("%lf",P+i);
		for(int i = 1;i<=C;i++){
			for(int j = 1;j<=C;j++){
				scanf("%lf",&Dist[i][j]);
			}
		}
	}
	
	
	void LCP(){
		for(int i = 0;i<L;i++){
			for(int j = 1;j<=C+1;j++)
				for(int k = 1;k<=C+1;k++) if(j!=k)
					for(int r = f[j][i];r!=0;r=next[r]){
						Route tmp;
						tmp.dis = R[r].dis+Dist[j][k];
						tmp.pro = R[r].pro*(1.0-P[k]);
						tmp.ptd = R[r].ptd+Dist[j][k]*R[r].pro;
						tmp.last = r;
						
					}	
		}	
	}
	VI skyroute(int l,double d[]){
		for(int i = 1;i<=C;i++){
			double lambda = -log(1.0-P[i]);
			P2[i] = 1.0;
			for(int j = 0;j<=count[i];j++) P2[i] -= Poisson(lambda,j);
		}
		//for(int i =1;i<=C;i++) printf("%.3lf ",P2[i]); puts("");
		for(int i = 1;i<=C;i++) Dist[C+1][i] = d[i];
		L = l;
		//for(int i =1;i<=C+1;i++) for(int j = 1;j<=C+1;j++) for(int l = 0;l<=L;l++) skyline[i][j][l].clear();
		tot = 0;
		for(int i = 1;i<=C+1;i++) for(int j = 0;j<=L;j++) f[i][j] = 0;
		add(f[C+1][0],Route{0.0,1.0,0.0,0});
		LCP();
		VI ret;
		#define TYPE vector
		double E = INF*INF;
		int cnt = 0;
		for(int i = 1;i<=C;i++)
		for(TYPE<pair<Entity,VI> >::iterator it = skyline[C+1][i][L].begin();it != skyline[C+1][i][L].end();it++){
			//cnt++;
			double tmp  = 0, p = 1.,dis = 0;
			for(int j = 0;j<it->Y.size();j++){
				dis += 1;
				tmp += dis*p*P2[(it->Y)[j]];
				p *= 1.0-P2[(it->Y)[j]];
			} 
			tmp += p;
			if(dcmp(tmp,E)==-1){
				E = tmp;
				ret = it->Y;
			}
		}
		printf("%.5lf\n",E);
		for(int i = 0;i<2;i++){
			bool flag = true;
			for(int j = 0;j<i && flag;j++) if(ret[j]==ret[i]) flag = false;
			if(flag) count[ret[i]]++;
		}
		return ret;
	}
	void Test(){
		for(int i = 1;i<=C;i++) for(int j = 1;j<=C;j++) if(i!=j){
			for(int k = 1;k<=L;k++) printf("%d ",skyline[i][j][k].size());
			puts("");
		}
	}
}S;
double d[M][N];
vector<int> ans[M];
int remain[N];
int main(){
	srand(time(0));
	freopen("in.txt","r",stdin);
	S.Init();
	int Q;
	scanf("%d",&Q);
	vector<int> ret;
	//double tot = 0;
	int road = 0;
	int Time = 0;
	for(int QQ = 1;QQ<=Q;QQ++){
		for(int i = 1;i<=S.C;i++) scanf("%lf",d[QQ]+i);
		int start = clock();
		ans[QQ] = S.skyroute(5,d[QQ]);
		int finish = clock();
		Time += finish - start;
		//for(int i =0;i<ans[QQ].size();i++) cout<<ans[QQ][i]<<" ";cout<<endl;
	}
	printf("average time is %.5lf\n",1.0*Time/Q);
	int cnt = 0;
	for(int i =1;i<=S.C;i++) scanf("%d",remain+i);
	for(int i =1;i<=S.C;i++) printf("%d ",remain[i]); puts("");
	cnt = 0;
	double tot = 0;
	double effective = 0;
	for(int QQ = 1;QQ<=Q;QQ++){
		ret = ans[QQ];
		//for(int i =0;i<ret.size();i++) cout<<ret[i]<<" ";cout<<endl;
		int i;
		double tmp = 0;
		for(i = 0;i<ret.size();i++){
			if(i>0) tmp += S.Dist[ret[i-1]][ret[i]];
			else tmp += d[QQ][ret[i]];
			road++;
			if(remain[ret[i]]){
				remain[ret[i]]--;
				break;
			} 
		}
		tot += tmp;
		if(i==ret.size()) cnt++; 
		else effective += tmp;
	}
	puts("");
	
	//for(int i = 1;i<=S.C;i++) printf("%d ",S.count[i]); puts("");
	printf("Number of cabs failing to pick up passengers is %d\n",cnt);
	printf("Average distance is %.5lf\n",tot/Q);
	printf("Average road is %.5lf\n",1.0*road/Q);
	//printf("efficiency is %.5lf\n",effective/tot);
	printf("Average efficient distance is %.5lf\n",effective/(Q-cnt));
	return 0;
}
/*
5 3
0.1 0.2 0.3 0.4 0.5
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1 
1 1 1 1 1
1 1 1 1 1

3 3
0.1 0.2 0.3
1 1 1 
1 1 1 
1 1 1 

4 3
0.1 0.2 0.3 0.4
1 1 1 1 
1 1 1 1 
1 1 1 1  
1 1 1 1 

5 4
0.1 0.2 0.3 0.4 0.5
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1 
1 1 1 1 1
1 1 1 1 1

2
1 1 1  1 1
1 2 3 4 5 


*/
