/* Program to discover a process model from an event log.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  August 2022, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: [Enter your full name and student number here before submission]
  Dated:     [Enter the date that you "signed" the declaration]

*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define GOOD_LUCK   "GOOD LUCK CLASS!!!\n"      // good luck message

#define STAGE0 "STAGE 0"
#define STAGE1 "STAGE 1"
#define STAGE2 "STAGE 2"

/* return code from get_action if end of event found */
#define EVENT_END 1
#define TRACE_END 2

#define TRACE_EXISTS 1
#define TRACE_NON_EXISTS 0

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef unsigned int action_t;  // an action is identified by an integer

typedef struct event event_t;   // an event ...
struct event {                  // ... is composed of ...
    action_t actn;              // ... an action that triggered it and ...
    event_t* next;              // ... a pointer to the next event in the trace
};

typedef struct {                // a trace is a linked list of events
    event_t* head;              // a pointer to the first event in this trace
    event_t* foot;              // a pointer to the last event in this trace
    int      freq;              // the number of times this trace was observed
} trace_t;

typedef struct {                // an event log is an array of distinct traces
                                //     sorted lexicographically
    trace_t* trcs;              // an array of traces
    int      ndtr;              // the number of distinct traces in this log
    int      cpct;              // the capacity of this event log as the number
                                //     of  distinct traces it can hold
} log_t;

typedef action_t** DF_t;        // a directly follows relation over actions

/* total events linked list */
typedef struct event_tt total_events;
struct event_tt{
    action_t elem;
    int nums;
    total_events* next;
};

/* most frequent traces linked list */
typedef struct mftf_i mftf_idx;
struct mftf_i{
    int idx;                // most frequent traces' index
    int mftf;               // most frequent trace frequency
    mftf_idx* next;
};

struct candidate
{
    /* data */
    action_t u;
    action_t v;
    unsigned int pd;
    unsigned int weight;

    struct candidate* next;
};

typedef struct best
{
    /* data */
    action_t u;
    action_t v;
    unsigned int pd;
    unsigned int weight;
}bestCandidate;


typedef struct candidate candidates;

int code = 256;

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
/* FUNCTION'S  DECLARATION------------------------------------------------------*/
void        print_stage_line();
void        insert_at_foot(trace_t **trace, action_t action);
trace_t     *insert_at_head(trace_t *list, action_t action);
int         get_traces(trace_t **ptrace);
int         get_events(trace_t **event);


void        do_stage0();
void        do_stage1();
void        do_stage2();
void        print_blank();
int         whether_trace_exists(trace_t **ptrace1, trace_t **ptrace2);
void        insert_at_event_foot(total_events *pevents, action_t action);
total_events *remove_duplicate_elements(total_events *pevents);
void        BubbleSort(total_events *head);
log_t       *traces_linked_list(log_t *plog_t, int tnot, int *nodt);
total_events *events_linked_list(log_t *plog_t, int tnot);
int         calculate_number_of_distinct_events(total_events *t_events);
int         calculate_total_numbers_of_traces(total_events *t_events);
void        calculate_the_trace_frequency(log_t *plog_t, int tnot);
mftf_idx    *most_frequent_trace_frequency(log_t *plog_t, int nodt, int *mftf);
void        print_the_most_frequent_trace(log_t *plog_t, mftf_idx *mftf);
void        print_all_events(total_events *t_events);
total_events * stage0(log_t *plog_t, int tnot);
void        insert_at_mftf_foot(mftf_idx *pmftf, int idx);

int         calculateDim(total_events *t_events);
int         searchIndex(total_events *t_events, action_t target);
void        deleteMatrix(void* matrix, int size);
void        printMatrix(int size, total_events *t_events, void* matrix);
void*       makeMatrix(int size, total_events *t_events, log_t *plog_t, int tnot);
void        printLine();
bestCandidate findBest(candidates* head);
void        deleteCandidates(candidates* head);
candidates* makeCandidates(int size, total_events *t_events, void* matrix);
int         searchSupFromMatrix(total_events *t_events, void* matrix, int size, action_t a, action_t b);
void        printCode(action_t u, action_t v);
void        changeInputLog(bestCandidate b, log_t* plog_t, int tnot);
void        delete_total_events_linked_list(total_events *head);
bool        stage1(total_events **t_events, log_t *plog_t, int tnot);

