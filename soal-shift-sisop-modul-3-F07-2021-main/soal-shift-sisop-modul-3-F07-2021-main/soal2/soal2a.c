#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

long long firstMatrix[10][10], secondMatrix[10][10];

void enterData()
{
	int i, j;
	printf("Enter elements of matrix 1 (4x3)\n");
	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 3; ++j)
		{
			scanf("%lld", &firstMatrix[i][j]);
		}
	}

	printf("\nEnter elements of matrix 2 (3x6)\n");
	for(i = 0; i < 3; ++i)
	{
		for(j = 0; j < 6; ++j)
		{
			scanf("%lld", &secondMatrix[i][j]);
		}
	}
}


void multiplyMatrix(long long multy[4][6])
{
	int i, j, k;

	for(i=0; i<4; i++)
	{
		for(j=0; j<6; j++)
		{
			multy[i][j] = 0;
		}
	}

	for(i=0; i<4; i++)
	{
		for(j=0; j<6; j++)
		{
			for(k=0; k<3; k++)
			{
				multy[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}
}

void displayMatrix(long long multy[][6])
{
	int i, j;
	printf("\nOutput Matrix\n");
	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 6; ++j)
		{
			printf("%lld ", multy[i][j]);
			if(j == 5)
				printf("\n\n");
		}
	}
}

int main() 
{
    key_t key = 1234;
    void* memory;
    int shmid = shmget(key, 512, IPC_CREAT | 0666);
	memory = shmat(shmid, NULL, 0);
    long long (*multy)[6] = memory;
	
    enterData();
    multiplyMatrix(multy);
    displayMatrix(multy);
	return 0;
}
