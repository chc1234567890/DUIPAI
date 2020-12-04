#include<bits/stdc++.h>
using namespace std;

typedef long long D;
typedef pair<int,int> P;

struct _init{
	mt19937 rng;
	static D getTimeStamp(){
	    chrono::time_point<chrono::system_clock,chrono::microseconds> tp=chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now());
	    auto tmp=chrono::duration_cast<chrono::microseconds>(tp.time_since_epoch());
	    time_t timestamp=tmp.count();
	    return timestamp;
	}
	_init():rng(getTimeStamp()){}
}Init;

int rnd(int l,int r){return uniform_int_distribution<int>(l,r)(Init.rng);}
int rnd(int r=1e9){return rnd(1,r);}
D rndll(D l,D r){return uniform_int_distribution<D>(l,r)(Init.rng);}
D rndll(D r=1e18){return rndll(1,r);}
double rnddb(double l,double r){return uniform_real_distribution<double>(l,r)(Init.rng);}

struct OS{
	static const int MAXN=1e6;
	char _In[MAXN],*_at=_In;
	inline void flush(){fwrite(_In,1,_at-_In,stdout),_at=_In;}
	~OS(){flush();}
	inline void pc(char c){if(_at==_In+MAXN)flush();*_at++=c;}
	template<class int_type>
	void write_int(int_type x){
		if(!x){pc('0');return;}
		static char buf[43],*at;
		at=buf;
		if(x<0)pc('-'),x=-x;
		while(x)*at++=x%10+'0',x/=10;
		do pc(*--at); while(at!=buf);
	}
	template<class str_ptr_type>
	void write_str(str_ptr_type str){
		while(*str)pc(*str++);
	}

	OS &operator<<(char x){
		pc(x);
		return *this;
	}
	OS &operator<<(unsigned char x){
		write_int(x);
		return *this;
	}
#define DEF(type) \
	OS &operator<<(type x){ \
		write_int(x); \
		return *this; \
	} \
	OS &operator<<(unsigned type x){ \
		write_int(x); \
		return *this; \
	}
	DEF(short);
	DEF(int);
	DEF(long long);
	DEF(__int128);
#undef DEF
	OS &operator<<(const char *x){
		write_str(x);
		return *this;
	}
	OS &operator<<(char *x){
		write_str(x);
		return *this;
	}
	OS &operator<<(const string &s){
		for(char i:s)pc(i);
		return *this;
	}
	template<typename vector_type>
	OS &operator<<(const vector<vector_type> &v){
		for(const auto &i:v)(*this)<<i<<' ';
		return *this;
	}
	OS &operator<<(void f(OS&)){
		f(*this);
		return *this;
	}
}out;

void endl(OS &_out){
	_out<<'\n',_out.flush();
}

template<class type>
type randomly_choose(const initializer_list<type> &a){
	return *(a.begin()+rnd(a.size())-1);
}
template<class type>
type randomly_choose(const vector<type> &a){
	return a[rnd(a.size())-1];
}

vector<int> gPerm(int n){
	vector<int> a(n);
	for(int i=0;i<n;i++)a[i]=i+1;
	random_shuffle(a.begin(),a.end());
	return a;
}
vector<int> gArray(int n,int lower,int upper){
	vector<int> a(n);
	for(int i=0;i<n;i++)a[i]=rnd(lower,upper);
	return a;
}
string gStr(int n,const string &charset){
	assert(charset.size()%2==0);
	string st,s;
	for(int i=0;i<int(charset.size());i+=2)for(char j=charset[i];j<=charset[i+1];j++)st+=j;
	for(int i=0;i<n;i++)s+=st[rnd(st.size())-1];
	return s;
}
vector<int> f;
int find(int x){return x!=f[x]?f[x]=find(f[x]):x;}
vector<vector<int> > gTree(int n){
	vector<vector<int> > G(n);
	f.resize(n);
	for(int i=0;i<n;i++)f[i]=i;
	for(int i=1;i<n;i++){
		int u,v;
		do u=rnd(n)-1,v=rnd(n)-1;while(find(u)==find(v));
		f[find(u)]=find(v);
		G[u].push_back(v),G[v].push_back(u);
	}
	return G;
}
struct edge{
	int v,w;
	edge(){}
	edge(int _v,int _w):v(_v),w(_w){}
};
vector<vector<edge> > gTreew(int n,int lower,int upper){
	vector<vector<edge> > G(n);
	f.resize(n);
	for(int i=0;i<n;i++)f[i]=i;
	for(int i=1;i<n;i++){
		int u,v;
		do u=rnd(n)-1,v=rnd(n)-1;while(find(u)==find(v));
		f[find(u)]=find(v);
		int w=rnd(lower,upper);
		G[u].push_back(edge(v,w)),G[v].push_back(edge(u,w));
	}
	return G;
}
