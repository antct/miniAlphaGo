/*
	PoolManager
	内存池
*/
#pragma once
#include "util.h"
class PoolManager
{
	TreeNode *freeList, *allocList;
	int total;
public:
	PoolManager(int reserveNum = 100);
	TreeNode *Alloc(const unsigned long long &occ, const unsigned long long &pla, TreeNode *parent, bool isBlack,unsigned char change=255);
	void FreeAll();
	int getTotal();
	~PoolManager();
};