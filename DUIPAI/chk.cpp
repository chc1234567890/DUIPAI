#include"include/testlib.h"
#include<bits/stdc++.h>
using namespace std;
const int maxn=1003;
const double eps=1e-6;
int sgn(double x){return x<-eps?-1:x>eps;}
int a[maxn],b[maxn],c[maxn];
int main(int argc,char **argv){
	registerTestlibCmd(argc,argv);
	int T=inf.readInt();
	while(T--){
		int n=inf.readInt();
		for(int i=1;i<=n;i++)a[i]=inf.readInt(),b[i]=inf.readInt(),c[i]=inf.readInt();
		double A=ouf.readDouble();
		if(A==-1)continue;
		double B=ouf.readDouble(),C=ouf.readDouble();
		if(!(A<=B&&A<=C))quitf(_wa,"NOT A<=B AND A<=C");
		if(!(A>=100&&A<=1e6&&B>=100&&B<=1e6&&C>=100&&C<=1e6))quitf(_wa,"VIOLATES THE RANGE");
		double f1=A*a[1]+B*b[1]+C*c[1];
		bool flag=1;
		for(int i=2;i<=n;i++)if(sgn(f1-(A*a[i]+B*b[i]+C*c[i]))<0)quitf(_wa,"ILLEGAL i:%d f1:%.9lf fi:%.9lf",i,f1,A*a[i]+B*b[i]+C*c[i]);
	}
	quitf(_ok,"OK");
	return 0;
}