int
main(int argc, char *argv[]){

    log_t *plog_t;
    plog_t = (log_t*)malloc(sizeof(log_t));
    assert(plog_t);
    int c, n = 1;

    /* Read data */
    while ((c = get_traces(&(plog_t[n-1].trcs))) != EOF){
        ++n;
        plog_t = (log_t*)realloc(plog_t, n * (sizeof(log_t)));
    }
    int tnot = n;
    
    // for(int i = 0; i <= tnot; ++i){
    //     event_t *head = plog_t[i].trcs->head;
    //     while(head != NULL) {
    //         if (head->actn < 256)
    //             printf("%c ", head->actn);
    //         else 
    //             printf("%d ", head->actn);
    //         head = head->next;
    //     }
    //     // printf("%d ", plog_t[i].trcs->freq);
    //     print_blank();
    // }

    /* stage 0 */
    print_stage_line(STAGE0);
    total_events * t_events = stage0(plog_t, tnot);

    /* stage 1 */
    print_stage_line(STAGE1);
    while(stage1(&t_events, plog_t, tnot));

    // /* all data */
    


    return EXIT_SUCCESS;
}

int 
get_traces(trace_t **ptrace){
    *ptrace = (trace_t*)malloc(sizeof(trace_t));
    assert(ptrace);
    int c;
    (*ptrace)->freq = 1;
    (*ptrace)->foot = NULL;
    (*ptrace)->head = NULL;
    while((c = get_events(ptrace)) == EVENT_END){}
    return c;
}

int
get_events(trace_t **ptrace){
    action_t c;
    while ((c = getchar()) != EOF && !isalpha(c) && c != '\n'){
    }
    if(c == EOF){
        return EOF;
    }else if(c == '\n'){
        return TRACE_END;
    }else{
        insert_at_foot(ptrace, c);
        return EVENT_END;
    }
    
}

void
insert_at_foot(trace_t **ptrace, action_t action){
    event_t *new;
    new = (event_t*)malloc(sizeof(*new));
    assert(*ptrace && new);
    new->actn = action;
    new->next = NULL;

    if((*ptrace)->head == NULL){
        (*ptrace)->head = new;
        (*ptrace)->foot = new;
    }else{
        (*ptrace)->foot->next = new;
        (*ptrace)->foot = new;
    }
}


void
print_stage_line(char* stage){
    printf("==%s==========================\n", stage);
}

void
print_blank(){
    printf("\n");
}


// 计算矩阵大小（// INPUT: 去重后的链表，排序（黄））
int calculateDim(total_events *t_events)
{
    int size = 0;
    total_events *t_events_node = t_events;
    while (t_events_node != NULL){
        size++;
        t_events_node = t_events_node->next;
    }
    return size;
}

// 获取action_t在矩阵中对应的索引值（// INPUT: 去重后的链表，排序（黄））
int searchIndex(total_events *t_events, action_t target)
{
    int index = 0;
    
    total_events *t_events_node = t_events;
    while (t_events_node != NULL){

        if (target == t_events_node->elem){
            break;
        }

        t_events_node = t_events_node->next;
        index += 1;
    }

    return index;
}

