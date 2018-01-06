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

//������̵�״̬ö��
typedef enum
{
    READY,      //����̬
    RUNNING,    //����̬
    BLOCK,      //����̬
    FINISH      //���̬
} process_state;

//������̿��ƿ�
typedef struct PCB
{
    int PID;                //����ID
    int priority;           //�������ȼ�
    int process_reach_time; //���̽���ʱ��
    int need_time;          //������Ҫ���е�ʱ��
    int cpu_time;           //�����Ѿ�ռ��CPU��ʱ��
    process_state state;    //���̵�״̬
    double R;               //���ڸ���Ӧ���㷨
} PCB;

int n;                          //���̵ĸ���
PCB *PCB_array[150];            //��ָ����̵�ָ�룬���ڲ�ͬ�Ľ��̵����㷨

//��ͷ
void table_head()
{
    printf("%6s%5s%8s%10s%10s%10s\n", "ʱ��", "PID", "���ȼ�", "����ʱ��", "����ʱ��", "��Ҫʱ��");
}

//����
void display(int time, PCB *process)
{
    if(process != NULL)
    {
        printf("%5d%5d%7d%8d%10d%10d\n", time, process->PID, process->priority, process->process_reach_time, process->cpu_time, process->need_time);
    }
    else
    {
        printf("%5d%24s\n", time, "�����У�");
    }
}

//��ʾ�����̵߳Ļ�����Ϣ
void process_infor()
{
    printf("\n\n\n");
    printf("%5s%8s%10s%10s%10s\n", "PID", "���ȼ�", "����ʱ��", "����ʱ��", "��Ҫʱ��");
    //printf("%5s%8s%10s%10s\n", "PID", "���ȼ�", "����ʱ��", "��Ҫʱ��");

    for(int i = 0; i < n; i++)
    {
        PCB *process = PCB_array[i];
        printf("%5d%7d%8d%10d%10d\n", process->PID, process->priority, process->process_reach_time, process->cpu_time, process->need_time);
        //printf("%5d%7d%8d%10d\n", process->PID, process->priority, process->process_reach_time, process->need_time);
    }
}

//�����ȵ��ȷ����㷨����
int FSFC_cmp(PCB *a, PCB *b)
{
    return a->process_reach_time < b->process_reach_time;
}

//�����ȷ���
void FCFS()
{
    //��ʾ��ͷ
    table_head();

    //���ս��̽���ʱ���������
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //��ʼ��ʱ��
    int time = 1;

    //��ɵĽ�����
    int process_finsih_count = 0;

    for(; process_finsih_count < n; time++)
    {
        PCB *tmp = PCB_array[process_finsih_count];

        //�����̵����ʱ��С�ڵ�ǰ��ʱ�䲢�ҽ��̵�״̬Ϊ��READY��ʱ
        if(tmp->process_reach_time <= time && tmp->state == READY)
        {
            //�޸Ľ��̵�״̬Ϊ����
            tmp->state = RUNNING;

            display(time, tmp);
        }
        else if(tmp->cpu_time < tmp->need_time && tmp->state == RUNNING)
        {
            //���̵�CPUʱ���1
            tmp->cpu_time ++;

            display(time, tmp);
        }
        else if(tmp->cpu_time == tmp->need_time && tmp->state == RUNNING)
        {
            tmp->state = FINISH;
            //����ʱû������ʱ��
            time --;
            //��ɽ�������+1
            process_finsih_count ++;
        }
        else
        {
            display(time, NULL);
        }
    }
}

//���ڶ̽������ȣ�����ռ���㷨����
int SPN_tmp_non_preemptive(PCB *a, PCB *b)
{
    return a->need_time < b->need_time;
}

//�̽������ȣ�����ռ��
void SPN_non_preemptive()
{
    //��ʾ��ͷ
    table_head();

    //���ս��̽���ʱ���������
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //��ʼ��ʱ��
    int time = 1;
    //�ѽ���Ľ��̸���
    int reach_process_num = 0;
    //��ɵĽ�����
    int process_finsih_count = 0;
    //�н��̿�ʼִ�б�־
    int startFlag = 0;

    for(; process_finsih_count < n; time++)
    {
        PCB *tmp = PCB_array[process_finsih_count];

        //�����̵����ʱ��С�ڵ�ǰ��ʱ�䲢�ҽ��̵�״̬Ϊ��READY��ʱ
        if(tmp->process_reach_time <= time && tmp->state == READY)
        {
            //�޸Ľ��̵�״̬Ϊ����
            tmp->state = RUNNING;

            //���̿�ʼִ�б�־
            if(!startFlag)
            {
                startFlag = 1;
            }

            display(time, tmp);
        }
        else if(tmp->cpu_time < tmp->need_time && tmp->state == RUNNING)
        {
            //���̵�CPUʱ���1
            tmp->cpu_time ++;

            display(time, tmp);
        }
        else if(tmp->cpu_time == tmp->need_time && tmp->state == RUNNING)
        {
            tmp->state = FINISH;
            //����ʱû������ʱ��
            time --;
            //��ɽ�������+1
            process_finsih_count ++;
        }
        else
        {
            display(time, NULL);
        }

        //ÿ��ʱ��Ƭ����Ƿ����µĽ��̽���
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
            //ÿ��ʱ��Ƭ������Ҫʱ��ĳ��̶��Ѿ����뵽CPU����δ��ɵĽ��̽�������
            /**
                ����ڵ�ǰʱ��Ƭ����δ��ɣ���process_finsih_countָ��ǰ����
                ����process_finsih_count - 1ָ�������ɽ���
                process_finsih_countָ���µĽ���
            **/
            if(PCB_array[process_finsih_count]->state == READY)
            {
                sort(PCB_array + process_finsih_count, PCB_array + reach_process_num, SPN_tmp_non_preemptive);
            }
        }
    }
}

