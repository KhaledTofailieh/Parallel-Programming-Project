#include<iostream>
#include<math.h>
using namespace std;
int *players;
int players_count;
string s[] = { "Hajara","Waraka","Miqas" };

int getNextPlayer(int i) {
	int j = i + 1;
	while (j<players_count ) {
		if (players[j] != -1) {
			return j;
		}
		j++;
	}
	return -1;
}
int play(int id1, int id2) {
	int player1 = 0, player2 = 0,score1=0,score2=0,rounds=10;

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
			else if ((player1 == 0 && player2 == 1) ||
				(player1 == 1 && player2 == 2) || (player1 == 2 && player2 == 0)) {
				score2 += 2;
			}
			else {
				score1 += 2;
			}

		}
		rounds = 1;
	}
	cout << "(" << s[player1].data() << ", " << s[player2].data() << ")" << "s(" << score1 << "," << score2 << ")" << endl;

	if (score1 > score2) 
		return id2;
	return id1;
	
}


int main() 
{
	int rounds_count;
	cout << "Players: ";
	cin >> players_count;
	
	rounds_count = log2(players_count)+1;
	cout <<"Rounds: "<< rounds_count<<endl;

	players = new int[players_count];
	for (int i = 0;i < players_count;i++) {
		players[i] = i;
	}
	int j = 0,id1=0,id2,loser;
	while (j < rounds_count){
		id1 = getNextPlayer(-1);
		id2 = getNextPlayer(id1);
		if (id1==-1||id2 == -1) {
			break;
		}
		while (id2!=-1 && id1!=-1){
			loser = play(id1, id2);
			players[loser] = -1;
			
			id1 = getNextPlayer(id2);
			id2 = getNextPlayer(id1);
		}

		j++;
	}
	/*for (int i = 0;i < players_count;i++) {
		cout << players[i] << endl;
	}*/

	if (id2 != -1) {
		cout << "Winner: " << id2 << endl;
	}
	else {
		cout << "Winner: " << id1<<endl;
	}
	
	system("pause");
	
}

