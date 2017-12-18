#include <iostream>
#include <stdio.h>
#include <string.h>
#include <queue>
#include <math.h>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <stack>
#include <queue>
#include <stdlib.h>
#define INF 0x3f3f3f3f
#define maxn 1000005
#define ms(a,b) memset(a,b,sizeof(a))
using namespace std;

//定义进程的状态枚举
typedef enum
{
    READY,      //就绪态
    RUNNING,    //运行态
    BOLCK,      //阻塞态
    FINISH      //完成态
} process_state;

//定义进程控制块
typedef struct PCB
{
    int PID;                //进程ID
    int priority;           //进程优先级
    int process_reach_time; //进程进入时间
    int need_time;          //进程需要运行的时间
    int cpu_time;           //进程已经占用CPU的时间
    process_state state;    //进程的状态
    PCB *next;              //下一个进程的指针
} PCB;

int n;                          //进程的个数
PCB *PCB_array[150];            //存指向进程的指针，用于不同的进程调度算法

//用于排序
int cmp(PCB *a, PCB *b)
{
    return a->process_reach_time < b->process_reach_time;
}

//表头
void tableHead()
{
    printf("%6s%5s%8s%10s%10s%10s\n", "时间", "PID", "优先级", "进入时间", "运行时间", "需要时间");
}

//内容
void display(int time, PCB *process)
{
    printf("%6d%5d%8d%10d%10d%10d\n", time, process->PID, process->priority, process->process_reach_time, process->cpu_time, process->need_time);
}

//先来先服务
void FCFS()
{
    //显示表头
    tableHead();

    //初始化时间
    int time = 0;
    for(int i = 0; i < n; i++)
    {
        PCB *tmp = PCB_array[i];
        while(tmp->need_time >= tmp->cpu_time)
        {
            display(time, tmp);

            tmp->cpu_time ++;
            time ++;
        }
    }
}

//菜单
void menu()
{
    printf("选择调度算法：\n");
    printf("\t1.FCFS\n");

    int cas;
    scanf("%d", &cas);
    switch(cas)
    {
        case 1:
            FCFS();
            break;
        case 2:

            break;
        default:
            printf("输入错误！\n");
            system("cls");
    }

    //menu();
}

int main()
{
    #ifndef ONLINE_JUDGE
        freopen("in.txt", "r", stdin);
        //freopen("out.txt", "w", stdout);
    #endif

    printf("输入进程数：");
    scanf("%d", &n);
    for(int i = 0;i < n; i++)
    {
        PCB *tmp = (PCB*)malloc(sizeof(PCB));

        printf("输入进程PID：");
        scanf("%d", &(tmp->PID));

        printf("输入进程优先级：");
        scanf("%d", &(tmp->priority));

        printf("输入进程进入时间：");
        scanf("%d", &(tmp->process_reach_time));

        printf("输入进程需要运行的时间：");
        scanf("%d", &(tmp->need_time));

        tmp->cpu_time = 0;
        tmp->state = READY;
        tmp->next = NULL;

        PCB_array[i] = tmp;

        //printf("%d, %d\n", tmp->PID, tmp->priority);
    }

    //按照进程进入时间进行排序
    sort(PCB_array, PCB_array + n, cmp);

    for(int i = 0; i < n; i++)
    {
        printf("%d, %d\n", PCB_array[i]->PID, PCB_array[i]->process_reach_time);
    }

    menu();

//    for(int i = 0; i < n; i++)
//    {
//        printf("%d\n", PCB_array[i]->PID);
//    }

    return 0;
}

















