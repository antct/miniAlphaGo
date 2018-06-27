#include "Search.h"
#include <assert.h>
#include <iostream>
#include <time.h>
#include <cmath>

using namespace std;

double posWeight[64] = {2.5, 0.8, 1.4, 1.2, 1.2, 1.4, 0.8, 2.5,
					   0.8, 0.6, 0.9, 1.0, 1.0, 0.9, 0.6, 0.8,	
					   1.4, 0.9, 1.1, 1.1, 1.1, 1.1, 0.9, 1.4,
                       1.2, 1.0, 1.1, 0.8, 0.8, 1.1, 1.0, 1.2,
					   
					   1.2, 1.0, 1.1, 0.8, 0.8, 1.1, 1.0, 1.2,
					   1.4, 0.9, 1.1, 1.1, 1.1, 1.1, 0.9, 1.4,
	                   0.8, 0.6, 0.9, 1.0, 1.0, 0.9, 0.6, 0.8,
					   2.5, 0.8, 1.4, 1.2, 1.2, 1.4, 0.8, 2.5 };
				
					   /*
double posWeight[64] = { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0, 
1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0, 
1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0, 
1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0, 
1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0, 
1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};*/
//如果当前玩家有棋 返回1；如果当前玩家无棋可下但是对方可下，则返回-1；都无棋可下，返回0
int FillChildren(PoolManager &manager, TreeNode &node)
{
	node.childN = 0;
	//如果当前局面已经满，则childN为0
	if (node.occ == (~0LL))
		return 0;
	unsigned long long nextOcc, nextPla;
	//填充合法走法以及局面
	for (unsigned char x = 0; x<8; x++)
		for (unsigned char y = 0; y < 8; y++)
		{
			if (SingleStep(node.occ, node.pla, x, y, node.isBlack, nextOcc, nextPla) != 0)
				node.child[node.childN++] = manager.Alloc(nextOcc, nextPla, &node, !node.isBlack,((x<<3)+y));
		}
	//如果当前玩家无棋可下，则换玩家
	if (node.childN == 0)
	{
		for (unsigned char x = 0; x<8; x++)
			for (unsigned char y = 0; y < 8; y++)
			{
				if (SingleStep(node.occ, node.pla, x, y, !node.isBlack, nextOcc, nextPla) != 0)
					node.child[node.childN++] = manager.Alloc(nextOcc, nextPla, &node, node.isBlack,((x << 3) + y));
			}
	}
	else
		return 1;
	if (node.childN == 0)
		return 0;
	return -1;

}

TreeNode *BestChild(TreeNode *node,float c,bool isBlack)
{
	//assert(node != NULL&&node->childN>0&&node->childN==node->expandN);
	TreeNode *best = NULL, *p;
	float maxScore = -1.0f;


		for (int i = 0; i < node->childN; i++)
		{
			p = node->child[i];
			float score = posWeight[p->change]*((node->isBlack == isBlack) ? (((float)p->win) / p->total) : (1 - ((float)p->win) / p->total)) + sqrt(2 * log(node->total) / ((float)p->total))*c;
			if (maxScore < score)
			{
				maxScore = score;
				best = p;
			}
		}

	
	return best;
}
/*
TreeNode * BestChildRAVE(TreeNode * node, float c,float k, bool isBlack)
{
	//assert(node != NULL && node->childN>0 && node->childN == node->expandN);
	TreeNode *best = NULL, *p;
	float maxScore = -1.0f, beta = sqrt(k / (3.0*node->total + k));
	for (int i = 0; i < node->childN; i++)
	{
		p = node->child[i];
	//	beta = sqrt(k / (3.0*p->total + k));
		//assert(node->totalAMAF[p->change]!=0);
		float score = (1.0 - beta)* (
			(node->isBlack == isBlack) ? 
			( (((float)p->win) / p->total) ) 
			: 
			((1 - ((float)p->win) / p->total))
			) 
			+
			beta*(
				((float)node->winAMAF[p->change])/(node->totalAMAF[p->change])
			)
			+ sqrt(2 * log(node->total) / ((float)p->total))*c;
		if (maxScore < score)
		{
			maxScore = score;
			best = p;
		}
	}
	return best;
}
*/

