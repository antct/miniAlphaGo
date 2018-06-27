#include "Player.h"
#include "Search.h"
#include <iostream>
#include <time.h>


using namespace std;

int AIPlayer::NotifySkip()
{
	cout << "No legal step for \"" << Player::getName() << "\"." << endl;
	return 0;
}

int AIPlayer::NotifyWin()
{
	cout << "\"" << getName() << "\" win." << endl;
	return 0;
}

int AIPlayer::NotifyLose()
{
	cout << "\"" << getName() << "\" lose." << endl;
	return 0;
}

int AIPlayer::NotifyDraw()
{
	return 0;
}

int AIPlayer::NotifyIllegal(unsigned char x, unsigned char y)
{
	cout << "Illegal step at (" << (int)x << "," << (int)y << ")." << endl;
	return 0;
}

int AIPlayer::Play(const unsigned long long & occ, const unsigned long long & pla, bool isBlack, unsigned char & x, unsigned char & y)
{
	cout << "Searching..." << endl;
	
	long iterTime=0,startTime=clock();
	int res;
	if (times <= 0)
	{
		if(useRAVE)
			res = UCTSearchRAVEWithTimeBudget(manager, occ, pla, x, y, isBlack, timeBudget*BUDGET_RATIO, iterTime);
		else
			res = UCTSearchWithTimeBudget(manager, occ, pla, x, y, isBlack, timeBudget*BUDGET_RATIO, iterTime);
	}
	else
	{
		res = UCTSearch(manager, occ, pla, x, y, isBlack, times);
		iterTime = times;
	}
	if (res == 1)
	{
		cout << "Search complete, next step is (" << (int)x << "," << (int)y << ")." << endl;
		cout << "Search time: " <<( ((float)(clock() - startTime)) / CLK_TCK )<< " s, " << iterTime << " iteration." << endl;
		if(IF_AI_PAUSE)
			system("pause");
		return 0;
	}
	else
		cout << "No Illegal Step" << endl;
	if (IF_AI_PAUSE)
		system("pause");
	return 1;
}

int LocalPlayer::NotifySkip()
{
	cout << "No legal step for \"" << getName() << "\"." << endl;
	return 0;
}

int LocalPlayer::NotifyWin()
{
	cout << "\"" << getName() << "\" win." << endl;
	return 0;
}

int LocalPlayer::NotifyLose()
{
	cout << "\"" << getName() << "\" lose." << endl;
	return 0;
}

int LocalPlayer::NotifyDraw()
{
	return 0;
}

int LocalPlayer::NotifyIllegal(unsigned char x, unsigned char y)
{
	cout<<"Illegal step at (" << (int)x << "," << (int)y << ")." << endl;
	return 0;
}

int LocalPlayer::Play(const unsigned long long & occ, const unsigned long long & pla, bool isBlack, unsigned char & x, unsigned char & y)
{
	cout << "Please input next step. (column number first)" << endl;
	unsigned int ix, iy;
	while (true)
	{
		cin >> ix >> iy;
		if (ix >= 0 && ix < 8 && iy >= 0 && iy < 8)
			break;
		cout << "Illegal step. Input again." << endl;
		cin.clear();
		cin.ignore();
	}
	x = (unsigned char)ix;
	y = (unsigned char)iy;
	return 0;
}

int RemotePlayer::NotifySkip()
{
	return 0;
}

int RemotePlayer::NotifyWin()
{
	return 0;
}

int RemotePlayer::NotifyLose()
{
	return 0;
}

int RemotePlayer::NotifyDraw()
{
	return 0;
}

int RemotePlayer::NotifyIllegal(unsigned char x, unsigned char y)
{
	return 0;
}

int RemotePlayer::Play(const unsigned long long & occ, const unsigned long long & pla, bool isBlack, unsigned char & x, unsigned char & y)
{
	return 0;
}
