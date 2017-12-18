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
    BOLCK,      //����̬
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

//��������
int cmp(PCB *a, PCB *b)
{
    return a->process_reach_time < b->process_reach_time;
}

//��ͷ
void tableHead()
{
    printf("%6s%5s%8s%10s%10s%10s\n", "ʱ��", "PID", "���ȼ�", "����ʱ��", "����ʱ��", "��Ҫʱ��");
}

//����
void display(int time, PCB *process)
{
    printf("%6d%5d%8d%10d%10d%10d\n", time, process->PID, process->priority, process->process_reach_time, process->cpu_time, process->need_time);
}

//�����ȷ���
void FCFS()
{
    //��ʾ��ͷ
    tableHead();

    //��ʼ��ʱ��
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

//�˵�
void menu()
{
    printf("ѡ������㷨��\n");
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

    //���ս��̽���ʱ���������
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

















