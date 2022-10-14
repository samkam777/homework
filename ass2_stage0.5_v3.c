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

#define TRACE_EXISTS 3
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

typedef struct event_tt total_events_t;
struct event_tt{
    action_t elem;
    int nums;
    total_events_t* next;
};

/* most frequent traces linked list */
typedef struct mftf_i mftf_idx_t;
struct mftf_i{
    int idx;                // most frequent traces' index
    int mftf;               // most frequent trace frequency
    mftf_idx_t* next;
};


/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
/* FUNCTION'S  DECLARATION------------------------------------------------------*/
void            print_stage_line();
void            insert_at_foot(trace_t **trace, action_t action);
trace_t         *insert_at_head(trace_t *list, action_t action);
int             get_traces(trace_t **ptrace);
int             get_events(trace_t **event);

void            print_blank();
int             whether_trace_exists(trace_t **ptrace1, trace_t **ptrace2);
void            insert_at_event_foot(total_events_t *pevents, action_t action);
total_events_t    *remove_duplicate_elements(total_events_t *pevents);
void            bubble_sort(total_events_t *head);
log_t           *traces_linked_list(log_t *plog_t, int tnot, int *nodt);
total_events_t    *events_linked_list(log_t *plog_t, int tnot);
int             calculate_number_of_distinct_events(total_events_t *t_events);
int             calculate_total_numbers_of_traces(total_events_t *t_events);
void            calculate_the_trace_frequency(log_t *plog_t, int tnot);
mftf_idx_t        *most_frequent_trace_frequency(log_t *plog_t, int nodt, int *mftf);
void            print_the_most_frequent_trace(log_t *plog_t, mftf_idx_t *mftf);
void            print_all_events(total_events_t *t_events);
void            insert_at_mftf_foot(mftf_idx_t *pmftf, int idx);
total_events_t    *stage0(log_t *plog_t, int tnot);


int             calculate_dim(total_events_t *t_events);
void            print_matrix(int size, total_events_t *t_events, void* matrix);
void*           make_matrix(int size, total_events_t *t_events, log_t *plog_t, int tnot);
int             search_index(total_events_t *t_events, action_t target);
void            print_line();
void            stage1(total_events_t **t_events, log_t *plog_t, int tnot);




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
    int total_number_of_traces = n;

    /* stage 0 */
    print_stage_line(STAGE0);
    total_events_t * t_events = stage0(plog_t, total_number_of_traces);

    /* stage 1 */
    print_stage_line(STAGE1);
    stage1(&t_events, plog_t, total_number_of_traces);


    /* stage 2 */
    print_stage_line(STAGE2);

    return EXIT_SUCCESS; // remember, algorithms are fun!!!
}

/* -------------------read data functions---------------------- */

