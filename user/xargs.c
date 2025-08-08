#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXSIZE 512 // 每行最大字符数

// 字符状态
enum char_type{
    C_SPACE,
    C_CHAR,
    C_NEWLINE
};

// 读取命令行状态
enum state{
    S_WAIT,
    S_ARG,          //参数内
    S_ARG_END,      //参数读取结束
    S_ARG_LINE_END, //换行左侧为参数，agu\n
    S_LINE_END,     //换行左侧为空格，agr \n
    S_END           //命令行结束
};

/*
**function:根据字符类型返回字符类型
**param: c 字符
**return: 字符类型
*/
enum char_type get_char_type(char c)
{
    if (c == ' ')
        return C_SPACE;
    else if (c == '\n')
        return C_NEWLINE;
    else
        return C_CHAR;
}


/*
**function:根据读取的下一个字符和当前状态转换到下一个状态
**param: st 当前状态
**param: ct 下一个字符类型
**return: 下一个状态
*/
enum state transform_state(enum state st, enum char_type ct)
{
    switch (st) {
        case S_WAIT:
            if (ct == C_CHAR)
                return S_ARG;
            else if (ct == C_NEWLINE)
                return S_LINE_END;
            else
                return S_WAIT;

        case S_ARG:
            if (ct == C_CHAR)
                return S_ARG;
            else if (ct == C_SPACE)
                return S_ARG_END;
            else if (ct == C_NEWLINE)
                return S_ARG_LINE_END;

        case S_ARG_END:
        case S_ARG_LINE_END:
        case S_LINE_END:
            if (ct == C_CHAR)
                return S_ARG;
            else if (ct == C_SPACE)
                return S_WAIT;
            else if (ct == C_NEWLINE)
                return S_LINE_END;


        default:
            return st; // 保持当前状态
    }
}


/**
 * function 将参数列表后面的元素全部置为空
 *        用于换行时，重新赋予参数
 *
 * param x_argv 参数指针数组
 * param beg 要清空的起始下标
 */
void clearArgv(char *x_argv[MAXARG], int beg)
{
  for (int i = beg; i < MAXARG; ++i)
    x_argv[i] = 0;
}


/*
**function:从标准输出读取，每一行执行args后命令
**param: argc 参数个数
**param: argv 参数列表
**return: 无
*/
void main(int argc, char *argv[])
{
    if(argc - 1 > MAXARG) {
        fprintf(2, "xargs: too many arguments\n");
        exit(1);
    }

    char *ex_argv[MAXARG] = {0};     // 执行程序的参数列表
    char lines[MAXSIZE];             // 存储读取的额外参数行
    char *line_ptr = lines;          // 指向当前读取的输出区额外参数
    enum state cur_st = S_WAIT;      // 当前状态
    int arg_bgn = 0;                 // 参数起始位置
    int arg_end = 0;                 // 参数结束位置
    int arg_cnt = argc - 1;          // 参数个数


    for (int i = 0; i < argc - 1; i++) {
        ex_argv[i] = argv[i + 1];   // 将命令行参数复制到执行参数列表
    }


    while(cur_st != S_END) {
        if(read(0,line_ptr,sizeof(char)) != sizeof(char)){
            cur_st = S_END; // 读取失败，结束状态
            continue;
        }
        else{
            cur_st = transform_state(cur_st, get_char_type(*line_ptr));
        }

        if (++arg_end >= MAXSIZE) {
        fprintf(2, "xargs: arguments too long.\n");
        exit(1);
        }

        if(cur_st == S_WAIT){
            arg_bgn++;
        }
        else if(cur_st == S_ARG_END){
            ex_argv[arg_cnt++] = &lines[arg_bgn]; // 将参数添加到执行参数列表
            arg_bgn = arg_end;
            *line_ptr = '\0'; // 在参数末尾添加终止符

        }
        else if(cur_st == S_ARG_LINE_END || cur_st == S_LINE_END){
            if(cur_st == S_ARG_LINE_END){
                ex_argv[arg_cnt++] = &lines[arg_bgn]; // 将参数添加到执行参数列表
            }
            arg_bgn = arg_end;
            *line_ptr = '\0'; // 在参数末尾添加终止符
            if(fork() == 0){
                exec(argv[1], ex_argv); // 执行命令
            }
            arg_cnt = argc - 1;
            clearArgv(ex_argv, arg_cnt);
            wait(0);

                
        }

        line_ptr++; // 移动到下一个字符位置
    }


    exit(0);
}