// 创建二维矩阵（// INPUT: 原始输入（林）// INPUT: 去重后的链表，排序（黄））
void* makeMatrix(int size, total_events *t_events, log_t *plog_t, int tnot)
{
    int **matrix = (int **)malloc(size * sizeof(int*));
    for (int i=0; i<size; i++){
        matrix[i] = (int*)malloc(size * sizeof(int));
        memset(matrix[i], 0, size * sizeof(int));
    }
        
    // 遍历原始输入，计算SUP（例如SUP（a, b）表示b在a后面出现的次数），每出现一次，对二维矩阵（a, b）进行+1操作，每次索引需要遍历链表获取。
    for(int i = 0; i < tnot; ++i){
        event_t *head = plog_t[i].trcs->head;

        action_t record_a = -1; 
        action_t record_b = -1; 
        int index_a = -1;
        int index_b = -1;
        // printf("##################\n");
        while(head != NULL) {
            
            if (record_a == -1){ //记录第一个节点
                record_a = head->actn;
                index_a = searchIndex(t_events, record_a);
            }else{
                record_b = head->actn;
                // if (record_b == 256)
                //     printf("fuck");
                index_b = searchIndex(t_events, record_b);
                matrix[index_a][index_b] += 1;
                // printf("u: %c, v: %c \n", record_a, record_b);
                // printf("matrix[%d][%d] += 1\n", index_a, index_b);
                record_a = record_b;
                index_a = index_b;
            } 
            head = head->next;
        }
    }

    return (void*)matrix;//返回万能指针出去，由于知道了size，很容易再转回来
}

void deleteMatrix(void* matrix, int size)
{
    int **matrix_tmp = matrix;
    for (int i=0; i<size; i++){
        free(matrix_tmp[i]);
    }
    free(matrix_tmp);
}

int searchSupFromMatrix(total_events *t_events, void* matrix, int size, action_t a, action_t b)
{
    int **matrix_tmp = (int **)matrix;
    int a_index = searchIndex(t_events, a);
    int b_index = searchIndex(t_events, b);
    return matrix_tmp[a_index][b_index];
}

void printMatrix(int size, total_events *t_events, void* matrix)
{
    int **matrix_tmp = (int **)matrix;
    total_events *t_events_p = NULL;

    // 打印第一行
    printf(" \t");
    t_events_p = t_events;
    while (t_events_p != NULL){
        if (t_events_p->elem < 256)
            printf("%c\t", t_events_p->elem);
        else
            printf("%d\t", t_events_p->elem);
        t_events_p = t_events_p->next;
    }
    printf("\n");
    // 依次打印其它行
    t_events_p = t_events;
    for (int i = 0; i < size; i++)
    {
        // 第一列的输出
        if (t_events_p->elem < 256)
            printf("%c\t", t_events_p->elem);
        else
            printf("%d\t", t_events_p->elem);
        t_events_p = t_events_p->next;

        for (int j = 0; j < size; j++)
        {
            printf("%d\t", matrix_tmp[i][j]);
        }

        printf("\n");
    }
}

void printLine()
{
    printf("-------------------------------------\n");
}

// 创建候选集
candidates* makeCandidates(int size, total_events *t_events, void* matrix)
{
    int **matrix_tmp = (int **)matrix;
    unsigned int pd, weight;
    unsigned int sup_uv, sup_vu;
    int index_u, index_v;

    candidates* head = (candidates* )malloc(sizeof(candidates)); //候选集的虚头节点
    head->next = NULL;
    candidates* foot = head;

    // 获取所有events(u, v)，若满足条件则加入到候选链表中(注意events(u, v) ！= events(v, u))
    for (total_events* u = t_events; u != NULL; u=u->next)
        for (total_events* v = t_events; v != NULL; v=v->next)
        {
            if (u == v)
                continue;
            

            // 是否满足条件
            index_u = searchIndex(t_events, u->elem);
            index_v = searchIndex(t_events, v->elem);

            sup_uv = matrix_tmp[index_u][index_v];
            sup_vu = matrix_tmp[index_v][index_u];

            if (u->elem >=256 || v->elem >=256) // 抽象出来的就不能加进候选集
                continue;

            if (sup_uv <= sup_vu) // 不满足跳过
                continue;
            
            pd = (100 * abs(sup_uv - sup_vu));
            pd /= sup_uv > sup_vu ? sup_uv: sup_vu; 
            if (pd <= 70) // 不满足跳过
                continue;
            
            weight = abs(50-pd);
            weight *= sup_uv > sup_vu ? sup_uv: sup_vu;
            
            // 加入候选集中
            candidates* node = (candidates* )malloc(sizeof(candidates));
            node->next = NULL;
            node->pd = pd;
            node->weight = weight;
            node->u = u->elem;
            node->v = v->elem;
            foot->next = node;
            foot = foot->next;
        }
    
    // 删除虚头节点
    foot = head->next;
    free(head);

    return foot;
}

