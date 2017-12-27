#include <bits/stdc++.h>
using namespace std;
const int N = 30;
const int M = 10000;
const double INF = 1e4;
const double eps = 1e-8;
	FILE *fp  ;
inline double Poisson(double lambda,int k){
	long double ret = exp(-lambda);
	for(int i =1;i<=k;i++) ret*=lambda;
	for(int i =1;i<=k;i++) ret/=i;
	return ret;
}
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
	double P[N],P2[N];
	double Dist[N][N];
	int count[N];
	int C ;// set of cluster node with central positions
	int L;// the length of suggested drive route
	int PoCab; // the position of one empty cab
	vector<pair<Entity,VI> > skyline[N][N][N];
	void Init(){
		memset(count,0,sizeof(count));
		fscanf(fp,"%d%d",&C,&L);
		for(int i = 1;i<=C;i++) fscanf(fp,"%lf",P+i);
		for(int i = 1;i<=C;i++){
			for(int j = 1;j<=C;j++){
				fscanf(fp,"%lf",&Dist[i][j]);
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
			t2 = dcmp(1-P2[a[i]],1-P2[b[i]]);
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
							//cout<<l<<' '<<i<<' '<<j<<' '<<k<<endl;
							int t;
							if(l-1>1 && j==skyline[s][i][l-1][k].Y[l-3]) continue;
							tmp = skyline[s][i][l-1][k];
							tmp.Y.push_back(j);
							tmp.X.dis += Dist[i][j];
							tmp.X.fare += tmp.X.pro*P2[j]*tmp.X.dis;
							tmp.X.pro *= (1.0-P2[j]);
							check(s,j,l,tmp);
						}
					}
				}
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
				tmp += dis*p*P2[(it->Y)[j]];
				p *= 1.0-P2[(it->Y)[j]];
			} 
			tmp += INF*p;
			if(dcmp(tmp,E)==-1){
				E = tmp;
				ret = it->Y;
			}
		}
		//printf("%.5lf\n",E);
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
	fp = fopen("in.txt","r");
	//freopen("in.txt","r",stdin);
	S.Init();
	int Q;
	fscanf(fp,"%d",&Q);
	//printf("%d\n",Q);
	vector<int> ret;
	//double tot = 0;
	int road = 0;
	int Time = 0;
	for(int QQ = 1;QQ<=Q;QQ++){
		for(int i = 1;i<=S.C;i++) fscanf(fp,"%lf",d[QQ]+i);
		int start = clock();
		ans[QQ] = S.skyroute(5,d[QQ]);
		int finish = clock();
		Time += finish - start;
		//for(int i =0;i<ans[QQ].size();i++) cout<<ans[QQ][i]<<" ";cout<<endl;
	}
	
	int cnt = 0;
	for(int i =1;i<=S.C;i++) fscanf(fp,"%d",remain+i);
	//for(int i =1;i<=S.C;i++) printf("%d ",remain[i]); puts("");
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
		if(i==ret.size()) cnt++;//putchar('0'); 
		else effective += tmp;//putchar('1');
	}
	//puts("");
	//printf("%d\n",cnt);
	fclose(fp);
	//freopen("multi.txt","r",stdin);
	fp = fopen("multi.txt","a");
	//puts("YYY");
	double out[4];
	memset(out,0,sizeof(out));
//	for(int i =0;i<4;i++) fscanf(fp,"%lf",out[i]);
	//puts("YYY");    
	//for(int i =0 ;i<4;i++) fprintf(fp,"%lf\n",out[i]);
	//fclose(fp);
	//fp = fopen("multi.txt","w");
	out[0] += 1.0*Time/Q;
	out[1] += effective/(Q-cnt);
	out[2] += tot/Q;
	out[3] += cnt;
	for(int i = 0;i<4;i++) fprintf(fp,"%lf ",out[i]);
	fprintf(fp,"\n");
	fclose(fp);
	//for(int i = 1;i<=S.C;i++) printf("%d ",S.count[i]); puts("");
	/*
	printf("average time is %.5lf\n",1.0*Time/Q);
	printf("Number of cabs failing to pick up passengers is %d\n",cnt);
	printf("Average distance is %.5lf\n",tot/Q);
	printf("Average road is %.5lf\n",1.0*road/Q);
	//printf("efficiency is %.5lf\n",effective/tot);
	printf("Average efficient distance is %.5lf\n",effective/(Q-cnt));
	*/
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
