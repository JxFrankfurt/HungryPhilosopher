//Jackson Frankfurt
//OS Class: Chapter 5: Dining Philosophers Problem
//Debian
//2/28/2018
/*
Description:
This program is a pthread-mutex-lock solution to the Dining Philosophers Problem.
The 5 philosophers share 5 chopsticks. They each eat 5 times, taking turns with the chopsticks.
*/
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define APPETITE 5

enum state {thinking, hungry, eating}; //0,1,2 respectively
enum state stateOfPhilosopher[5] = {1, 1, 1, 1, 1};
pthread_mutex_t mutex;//for locking and unlocking

pthread_cond_t cond_vars[5];//for signaling


void* sit(void *whichPhilosopher);
void think(void);
void pickup_forks(int whichPhilosopher);
void return_forks(int whichPhilosopher);
bool test(int i);
void initHunger(int whichPhilosopher);
pthread_t thread[5];

int main(void){
	printf("Program started.\n");
	pthread_mutex_init (&mutex,NULL);//one for each set of critical vars
	int i[5] = {0, 1, 2, 3, 4}; //the pointer was the issue, index variable gets changed, so a pointer to it isn't stable.
	int k;
	for(k=0; k<5; k++){
		pthread_cond_init(&cond_vars[k],NULL);
		pthread_create(&thread[k],NULL, sit, &i[k]);//sending thread id is automatic
		printf("thread[%d] has begun\n", k);
	}
	for(k=0; k<5; k++){
		pthread_join(thread[k],NULL);
		printf("thread[%d] has completed.\n",k);
	}
return 0;
}
//the parameters must be in this form for pthread_create()
void* sit(void *param){//this inherits from main
	
	int *whichPhilosopher= (int*)param;
	int j;
	for(j=0; j<APPETITE; j++){//each philosopher(thread) should eat 5 times
		//if wait times out, this might fail to execute each thread 5 times(maybe use a counter to test.)
		think();//this needs *, right? anything weird about the parameter sent? No
		pickup_forks(*whichPhilosopher);//trying to eat, locks
		return_forks(*whichPhilosopher);//done eating, unlocks
		printf("Philosopher %d has eaten %d times.\n",*whichPhilosopher,(j+1));
	}
return NULL;
}
void think(void){
	
	sleep(rand()%2+1);
}
void pickup_forks(int whichPhilosopher){
	pthread_mutex_lock(&mutex);//which of the four blocking threads gets to lock next?

	stateOfPhilosopher[whichPhilosopher] = hungry;//hungry
	printf("Philosopher %d is hungry\n",whichPhilosopher);
	while(!test(whichPhilosopher)){//<-condition clause 
		printf("~Philosopher %d is waiting.\n", whichPhilosopher);
		pthread_cond_wait(&cond_vars[whichPhilosopher], &mutex);//if only one threads goes through at a time, it will never wait
	}
	stateOfPhilosopher[whichPhilosopher] = eating;
	printf("Philosopher %d is eating\n",whichPhilosopher);
	pthread_mutex_unlock(&mutex);
	sleep(rand()%2+4);//wait for a few seconds to let other threads wait/eat
}
void return_forks(int i){
	pthread_mutex_lock(&mutex);
	stateOfPhilosopher[i] = thinking;
	printf("Philosopher %d is thinking\n",i);
	pthread_cond_signal(&cond_vars[(i+1)%5]);
	pthread_cond_signal(&cond_vars[(i+4)%5]);
	pthread_mutex_unlock(&mutex);
}
bool test(int i){
	if(stateOfPhilosopher[(i+4)%5]!=2 && //left isn't eating
		stateOfPhilosopher[(i+1)%5]!=2 && //right isn't eating
		stateOfPhilosopher[i] == 1){ //self is hungry
		//pthread_cond_signal(&cond_vars[i]);//if we don't need to signal, it's ok	
		return true;
	}
	else return false;
}