#include "mpi.h"
#include<iostream>
#include<stdlib.h>
#include<math.h>
using namespace std;


string s[] = { "Hajara","Waraka","Miqas" };
int getNextPlayer(int i,int *players,int count) {
	int j = i + 1;
	while (j<count) {
		if (players[j] != -1) {
			
			return j;
		}
		j++;
	}
	return -1;
}
int play(int id1, int id2) {
	int player1 = 0, player2 = 0, score1 = 0, score2 = 0, rounds = 10;

	while (score1 == score2)
	{
		for (int i = 0;i < rounds;i++) {
			player1 = rand() % 3;
			player2 = rand() % 3;
			//0=حجرة
			//1=ورقة
			//2=مقص
			if (player1 == player2) {
				score1++; score2++;
			}
			else if ((player1 == 0 && player2 == 1) || (player1 == 1 && player2 == 2) || (player1 == 2 && player2 == 0)) {
				score2 += 2;
			}
			else {
				score1 += 2;
			}

		}
		rounds = 1;

	}

	if (score1 > score2)
		return id2;
	return id1

}

int playing_stratigy(int *players,int elements_count,int rank) {

	int j = 0, id1 = 0, id2, loser, sub_rounds_count;
	sub_rounds_count = log2(elements_count) + 1;

	while (j < sub_rounds_count) {

		id1 = getNextPlayer(-1, players, elements_count);
		id2 = getNextPlayer(id1, players, elements_count);
		if (id1 == -1 || id2 == -1) {
			break;
		}
		while (id2 != -1 && id1 != -1) {
			loser = play(id1, id2);
			players[loser] = -1;

			id1 = getNextPlayer(id2, players, elements_count);
			id2 = getNextPlayer(id1, players, elements_count);
		}

		j++;
	}
	if (id2 != -1) 
		return (rank*elements_count) + id2;
	return (rank*elements_count) + id1;
}
const int max_num = 1000;
bool is_true_input(int input) {
	return input <= max_num && input % 2 == 0;
}

int main(int argc,char **argv) {
	int players[max_num];
	int elements_count;
	int players_count;

	int rounds_count;
	int rank=0, source = 0, dest = 0,rc,numtasks=8,tag=1,inmsg;

	MPI_Status Stat;

	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS)
	{
		cout << ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	

	

	if (rank == 0) {
		/*this task job*/
		do {
			cout << "Players: ";
			cin >> players_count;
		} while (!is_true_input(players_count));
		
		rounds_count = log2(players_count) + 1;
		cout << "Rounds: " << rounds_count << endl;
		elements_count = players_count / numtasks;

		int n = numtasks + (players_count%numtasks);
		int  *finalls=new int[n];

		for (int i = 0;i < elements_count;i++) {
			players[i] = i;
		}
	
		finalls[0] = playing_stratigy(players, elements_count, rank);
		
		for (int i = 1;i < numtasks;i++) {
			dest = i;
			source = i;
			int outmsg = elements_count;
			
			rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
			rc = MPI_Send(&players[i * elements_count], 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

		    rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
			finalls[i] = inmsg;
		}

		int s = (players_count%numtasks);
		for (int i = 0;i < s;i++){
			int j = s - i;
			finalls[numtasks + i] = players_count - j;
		}
		for (int i = 0;i < n;i++) {
			cout << finalls[i] << endl;
		}
		int Winnerid = playing_stratigy(finalls, n, 0);
		cout << "Winner is:" <<finalls[Winnerid];
	}
	else 
	{
		source = 0;
		dest = 0;
		rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
		//getting count 
		elements_count = inmsg;
		rc = MPI_Recv(&players, elements_count, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);

		for (int i = 0;i < elements_count;i++) {
			players[i] = i;
		}
		int outmsg = playing_stratigy(players, elements_count, rank);
		
		rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	
	}
	MPI_Finalize();
}