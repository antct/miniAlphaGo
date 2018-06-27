#include <iostream>
#include <Windows.h>
#include <assert.h>
#include "util.h"
#include "PoolManager.h"
#include "Search.h"
#include "Player.h"
#include "PlayerServer.h"

using namespace std;


void Game(Player &blackPlayer,Player &whitePlayer)
{
	unsigned long long occ = 0, pla = 0, nextOcc, nextPla;
	
	SetPosition(occ, pla, 3, 3, false);
	SetPosition(occ, pla, 3, 4, true);
	SetPosition(occ, pla, 4, 3, true);
	SetPosition(occ, pla, 4, 4, false);
	/*
	SetPosition(occ, pla, 3, 3, true);
	SetPosition(occ, pla, 3, 4, false);
	SetPosition(occ, pla, 4, 3, false);
	SetPosition(occ, pla, 4, 4, true);
	*/

	int legalStep;
	unsigned char x,y;
	bool end = false;
	while (!end)
	{
		cout << "O 's turn" << endl;
		system("CLS");
		legalStep = PrintBoardWithHint(occ, pla, true);
		if (legalStep == 0)
		{
			if (CountLegalSteps(occ, pla, false) != 0)
				blackPlayer.NotifySkip();
			else
				break;
		}
		else
		{
			blackPlayer.Play(occ,pla,true,x,y);
			while (x < 0 || x>7 || y < 0 || y>7 || SingleStep(occ, pla, x, y, true, nextOcc, nextPla) == 0)
			{
				blackPlayer.NotifyIllegal(x,y);
				blackPlayer.Play(occ, pla, true, x, y);
			}
			occ = nextOcc;
			pla = nextPla;
		}



		cout << "X 's turn" << endl;
		system("CLS");
		legalStep = PrintBoardWithHint(occ, pla, false);
		if (legalStep == 0)
		{
			if (CountLegalSteps(occ, pla, true) != 0)
				whitePlayer.NotifySkip();
			else
				break;
		}
		else
		{
			whitePlayer.Play(occ, pla, false, x, y);
			while (x < 0 || x>7 || y < 0 || y>7 || SingleStep(occ, pla, x, y, false, nextOcc, nextPla) == 0)
			{
				whitePlayer.NotifyIllegal(x, y);
				whitePlayer.Play(occ, pla, false, x, y);
			}
			occ = nextOcc;
			pla = nextPla;
		}

	}
	switch (CheckWinner(occ, pla))
	{
	case -1:
		cout << "X win" << endl;
		blackPlayer.NotifyLose();
		whitePlayer.NotifyWin();
		break;
	case 0:
		cout << "Draw" << endl;
		whitePlayer.NotifyDraw();
		blackPlayer.NotifyDraw();
		break;
	case 1:
		cout << "O win" << endl;
		whitePlayer.NotifyLose();
		blackPlayer.NotifyWin();
		break;
	}
	system("pause");
}

int main(int argc, char **argv)
{
	SetConsoleOutputCP(437);
	

	InitRandomSeq();
	PoolManager manager(POOL_RESERVE);
	
	Player *whitePlayer=NULL, *blackPlayer=NULL;

	/*
	cout << "Input the times for searching" << endl;
	long times;
	cin >> times;
	if (times < 1)
		times = 1;
	*/
	char c;
	
	cout << "Input the time budget. (unit: second)" << endl;
	float timeBudget = 0;
	cin >> timeBudget;

	unsigned int sessionId;
	cout << "Input session id" << endl;
	cin >> sessionId;
//	PlayerServer server(new AIPlayer(true, timeBudget, manager, false), "47.89.179.202", 5000, sessionId);
	PlayerServer server(new AIPlayer(true, timeBudget, manager, false), "47.89.179.202", 5000, sessionId);
	
	cout << "Create new session?(y/n)" << endl;
	do
	{
		cin >> c;
		getchar();
	} while (c != 'y' && c != 'n');
	if (c == 'y')
	{
		server.StartGame(true);
	}
	else
	{
		cout << "Choose black or white? (b/w)" << endl;

		do
		{
			cin >> c;
			getchar();
		} while (c != 'b' && c != 'w');

		if (c == 'b')
			server.setIsBlack(true);
		else
			server.setIsBlack(false);
		server.StartGame(false);
	}
	
	
	
	
	//server.StartWithoutCreate(c=='b');
	//server.Start();
	//system("pause");
	
	/*
	
	if (c == 'b')
	{
		whitePlayer = new AIPlayer(false, timeBudget,manager);
		//blackPlayer = new LocalPlayer(true);
		blackPlayer = new AIPlayer(true,timeBudget,manager,false);
	}
	else
	{
		blackPlayer = new AIPlayer(true, timeBudget, manager,false);
		whitePlayer = new LocalPlayer(false);
	}

	Game(*blackPlayer,*whitePlayer);
	delete blackPlayer;
	delete whitePlayer;
	
	*/
	FreeRandomSeq();
}