TreeNode *TreePolicy(PoolManager &manager, TreeNode *root,bool isBlack)
{
	TreeNode *p = root;
	while (true)
	{
		if (p->childN == 0)
			return p;
		//有未探索的子节点
		if (p->childN > p->expandN)
		{
			FillChildren(manager, *p->child[p->expandN]);
			return p->child[p->expandN++];
		}
		p = BestChild(p,TREE_POLICY_C,isBlack);
	}
}
/*
TreeNode * TreePolicyRAVE(PoolManager & manager, TreeNode * root, bool isBlack)
{
	TreeNode *p = root;
	while (true)
	{
		if (p->childN == 0)
			return p;
		//有未探索的子节点
		if (p->childN > p->expandN)
		{
			FillChildren(manager, *p->child[p->expandN]);
			return p->child[p->expandN++];
		}
		p = BestChildRAVE(p, TREE_POLICY_C, RAVE_K,isBlack);
	}
}
*/


int DefaultPolicy(TreeNode *node)
{
	unsigned char *xSeq, *ySeq;
	unsigned long long occ = node->occ, pla = node->pla, nextOcc, nextPla;
	bool turn = node->isBlack, flag = false,flag2=false;
	while (true)
	{
		GetRandomSeq(&xSeq, &ySeq);
		flag = false;
		for (int i = 0; i < 64; i++)
		{
			if (SingleStep(occ, pla, xSeq[i], ySeq[i], turn, nextOcc, nextPla) > 0)
			{
				flag = true;
				occ = nextOcc;
				pla = nextPla;
				break;
			}
		}
		turn = !turn;

		if (flag == true)
		{
			flag2 = false;
			continue;
		}

		//一方无棋

		if (flag2==false)
			flag2 = true;
		else
			return CheckWinner(occ, pla);
	}
}
/*
int DefaultPolicyRAVE(TreeNode * node, int & blackCount, unsigned char * blackSteps, int & whiteCount, unsigned char * whiteSteps)
{
	blackCount =whiteCount= 0;
	unsigned char *xSeq, *ySeq;
	unsigned long long occ = node->occ, pla = node->pla, nextOcc, nextPla;
	bool turn = node->isBlack, flag = false, flag2 = false;

	while (true)
	{
		PrintBoardWithHint(occ, pla, turn);
		GetRandomSeq(&xSeq, &ySeq);
		flag = false;
		for (int i = 0; i < 64; i++)
		{
			if (SingleStep(occ, pla, xSeq[i], ySeq[i], turn, nextOcc, nextPla) > 0)
			{
				if (turn)
					blackSteps[blackCount++] = (xSeq[i] << 3) + ySeq[i];
				else
					whiteSteps[whiteCount++] = (xSeq[i] << 3) + ySeq[i];
				flag = true;
				occ = nextOcc;
				pla = nextPla;
				break;
			}
		}
		turn = !turn;

		if (flag == true)
		{
			flag2 = false;
			continue;
		}

		//一方无棋

		if (flag2 == false)
			flag2 = true;
		else
			return CheckWinner(occ, pla);
	}
}
*/
void BackUp(TreeNode *node, bool isWinner)
{
	while (node != NULL)
	{
		node->total++;
		if (isWinner)
			node->win++;
		node = node->parent;
	}
}
/*
void BackUpRAVE(TreeNode * node, bool isWinner, bool winnerIsBlack, int & blackCount, unsigned char * blackSteps, int & whiteCount, unsigned char * whiteSteps)
{
	while (node != NULL)
	{
		node->total++;
		if (isWinner)
			node->win++;
		if (node->isBlack)
		{
			for (int i = 0; i < blackCount; i++)
			{
				node->totalAMAF[blackSteps[i]]++;
				if (winnerIsBlack)
					node->winAMAF[blackSteps[i]]++;
			}
		}
		else
		{
			for (int i = 0; i < whiteCount; i++)
			{
				node->totalAMAF[whiteSteps[i]]++;
				if (!winnerIsBlack)
					node->winAMAF[whiteSteps[i]]++;
			}
		}
		if (node->parent != NULL && node->parent->isBlack)
			blackSteps[blackCount++] = node->change;
		if (node->parent != NULL && (!node->parent->isBlack))
			whiteSteps[whiteCount++] = node->change;
		node = node->parent;
	}
}
*/
//如果当前玩家有棋 返回1；如果当前玩家无棋可下但是对方可下，则返回-1；都无棋可下，返回0
int UCTSearch(PoolManager &manager, const unsigned long long &occupied, const unsigned long long &player, unsigned char &x, unsigned char &y, bool isBlack, long time)
{
	TreeNode *root = manager.Alloc(occupied, player, NULL, isBlack), *best;
	int firstRes = FillChildren(manager, *root);
	if (firstRes != 1)
		return firstRes;	//如果没有合法走法，直接返回
	if (root->childN > 1)
	{
		long i = 0;
		while (i++ < time)
		{
			TreeNode *p = TreePolicy(manager, root, root->isBlack);
			int res = DefaultPolicy(p);
			if (isBlack)
				BackUp(p, res > 0);
			else
				BackUp(p, res < 0);
		}
		best = BestChild(root, 0.0, root->isBlack);
	}
	else
		best = root->child[0]; //如果只有一种合法走法，不用算直接走
	
	FindDiff(root->occ, best->occ, x, y);
	manager.FreeAll();
	return 1;
}

