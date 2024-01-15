# 42 Project: Philosophers
# Overview

This project involves simulating a group of philosophers who alternately eat, think, and sleep at a round table. Each philosopher possesses one fork, but needs two forks to eat. They are not allowed to communicate in any way. The goal is to prevent philosophers from starving.

**Common Instructions:**  
The project must be written in C and adhere to the Norm.
Unexpected program terminations (e.g., segmentation faults) are not allowed.
Proper memory management, including freeing allocated memory, is mandatory.
Global variables are forbidden.

**To run the program:**
```
make
./philo 4 410 200 200
```
The program parameters are:  
`./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]`
## Mandatory Part (philo)
Each philosopher is implemented as a thread.
One fork is placed between each pair of philosophers, protected by a mutex.

In my solution the philosophers keep counting the meals and each round one of them waits with eating so that meals are shared more evenly. A deadlock is impossible because all mutexes are always locked in the same order.

## Bonus Part (philo_bonus)
All forks are placed in the middle of the table, represented by a semaphore.
Philosophers are implemented as processes.
Main process is not a philosopher.

In my solution each philosopher child process contains multiple threads which monitor the starvation status of the philosopher and the global termination status. The global termination status is monitored and set by the main thread. Every single thread joins and no child process needs to be terminated with kill(). There should be no memory leaks with valgrind.
