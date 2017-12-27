#include <bits/stdc++.h>
using namespace std;
const int N = 30;
const double INF = 1e5;
const double eps = 1e-8;
struct Entity{
	 double fare,pro,dis;
	 Entity(double a=0,double b = 1.0,double c = 0.0):fare(a),pro(b),dis(c){}
};
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
	double P[N];
	double Dist[N][N];
	int count[N];
	int C ;// set of cluster node with central positions
	int L;// the length of suggested drive route
	int PoCab; // the position of one empty cab
	vector<pair<Entity,VI> > skyline[N][N][N];
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
	inline int cmp(const VI &a,const VI& b,int s){
		int t = 0,t2;
		for(int i = 0;i<a.size();i++){
			t2 = dcmp(Dist[i==0?s:a[i-1]][a[i]],Dist[i==0?s:b[i-1]][b[i]]);
			if(t*t2==-1) {
				t = 0;
				break;
			}
			if(t2) t = t2;
			t2 = dcmp(1-P[a[i]],1-P[b[i]]);
			if(t*t2==-1){
				t = 0;
				break;
			}
			if(t2) t = t2;
		}
		return t;
	}
	inline cmp(const pair<Entity,VI> &a,const pair<Entity,VI> &b){
		if(dcmp(a.X.fare,b.X.fare)==-1 && dcmp(a.X.pro,b.X.pro)==-1 && dcmp(a.X.dis,b.X.dis)==-1) return -1;
		if(dcmp(a.X.fare,b.X.fare)==1 && dcmp(a.X.pro,b.X.pro)==1 && dcmp(a.X.dis,b.X.dis)==1) return 1;
		return 0;
	}
	inline void check(int s,int t,int l,pair<Entity,VI> & v){
		static int cnt = 0;
		cnt++;
		vector<pair<Entity,VI> >::iterator it;
		bool flag = true;
		for(it = skyline[s][t][l].begin();it!=skyline[s][t][l].end();){
			int tmp = cmp(*it,v);
			if(tmp==-1){
				flag = false;
				break;
			} 
			if(tmp==1){
				it = skyline[s][t][l].erase(it);
			} else it++;
		}
		if(flag){
			skyline[s][t][l].push_back(v);
		}
	}
	void LCP(){
		pair<Entity,VI> tmp;
		for(int i = 1;i<=C+1;i++) skyline[i][i][0].push_back(tmp);
		for(int l = 1;l<=L;l++){
			for(int i = 1;i<=C+1;i++){
				for(int j = 1;j<=C;j++) if(i!=j){
					for(int s = C+1;s<=C+1;s++) {
						for(int k = 0;k<skyline[s][i][l-1].size();k++){
							int t;
							if(l-1>1 && j==skyline[s][i][l-1][k].Y[l-3]) continue;
							tmp = skyline[s][i][l-1][k];
							tmp.Y.push_back(j);
							tmp.X.dis += Dist[i][j];
							tmp.X.fare += tmp.X.pro*P[j]*tmp.X.dis;
							tmp.X.pro *= (1.0-P[j]);
							check(s,j,l,tmp);
						}
					}
				}
			}
		} 
	}
	VI skyroute(int l,double d[]){
		for(int i = 1;i<=C;i++) Dist[C+1][i] = d[i];
		L = l;
		for(int i =1;i<=C+1;i++) for(int j = 1;j<=C+1;j++) for(int l = 0;l<=L;l++) skyline[i][j][l].clear();
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
				dis += Dist[j==0?C+1:(it->Y)[j-1]][(it->Y)[j]];
				tmp += dis*p*P[(it->Y)[j]];
				p *= 1.0-P[(it->Y)[j]];
			} 
			tmp += INF*p;
			if(dcmp(tmp,E)==-1){
				E = tmp;
				ret = it->Y;
			}
		}
		printf("%.5lf\n",E);
		return ret;
	}
	void Test(){
		for(int i = 1;i<=C;i++) for(int j = 1;j<=C;j++) if(i!=j){
			for(int k = 1;k<=L;k++) printf("%d ",skyline[i][j][k].size());
			puts("");
		}
	}
}S;
int main(){
	srand(time(0));
	freopen("in.txt","r",stdin);
	S.Init();
	int Q;
	scanf("%d",&Q);
	double d[100];
	vector<int> ret;
	double tot = 0;
	int road = 0;
	for(int QQ = 1;QQ<=Q;QQ++){
		int start = clock();
		for(int i = 1;i<=S.C;i++) scanf("%lf",d+i);
		ret = S.skyroute(5,d);
		//for(int i = 0;i<ret.size();i++) printf("%d ",ret[i]); puts("");
		int finish = clock();
		int i;
		for(i = 0;i<ret.size();i++){
			double result = rand()%100/100.0;
			S.count[ret[i]]++;
			tot += S.Dist[i==0?S.C+1:ret[i-1]][ret[i]];
			road++;
			if(result<=S.P[ret[i]]){
				//S.P[ret[i]]*=0.75;
				S.P[ret[i]] = S.P[ret[i]]/(1+S.P[ret[i]]);
				//printf("pick up passengers at %d\n",ret[i]);
				break;
			}
		}
		//tot += i;
		//printf("Query time is %d\n",finish-start);
		//printf("till now total distance is %.5lf\n",tot);
	}
	//for(int i = 1;i<=S.C;i++) printf("%d ",S.count[i]); puts("");
	printf("Average distance is %.5lf\n",tot/Q);
	printf("Average road is %.5lf",1.0*road/Q);
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
