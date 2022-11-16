###########环境依赖###############  
CentOS 7.2.1511  
gcc 8.3.1   

###########目录结构描述###########  
.  
├── ReadMe.md                  //说明文档  
├── abc-master (2).zip         //ABC源代码  
├── kissat                     //SAT求解器的可执行文件  
├── kissat_sat+cf.zip          //SAT求解器的源代码  
├── lec_kissat+cf              //逻辑等价性验证的可执行文件  
├── lec_kissat+cf.c            //逻辑等价性验证的源代码  
└── libabc.a                   //ABC静态库  
  
###########算法基本说明###########  
lec_kissat+cf是编译好的code二进制文件，包含了调用ABC进行AIGER文件读取、化简、转化，以及调用求解器kissat进行可满足性验证的过程。  
kissat是编译好的code二进制文件，作用是使用了改进的增加了冲突频率的Kissat作为求解器求解可满足性问题。  
只需这两个可执行文件就可以进行完成逻辑等价性的过程，命令是：  
`./lec_kissat+cf <AIGER>`  
输入：其中，<AIGER>是需要求解的测试用例的路径，测试用例需是以.aiger或.aig结尾的AIGER文件。  
输出：结果在终端result中展示，UNSATISFIABLE无解或SATISFIABLE有解包括DIMACS格式的反例输出解。  
  
###########过程文件说明###########  
正确运行的话，在运行过程中会有两个文件生成，分别是：  
├── work.cnf                   //正在处理的AIGER文件转化成CNF格式  
└── work_out                   //SAT求解器求解结果的proof二进制文件  
  
########第三方库及编译说明########  
以下说明是涉及到的源代码如何编译，已经在服务器上编译过，正常来说不需要重复。  
本作品使用了官方提供的ABC，采用把ABC编译成静态库再脚本包装至逻辑等价性验证的可执行文件中，其中静态库的编译操作如下：  
`unzip abc-master.zip`    
`cd abc-master`  
`make libabc.a`  
`cp libabc.a ..`  
`cd ..`  
本作品需要使用改进的增加cf（冲突频率）的kissat求解器，其编译过程如下：  
`unzip kissat_sat+cf.zip`   
`cd kissat_sat+cf/`  
`chmod +x configure`  
`./configure`  
`chmod +x scripts/generate-build-header.sh`  
`cp build/kissat ..`  
`cd ..`  
逻辑等价性验证的源代码编译（需要用到libabc.a):  
`gcc -Wall -g -c lec_kissat+cf.c -o lec.o`  
`g++ -g -o lec_kissat+cf lec.o libabc.a -lm -ldl -lreadline -lpthread`  
`chmod +x lec_kissat+cf`  