//���ڶ̽������ȣ���ռ���㷨����
int SPN_tmp_preemptive(PCB *a, PCB *b)
{
    return a->need_time < b->need_time;
}

//�̽������ȣ���ռ��
void SPN_preemptive()
{
    //��ʾ��ͷ
    table_head();

    //���ս��̽���ʱ���������
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //��ʼ��ʱ��
    int time = 1;
    //�ѽ���Ľ��̸���
    int reach_process_num = 0;
    //��ɵĽ�����
    int process_finsih_count = 0;
    //�н��̿�ʼִ�б�־
    int startFlag = 0;

    for(; process_finsih_count < n; time++)
    {
        //ÿ��ʱ��Ƭ����Ƿ����µĽ��̽���
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
            //ÿ��ʱ��Ƭ������Ҫʱ��ĳ��̶��Ѿ����뵽CPU����δ��ɵĽ��̽�������
            sort(PCB_array + process_finsih_count, PCB_array + reach_process_num, SPN_tmp_preemptive);
        }

        PCB *tmp = PCB_array[process_finsih_count];

        //�����̵����ʱ��С�ڵ�ǰ��ʱ�䲢�ҽ��̵�״̬Ϊ��READY��ʱ
        if(tmp->process_reach_time <= time && (tmp->state == READY || tmp->state == BLOCK))
        {
            //�޸Ľ��̵�״̬Ϊ����
            tmp->state = RUNNING;

            //���̿�ʼִ�б�־
            if(!startFlag)
            {
                startFlag = 1;
            }

            display(time, tmp);
        }
        else if(tmp->cpu_time < tmp->need_time && tmp->state == RUNNING)
        {
            //���̵�CPUʱ���1
            tmp->cpu_time ++;

            display(time, tmp);
        }
        else if(tmp->cpu_time == tmp->need_time && tmp->state == RUNNING)
        {
            tmp->state = FINISH;
            //����ʱû������ʱ��
            time --;
            //��ɽ�������+1
            process_finsih_count ++;
        }
        else
        {
            display(time, NULL);
        }
    }
}

//ʱ��Ƭ��ת
void RR()
{
    //��ʾ��ͷ
    table_head();

    //���ս��̽���ʱ���������
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //��ʼ��ʱ��
    int time = 1;
    //ʱ��Ƭ
    int time_slice = 5;
    //ʱ��Ƭ����
    int time_slice_count = 0;

    //�ѽ���Ľ��̸���
    int reach_process_num = 0;
    //��ɵĽ�����
    int process_finsih_count = 0;
    int first = 0;

    queue<PCB*> pcb_queue;

    for(; process_finsih_count < n;)
    {
        //������Ϊ�գ������̻�δȫ������ʱ������Ƿ����µĽ��̽���
        if(pcb_queue.empty())
        {
            for(int i = reach_process_num; i < n; i++)
            {
                PCB *temp = PCB_array[i];

                if(temp->process_reach_time <= time)
                {
                    reach_process_num ++;
                    //�����̼����β
                    pcb_queue.push(temp);
                }
                else if(temp->process_reach_time > time)
                {
                    break;
                }
            }
        }

        //�����в�Ϊ��ʱ
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

            //��ʱ��Ƭ��ִ�н���
            while(time_slice_count < time_slice && now->cpu_time < now->need_time)
            {
                time ++;
                time_slice_count ++;
                now->cpu_time ++;

                display(time, now);
            }

            //�ڵ�ǰ���̼����β֮ǰ������Ƿ����µĽ��̽���
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

            //���������
            if(now->cpu_time == now->need_time)
            {
                now->state = FINISH;
                process_finsih_count ++;
            }
            //���̻�δ���
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

//���ڶ�̬���ȼ��㷨����
struct priority_cmp
{
    bool operator()(PCB a, PCB b)
    {
        return a.priority < b.priority;
    }
};

//��̬���ȼ�
void PRIORITY()
{
    //��ʾ��ͷ
    table_head();

    //���ս��̽���ʱ���������
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //������priority_cmp��������ȶ���
    //Ϊ�˱���ԭ���ݣ����ﲻʹ��ָ��
    priority_queue<PCB, vector<PCB>, priority_cmp> pq;

    //��ʼ��ʱ��
    int time = 1;
    //�ѽ���Ľ��̸���
    int reach_process_num = 0;
    //��ɵĽ�����
    int process_finsih_count = 0;

    for(; process_finsih_count < n; time++)
    {
        //ÿ��ʱ��Ƭ����Ƿ����µĽ��̽���
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

        //���ȶ��в�Ϊ��ʱ
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
                //����ʱû������ʱ��
                time --;
                //��ɽ�������+1
                process_finsih_count ++;
            }

            //�����ǰ�������У���������PCB�����ȼ�����
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

            //��ǰ����δ���
            if(flag)
            {
                //���·������ȶ���
                pq.push(*tmp);
            }
        }
        else
        {
            display(time, NULL);
        }
    }
}

