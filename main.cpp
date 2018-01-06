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
#include <time.h>
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
    double R;               //用于高响应比算法
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

//显示输入线程的基本信息
void process_infor()
{
    printf("\n\n\n");
    printf("%5s%8s%10s%10s%10s\n", "PID", "优先级", "进入时间", "运行时间", "需要时间");
    //printf("%5s%8s%10s%10s\n", "PID", "优先级", "进入时间", "需要时间");

    for(int i = 0; i < n; i++)
    {
        PCB *process = PCB_array[i];
        printf("%5d%7d%8d%10d%10d\n", process->PID, process->priority, process->process_reach_time, process->cpu_time, process->need_time);
        //printf("%5d%7d%8d%10d\n", process->PID, process->priority, process->process_reach_time, process->need_time);
    }
}

//用于先到先服务算法排序
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
    int time = 1;

    //完成的进程数
    int process_finsih_count = 0;

    for(; process_finsih_count < n; time++)
    {
        PCB *tmp = PCB_array[process_finsih_count];

        //当进程到达的时间小于当前的时间并且进程的状态为‘READY’时
        if(tmp->process_reach_time <= time && tmp->state == READY)
        {
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

//用于短进程优先（非抢占）算法排序
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
    int time = 1;
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

            //进程开始执行标志
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

//用于短进程优先（抢占）算法排序
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
    int time = 1;
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

            //进程开始执行标志
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

    //按照进程进入时间进行排序
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //初始化时间
    int time = 1;
    //时间片
    int time_slice = 5;
    //时间片计数
    int time_slice_count = 0;

    //已进入的进程个数
    int reach_process_num = 0;
    //完成的进程数
    int process_finsih_count = 0;
    int first = 0;

    queue<PCB*> pcb_queue;

    for(; process_finsih_count < n;)
    {
        //当队列为空，但进程还未全部结束时，检查是否有新的进程进入
        if(pcb_queue.empty())
        {
            for(int i = reach_process_num; i < n; i++)
            {
                PCB *temp = PCB_array[i];

                if(temp->process_reach_time <= time)
                {
                    reach_process_num ++;
                    //将进程加入队尾
                    pcb_queue.push(temp);
                }
                else if(temp->process_reach_time > time)
                {
                    break;
                }
            }
        }

        //当队列不为空时
        if(!pcb_queue.empty())
        {
            PCB *now = pcb_queue.front();
            pcb_queue.pop();

            if(now->state == READY)
            {
            	if(first)
            	{
                	time++;
                }
                display(time, now);
                first = 1;
            }

            now->state = RUNNING;

            time_slice_count = 0;

            //在时间片内执行进程
            while(time_slice_count < time_slice && now->cpu_time < now->need_time)
            {
                time ++;
                time_slice_count ++;
                now->cpu_time ++;

                display(time, now);
            }

            //在当前进程加入队尾之前，检查是否有新的进程进入
            for(int i = reach_process_num; i < n; i++)
            {
                PCB *temp = PCB_array[i];

                if(temp->process_reach_time <= time)
                {
                    reach_process_num ++;
                    pcb_queue.push(temp);
                }
                else if(temp->process_reach_time > time)
                {
                    break;
                }
            }

            //进程已完成
            if(now->cpu_time == now->need_time)
            {
                now->state = FINISH;
                process_finsih_count ++;
            }
            //进程还未完成
            else
            {
                pcb_queue.push(now);
            }

            time_slice_count = 0;
        }
        else
        {
            display(time, NULL);
            first = 0;
            time++;
        }
    }
}

//用于动态优先级算法排序
struct priority_cmp
{
    bool operator()(PCB a, PCB b)
    {
        return a.priority < b.priority;
    }
};

//动态优先级
void PRIORITY()
{
    //显示表头
    table_head();

    //按照进程进入时间进行排序
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //创建以priority_cmp排序的优先队列
    //为了保存原数据，这里不使用指针
    priority_queue<PCB, vector<PCB>, priority_cmp> pq;

    //初始化时间
    int time = 1;
    //已进入的进程个数
    int reach_process_num = 0;
    //完成的进程数
    int process_finsih_count = 0;

    for(; process_finsih_count < n; time++)
    {
        //每个时间片检查是否有新的进程进入
        for(int i = reach_process_num; i < n; i++)
        {
            PCB *temp = PCB_array[i];

            if(temp->process_reach_time == time)
            {
                reach_process_num ++;
                pq.push(*temp);
            }
            else if(temp->process_reach_time > time)
            {
                break;
            }
        }

        //优先队列不为空时
        if(!pq.empty())
        {
            PCB temp = pq.top();
            PCB *tmp = &temp;
            pq.pop();

            int flag = 0;

            if(tmp->state == READY)
            {
                tmp->state = RUNNING;

                flag = 1;

                display(time, tmp);
            }
            else if(tmp->cpu_time < tmp->need_time && tmp->state == RUNNING)
            {
                tmp->cpu_time ++;
                tmp->priority = (tmp->priority -= 2) >= 0 ? tmp->priority : 0;
                flag = 2;

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

            //如果当前进程运行，则其他的PCB的优先级自增
            if(flag == 2)
            {
                queue<PCB> q;
                while(!pq.empty())
                {
                    PCB tmp1 = pq.top();
                    tmp1.priority = tmp1.priority > 32 ? 32 : tmp1.priority;
                    q.push(tmp1);
                    pq.pop();
                }

                while(!q.empty())
                {
                    pq.push(q.front());
                    q.pop();
                }
            }

            //当前进程未完成
            if(flag)
            {
                //重新放入优先队列
                pq.push(*tmp);
            }
        }
        else
        {
            display(time, NULL);
        }
    }
}

//用于动态优先级算法排序
struct HHRN_cmp
{
    bool operator()(PCB *a, PCB *b)
    {
        return a->R < b->R;
    }
};

//高响应比优先
void HHRN()
{
    //显示表头
    table_head();

    //按照进程进入时间进行排序
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //创建以HHRN_cmp排序的优先队列
    priority_queue<PCB*, vector<PCB*>, HHRN_cmp> pq;

    //初始化时间
    int time = 1;
    //已进入的进程个数
    int reach_process_num = 0;
    //完成的进程数
    int process_finsih_count = 0;

    for(; process_finsih_count < n; time++)
    {
        //每个时间片检查是否有新的进程进入
        for(int i = reach_process_num; i < n; i++)
        {
            PCB *temp = PCB_array[i];

            if(temp->process_reach_time == time)
            {
                reach_process_num ++;
                pq.push(temp);
            }
            else if(temp->process_reach_time > time)
            {
                break;
            }
        }

        //优先队列不为空时
        if(!pq.empty())
        {
            //计算每个在就绪队列的进程的响应比
            for(int i = process_finsih_count; i < reach_process_num; i++)
            {
                PCB *temp = PCB_array[i];
                temp->R = (double)time / temp->need_time;
            }

            PCB *tmp = pq.top();
            pq.pop();

            int flag = 0;

            if(tmp->state == READY)
            {
                tmp->state = RUNNING;

                display(time, tmp);
            }
            else if(tmp->cpu_time < tmp->need_time && tmp->state == RUNNING)
            {
                tmp->cpu_time ++;

                display(time, tmp);
            }
            else if(tmp->cpu_time == tmp->need_time && tmp->state == RUNNING)
            {
                flag = 1;
                tmp->state = FINISH;
                //结束时没有消耗时间
                time --;

                //当前执行的与process_finsih_count位置的PCB_array的进程交换位置，便于操作
                for(int i = 0; i < n; i++)
                {
                    if(PCB_array[i] == tmp)
                    {
                        PCB_array[i] = PCB_array[process_finsih_count];
                        PCB_array[process_finsih_count] = tmp;
                        break;
                    }
                }

                //完成进程数加+1
                process_finsih_count ++;
            }

            //当前进程未完成
            if(!flag)
            {
                //重新放入优先队列
                pq.push(tmp);
            }
        }
        else
        {
            display(time, NULL);
        }
    }
}

int init_cmp(PCB *a, PCB *b)
{
    return a->PID < b->PID;
}

//初始化
void init()
{
    //根据PID大小排序
    sort(PCB_array, PCB_array + n, init_cmp);

    //对每个PCB进行初始化
    for(int i = 0; i < n; i++)
    {
        PCB *tmp = PCB_array[i];
        tmp->cpu_time = 1;
        tmp->state = READY;
    }
}

//释放创建的内存
void free_pcb()
{
    for(int i = 0; i < n; i++)
    {
        free(PCB_array[i]);
    }
}

//菜单
void menu()
{
    process_infor();

    printf("选择调度算法：\n");
    printf("\t1.先来先服务\n");
    printf("\t2.短进程优先（非抢占）\n");
    printf("\t3.短进程优先（抢占）\n");
    printf("\t4.时间片轮换\n");
    printf("\t5.动态优先级\n");
    printf("\t6.高响应比优先\n");
    printf("\t7.退出\n");
    printf("请选择：");

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

        case 5:
            PRIORITY();
            break;

        case 6:
            HHRN();
            break;

        case 7:
            free_pcb();
            exit(0);

        default:
            printf("输入错误！\n");
            system("cls");
    }

    init();
    menu();
}

int main()
{
    #ifndef ONLINE_JUDGE
        //freopen("in.txt", "r", stdin);
        //freopen("out.txt", "w", stdout);
    #endif

    printf("输入进程数：");
    scanf("%d", &n);
//    for(int i = 0;i < n; i++)
//    {
//        PCB *tmp = (PCB*)malloc(sizeof(PCB));
//
//        tmp->PID = i + 1;
//
//        printf("输入进程优先级：");
//        scanf("%d", &(tmp->priority));
//
//        printf("输入进程进入时间：");
//        scanf("%d", &(tmp->process_reach_time));
//
//        printf("输入进程需要运行的时间：");
//        scanf("%d", &(tmp->need_time));
//
//        tmp->cpu_time = 1;
//        tmp->state = READY;
//
//        PCB_array[i] = tmp;
//    }


    srand((unsigned)time(NULL));
    for(int i = 0; i < n; i++)
    {
        PCB *tmp = (PCB*)malloc(sizeof(PCB));

        tmp->PID = i + 1;
        //优先级在0-32
        tmp->priority = (rand() % (32 - 0 + 1)) + 0;
        //到达时间在1-20
        tmp->process_reach_time = (rand() % (20 - 1)) + 1;
        //需要运行时间1-30
        tmp->need_time = (rand() % (30 - 1)) + 1;
        tmp->cpu_time = 1;
        tmp->state = READY;

        PCB_array[i] = tmp;
    }

    menu();

    return 0;
}

