// 删除候选集
void deleteCandidates(candidates* head)
{
    candidates* l = head;
    candidates* r = l->next;

    while(r!=NULL){
        free(l);
        l = r;
        r = r->next;
    }

    free(l);
}

// 找到权重最高的候选
bestCandidate findBest(candidates* head)
{
    bestCandidate m;

    unsigned int best_weight = 0;
    while(head != NULL)
    {
        if (best_weight < head->weight)
        {
            best_weight = head->weight;
            m.weight = best_weight;
            m.pd = head->pd;
            m.u = head->u;
            m.v = head->v;
        }
        head = head->next; 
    }
    return m;
}

void printCode(action_t u, action_t v)
{
    printf("%d = SEQ(", code);
    if (u < 256)
        printf("%c, ", u);
    else
        printf("%d, ", u);

    if (v < 256)
        printf("%c)\n", v);
    else
        printf("%d)\n", v);
}


void changeInputLog(bestCandidate b, log_t* plog_t, int tnot)
{
    for(int i = 0; i < tnot; ++i){
        event_t *head = (event_t *)malloc(sizeof(event_t)); // 创建虚头节点, 更容易操作
        head->next = plog_t[i].trcs->head;

        event_t *p_slow = head; // uv是两个连在一起的节点，删掉的话，需要保存uv前面的节点（p_slow）
        event_t *p_fast = head->next; // 用于正常遍历

        event_t * p_u = NULL; // 保存u的节点地址

        while(p_fast != NULL) {
            //先发现u,但还需要判断后面是否接着v，是则删除，否则重新找
            if (p_fast->actn == b.u)// 1. 先找到u
            {
                p_u = p_fast;
                p_fast = p_fast->next; // p_slow 此时在u的前面，这里只前进p_fast来检查后面是否是v
            }else if(p_u != NULL && p_fast->actn != b.v) //2. 情况1：u后面不是v
            {
                p_u->actn = code; // u是独立的
                
                p_u = NULL; //重新找u
                p_slow = p_slow->next->next;  
                p_fast = p_fast->next;
            }else if (p_u != NULL && p_fast->actn == b.v) //2. 情况2：u后面是v
            {
                // 创建新的节点
                event_t *new_node = (event_t *)malloc(sizeof(event_t));
                new_node->actn = code;
                p_slow->next = new_node; new_node->next = p_fast->next; //重新连： u前一个节点->新节点->v后面的节点
                free(p_u); //释放u 和 v节点
                free(p_fast);
                break;

            }else if (p_u == NULL && p_fast->actn == b.v)// 情况3：遇到第一个是v
            {
                p_fast->actn = code; // v是独立的
                p_slow = p_slow->next; //两个指针正常前进
                p_fast = p_fast->next;
            }
            else{ // u，v没有遇到的情况
                p_slow = p_slow->next; //两个指针正常前进
                p_fast = p_fast->next;
            }
        }

        // 需要释放虚头节点
        event_t * new_head = head->next;
        free(head);
        

        // plog_t[i].trcs 应该指向新的头节点和尾节点
        plog_t[i].trcs->head = new_head;
        while(new_head->next != NULL)
            new_head = new_head->next;
        plog_t[i].trcs->foot = new_head;
    }
}

