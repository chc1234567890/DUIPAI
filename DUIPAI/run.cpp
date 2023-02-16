#if defined(__WIN32)||defined(__WIN64)
#define WINDOWS
#endif

#ifdef WINDOWS
#define SLASH '\\'
#else
#define SLASH '/'
#endif

#include<bits/stdc++.h>
using namespace std;
const string nullfile="NULL";
string name,in=nullfile,out=nullfile,args;
int time_limit;
bool done;
bool is_null(const string &prog){
	return prog.substr(prog.rfind(SLASH)+1)==nullfile;
}
string offdir(const string &s){
	int p=s.rfind(SLASH);
	return s.substr(p+1);
}
void quit(int return_val,int Time=-1){
	FILE *f=fopen("run_log.txt","w");
	fprintf(f,"%d %d",return_val,Time);
	fclose(f);
	exit(0);
}
void TL(){
	this_thread::sleep_for(chrono::milliseconds(time_limit+200));
	fclose(stdout);
	done=1;
	system(("taskkill /f /im "+offdir(name)+".exe").c_str());
	quit(0,time_limit+200);
}
void MAIN(){
	auto Tstart=chrono::system_clock::now();
	int return_val=system((name+".exe "+args
		+(is_null(in)?"":" < "+in)+(is_null(out)?"":" > "+out)
	).c_str());
	if(done)return;
	if(return_val)quit(return_val);
	auto Tend=chrono::system_clock::now();
	quit(0,max(int(chrono::duration_cast<chrono::milliseconds>(Tend-Tstart).count())-22,0));
}

/*
parameters:
@0 "run.exe"
@1 program name
@2 input file
@3 output file
@4 time limit
[@5 arguments of the program]
*/

int main(int argc,char **argv){
	assert(argc>=5||argc<=7);
	name=argv[1];
	in=argv[2];
	out=argv[3];
	sscanf(argv[4],"%d",&time_limit);
	if(argc>=6)args=argv[5];
	thread th1(MAIN),th2(TL);
	th1.join(),th2.join();
	return 0;
}
