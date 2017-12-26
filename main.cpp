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
    BLOCK,      //阻塞态
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

//表头
void table_head()
{
    printf("%6s%5s%8s%10s%10s%10s\n", "时间", "PID", "优先级", "进入时间", "运行时间", "需要时间");
}

//内容
void display(int time, PCB *process)
{
    if(process != NULL)
    {
        printf("%5d%5d%7d%8d%10d%10d\n", time, process->PID, process->priority, process->process_reach_time, process->cpu_time, process->need_time);
    }
    else
    {
        printf("%5d%24s\n", time, "（空闲）");
    }
}

//用于先到先服务排序
int FSFC_cmp(PCB *a, PCB *b)
{
    return a->process_reach_time < b->process_reach_time;
}

//先来先服务
void FCFS()
{
    //显示表头
    table_head();

    //按照进程进入时间进行排序
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //初始化时间
    int time = 0;

    //完成的进程数
    int process_finsih_count = 0;
    //一个进程开始的时间
    //int start_time = 0;
    for(; process_finsih_count < n; time++)
    {
        PCB *tmp = PCB_array[process_finsih_count];

        //当进程到达的时间小于当前的时间并且进程的状态为‘READY’时
        if(tmp->process_reach_time <= time && tmp->state == READY)
        {
            //记录进程开始时间（基本都不是进程到达的时间）
            //start_time = time;
            //修改进程的状态为运行
            tmp->state = RUNNING;

            display(time, tmp);
        }
        else if(tmp->cpu_time < tmp->need_time && tmp->state == RUNNING)
        {
            //进程的CPU时间加1
            tmp->cpu_time ++;

            display(time, tmp);
        }
        else if(tmp->cpu_time == tmp->need_time && tmp->state == RUNNING)
        {
            tmp->state = FINISH;
            //结束时没有消耗时间
            time --;
            //完成进程数加+1
            process_finsih_count ++;
        }
        else
        {
            display(time, NULL);
        }
    }
}

int SPN_tmp_non_preemptive(PCB *a, PCB *b)
{
    return a->need_time < b->need_time;
}

//短进程优先（非抢占）
void SPN_non_preemptive()
{
    //显示表头
    table_head();

    //按照进程进入时间进行排序
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //初始化时间
    int time = 0;
    //已进入的进程个数
    int reach_process_num = 0;
    //完成的进程数
    int process_finsih_count = 0;
    //有进程开始执行标志
    int startFlag = 0;

    for(; process_finsih_count < n; time++)
    {
        PCB *tmp = PCB_array[process_finsih_count];

        //当进程到达的时间小于当前的时间并且进程的状态为‘READY’时
        if(tmp->process_reach_time <= time && tmp->state == READY)
        {
            //修改进程的状态为运行
            tmp->state = RUNNING;

            if(!startFlag)
            {
                startFlag = 1;
            }

            display(time, tmp);
        }
        else if(tmp->cpu_time < tmp->need_time && tmp->state == RUNNING)
        {
            //进程的CPU时间加1
            tmp->cpu_time ++;

            display(time, tmp);
        }
        else if(tmp->cpu_time == tmp->need_time && tmp->state == RUNNING)
        {
            tmp->state = FINISH;
            //结束时没有消耗时间
            time --;
            //完成进程数加+1
            process_finsih_count ++;
        }
        else
        {
            display(time, NULL);
        }

        //每个时间片检查是否有新的进程进入
        for(int i = reach_process_num; i < n; i++)
        {
            PCB *temp = PCB_array[i];

            if(temp->process_reach_time == time)
            {
                reach_process_num ++;
            }
        }

        if(process_finsih_count != n && startFlag == 1)
        {
            //每个时间片按照需要时间的长短对已经进入到CPU但是未完成的进程进程排序
            /**
                如果在当前时间片进程未完成，则process_finsih_count指向当前进程
                否则process_finsih_count - 1指向刚已完成进程
                process_finsih_count指向新的进程
            **/
            if(PCB_array[process_finsih_count]->state == READY)
            {
                sort(PCB_array + process_finsih_count, PCB_array + reach_process_num, SPN_tmp_non_preemptive);
            }
        }
    }
}

int SPN_tmp_preemptive(PCB *a, PCB *b)
{
    return a->need_time < b->need_time;
}

//短进程优先（抢占）
void SPN_preemptive()
{
    //显示表头
    table_head();

    //按照进程进入时间进行排序
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //初始化时间
    int time = 0;
    //已进入的进程个数
    int reach_process_num = 0;
    //完成的进程数
    int process_finsih_count = 0;
    //有进程开始执行标志
    int startFlag = 0;

    for(; process_finsih_count < n; time++)
    {
        //每个时间片检查是否有新的进程进入
        for(int i = reach_process_num; i < n; i++)
        {
            PCB *temp = PCB_array[i];

            if(temp->process_reach_time == time)
            {
                reach_process_num ++;
            }
        }

        if(process_finsih_count != n && startFlag == 1)
        {
            //每个时间片按照需要时间的长短对已经进入到CPU但是未完成的进程进程排序
            sort(PCB_array + process_finsih_count, PCB_array + reach_process_num, SPN_tmp_preemptive);
        }

        PCB *tmp = PCB_array[process_finsih_count];

        //当进程到达的时间小于当前的时间并且进程的状态为‘READY’时
        if(tmp->process_reach_time <= time && (tmp->state == READY || tmp->state == BLOCK))
        {
            //修改进程的状态为运行
            tmp->state = RUNNING;

            if(!startFlag)
            {
                startFlag = 1;
            }

            display(time, tmp);
        }
        else if(tmp->cpu_time < tmp->need_time && tmp->state == RUNNING)
        {
            //进程的CPU时间加1
            tmp->cpu_time ++;

            display(time, tmp);
        }
        else if(tmp->cpu_time == tmp->need_time && tmp->state == RUNNING)
        {
            tmp->state = FINISH;
            //结束时没有消耗时间
            time --;
            //完成进程数加+1
            process_finsih_count ++;
        }
        else
        {
            display(time, NULL);
        }
    }
}

//时间片轮转
void RR()
{
    //显示表头
    table_head();

    //初始化时间
    int time = 0;
    //时间片
    int time_slice = 5;

    //记录完成的进程数
    int process_count = 0;

}

//菜单
void menu()
{
    printf("选择调度算法：\n");
    printf("\t1.先来先服务\n");
    printf("\t2.短进程优先（非抢占）\n");
    printf("\t3.短进程优先（抢占）\n");
    printf("\t4.时间片轮换\n");

    int cas;
    scanf("%d", &cas);
    switch(cas)
    {
        case 1:
            FCFS();
            break;

        case 2:
            SPN_non_preemptive();
            break;

        case 3:
            SPN_preemptive();
            break;

        case 4:
            RR();
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

