bool stage1(total_events **t_events, log_t *plog_t, int tnot)
{
    static bool first_exc = false;

    // 计算矩阵的大小
    int matrix_size = calculateDim(*t_events);

    // 创建矩阵
    void * matrix = makeMatrix(matrix_size, *t_events, plog_t, tnot);

    // 创建候选集
    candidates* cans = makeCandidates(matrix_size, *t_events, matrix);

    candidates* p = cans;
    // printf("candidates: ");
    // while (p != NULL)
    // {
    //     if (p->u < 256)
    //         printf("u=%c ", p->u);
    //     else 
    //         printf("u=%d ", p->u);

    //     if (p->v < 256)
    //         printf("v=%c ", p->v);
    //     else 
    //         printf("v=%d ", p->v);
        
    //     printf("pd=%d weight=%d ,", p->pd, p->weight);
    //     p = p->next;
    // }
    // printf("\n");

    if (cans == NULL)//无候选集，stage1执行完毕
        return false;
    
    if (first_exc) //stage1的第一个矩阵不会打印
        printf("=====================================\n");

    // 从候选集中选出best
    bestCandidate b = findBest(cans);

    // 打印矩阵
    printMatrix(matrix_size, *t_events, matrix);

    printLine();
    
    // 打印SEQ（，）、CON（，）
    printCode(b.u, b.v);

    // 打印 Number of events removed
    int removeSup = 0;
    removeSup += searchSupFromMatrix(*t_events, matrix, matrix_size, b.u, b.v);
    removeSup += searchSupFromMatrix(*t_events, matrix, matrix_size, b.v, b.u);
    printf("Number of events removed: %d\n", removeSup);

    // 改变原始输入
    changeInputLog(b, plog_t, tnot);

    // 统计输入改变后的event次数
    delete_total_events_linked_list(*t_events);
    /* create new events linked list */
    *t_events = events_linked_list(plog_t, tnot);
    /* remove duplicate elements in new events linked list */
    *t_events = remove_duplicate_elements(*t_events);
    /* print number of all events */
    print_all_events(*t_events);

    // 释放资源
    deleteMatrix(matrix, matrix_size);
    deleteCandidates(cans);

    code += 1;

    first_exc = true;
    return true;
}

total_events *
stage0(log_t *plog_t, int tnot){
    /*
     * @brief stage 01
     * @param plog_t: original data log
     * @param tnot: total numbers of traces
     * @return t_events: total events linked list
     */

    /* create new events linked list */
    total_events *t_events = events_linked_list(plog_t, tnot);
    /* calcute total number of events */
    int tnoe = calculate_total_numbers_of_traces(t_events);
    /* remove duplicate elements in new events linked list */
    t_events = remove_duplicate_elements(t_events);
    /* calculate number of distinct events */
    int node = calculate_number_of_distinct_events(t_events);
    /* Caculate the trace frequency */
    calculate_the_trace_frequency(plog_t, tnot);
    /* Number of distinct traces */
    int nodt = 1;
    log_t * log_tt = traces_linked_list(plog_t, tnot, &nodt);
    /* Most frequent trace frequency */
    int mftf_nums=0;
    mftf_idx *mftf = most_frequent_trace_frequency(log_tt, nodt, &mftf_nums);

    printf("Number of distinct events: %d", node);
    print_blank();

    printf("Number of distinct traces: %d", nodt);
    print_blank();

    printf("Total number of events: %d", tnoe);
    print_blank();
    
    printf("Total number of traces: %d", tnot);
    print_blank();

    printf("Most frequent trace frequency: %d", mftf_nums);
    print_blank();

    /* print the most frequent trace */
    print_the_most_frequent_trace(log_tt, mftf);
    /* print number of all events */
    print_all_events(t_events);
    // print_stage_line(STAGE1);

    return t_events;
}


void
insert_at_event_foot(total_events *pevents, action_t action){
    /*
     * @brief insert the new node at events linked list's foot
     * @param pevents: total events linked list
     * @param action: the new event
     */
    total_events *new;
    new = (total_events*)malloc(sizeof(*new));
    assert(new);
    new->elem = action;
    new->nums = 1;
    new->next = NULL;

    total_events *temp = pevents;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = new;
}

total_events *
remove_duplicate_elements(total_events *pevents){
    /*
     * @brief remove duplicate elements
     * @param pevents: total events linked list
     * @return dummy->next: the new total events linked list which remove duplicate elements
     */
    BubbleSort(pevents);

    if(!pevents)
        return pevents;
    
    total_events *dummy = (total_events*)malloc(sizeof(total_events));
    dummy->next = pevents;

    total_events *pre = dummy;
    total_events *cur = pevents;

    while(cur){
        while(cur->next && cur->elem == cur->next->elem){
            cur->next->nums = cur->nums;
            cur->next->nums++;
            cur = cur->next;
        }
        pre->next = cur;
        pre = pre->next;
        cur = cur->next;
    }
    return dummy->next;
}

