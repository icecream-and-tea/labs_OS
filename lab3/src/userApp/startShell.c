#include "myPrintk.h"
#include "uart.h"
#include "tick.h"
#include "wallClock.h"
#include "mystring.h"
//要修改

#define SIZE 10
//一、结构体定义
typedef struct myCommand
{
    char name[80];
    char help_content[200];
    int (*func)(int argc, char (*argv)[8]); //函数指针
} myCommand;

//二、全局变量与函数声明
int cmd_num  = 2;
           /*cmd函数*/
int func_cmd(int argc, char (*argv)[8]);
          /*help函数*/
int func_help(int argc, char (*argv)[8]);

//维护一个列表
myCommand cmd_list[SIZE] = {//初始化
    {"cmd\0", "List all command\n\0", func_cmd},
    {"help\0", "Usage: help [command]\nDisplay info about [command]\n", func_help}
};

int findCommand(int argc, char *argv)
{
    //寻找对应实例，利用字符串比较函数进行
    for(int i = 0; i < cmd_num; i++)
    {
        if(strcmp(cmd_list[i].name, argv) == 0)
        {
            return i;
        }      
    }
    return -1;
}  

int func_cmd(int argc, char (*argv)[8])
{//列出所有支持的命令
    for(int i = 0; i < cmd_num; i++)
    {
        myPrintk(0x07,cmd_list[i].name);
        myPrintk(0x07,"\n");
    }
	return 0;
}

int func_help(int argc, char (*argv)[8])
{
    int i = findCommand(argc, argv[1]);
    if(i != -1)
        myPrintk(0x07,cmd_list[i].help_content);
    else//若找不到指令，调用help的help_content
        myPrintk(0x07,cmd_list[1].help_content);
}

//主函数实现
void startShell(void)
{
    //我们通过串口来实现数据的输入
    int i = 0, j = 0, k = 0;

    char BUF[256];   //输入缓存区
    int BUF_len = 0; //输入缓存区的长度

    int argc;
    char argv[8][8];

    do
    {
        BUF_len = 0;
        myPrintk(0x07, "Student>>");
        //从终端获得命令
        while ((BUF[BUF_len] = uart_get_char()) != '\r')
        {
            myPrintf(0x07,"%c",BUF[BUF_len]); //将串口输入的数存入BUF数组中
            BUF_len++;                   // BUF数组的长度加1
        }
        myPrintf(0x07, " -pseudo_terminal\n");

        //从BUF数组中提取相应的argc和argv参数
        i = 0; j = 0;
        while(i < BUF_len)
        {
            k = 0;
            j ++;
            while (i < BUF_len && BUF[i] != ' ')
            {
                argv[j-1][k++] = BUF[i++];
            }
            argv[j-1][k] = '\0';
            while (i < BUF_len && BUF[++i] == ' ');//处理多空格
        }
        argc = j;
        //寻找相应的myCommand实例
        i = findCommand(argc, argv[0]);
        //调用相关函数
        if(i != -1)
            cmd_list[i].func(argc, argv);
        else
            myPrintf(0x07,"command error!\n");

    } while (1);
}
