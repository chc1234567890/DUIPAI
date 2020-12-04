// define c_str() to decrease code length
#define _C c_str()

// definitions different from windows and linux
#ifdef WINDOWS
#define CUR ""
#define SLASH '\\'
#define EXTENSION_EXE ".exe"
#define DEL_EXE "del"
#else
#define CUR "./"
#define SLASH '/'
#define EXTENSION_EXE ""
#define DEL_EXE "rm"
#endif

#include"color.cpp"

// length of char arrays
const int MAXSTR=120,MAXMSG=1000,MAXBUF=100000000,MAXARG=30,STANDARD_TIME_LIMIT=10000;

// empty filename with "./"
const std::string nullfile=CUR"NULL";

// @buf temporary buffer
char buf[MAXBUF];

// pause
void halt(){
#ifdef WINDOWS
	system("pause");
#else
	std::cout<<"Press 'c\\n' to continue . . ."<<std::endl;
	for(char c=getchar();c!='c';c=getchar());
#endif
} 

// output a message if the given condition is false
__attribute__((format(printf,2,3)))
void ensure(bool cond,const char *form,...){
	if(!cond){
		color(Red);
		va_list ap;
		va_start(ap,form);
		int val=vsnprintf(buf,MAXMSG,form,ap);
		va_end(ap);
		if(val<0||val>=MAXMSG)std::cout<<"Error: vsnprintf buffer length error"<<std::endl;
		std::cout<<"Error: "<<buf<<std::endl;
		color();
		halt();
		exit(0);
	}
}

// format a string
__attribute__((format(printf,1,2)))
std::string format(const char *form,...){
	va_list ap;
	va_start(ap,form);
	int val=vsnprintf(buf,MAXSTR,form,ap);
	va_end(ap);
	ensure(val>=0&&val<MAXSTR,"vsnprintf buffer length error");
	return buf;
}

// file object
struct file{
	/*
	@nm file name WITH directory (e.g. "a","./a","C:\a","/usr/bin/a")
		note: if the file is executable, there is no extension following it;
		      otherwise it must be followed by an extension.
	@cont content of file
	*/
	std::string nm,cont;
	
	// constructor function (default: nullfile)
	file():nm(nullfile){}
	
	// append an extension (e.g. ".cpp")
	file append(const std::string &ext)const{
		file f;
		f.nm=nm+ext;
		return f;
	}
	
	// check whether the file exists
	bool exist()const{
		FILE *f=fopen(nm._C,"r");
		if(f==NULL)return 0;
		fclose(f);
		return 1;
	}
	
	void checkexist()const{
		ensure(exist(),"file %s doesn't exist",nm._C);
	}
	
	// read the content from the file (token supported)
	void read(const std::string &token=std::string()){
		checkexist();
		FILE* f=fopen(nm._C,"r");
		char c=fgetc(f);
		int cnt=0;
		bool flag=1;
		if(!token.empty()){
			for(;~c&&c!='\n'&&c!='\r';c=fgetc(f),cnt++)buf[cnt]=c;
			buf[cnt]=0;
			if(token!=buf){
				cont="(Unrecognized output)";
				flag=0;
			}
			cnt=0;
			for(;~c&&(c=='\n'||c=='\r');c=fgetc(f));
		}
		if(flag){
			for(;~c;c=fgetc(f),cnt++){
				buf[cnt]=c;
				if(buf[cnt]=='\n'){
					cnt--;
					while(cnt&&buf[cnt]==' ')cnt--;
					buf[++cnt]='\n';
				}
			}
			cnt--;
			while(cnt&&buf[cnt]==' ')cnt--;
			cnt++;
			if(buf[cnt-1]=='\n')cnt--;
			buf[cnt]=0;
			fclose(f);
			cont=buf;
		}
		if(!token.empty()){
			f=fopen(nm._C,"w");
			fprintf(f,"%s",cont._C);
			fclose(f);
		}
	}
	
	// delete the file
	void del(){
		if(!exist())return;
		system(format(buf,DEL_EXE" %s",nm._C)._C);
	}
};



// whether the file is nullfile
bool is_null(const file &prog){
	return prog.nm.substr(prog.nm.rfind(SLASH)+1)=="NULL";
}

void init_run(int time_lim,const std::string &dir,file &runexe){
	runexe.nm="run";
}
struct runRetVal{
	int status,Time;
	runRetVal():status(0),Time(-1){}
}; 
runRetVal get_run_return_val(){
	file f;
	runRetVal ret;
	std::stringstream tmp;
	f.nm="run_log.txt";
	f.checkexist();
	f.read();
	tmp<<f.cont;
	tmp>>ret.status;
	if(ret.status==0)tmp>>ret.Time;
	return ret;
}

// run a program with no input
runRetVal run_mk(file &prog,file &out,file &run){
	if(is_null(prog))return runRetVal();
	prog.append(EXTENSION_EXE).checkexist();
	int run_status=system(format("%s %s %s %s %d",
		run.nm._C,prog.nm._C,nullfile._C,out.nm._C,STANDARD_TIME_LIMIT
	)._C);
	ensure(run_status==0,"run.exe not returning 0 but %d",run_status);
	return get_run_return_val();
}