void 
BubbleSort(total_events *head){
    /*
     * @brief bubble sort
     * @param head: total events linked list
     */
    total_events *p, *tail, *next;
    int temp;
    if(head->next == NULL)
        return;
    for(p=head; p!=NULL; p=p->next);
    tail = p;
    while(tail != head->next->next){
        for(p = head; p->next != tail; p = p->next){
            next = p->next;
            if(p->elem > next->elem){
                temp = p->elem;
                p->elem = next->elem;
                next->elem = temp;
            }
        }
        tail = p;
    }
}

log_t *
traces_linked_list(log_t *plog_t, int tnot, int *nodt){
    /*
     * @brief traces linked list
     * @param plog_t: old plog_t
     * @param tnot: total numbers of traces
     * @return nodt: number of distinct traces
     */
    log_t *plog_tt;
    plog_tt = (log_t*)malloc(sizeof(log_t));
    assert(plog_tt);
    int exists=TRACE_NON_EXISTS;


    plog_tt[0].trcs = (trace_t*)malloc(sizeof(trace_t));
    plog_tt[0].trcs->freq = plog_t[0].trcs->freq;
    plog_tt[0].trcs->foot = NULL;
    plog_tt[0].trcs->head = NULL;
    event_t* tmp_head = plog_t[0].trcs->head;
    while(tmp_head != NULL){
        insert_at_foot(&(plog_tt[0].trcs), tmp_head->actn);
        tmp_head = tmp_head->next;
    }

    for(int i=0; i<tnot; ++i){
        for(int j=0; j<*nodt; j++){
            exists = whether_trace_exists(&(plog_tt[j].trcs), &(plog_t[i].trcs));
            if(exists == TRACE_EXISTS){
                break;
            }
        }
        if(exists == TRACE_NON_EXISTS){
            ++*nodt;
            plog_tt = (log_t*)realloc(plog_tt, *nodt * (sizeof(log_t)));
            plog_tt[*nodt-1].trcs = (trace_t*)malloc(sizeof(trace_t));
            plog_tt[*nodt-1].trcs->freq = plog_t[i].trcs->freq;             // copy freq
            plog_tt[*nodt-1].trcs->foot = NULL;
            plog_tt[*nodt-1].trcs->head = NULL;
            tmp_head = plog_t[i].trcs->head;
            while(tmp_head != NULL){
                insert_at_foot(&(plog_tt[*nodt-1].trcs), tmp_head->actn);
                tmp_head = tmp_head->next;
            }
        }
    }
    return plog_tt;
}

total_events *
events_linked_list(log_t *plog_t, int tnot){
    /*
     * @brief create total events linked list 
     * @param plog_t: original data log
     * @param tnot: total numbers of traces
     * @return t_events: total events linked list
     */

    total_events *t_events;
    t_events = (total_events*)malloc(sizeof(total_events));
    assert(t_events);
    t_events->next = NULL;
    int tnoe = 0;
    for(int i=0; i<tnot; ++i){
        event_t *t_events_head = plog_t[i].trcs->head;
        while(t_events_head != NULL) {
            insert_at_event_foot(t_events, t_events_head->actn);
            t_events_head = t_events_head->next;
            tnoe++;
        }
    }
    t_events = t_events->next;
    return t_events;
}

int
calculate_total_numbers_of_traces(total_events *t_events){
    /*
     * @brief calculate total numbers of traces
     * @param t_events: total events linked list
     * @return tnot: total numbers of traces
     */
    int tnot = 0;
    total_events *t_events_node = t_events;
    while (t_events_node != NULL){
        tnot++;
        t_events_node = t_events_node->next;
    }
    return tnot;
}

int
calculate_number_of_distinct_events(total_events *t_events){
    /*
     * @brief calculate the number of distinct events
     * @param t_events: total events linked list
     * @return node: number of distinct events
     */
    int node = 0;
    total_events *t_events_node = t_events;
    while (t_events_node != NULL){
        node++;
        t_events_node = t_events_node->next;
    }
    return node;
}