int 
get_traces(trace_t **ptrace){
    /*
     * @brief get one traces
     * @param ptrace: Pointers to pointers for malloc 
     * @return c: the end of traces 
     */
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
    /*
     * @brief get event and store in strcut one by one
     * @param ptrace: Pointers to pointers for malloc 
     * @return c: the end of event 
     */
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
    /*
     * @brief insert a event at event list foot
     * @param ptrace: Pointers to pointers for malloc 
     * @param action: action 
     */
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


/* -------------------stage 0 functions---------------------- */

total_events_t *
stage0(log_t *plog_t, int tnot){
    /*
     * @brief stage 01
     * @param plog_t: original data log
     * @param tnot: total numbers of traces
     * @return t_events: total events linked list
     */

    /* create new events linked list */
    total_events_t *t_events = events_linked_list(plog_t, tnot);
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
    mftf_idx_t *mftf = most_frequent_trace_frequency(log_tt, nodt, &mftf_nums);

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
insert_at_event_foot(total_events_t *pevents, action_t action){
    /*
     * @brief insert the new node at events linked list's foot
     * @param pevents: total events linked list
     * @param action: the new event
     */
    total_events_t *new;
    new = (total_events_t*)malloc(sizeof(*new));
    assert(new);
    new->elem = action;
    new->nums = 1;
    new->next = NULL;

    total_events_t *temp = pevents;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = new;
}

total_events_t *
remove_duplicate_elements(total_events_t *pevents){
    /*
     * @brief remove duplicate elements
     * @param pevents: total events linked list
     * @return dummy->next: the new total events linked list which remove duplicate elements
     */
    bubble_sort(pevents);

    if(!pevents)
        return pevents;
    
    total_events_t *dummy = (total_events_t*)malloc(sizeof(total_events_t));
    dummy->next = pevents;

    total_events_t *pre = dummy;
    total_events_t *cur = pevents;

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
bubble_sort(total_events_t *head){
    /*
     * @brief bubble sort
     * @param head: total events linked list
     */
    total_events_t *p, *tail, *next;
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

total_events_t *
events_linked_list(log_t *plog_t, int tnot){
    /*
     * @brief create total events linked list 
     * @param plog_t: original data log
     * @param tnot: total numbers of traces
     * @return t_events: total events linked list
     */

    total_events_t *t_events;
    t_events = (total_events_t*)malloc(sizeof(total_events_t));
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
calculate_total_numbers_of_traces(total_events_t *t_events){
    /*
     * @brief calculate total numbers of traces
     * @param t_events: total events linked list
     * @return tnot: total numbers of traces
     */
    int tnot = 0;
    total_events_t *t_events_node = t_events;
    while (t_events_node != NULL){
        tnot++;
        t_events_node = t_events_node->next;
    }
    return tnot;
}

int
calculate_number_of_distinct_events(total_events_t *t_events){
    /*
     * @brief calculate the number of distinct events
     * @param t_events: total events linked list
     * @return node: number of distinct events
     */
    int node = 0;
    total_events_t *t_events_node = t_events;
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

mftf_idx_t *
most_frequent_trace_frequency(log_t *plog_tt, int nodt, int *mftf_nums){
    /*
     * @brief most frequent trace frequency
     * @param plog_t: original data log
     * @param tnot: total numbers of traces
     * @param mftf_idx_t: most frequent trace frequency index
     * @param mftf: most frequent trace frequency
     */

    mftf_idx_t *mftf;
    mftf = (mftf_idx_t*)malloc(sizeof(*mftf));
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
insert_at_mftf_foot(mftf_idx_t *pmftf, int idx){
    /*
     * @brief insert the new node at events linked list's foot
     * @param pmftf: total events linked list
     * @param action: the new event
     */
    mftf_idx_t *new;
    new = (mftf_idx_t*)malloc(sizeof(*new));
    assert(new);
    new->idx = idx;
    new->next = NULL;

    mftf_idx_t *temp = pmftf;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = new;
}

void
print_the_most_frequent_trace(log_t *plog_tt, mftf_idx_t *mftf){
    /*
     * @brief find the most frequent trace
     * @param plog_t: original data log
     * @param mftf_idx_t: most frequent trace frequency index
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
print_all_events(total_events_t *t_events){
    /*
     * @brief print number of all events
     * @param t_events: total events linked list
     */
    total_events_t *t_events_all = t_events;
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

/* -------------------stage 1 functions---------------------- */

void stage1(total_events_t **t_events, log_t *plog_t, int tnot)
{
    int matrix_size = calculate_dim(*t_events);
    void * matrix = make_matrix(matrix_size, *t_events, plog_t, tnot);
    print_matrix(matrix_size, *t_events, matrix);
    print_line();
}

int calculate_dim(total_events_t *t_events)
{
    int size = 0;
    total_events_t *t_events_node = t_events;
    while (t_events_node != NULL){
        size++;
        t_events_node = t_events_node->next;
    }
    return size;
}

int search_index(total_events_t *t_events, action_t target)
{
    int index = 0;
    
    total_events_t *t_events_node = t_events;
    while (t_events_node != NULL){

        if (target == t_events_node->elem){
            break;
        }

        t_events_node = t_events_node->next;
        index += 1;
    }

    return index;
}

void* make_matrix(int size, total_events_t *t_events, log_t *plog_t, int tnot)
{
    int **matrix = (int **)malloc(size * sizeof(int*));
    for (int i=0; i<size; i++){
        matrix[i] = (int*)malloc(size * sizeof(int));
        memset(matrix[i], 0, size * sizeof(int));
    }
        
    for(int i = 0; i < tnot; ++i){
        event_t *head = plog_t[i].trcs->head;

        action_t record_a = -1; 
        action_t record_b = -1; 
        int index_a = -1;
        int index_b = -1;
        while(head != NULL) {
            
            if (record_a == -1){ 
                record_a = head->actn;
                index_a = search_index(t_events, record_a);
            }else{
                record_b = head->actn;
                index_b = search_index(t_events, record_b);
                matrix[index_a][index_b] += 1;
                record_a = record_b;
                index_a = index_b;
            } 
            head = head->next;
        }
    }

    return (void*)matrix;
}


void print_matrix(int size, total_events_t *t_events, void* matrix)
{
    int **matrix_tmp = (int **)matrix;
    total_events_t *t_events_p = NULL;


    printf("     ");
    t_events_p = t_events;
    while (t_events_p != NULL){
        if (t_events_p->elem < 256)
            printf("%5c", t_events_p->elem);
        else
            printf("%5d", t_events_p->elem);
        t_events_p = t_events_p->next;
    }
    printf("\n");

    t_events_p = t_events;
    for (int i = 0; i < size; i++)
    {
  
        if (t_events_p->elem < 256)
            printf("%5c", t_events_p->elem);
        else
            printf("%5d", t_events_p->elem);
        t_events_p = t_events_p->next;

        for (int j = 0; j < size; j++)
        {
            printf("%5d", matrix_tmp[i][j]);
        }

        printf("\n");
    }
}



void
print_stage_line(char* stage){
    printf("==%s============================\n", stage);
}

void
print_blank(){
    printf("\n");
}

void print_line()
{
    printf("-------------------------------------\n");
}