// run special judge checker
runRetVal run_checker(file &prog,file &inf,file &ouf,file &ans,file &lg,file &run){
	if(is_null(prog))return runRetVal();
	prog.append(EXTENSION_EXE).checkexist();
	int run_status=system(format("%s %s %s %s %d \"%s %s %s %s\" %s",
		run.nm._C,prog.nm._C,nullfile._C,nullfile._C,STANDARD_TIME_LIMIT,
		inf.nm._C,ouf.nm._C,ans.nm._C,lg.nm._C,
		lg.nm._C
	)._C);
	ensure(run_status==0,"run.exe not returning 0 but %d",run_status);
	return get_run_return_val();
}

// run a program
runRetVal run(file &prog,file &in,file &out,file &run,int time_lim){
	if(is_null(prog))return runRetVal();
	prog.append(EXTENSION_EXE).checkexist();
	int run_status=system(format("%s %s %s %s %d",run.nm._C,prog.nm._C,in.nm._C,out.nm._C,time_lim)._C);
	ensure(run_status==0,"run.exe not returning 0 but %d",run_status);
	return get_run_return_val();
}

bool compile(file &src,const std::string &O2,const char *options=""){
	if(is_null(src))return 1;
	src.append(".cpp").checkexist();
	std::cout<<"Compiling "<<src.nm<<".cpp ..."<<std::endl;
	std::string com=format("g++ %s.cpp -o %s -Wall -Wl,--stack=128000000 -std=c++11 %s %s -ID:\\DUIPAI\\include",src.nm._C,src.nm._C,O2._C,options);
	if(system(com._C)){
		output_verdict("Compile Error");
		std::cout<<src.nm._C<<std::endl;
		return 0;
	}
	return 1;
}

// output a string with format "-----XXX-----"
std::string line(const std::string &str){
	static const int width=24;
	int len=str.size(),mid=len/2;
	return std::string(width/2-mid,'-')+str+std::string(width/2-(len-mid),'-');
}

// return the verdict string
const char *verdict(int usr_time,int std_time,int time_lim,file &inftxt,file &ouftxt,file &anstxt,file &chkexe,file &lgtxt,file &run){
	if(std_time>time_lim)return "std Time Limit Exceeded";
	if(usr_time>time_lim)return "Time Limit Exceeded";
	bool ok=1;
	if(is_null(chkexe))ok=(ouftxt.cont==anstxt.cont);
	else{
		runRetVal result=run_checker(chkexe,inftxt,ouftxt,anstxt,lgtxt,run);
		if(result.status)ok=0;
		else if(result.Time>STANDARD_TIME_LIMIT)return "checker Time Limit Exceeded";
	}
	return ok?"Accepted":"Wrong Answer";
}

// output a prefix of the data
void output_data(const char *str){
	int i;
	for(i=0;i<MAXSTR&&str[i];i++)std::cout<<str[i];
	if(i>=MAXSTR)std::cout<<"...";
	std::cout<<std::endl;
}

// run a program and judge it
void judge(int test,int time_lim,file &runexe,file &prog,file &inftxt,file &ouftxt,file &anstxt,file &chkexe,
		file &lgtxt,const std::string &token,int pause,int std_time=-1){
	std::string ver;
	runRetVal usr_status=run(prog,inftxt,ouftxt,runexe,time_lim);
	if(usr_status.status){
		ver="Runtime Error";
		ouftxt.cont.clear();
	}
	else{
		ouftxt.read(token);
		if(!is_null(anstxt))anstxt.read();
		else anstxt.cont=ouftxt.cont;
		ver=verdict(usr_status.Time,std_time,time_lim,inftxt,ouftxt,anstxt,chkexe,lgtxt,runexe);
	}
	std::cout<<"usr: "<<usr_status.Time<<" ms ";
	output_verdict(ver);
	if(ver[0]!='s'&&ver[0]!='A'){
		inftxt.read();
		std::cout<<line("Input file:")<<std::endl,output_data(inftxt.cont._C);
		std::cout<<line("Output file:")<<std::endl,output_data(ouftxt.cont._C);
		std::cout<<line("Answer file:")<<std::endl,output_data(anstxt.cont._C);
	}
	if(!is_null(chkexe)&&lgtxt.exist()){
		lgtxt.read();
		std::cout<<line("Checker log:")<<std::endl,output_data(lgtxt.cont._C);
	}
	if(ver[0]!='s'&&ver[0]!='A'&&pause){
		std::cout<<line("")<<std::endl;
		MessageBox(NULL,(std::string(ver)+" on test "+itos(test))._C,"duipai",MB_OK|MB_ICONERROR);
		halt();
	}
}

