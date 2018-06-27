#pragma once
#include "util.h"
#include "PoolManager.h"

int FillChildren(PoolManager &manager, TreeNode &node);

TreeNode *BestChild(TreeNode *node, float c, bool isBlack);

TreeNode *BestChildRAVE(TreeNode *node, float c, float f,bool isBlack);

TreeNode *TreePolicy(PoolManager &manager, TreeNode *root, bool isBlack);

TreeNode *TreePolicyRAVE(PoolManager &manager, TreeNode *root, bool isBlack);

int DefaultPolicy(TreeNode *node);

int DefaultPolicyRAVE(TreeNode *node,int &blackCount,unsigned char *blackSteps,int &whiteCount,unsigned char *whiteSteps);

void BackUp(TreeNode *node, bool isWinner);

void BackUpRAVE(TreeNode *node, bool isWinner, bool winnerIsBlack, int &blackCount, unsigned char *blackSteps, int &whiteCount, unsigned char *whiteSteps);

//如果当前玩家有棋 返回1；如果当前玩家无棋可下但是对方可下，则返回-1；都无棋可下，返回0
int UCTSearch(PoolManager &manager, const unsigned long long &occupied, const unsigned long long &player, unsigned char &x, unsigned char &y, bool isBlack, long time);

int UCTSearchWithTimeBudget(PoolManager &manager, const unsigned long long &occupied, const unsigned long long &player, unsigned char &x, unsigned char &y, bool isBlack, float ms, long &iterTime);

int UCTSearchRAVEWithTimeBudget(PoolManager &manager, const unsigned long long &occupied, const unsigned long long &player, unsigned char &x, unsigned char &y, bool isBlack, float ms, long &iterTime);