//���ڶ�̬���ȼ��㷨����
struct HHRN_cmp
{
    bool operator()(PCB *a, PCB *b)
    {
        return a->R < b->R;
    }
};

//����Ӧ������
void HHRN()
{
    //��ʾ��ͷ
    table_head();

    //���ս��̽���ʱ���������
    sort(PCB_array, PCB_array + n, FSFC_cmp);

    //������HHRN_cmp��������ȶ���
    priority_queue<PCB*, vector<PCB*>, HHRN_cmp> pq;

    //��ʼ��ʱ��
    int time = 1;
    //�ѽ���Ľ��̸���
    int reach_process_num = 0;
    //��ɵĽ�����
    int process_finsih_count = 0;

    for(; process_finsih_count < n; time++)
    {
        //ÿ��ʱ��Ƭ����Ƿ����µĽ��̽���
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

        //���ȶ��в�Ϊ��ʱ
        if(!pq.empty())
        {
            //����ÿ���ھ������еĽ��̵���Ӧ��
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
                //����ʱû������ʱ��
                time --;

                //��ǰִ�е���process_finsih_countλ�õ�PCB_array�Ľ��̽���λ�ã����ڲ���
                for(int i = 0; i < n; i++)
                {
                    if(PCB_array[i] == tmp)
                    {
                        PCB_array[i] = PCB_array[process_finsih_count];
                        PCB_array[process_finsih_count] = tmp;
                        break;
                    }
                }

                //��ɽ�������+1
                process_finsih_count ++;
            }

            //��ǰ����δ���
            if(!flag)
            {
                //���·������ȶ���
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

//��ʼ��
void init()
{
    //����PID��С����
    sort(PCB_array, PCB_array + n, init_cmp);

    //��ÿ��PCB���г�ʼ��
    for(int i = 0; i < n; i++)
    {
        PCB *tmp = PCB_array[i];
        tmp->cpu_time = 1;
        tmp->state = READY;
    }
}

//�ͷŴ������ڴ�
void free_pcb()
{
    for(int i = 0; i < n; i++)
    {
        free(PCB_array[i]);
    }
}

//�˵�
void menu()
{
    process_infor();

    printf("ѡ������㷨��\n");
    printf("\t1.�����ȷ���\n");
    printf("\t2.�̽������ȣ�����ռ��\n");
    printf("\t3.�̽������ȣ���ռ��\n");
    printf("\t4.ʱ��Ƭ�ֻ�\n");
    printf("\t5.��̬���ȼ�\n");
    printf("\t6.����Ӧ������\n");
    printf("\t7.�˳�\n");
    printf("��ѡ��");

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
            printf("�������\n");
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

    printf("�����������");
    scanf("%d", &n);
//    for(int i = 0;i < n; i++)
//    {
//        PCB *tmp = (PCB*)malloc(sizeof(PCB));
//
//        tmp->PID = i + 1;
//
//        printf("����������ȼ���");
//        scanf("%d", &(tmp->priority));
//
//        printf("������̽���ʱ�䣺");
//        scanf("%d", &(tmp->process_reach_time));
//
//        printf("���������Ҫ���е�ʱ�䣺");
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
        //���ȼ���0-32
        tmp->priority = (rand() % (32 - 0 + 1)) + 0;
        //����ʱ����1-20
        tmp->process_reach_time = (rand() % (20 - 1)) + 1;
        //��Ҫ����ʱ��1-30
        tmp->need_time = (rand() % (30 - 1)) + 1;
        tmp->cpu_time = 1;
        tmp->state = READY;

        PCB_array[i] = tmp;
    }

    menu();

    return 0;
}

















