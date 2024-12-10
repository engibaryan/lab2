#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_ITERATIONS 1000000

volatile int counter = 0;

#define USE_SPIN_LOCK
//#define USE_MUTEX
//#define USE_SEMAPHORE

#ifdef USE_SPIN_LOCK
pthread_spinlock_t spinlock;
#endif

#ifdef USE_MUTEX
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

#ifdef USE_SEMAPHORE
sem_t semaphore;
#endif

void *increment_counter(void *arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
#ifdef USE_SPIN_LOCK
        pthread_spin_lock(&spinlock);
        counter++;
        pthread_spin_unlock(&spinlock);
#elif defined(USE_MUTEX)
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
#elif defined(USE_SEMAPHORE)
        sem_wait(&semaphore);
        counter++;
        sem_post(&semaphore);
#else
        // No synchronization
        counter++;
#endif
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

#ifdef USE_SPIN_LOCK
    pthread_spin_init(&spinlock, 0);
#endif

#ifdef USE_SEMAPHORE
    sem_init(&semaphore, 0, 1); // Binary semaphore
#endif

    pthread_create(&thread1, NULL, increment_counter, NULL);
    pthread_create(&thread2, NULL, increment_counter, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final Counter: %d\n", counter);

#ifdef USE_SPIN_LOCK
    pthread_spin_destroy(&spinlock);
#endif

#ifdef USE_MUTEX
    pthread_mutex_destroy(&mutex);
#endif

#ifdef USE_SEMAPHORE
    sem_destroy(&semaphore);
#endif

    return 0;
}

