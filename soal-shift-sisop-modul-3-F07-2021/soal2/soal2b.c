#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

long long thirdMatrix[10][10], mult2[10][10];

struct result
{
    long long mat1;
    long long mat2;
    long long *lastRes;
};

void enterData()
{
	int i, j;
	printf("\nEnter elements of matrix 3:\n");
	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 6; ++j)
		{
			scanf("%lld", &thirdMatrix[i][j]);
		}
	}
}

long long factorial(long long n)
{
    long long i, fact = 1;
    if (n < 0)
        printf("Error! Factorial of a negative number doesn't exist.\n");
    else {
        for (i = 1; i <= n; ++i) {
            fact *= i;
        }
    }
    return fact;
}

long long midCase (long long a, long long b)
{
    long long hasil=1;
    long long start=a-b+1;
    
	if(a == b)
	{
		return factorial(a);
	}

	if(a==b-1)
	{
		return a;
	}

    for(long long i=start; i<=a; i++)
    {
        hasil*=i;
    }
    printf("hasil dari %lld dan %lld: %lld\n",a,b, hasil);

    return hasil;
}

void *function(void *temp)
{
    printf("FUNGSI TERPANGGIL\n");
    struct result *res = temp;
    printf("mat1 : %lld\n", res->mat1);

    if(res->mat1 == 0 || res->mat2 == 0)
    {
        *res->lastRes = 0;
    }
    else if(res->mat1 >= res->mat2)
    {
        *res->lastRes = midCase(res->mat1, res->mat2);
    }
    else if(res->mat1 < res->mat2)
    {
        *res->lastRes = factorial(res->mat1);
    }

    free(res);
    pthread_exit(NULL);
    return NULL;
}

void displayMatrix()
{
	int i, j;
	printf("\nOutput Matrix\n");
	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 6; ++j)
		{
			printf("%lld ", mult2[i][j]);
			if(j == 5)
				printf("\n\n");
		}
	}
}

int main()
{
    key_t key = 1234;
    int shmid = shmget(key, 512, IPC_CREAT | 0666);
    void* memory;
    memory = shmat(shmid, NULL, 0);
    long long (*mult)[6] = memory;

    pthread_t tid[25][25];
    int i, j;
    struct result *res;

    printf("\nOutput Matrix:\n");
    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j < 6; ++j)
        {
            printf("%lld ", mult[i][j]);
        }
        printf("\n");
    }

    enterData();

    for(i=0; i<4; i++)
	{
		for(j=0; j<6; j++)
		{		
            res = (struct result *)malloc(sizeof(struct result));
            res->mat1 = mult[i][j];
            res->mat2 = thirdMatrix[i][j];
            res->lastRes = &mult2[i][j];

            pthread_create(&tid[i][j], NULL, &function, (void*) res);
            
		}
	}

    for(i=0; i<4; i++)
	{
		for(j=0; j<6; j++)
		{		
            pthread_join(tid[i][j], NULL);
		}
	}

    displayMatrix();
}
