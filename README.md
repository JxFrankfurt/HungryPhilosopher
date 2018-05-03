# HungryPhilosopher

The idea is that 5 philosophers sit down to eat with only 5 chopsticks. 
They must take turns eating with the chopsticks in order to accomplish anything.

The philosophers represent different threads of a process. 
I use mutex locks to protect critical sections of code. 
Access to two chopsticks represents access to the critical section.