void duipai(int test,int time_lim,file &runexe,file &usrexe,file &stdexe,file &inftxt,file &ouftxt,file &anstxt,file &chkexe,file &lgtxt,const std::string &token,int pause){
	runRetVal std_status=run(stdexe,inftxt,anstxt,runexe,time_lim);
	std::cout<<"std: "<<std_status.Time<<" ms ";
	if(std_status.status){
		output_verdict("std Runtime Error");
		return;
	}
	else{
		judge(test,time_lim,runexe,usrexe,inftxt,ouftxt,anstxt,chkexe,lgtxt,token,pause,std_status.Time);
	}
}

// arguments of duipai.exe

// check an integer argument
#define RETURN(...) {printf(__VA_ARGS__);return 0;}
bool chkint(int &x,int l,int r,const std::string &s){
	if(s.empty())RETURN("The string is empty.\n");
	if(!isdigit(s[0])&&s[0]!='-')RETURN("Not a integer.\n");
	for(int i=1;s[i];i++)if(!isdigit(s[i]))RETURN("Not a integer.\n");
	sscanf(s._C,"%d",&x);
	if(x<l||x>r)RETURN("Integer violates range [%d, %d].\n",l,r);
	return 1;
}
#undef RETURN

struct _ptr{int *p,l,r;};
/* argument object
@k argument's name
@mode the type of the argument (0: integer 1: string)
@must whether the argument is necessary (0: not necessary 1: necessary 2: help mode)
*/
struct arg{
	std::string k;
	int mode,must;
	std::string *pstr;
	std::string default_val_str;
	_ptr pint;
	int default_val_int;
	bool *pbool;
	arg():mode(-1){}
	// integer argument
	arg(const std::string &_k,int *_p,int _l,int _r,int _mu=0,int _d=0):k(_k),mode(0),must(_mu),pint((_ptr){_p,_l,_r}),default_val_int(_d){}
	// string argument
	arg(const std::string &_k,std::string *_p,int _mu=0,const std::string &_d=""):k(_k),mode(1),must(_mu),pstr(_p),default_val_str(_d){} 
	// boolean argument
	arg(const std::string &_k,bool *_p,int _mu=0):k(_k),mode(2),must(_mu),pbool(_p){}
	
	void init(){
		if(mode==0)*pint.p=default_val_int;
		if(mode==1)*pstr=default_val_str;
		if(mode==2)*pbool=false;
	}
};
// have[i]: check if argument i exists
bool have[MAXARG];

// check if the argument is valid and assign the value of the argument to the pointing argument
bool chkarg(arg args[],std::string val){
	std::string key;
	int p;
	if(val[0]!='-')return 0;
	p=val.find('=');
	if(p==-1){
		key=val.substr(1);
		val=std::string();
	}
	else{
		key=val.substr(1,p-1);
		val=val.substr(p+1);
	}
	int i;
	for(i=0;args[i].mode!=-1&&args[i].k!=key;i++);
	arg &cur=args[i];
	if(cur.mode==-1)return 0;
	have[i]=1;
	if(cur.mode==0){if(!chkint(*cur.pint.p,cur.pint.l,cur.pint.r,val))return 0;}
	if(cur.mode==1)*cur.pstr=val;
	if(cur.mode==2)*cur.pbool=true;
	return 1;
}

void writeargshelp(){
	/*std::cout<<"argument lists:"<<std::endl;
	for(int i=0;args[i].mode!=-1;i++){
		std::cout<<(args[i].must?"  ":"[ ")<<args[i].k._C<<"\t(";
		if(args[i].mode)std::cout<<"string";
		else std::cout<<"integer between ["<<args[i].pint.l<<", "<<args[i].pint.r<<"]";
		std::cout<<")"<<(args[i].must?"":" ]")<<std::endl;
	}*/
	file readme;
	readme.nm="readme.txt";
	readme.read();
	std::cout<<readme.cont<<std::endl;
}

std::string getargs(int argc,char **argv){
	std::string ret;
	for(int i=1;i<argc;i++)ret=ret+argv[i]+' ';
	return ret;
}
std::string readargs(){
	file arglst;
	arglst.nm="arglst.txt";
	if(arglst.exist()){
		arglst.read();
		return arglst.cont;
	}
	else{
		std::cout<<"Input arguments:"<<std::endl;
		std::string ret;
		std::getline(std::cin,ret);
		return ret;
	}
}
void dealargs(const std::string &args_str,arg args[]){
	memset(have,0,sizeof(have));
	std::stringstream tmp;
	std::string str;
	for(int i=0;args[i].mode!=-1;i++)args[i].init();
	tmp<<args_str;
	for(int i=1;tmp>>str;i++){
		if(!chkarg(args,str)){
			std::cout<<"Invalid format on argument "<<i<<"."<<std::endl;
			exit(1);
		}
	}
	bool help_mode=0;
	for(int i=0;args[i].mode!=-1;i++){
		if(args[i].must==2&&have[i]){
			help_mode=1;
			break;
		}
	}
	if(!help_mode){
		for(int i=0;args[i].mode!=-1;i++){
			if(args[i].must==1&&!have[i]){
				std::cout<<"Too few arguments: argument "<<i<<" is empty."<<std::endl;
				exit(1);
			}
		}
	}
	else{
		writeargshelp();
		exit(0);
	}
}
