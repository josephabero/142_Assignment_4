#include <err.h>
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <semaphore.h>

enum combo
{
	fries_soda 	= 0,
	ham_soda 	= 1,
	ham_fries 	= 2
};

pthread_t chef, cust1, cust2, cust3;
sem_t c1, c2, c3;
sem_t chef_ready; 
int i;

int c1_count, c2_count, c3_count;

void* chef_function(void *argument)
{
	int produce_food;

	for(i = 0; i < 100; i++)
	{
		sem_wait(&chef_ready);
		// printf("%i. chef cooking up some food\n", i);

		produce_food = rand() % 3;

		switch(produce_food)
		{
			case fries_soda:
				// printf("made fries & soda\n");
				sem_post(&c1);
				break;
			case ham_soda:
				// printf("made ham & soda\n");
				sem_post(&c2);
				break;
			case ham_fries:
				// printf("made ham & fries\n");
				sem_post(&c3);
				break;
		}
	}
	pthread_cancel(cust1);
	pthread_cancel(cust2);
	pthread_cancel(cust3);
}

void* cust1_function(void *argument)
{
	while(1)
	{
		sem_wait(&c1);
		c1_count++;
		printf("c1: %i\n", c1_count);
		sem_post(&chef_ready);
	}
}

void* cust2_function(void *argument)
{
	while(1)
	{
		sem_wait(&c2);
		c2_count++;
		printf("c2: %i\n", c2_count);
		sem_post(&chef_ready);
	}
}

void* cust3_function(void *argument)
{
	while(1)
	{
		sem_wait(&c3);
		c3_count++;
		printf("c3: %i\n", c3_count);
		sem_post(&chef_ready);
	}
}


int main(int argc, char **argv)
{

	c1_count = 0;
	c2_count = 0;
	c3_count = 0;
	i = 0;

	sem_init(&chef_ready, 0, 1);
	sem_init(&c1, 0, 0);
	sem_init(&c2, 0, 0);
	sem_init(&c3, 0, 0);

	if (pthread_create(&cust1, NULL, cust1_function, NULL))
		err(1, "pthread_create cust1");

	if (pthread_create(&cust2, NULL, cust2_function, NULL))
		err(1, "pthread_create cust2");

	if (pthread_create(&cust3, NULL, cust3_function, NULL))
		err(1, "pthread_create cust3");

	if (pthread_create(&chef, NULL, chef_function, NULL))
		err(1, "pthread_create chef");

	if(pthread_join(cust3, NULL))
		err(1, "pthread_join cust3");

	if(pthread_join(cust1, NULL))
		err(1, "pthread_join cust1");

	if(pthread_join(cust2, NULL))
		err(1, "pthread_join cust2");

	if(pthread_join(chef, NULL))
		err(1, "pthread_join chef");

	
	printf("cust 1 count: %i\n", c1_count);
	printf("cust 2 count: %i\n", c2_count);
	printf("cust 3 count: %i\n", c3_count);
	return 0;
}