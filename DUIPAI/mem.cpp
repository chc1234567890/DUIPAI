#include<bits/stdc++.h>
using namespace std;
int main(int argc,char **argv){
	assert(argc==2);
	system("tasklist > log1.txt");
	ifstream fin("log1.txt");
	string s;
	while(getline(fin,s)){
		if(s.substr(0,s.find(".exe")+4)==argv[1]){
			char t[100];
			sscanf(s.c_str(),"%*s%*s%*s%*s%s%*s",t);
			for(int i=0;t[i];i++)if(isdigit(t[i]))putchar(t[i]);
			return 0;
		}
	}
	printf("-1");
	return 1;
}
