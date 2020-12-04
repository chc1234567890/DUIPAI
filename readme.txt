Usage:
duipai [-mo=MODE] [-dir=DIRECTORY] [-mk=GENERATOR] -usr=USER_PROGRAM [-std=STANDARD_PROGRAM] [-mx=MAX_TEST_CASES] [-tl=TIME_LIMIT] [-nd=NEED_COMPILE] [-o2=OPTIMIZATION] [-chk=SPECIAL_JUDGE] [-in=INPUT_FILE] [-out=OUTPUT_FILE] [-token=OUTPUT_FILE_TOKEN] [-pause=PAUSE] [-?]
	
	-mo		评测模式。0为对拍，1为数据测试。
	-dir		生成器、用户程序、标准程序所在的目录。默认为当前目录。
	-mk		生成器名称（无后缀名）。默认为空文件。
	-usr		用户程序名称（无后缀名）。默认为空文件。
	-std		标准程序名称（无后缀名）。默认为空文件。
	-mx		对拍数据组数。默认为100。
	-tl		用户程序、标准程序的时间限制。默认为1000。
	-nd		生成器、用户程序、标准程序、special judge是否需要编译。
			默认为false。
	-o2		编译优化选项。支持 -Ox(1≤x≤3)、-Ofast。
	-chk		special judge文件名。默认为空文件。
	-in		输入文件名。当mode=1时，支持"%d"格式，如"a%d.in"，
			当-mx=100时，将会把a1.in - a100.in作为输入文件逐个评测。
	-out		输出文件名。同上。
	-token		指定的输出文件标识字符串(TOKEN)。默认没有。
	-pause		当评测结果不为Accepted时，暂停评测。默认为false。
	-?, --help	显示帮助。

	参数读入方式：
	  1. 优先从命令行("duipai.exe -xxx=xxx")读入。
	  2. 如果命令行没有参数，则从默认存参数的文件("arglst.txt")读入。
	  3. 否则，从标准输入读入。