inline long clamp(long t,long min,long max)
{
	if (t > max)
		t = max;
	if (t < min)
		t = min;
	return t;
}

int UCTSearchWithTimeBudget(PoolManager &manager, const unsigned long long &occupied, const unsigned long long &player, unsigned char &x, unsigned char &y, bool isBlack, float second, long &iterTime)
{
	TreeNode *root = manager.Alloc(occupied, player, NULL, isBlack),*best;
	int firstRes = FillChildren(manager, *root);
	if (firstRes != 1)
		return firstRes;
	long i = 0, total = 0;
	long checkInterval = CHECK_INTERVAL;
	float lastSecond = 0;
	long lastIter = 0;
	if (root->childN > 1)
	{
		long startTime = clock();
		while (true)
		{
			total++;
			if (i++ >= checkInterval)
			{
				i = 0;
				float currSecond = ((float)(clock() - startTime)) / CLK_TCK;
			//	cout << "check :" << total << ", " << ((float)(clock() - startTime)) / CLK_TCK << " s" << endl;
				
				if (currSecond >= second)
					break;
				checkInterval = clamp((total - lastIter) / (currSecond - lastSecond)*(second-currSecond)*0.8,MIN_CHECK_INTERVAL,MAX_CHECK_INTERVAL);
				//cout << (total - lastIter) / (currSecond - lastSecond)*(second - currSecond)*0.5 << endl;
				lastIter = total;
				lastSecond = currSecond;
				//cout << lastIter << " " << lastSecond << " " << currSecond << " " << checkInterval << endl;
			}
			TreeNode *p = TreePolicy(manager, root, root->isBlack);
			int res = DefaultPolicy(p);
			if (isBlack)
				BackUp(p, res > 0);
			else
				BackUp(p, res < 0);
		}
		best = BestChild(root, 0.0, root->isBlack);
	}
	else
		best = root->child[0];
	FindDiff(root->occ, best->occ, x, y);
	manager.FreeAll();
	iterTime = total;
	return 1;
}
/*
int UCTSearchRAVEWithTimeBudget(PoolManager & manager, const unsigned long long & occupied, const unsigned long long & player, unsigned char & x, unsigned char & y, bool isBlack, float second, long & iterTime)
{
	TreeNode *root = manager.Alloc(occupied, player, NULL, isBlack),*best;
	int firstRes = FillChildren(manager, *root);
	if (firstRes != 1)
		return firstRes;

	long i = 0, total = 0;
	int blackCount = 0, whiteCount = 0;
	if (root->childN > 1)
	{
		unsigned char blackSteps[64], whiteSteps[64];
		long startTime = clock();
		while (true)
		{
			total++;
			if (i++ >= CHECK_INTERVAL)
			{
				i = 0;
				if (((float)(clock() - startTime)) / CLK_TCK >= second)
					break;
			}
			TreeNode *p = TreePolicyRAVE(manager, root, root->isBlack);
			int res = DefaultPolicyRAVE(p, blackCount, blackSteps, whiteCount, whiteSteps);
			if (isBlack)
				BackUpRAVE(p, res > 0, res > 0, blackCount, blackSteps, whiteCount, whiteSteps);
			else
				BackUpRAVE(p, res < 0, res>0, blackCount, blackSteps, whiteCount, whiteSteps);
		}
		best = BestChild(root, 0.0, root->isBlack);
	}
	else
		best = root->child[0];
	FindDiff(root->occ, best->occ, x, y);
	manager.FreeAll();
	iterTime = total;
	return 1;
}
*/
int UCTSearchRAVEWithTimeBudget(PoolManager & manager, const unsigned long long & occupied, const unsigned long long & player, unsigned char & x, unsigned char & y, bool isBlack, float second, long & iterTime)
{
	return 0;
}