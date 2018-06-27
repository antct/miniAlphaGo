#include "util.h"
#include <time.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

unsigned char **xSeq, **ySeq;

TreeNode * NewNode(TreeNode *next)
{
	TreeNode *newNode = (TreeNode*)malloc(sizeof(TreeNode));
	newNode->occ =0LL;
	newNode->pla = 0LL;
	newNode->parent = NULL;
	newNode->total = newNode->win = newNode->childN = newNode->expandN = 0;
	newNode->next = next;
	return newNode;
}

/*
先行后列，occupied为1代表有棋，player为0代表白旗
*/
int GetPosition(const unsigned long long &occupied, const unsigned long long &player, unsigned char x, unsigned char y)
{
	unsigned long long mask = 1LL << ((y << 3) + x);
	if (!(occupied&mask))
		return 0;
	if (!(player&mask))
		return -1;
	return 1;
}

void SetPosition(unsigned long long &occupied, unsigned long long &player, unsigned char x, unsigned char y, bool isBlack)
{
	unsigned long long mask = 1LL << ((y << 3) + x);
	occupied |= mask;
	if (isBlack)
		player |= mask;
	else
		player &= ~mask;
}

void PrintBoard(const unsigned long long &occupied, const unsigned long long &player)
{
	cout << "   0   1   2   3   4   5   6   7  " << endl;
	cout << " \xda \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xbf" << endl;
	for (int y = 0; y < 8; y++)
	{
		cout << y << "\xb3 ";
		for (int x = 0; x < 8; x++)
		{
			int tmp = GetPosition(occupied, player, x, y);
			if (tmp > 0)
				cout << "O ";
			else
			{
				if (tmp == 0)
					cout << "  ";
				else
					cout << "X ";
			}
			cout << "\xb3 ";
		}
		if (y != 7)
			cout << "\n \xc3 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xb4" << endl;
		else
			cout << "\n \xc0 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xd9" << endl;
	}
}

int PrintBoardWithHint(const unsigned long long & occupied, const unsigned long long & player, bool isBlack)
{
	cout << "   0   1   2   3   4   5   6   7  " << endl;
	cout << " \xda \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xc2 \xc4 \xbf" << endl;
	unsigned long long tmp1, tmp2;
	int legalStep = 0,whiteCount=0,blackCount=0;
	for (int y = 0; y < 8; y++)
	{
		cout << y << "\xb3 ";
		for (int x = 0; x < 8; x++)
		{
			int tmp = GetPosition(occupied, player, x, y);
			if (tmp > 0)
			{
				cout << "O ";
				blackCount++;
			}
			else
			{
				if (tmp == 0)
				{
					int hint = SingleStep(occupied, player, x, y, isBlack, tmp1, tmp2);
					if (hint == 0)
						cout << "  ";
					else
					{
						cout << hint << ' ';
						legalStep++;
					}
				}
				else
				{
					cout << "X ";
					whiteCount++;
				}
			}
			cout << "\xb3 ";
		}
		if (y != 7)
			cout << "\n \xc3 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xc5 \xc4 \xb4" << endl;
		else
			cout << "\n \xc0 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xc1 \xc4 \xd9" << endl;
	}
	cout << "O count: " << blackCount << ", X count: " << whiteCount << endl;
	cout << ((isBlack) ? "O" : "X") << "'s turn" << endl;
	return legalStep;
}

int CountLegalSteps(const unsigned long long & occupied, const unsigned long long & player, bool isBlack)
{
	unsigned long long tmp1, tmp2;
	int legalStep = 0;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			int tmp = GetPosition(occupied, player, x, y);		
			if (tmp == 0)
			{
				int hint = SingleStep(occupied, player, x, y, isBlack, tmp1, tmp2);
				legalStep += hint;
			}						
		}
	}
	return legalStep;
}



