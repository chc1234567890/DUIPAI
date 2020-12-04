#if defined(__WIN32)||defined(__WIN64)
#define WINDOWS
#endif

#include<bits/stdc++.h>
#ifdef WINDOWS
#include<windows.h>
#endif

int ACcnt;

#include"judger.h"

// default settings
int maxtest=100,time_lim=1000,mo=0;
bool need_compile=false,pause=false;
std::string in,out,token,dir,O2;
file mkexe,usrexe,stdexe,inftxt,ouftxt,anstxt,chkexe,runexe,lgtxt;
void *null=malloc(64);
arg args[MAXARG]={
	arg("dir",&dir),
	arg("mk",&mkexe.nm,false,nullfile),
	arg("usr",&usrexe.nm,true,nullfile),
	arg("std",&stdexe.nm,false,nullfile),
	arg("mx",&maxtest,1,10000),
	arg("tl",&time_lim,1,60000),
	arg("o2",&O2,false,""),
	arg("nd",&need_compile),
	arg("chk",&chkexe.nm,false,nullfile),
	arg("in",&in,false,nullfile),
	arg("out",&out,false,nullfile),
	arg("token",&token),
	arg("mo",&mo,0,1),
	arg("pause",&pause),
	arg("?",(bool*)null,2),
	arg("-help",(bool*)null,2)
};

// append the directory prefix to the front of file name (e.g. "a" -> "C:\a" or "a" -> "/usr/bin/a")
void makedir(file &f){
	if(dir.empty())dir=".";
	if(dir.back()!=SLASH)dir+=SLASH;
	if(!(f.nm[1]==':'||f.nm[0]==SLASH)){
		f.nm=dir+f.nm;
	}
}

void init(int argc,char **argv){
	std::string args_str;
	if(argc>1)args_str=getargs(argc,argv);
	else args_str=readargs();
	dealargs(args_str,args);
	init_run(time_lim,dir,runexe);
	makedir(mkexe);
	makedir(usrexe);
	makedir(stdexe);
	makedir(chkexe);
	lgtxt.nm="log.txt";
	ouftxt.nm="tmp_2.txt";
	if(mo==0){
		inftxt.nm="tmp_1.txt";
		if(!is_null(stdexe)){
			anstxt.nm="tmp_3.txt";
		}
	}
}

int main(int argc,char **argv){
	init(argc,argv);
	
	// compile
	if(need_compile){
		if(!compile(mkexe,"-O2")||
		!compile(usrexe,O2,"")||
		!compile(stdexe,O2,"")||
		!compile(chkexe,"-O2","")){
			//MessageBox(NULL,"Compile error","duipai",MB_OK|MB_ICONWARNING);
			halt();
			exit(0);
		}
	}
	std::cout<<"Compile Succeeded"<<std::endl;
	
	// judge
	for(int test=1;test<=maxtest;test++){
		std::cout<<"# "<<test<<": ";
		if(mo==0){
			runRetVal result=run_mk(mkexe,inftxt,runexe);
			if(result.status!=0){
				output_verdict("maker Runtime Error");
				continue;
			}
			else if(result.Time>STANDARD_TIME_LIMIT){
				output_verdict("maker Time Limit Exceeded");
				continue;
			}
			duipai(test,time_lim,runexe,usrexe,stdexe,inftxt,ouftxt,anstxt,chkexe,lgtxt,token,pause);
		}
		else{
			inftxt.nm=format(in._C,test);
			anstxt.nm=format(out._C,test);
			judge(test,time_lim,runexe,usrexe,inftxt,ouftxt,anstxt,chkexe,lgtxt,token,pause);
		}
	}
	
	// delete the log file
	lgtxt.del();
	std::cout<<"finished"<<std::endl;
	//MessageBox(NULL,"finished","duipai",MB_OK);
	halt();
	
	// the program returns the score the usr's program gets
	return round(double(ACcnt)*100/maxtest);
}