void 
calculate_the_trace_frequency(log_t *plog_t, int tnot){
    /*
     * @brief calculate the trace frequency
     * @param plog_t: original data log
     * @param tnot: total numbers of traces
     */
    for(int i=0; i<tnot; ++i){
        for(int j=0; j<tnot; ++j){
            if(whether_trace_exists(&(plog_t[i].trcs), &(plog_t[j].trcs)) && (i!=j)){
                plog_t[i].trcs->freq++;
            }
        }
    }
}

mftf_idx *
most_frequent_trace_frequency(log_t *plog_tt, int nodt, int *mftf_nums){
    /*
     * @brief most frequent trace frequency
     * @param plog_t: original data log
     * @param tnot: total numbers of traces
     * @param mftf_idx: most frequent trace frequency index
     * @param mftf: most frequent trace frequency
     */

    mftf_idx *mftf;
    mftf = (mftf_idx*)malloc(sizeof(*mftf));
    assert(mftf);
    mftf->next = NULL;

    /* first, to find the most frequent trace frequency */
    for(int i=0; i<nodt; ++i){
        if(plog_tt[i].trcs->freq > *mftf_nums){
            *mftf_nums = plog_tt[i].trcs->freq;
        }
    }

    /* second, to store all traces of the highest frequency */
    for(int i=0; i<nodt; ++i){
        if(plog_tt[i].trcs->freq == *mftf_nums){
            insert_at_mftf_foot(mftf, i);
        }
    }

    return mftf->next;
}

void
insert_at_mftf_foot(mftf_idx *pmftf, int idx){
    /*
     * @brief insert the new node at events linked list's foot
     * @param pmftf: total events linked list
     * @param action: the new event
     */
    mftf_idx *new;
    new = (mftf_idx*)malloc(sizeof(*new));
    assert(new);
    new->idx = idx;
    new->next = NULL;

    mftf_idx *temp = pmftf;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = new;
}

void
print_the_most_frequent_trace(log_t *plog_tt, mftf_idx *mftf){
    /*
     * @brief find the most frequent trace
     * @param plog_t: original data log
     * @param mftf_idx: most frequent trace frequency index
     */
    while(mftf != NULL){
        event_t *mftf_head = plog_tt[mftf->idx].trcs->head;
        while(mftf_head != NULL){
            if(mftf_head->actn < 256){
                printf("%c", mftf_head->actn);
            }else{
                printf("%d", mftf_head->actn);
            }
            mftf_head = mftf_head->next;
        }
        print_blank();
        mftf = mftf->next;
    }
    
}

void
print_all_events(total_events *t_events){
    /*
     * @brief print number of all events
     * @param t_events: total events linked list
     */
    total_events *t_events_all = t_events;
    while(t_events_all != NULL){
        if (t_events_all->elem < 256)
            printf("%c = ", t_events_all->elem);
        else 
            printf("%d = ", t_events_all->elem);
        printf("%d", t_events_all->nums);
        print_blank();
        t_events_all = t_events_all->next;
    }
}

int
whether_trace_exists(trace_t **ptrace1, trace_t **ptrace2){
    /*
     * @brief whether trace exists
     * @param ptrace1: trace linked list 1
     * @param ptrace2: trace linked list 2
     */
    event_t *p1 = (*ptrace1)->head;
    event_t *p2 = (*ptrace2)->head;
    if(p1 != NULL && p2 != NULL){
        while(p1 != NULL && p2 != NULL && p1->actn == p2->actn){
            p1 = p1->next;   
            p2 = p2->next;
        }
    }
    if(p1 == NULL && p2 == NULL)
        return TRACE_EXISTS;
    else
        return TRACE_NON_EXISTS;
}

void delete_total_events_linked_list(total_events *head){
    /*
     * @brief delete total events linked list
     * @param head: total events linked list
     */
    total_events *l = head;
    total_events *r = head->next;

    while(r != NULL){
        free(l);
        l = r;
        r = r->next;
    }
    free(l);
}
