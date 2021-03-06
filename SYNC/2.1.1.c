/**
 * We edit the program dining_philosophers.c to get into a deadlock condition with a high probability.
 *
 * We split the process of taking the forks into two steps - taking the forks separately,
 * each time we check if the left fork is first available (to take it from the table)
 * otherwise we will check if the right fork is available.
 * In this situation, a situation is created in which all philosophers in the first stage pick up the left fork
 * and wait for the right fork to be cleared by the diner to their right, something that will not happen as he also waits.
 *
 * And so Deadlock was created.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5
#define THINKING 0
#define HUNGRY 3
#define EATING 4
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

/** ----- NEW ------- **/
#define TAKE_RIGHT 2
#define TAKE_LEFT 1

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

sem_t mutex;
sem_t S[N];

/** ----- NEW ------- **/
void takeLeft(int phnum){
    if (state[phnum] == HUNGRY) {
        state[phnum] = TAKE_LEFT;
        printf("Philosopher %d takes fork %d \n",phnum + 1, LEFT + 1);
    }
    else if (state[phnum] == TAKE_RIGHT) {
        state[phnum] = EATING;
        printf("Philosopher %d takes fork %d\n",phnum + 1, LEFT + 1);
        printf("Philosopher --------------------------------------------- %d is EATING\n",phnum + 1);
        sleep(2);//eat 2 sec
    }
}

/** ----- NEW ------- **/
void takeRight(int phnum){
    if (state[phnum] == HUNGRY) {
        state[phnum] = TAKE_RIGHT;
        printf("Philosopher %d takes fork %d\n",phnum + 1, phnum + 1);
    }
    else if (state[phnum] == TAKE_LEFT) {
        state[phnum] = EATING;
        printf("Philosopher %d takes fork %d\n",phnum + 1, phnum + 1);
        printf("Philosopher %d is EATING\n",phnum + 1);
        sleep(2);//eat 2 sec
    }
}

/** ----- Changed ------- **/
void test(int phnum)
{
    if (state[phnum] == HUNGRY || state[phnum] == TAKE_RIGHT || state[phnum] == TAKE_LEFT){
        if (state[LEFT] != EATING && state[LEFT] != TAKE_RIGHT && state[phnum] != TAKE_LEFT) {
            takeLeft(phnum);

            sem_post(&S[phnum]);
        }
        else if (state[RIGHT] != EATING && state[RIGHT] != TAKE_LEFT) {
            takeRight(phnum);

            sem_post(&S[phnum]);
        }
        else printf("Philosopher %d not take anything because the second fork is occupied\n", phnum + 1);


    }
}

// take up chopsticks

/** ----- Changed ------- **/
void take_fork(int phnum) //get hungry
{
    sem_wait(&mutex);

    if (state[phnum] == THINKING) {
        // state that hungry
        state[phnum] = HUNGRY;
        printf("Philosopher %d is Hungry\n", phnum + 1);
    }

    // eat if neighbours are not eating
    test(phnum);

    sem_post(&mutex);

    // if unable to eat wait to be signalled
    sem_wait(&S[phnum]);

    sleep(1);

}

// put down chopsticks
void put_fork(int phnum) //get thinking
{
    sem_wait(&mutex);
    if (state[phnum] == EATING) {
        // state that thinking
        state[phnum] = THINKING;

        printf("Philosopher %d putting fork %d and %d down\n",
               phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is thinking\n", phnum + 1);

        test(LEFT);
        test(RIGHT);
    }
    sem_post(&mutex);
}

void* philospher(void* num)
{

    while (1) {

        int* i = num;

        sleep(1);

        take_fork(*i);

        sleep(0);

        put_fork(*i);
    }
}

int main()
{

    int i;
    pthread_t thread_id[N];

    // initialize the mutexes
    sem_init(&mutex, 0, 1);

    for (i = 0; i < N; i++)

        sem_init(&S[i], 0, 0);

    for (i = 0; i < N; i++) {

        // create philosopher processes
        pthread_create(&thread_id[i], NULL,
                       philospher, &phil[i]);

        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++)

        pthread_join(thread_id[i], NULL);
}