int SingleStep(const unsigned long long &occ, const unsigned long long &pla, unsigned char stepX, unsigned char stepY, bool isBlack, unsigned long long &nextOcc, unsigned long long &nextPla)
{
	if ((occ&(1LL << ((stepY << 3) + stepX))) != 0)
		return 0;
	unsigned long long reverseMask = 0LL;
	unsigned char reverseN = 0;
	int x = stepX, y = stepY;
	int tmp;
	bool flag;
	if (isBlack)
	{
		//向右
		for (x = stepX + 1, flag = false; x < 8; ++x)
		{
			tmp = GetPosition(occ, pla, x, y);
			if (tmp > 0)
			{
				flag = true;
				break;
			}
			if (tmp == 0)
				break;
		}
		if (flag)
		{
			reverseN += x - stepX - 1;
			for (--x; x != stepX; --x)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向左
		for (x = stepX - 1, flag = false; x >= 0; --x)
		{
			tmp = GetPosition(occ, pla, x, y);
			if (tmp > 0)
			{
				flag = true;
				break;
			}
			if (tmp == 0)
				break;
		}
		if (flag)
		{
			reverseN += stepX - x - 1;
			for (++x; x != stepX; ++x)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		x = stepX;
		//向下
		for (y = stepY + 1, flag = false; y < 8; ++y)
		{
			tmp = GetPosition(occ, pla, x, y);
			if (tmp > 0)
			{
				flag = true;
				break;
			}
			if (tmp == 0)
				break;
		}
		if (flag)
		{
			reverseN += y - stepY - 1;
			for (--y; y != stepY; --y)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向上
		for (y = stepY - 1, flag = false; y >= 0; --y)
		{
			tmp = GetPosition(occ, pla, x, y);
			if (tmp > 0)
			{
				flag = true;
				break;
			}
			if (tmp == 0)
				break;
		}
		if (flag)
		{
			reverseN += stepY - y - 1;
			for (++y; y != stepY; ++y)
				reverseMask |= 1LL << ((y << 3) + x);
		}


		//向右下
		if (stepX > stepY)
		{
			for (x = stepX + 1, y = stepY + 1, flag = false; x < 8; ++x, ++y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp > 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		else
		{
			for (x = stepX + 1, y = stepY + 1, flag = false; y < 8; ++x, ++y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp > 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		if (flag)
		{
			reverseN += x - stepX - 1;
			for (--x, --y; x != stepX; --x, --y)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向左下
		if (stepX < 7 - stepY)
		{
			for (x = stepX - 1, y = stepY + 1, flag = false; x >= 0; --x, ++y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp > 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		else
		{
			for (x = stepX - 1, y = stepY + 1, flag = false; y<8; --x, ++y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp > 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		if (flag)
		{
			reverseN += stepX - x - 1;
			for (++x, --y; x != stepX; ++x, --y)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向左上
		if (stepX < stepY)
		{
			for (x = stepX - 1, y = stepY - 1, flag = false; x >= 0; --x, --y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp > 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		else
		{
			for (x = stepX - 1, y = stepY - 1, flag = false; y >= 0; --x, --y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp > 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		if (flag)
		{
			reverseN += stepX - x - 1;
			for (++x, ++y; x != stepX; ++x, ++y)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向右上
		if (7 - stepX < stepY)
		{
			for (x = stepX + 1, y = stepY - 1, flag = false; x <8; ++x, --y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp > 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		else
		{
			for (x = stepX + 1, y = stepY - 1, flag = false; y >= 0; ++x, --y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp > 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		if (flag)
		{
			reverseN += x - stepX - 1;
			for (--x, ++y; x != stepX; --x, ++y)
				reverseMask |= 1LL << ((y << 3) + x);
		}
	}
	else
	{
		//向右
		for (x = stepX + 1, flag = false; x < 8; ++x)
		{
			tmp = GetPosition(occ, pla, x, y);
			if (tmp < 0)
			{
				flag = true;
				break;
			}
			if (tmp == 0)
				break;
		}
		if (flag)
		{
			reverseN += x - stepX - 1;
			for (--x; x != stepX; --x)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向左
		for (x = stepX - 1, flag = false; x >= 0; --x)
		{
			tmp = GetPosition(occ, pla, x, y);
			if (tmp < 0)
			{
				flag = true;
				break;
			}
			if (tmp == 0)
				break;
		}
		if (flag)
		{
			reverseN += stepX - x - 1;
			for (++x; x != stepX; ++x)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		x = stepX;
		//向下
		for (y = stepY + 1, flag = false; y < 8; ++y)
		{
			tmp = GetPosition(occ, pla, x, y);
			if (tmp < 0)
			{
				flag = true;
				break;
			}
			if (tmp == 0)
				break;
		}
		if (flag)
		{
			reverseN += y - stepY - 1;
			for (--y; y != stepY; --y)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向上
		for (y = stepY - 1, flag = false; y >= 0; --y)
		{
			tmp = GetPosition(occ, pla, x, y);
			if (tmp < 0)
			{
				flag = true;
				break;
			}
			if (tmp == 0)
				break;
		}
		if (flag)
		{
			reverseN += stepY - y - 1;
			for (++y; y != stepY; ++y)
				reverseMask |= 1LL << ((y << 3) + x);
		}


		//向右下
		if (stepX > stepY)
		{
			for (x = stepX + 1, y = stepY + 1, flag = false; x < 8; ++x, ++y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp < 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		else
		{
			for (x = stepX + 1, y = stepY + 1, flag = false; y < 8; ++x, ++y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp < 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		if (flag)
		{
			reverseN += x - stepX - 1;
			for (--x, --y; x != stepX; --x, --y)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向左下
		if (stepX < 7 - stepY)
		{
			for (x = stepX - 1, y = stepY + 1, flag = false; x >= 0; --x, ++y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp < 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		else
		{
			for (x = stepX - 1, y = stepY + 1, flag = false; y<8; --x, ++y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp < 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		if (flag)
		{
			reverseN += stepX - x - 1;
			for (++x, --y; x != stepX; ++x, --y)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向左上
		if (stepX < stepY)
		{
			for (x = stepX - 1, y = stepY - 1, flag = false; x >= 0; --x, --y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp < 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		else
		{
			for (x = stepX - 1, y = stepY - 1, flag = false; y >= 0; --x, --y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp < 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		if (flag)
		{
			reverseN += stepX - x - 1;
			for (++x, ++y; x != stepX; ++x, ++y)
				reverseMask |= 1LL << ((y << 3) + x);
		}

		//向右上
		if (7 - stepX < stepY)
		{
			for (x = stepX + 1, y = stepY - 1, flag = false; x <8; ++x, --y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp < 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		else
		{
			for (x = stepX + 1, y = stepY - 1, flag = false; y >= 0; ++x, --y)
			{
				tmp = GetPosition(occ, pla, x, y);
				if (tmp < 0)
				{
					flag = true;
					break;
				}
				if (tmp == 0)
					break;
			}
		}
		if (flag)
		{
			reverseN += x - stepX - 1;
			for (--x, ++y; x != stepX; --x, ++y)
				reverseMask |= 1LL << ((y << 3) + x);
		}
	}
	if (reverseN != 0)
	{
		nextOcc = occ;
		nextPla = pla;
		SetPosition(nextOcc, nextPla, stepX, stepY, isBlack);
		if (isBlack)
			nextPla |= reverseMask;
		else
			nextPla &= (~reverseMask);
	}
	return reverseN;

}

void InitRandomSeq()
{
	xSeq = (unsigned char**)malloc(sizeof(unsigned char*)*RANDOM_SEQ_NUM);
	ySeq = (unsigned char**)malloc(sizeof(unsigned char*)*RANDOM_SEQ_NUM);
	unsigned char *x, *y,*xp,*yp;
	x = (unsigned char*)malloc(sizeof(unsigned char)*64);
	y = (unsigned char*)malloc(sizeof(unsigned char) * 64);
	xp = x;
	yp = y;
	int tmp1 = 0,tmp2=0;
	for (int i = 0; i < 64; i++)
	{
		*xp++ = tmp2;
		*yp++ = tmp1;
		if (tmp1 <7)
			tmp1++;
		else
		{
			tmp1 = 0;
			tmp2++;
		}
		
	}
	srand(time(NULL));
	int ran1, ran2;
	unsigned char tmp;
	for (int i = 0; i < RANDOM_SEQ_NUM; i++)
	{
		xSeq[i] = (unsigned char*)malloc(sizeof(unsigned char) * 64);
		ySeq[i]= (unsigned char*)malloc(sizeof(unsigned char) * 64);
		for (int j = 0; j < SWAP_TIME; j++)
		{
			ran1 = (float)rand() / (RAND_MAX + 1) * 64;
			ran2 = (float)rand() / (RAND_MAX + 1) * 64;
			tmp = x[ran1];
			x[ran1] = x[ran2];
			x[ran2] = tmp;
			tmp = y[ran1];
			y[ran1] = y[ran2];
			y[ran2] = tmp;
		}
		memcpy(xSeq[i],x,sizeof(unsigned char)*64);
		memcpy(ySeq[i], y, sizeof(unsigned char) * 64);
	}
}

void GetRandomSeq(unsigned char ** x, unsigned char ** y)
{
	int i = (float)rand() / (RAND_MAX + 1) *RANDOM_SEQ_NUM;
	*x = xSeq[i];
	*y = ySeq[i];
}

void FreeRandomSeq()
{
	for (int i = 0; i < RANDOM_SEQ_NUM; i++)
	{
		free(xSeq[i]);
		free(ySeq[i]);
	}
	free(xSeq);
	free(ySeq);
}

int CheckWinner(unsigned long long  occ, unsigned long long  pla)
{
	int whiteCount=0, blackCount=0;
	for (int i = 0; i < 64; i++)
	{
		if ((occ & 1LL) == 1LL)
		{
			if ((pla & 1LL) == 0LL)
				whiteCount++;
			else
				blackCount++;
		}
		occ=occ >> 1;
		pla=pla >> 1;
	}
	if (whiteCount == blackCount)
		return 0;
	if (whiteCount > blackCount)
		return -1;
	return 1;
}

int FindDiff(unsigned long long occ0, unsigned long long occ1, unsigned char &x, unsigned char &y)
{
	unsigned long long diff = occ0^occ1;
	if (diff == 0LL)
		return 0;
	x=y= 0;
	while (diff!=1LL)
	{
		diff=diff >> 1;
		x++;
		if (x == 8)
		{
			x = 0;
			y++;
		}
	}
	return 1;
}