#include<iostream>
#include<stdlib.h>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[])
{
	int numtasks, rank = 5, rc, dest = 0, source = 0, tag = 1;
	int inmsg, outmsg = 10;
	int myplayers[30], *pls=new int();

	for (int i = 0;i < 10;i++) {
		myplayers[i] = 100;
		myplayers[i+10] = 200;
		myplayers[i + 20] = 300;
	}
	MPI_Status Stat;

	rc = MPI_Init(&argc, &argv);

	if (rc != MPI_SUCCESS)
	{
		cout << ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::printf("num task %d rank: %d \n", numtasks, rank);
	if (rank == 0) {
		dest = 1;
		source = 1;
		outmsg = 5;
		rc = MPI_Send(&myplayers[10], 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
		rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
		 
		dest = 2;
		source = 2;
		rc = MPI_Send(&myplayers[20], 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
		rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
		/*MPI_Send(const void *buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm)*/
		//MPI_Scatter(myplayers, 250, MPI_INT, pls, 250, MPI_INT, 0, MPI_COMM_WORLD);
		for (int i = 0;i < 10; i++) {
			myplayers[i] += 10;
			cout << myplayers << endl;
		}
		//printf("rank: %d , message %d \n",rank,inmsg);
	}
	else if (rank == 1) {
			dest = 0;
			source = 0;
			outmsg =1;
			
			rc = MPI_Recv(&myplayers, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
			rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
			//MPI_Scatter(NULL, 0, MPI_INT, pls, 250, MPI_INT, 0, MPI_COMM_WORLD);
			for (int i = 0;i < 10; i++) {
				myplayers[i] += 10;
				cout << myplayers << endl;
			}
			//printf("rank: %d message %d \n", rank,inmsg);
		}
		else if(rank==2) {
			outmsg = 2;
			source = 0;
			dest = 0;
			rc = MPI_Recv(&myplayers, 10, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
			rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

			for (int i = 0;i < 10;i++) {
				myplayers[i] += 30;
				cout << myplayers << endl;
			}
		} 
	MPI_Finalize();
}







 //MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
 //MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	