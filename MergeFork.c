#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void MainMerge(int *arr, int numCount);
void ReadMerge(int *arr, int numCount, int leftFd, int rightFd);

void PrintList(int arr[50], int numCount) {
	int itr;

	for (itr = 0; itr < numCount; itr++) {
		printf("%d ", arr[itr]);
	}
	printf("\n");
}

void ReadMerge(int *arr, int numCount, int leftFd, int rightFd) {
	int lCont = 1, rCont = 1, arrNdx, leftNum, rightNum;

	read(leftFd, &leftNum, sizeof(int));
	read(rightFd, &rightNum, sizeof(int));

	for (arrNdx = 0; arrNdx < numCount && lCont && rCont; arrNdx++) 
		if (leftNum < rightNum) {
			arr[arrNdx] = leftNum;
			lCont = read(leftFd, &leftNum, sizeof(int)) > 0; 
		}
		else {
			arr[arrNdx] = rightNum;
			rCont = read(rightFd, &rightNum, sizeof(int)) > 0; 
		}
	while (lCont && arrNdx < numCount) {
		arr[arrNdx++] = leftNum;
		lCont = read(leftFd, &leftNum, sizeof(int)) > 0; 
	}
	while (rCont && arrNdx < numCount) {
		arr[arrNdx++] = rightNum;
		rCont = read(rightFd, &rightNum, sizeof(int)) > 0; 
	}
}

int ForkMerge(int *arr, int numCount) {
	int fds[2], childPid, ndx;
	pipe(fds);

	childPid = fork();
	if (childPid < 0) {
		perror("Fork gone wrong");
		exit(1);
	}
	else if (childPid == 0) {
		close(fds[0]);
		MainMerge(arr, numCount);

		for (ndx = 0; ndx < numCount; ndx++) {
			write(fds[1], arr + ndx, sizeof(int));
		}
		close(fds[1]);
		exit(0);
	}
	close(fds[1]);
	return fds[0];

}

void MainMerge(int *arr, int numCount) {
	int mid = numCount/2,  leftFd, rightFd;

	if (numCount <= 1) {
		return;
	}
	leftFd = ForkMerge(arr, mid);
	rightFd = ForkMerge(arr + mid, numCount - mid);
	ReadMerge(arr, numCount, leftFd, rightFd);
	close(leftFd);
	close(rightFd);
}

int main(int argc, char **argv) {
	int arr[50], numCount = 0;

	while (scanf("%d", &(arr[numCount])) != EOF) {
		numCount++;
	}
	MainMerge(arr, numCount);
	PrintList(arr, numCount);
	return 0;
}
