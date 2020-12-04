// color constants
static const int Gray = 0x07;
static const int Red = 0x0C;
static const int Blue = 0x09;
static const int Green = 0x0A;
static const int Yellow = 0x0E;
static const int Purple = 0x0D;
static const int Cyan = 0x0B;
static const int Bold = 0x0100;

// transform an integer to a string
std::string itos(int x){
	static char buf[23];
	int cnt=0;
	if(x){
		while(x)buf[cnt++]=x%10+'0',x/=10;
		std::reverse(buf,buf+cnt);
	}
	else buf[cnt++]='0';
	buf[cnt]=0;
	return buf;
}

// change font color
void color(int color=Gray){
	const char *shell_path=getenv("SHELL");
	const std::string bin_bash=SLASH+std::string("bin")+SLASH+"bash";
	if(shell_path!=NULL&&strstr(shell_path,bin_bash._C)!=NULL){
		int form=0,col=0;
		switch(color>>8<<8){
			case Bold:
				form=1;
				break;
		}
		switch(color&((1<<8)-1)){
			case Gray:
				col=39;
				break;
			case Red:
				col=31;
				break;
			case Green:
				col=32;
				break;
			case Yellow:
				col=33;
				break;
			case Blue:
				col=34;
				break;
			case Purple:
				col=35;
				break;
			case Cyan:
				col=36;
				break;
		}
		std::cout<<std::string("\e[")+itos(form)+';'+itos(col)+'m';
	}
	else{
#ifdef WINDOWS
		HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle,color&((1<<8)-1));
#endif
	}
}

// output judgement verdict to console
void output_verdict(const std::string &msg){
	int pos=0;
	if(islower(msg[0]))pos=msg.find(' ')+1;
	switch(msg[pos]){
		case 'C':
			color(Yellow|Bold);
			break;
		case 'A':
			color(Green|Bold);
			ACcnt++;
			break;
		case 'W':
			color(Red|Bold);
			break;
		case 'T':
			color(Blue|Bold);
			break;
		case 'R':
			color(Purple|Bold);
			break;
		default:
			color(Cyan|Bold);
	}
	std::cout<<msg<<std::endl;
	color();
}
