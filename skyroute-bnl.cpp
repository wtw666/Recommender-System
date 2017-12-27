#include <bits/stdc++.h>
using namespace std;

const int N = 30;
const double INF = 1e5;
const double eps = 1e-8;
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
	int C ;// set of cluster node with central positions
	int L;// the length of suggested drive route
	int PoCab; // the position of one empty cab
	vector<VI> skyline[N][N][N];
	void Init(){
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
	inline void check(int s,int t,int l,VI & v){
		//puts("YYY");
		static int cnt = 0;
		cnt++;
		vector<VI>::iterator it;
		bool flag = true;
		for(it = skyline[s][t][l].begin();it!=skyline[s][t][l].end();){
			int tmp = cmp(*it,v,s);
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
		VI tmp;
		//puts("YYY");
		for(int i = 1;i<=C;i++) skyline[i][i][0].push_back(tmp);
		for(int l = 1;l<=L;l++){
			cout<<l<<endl;
			for(int i = 1;i<=C;i++){
				for(int j = 1;j<=C;j++) if(i!=j){
					for(int s = 1;s<=C;s++) if(s!=j){
						for(int k = 0;k<skyline[s][i][l-1].size();k++){
							tmp = skyline[s][i][l-1][k];
							tmp.push_back(j);
							check(s,j,l,tmp);
						}
					}
				}
			}
		} 
	}
	VI skyroute(int l,double d[]){
		#define TYPE list
		TYPE<pair<int,VI> > tmp,window,input;
		TYPE<pair<int,VI> >::iterator it,it2;
		TYPE<VI> cab_skyline;
		VI ret , tmp2;
		tmp.clear(),input.clear(),window.clear();cab_skyline.clear();
		for(int i = 1;i<=C;i++) Dist[C+1][i] = d[i];
		for(int i = 1;i<=C;i++)
			for(int j = 1;j<=C;j++) 
				for(int k = 0;k<skyline[i][j][l-1].size();k++){
					tmp2 =  skyline[i][j][l-1][k];
					tmp2.insert(tmp2.begin(),i);
					input.push_back(mp(0,tmp2));
		}
		printf("%d\n",input.size());
		int size = 500,time_stamp = 0,cnt = 0;
		while(!input.empty() || !window.empty()){
			for(it = input.begin();it != input.end();it++){
				int t = 0;
				for(it2 = window.end();it2!=window.begin();){
					it2--;
					if(it2->X<it->X) break;
					t = cmp(it2->Y,it->Y,C+1);
					if(t==-1) break;
					if(t==1) {
						it2 = window.erase(it2);
						cnt--;
					}
				}
				//printf("%d\n",time_stamp);
				if(t!=-1){
					if(cnt == size) tmp.push_back(mp(++time_stamp,it->Y));
					else window.push_back(mp(++time_stamp,it->Y)),cnt++;
				}
			}
			puts("YYY");
			if(!tmp.empty()){
				for(it = window.begin();it != window.end();){
					if(it->X<tmp.begin()->X) {
						cab_skyline.push_back(it->Y);
						it = window.erase(it);
						cnt--;
					} else break;
				} 
				
			} else 
				for(it = window.begin();it!=window.end();) {
					cab_skyline.push_back(it->Y);
					it = window.erase(it);
					cnt --;
				}
			input = tmp;
			tmp.clear();
			//puts("UUU");
		}
	
		double E = INF*INF;
		for(TYPE<VI>::iterator it = cab_skyline.begin();it != cab_skyline.end();it++){
			double tmp  = 0, p = 1.,dis = 0;
			for(int j = 0;j<it->size();j++){
				dis += Dist[j==0?C+1:(*it)[j-1]][(*it)[j]];
				tmp += dis*p*P[(*it)[j]];
				p *= 1.0-P[(*it)[j]];
			}
			tmp += INF*p;
			if(dcmp(tmp,E)==-1){
				E = tmp;
				ret = *it;
			}
		}
		//printf("%.5lf\n",E);
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
	freopen("in.txt","r",stdin);
	S.Init();
	//puts("YYY");
	int start  = time(0);
	S.LCP();
	int finish = time(0);
	printf("offline time is %d\n",finish-start);
	//S.Test();
	//puts("YYY");
	int Q;
	scanf("%d",&Q);
	double d[100];
	vector<int> ret;
	while(Q--){
		int start  = time(0);
		for(int i = 1;i<=S.C;i++) scanf("%lf",d+i);
		ret = S.skyroute(5,d);
		for(int i = 0;i<ret.size();i++) printf("%d ",ret[i]); puts("");
		int finish = time(0);
		printf("Query time is %d\n",finish-start);
	}
	
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
