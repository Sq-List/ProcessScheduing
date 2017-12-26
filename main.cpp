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
    PCB *next;              //��һ�����̵�ָ��
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

//�����ȵ��ȷ�������
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
    int time = 0;

    //��ɵĽ�����
    int process_finsih_count = 0;
    //һ�����̿�ʼ��ʱ��
    //int start_time = 0;
    for(; process_finsih_count < n; time++)
    {
        PCB *tmp = PCB_array[process_finsih_count];

        //�����̵����ʱ��С�ڵ�ǰ��ʱ�䲢�ҽ��̵�״̬Ϊ��READY��ʱ
        if(tmp->process_reach_time <= time && tmp->state == READY)
        {
            //��¼���̿�ʼʱ�䣨���������ǽ��̵����ʱ�䣩
            //start_time = time;
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
    int time = 0;
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
    int time = 0;
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

    //��ʼ��ʱ��
    int time = 0;
    //ʱ��Ƭ
    int time_slice = 5;

    //��¼��ɵĽ�����
    int process_count = 0;

}

//�˵�
void menu()
{
    printf("ѡ������㷨��\n");
    printf("\t1.�����ȷ���\n");
    printf("\t2.�̽������ȣ�����ռ��\n");
    printf("\t3.�̽������ȣ���ռ��\n");
    printf("\t4.ʱ��Ƭ�ֻ�\n");

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
            printf("�������\n");
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

    printf("�����������");
    scanf("%d", &n);
    for(int i = 0;i < n; i++)
    {
        PCB *tmp = (PCB*)malloc(sizeof(PCB));

        printf("�������PID��");
        scanf("%d", &(tmp->PID));

        printf("����������ȼ���");
        scanf("%d", &(tmp->priority));

        printf("������̽���ʱ�䣺");
        scanf("%d", &(tmp->process_reach_time));

        printf("���������Ҫ���е�ʱ�䣺");
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

















