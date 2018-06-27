#include "PoolManager.h"
#include <iostream>

PoolManager::PoolManager(int reserveNum)
{
	total = (reserveNum <= 0) ? 1 : reserveNum;
	freeList = allocList = NULL;
	for (int i = 0; i < total; i++)
		freeList = NewNode(freeList);
}

TreeNode *PoolManager::Alloc(const unsigned long long &occ, const unsigned long long &pla, TreeNode *parent, bool isBlack,unsigned char change)
{
	if (freeList == NULL)
	{
		for (int i = 0; i<total/2; i++)
			freeList = NewNode(freeList);
		total += total/2;
		std::cout << "Pool size: " << total << std::endl;
	}
	TreeNode *tmp = freeList;
	freeList = tmp->next;
	tmp->next = allocList;
	allocList = tmp;

	tmp->occ = occ;
	tmp->pla = pla;
	tmp->parent = parent;
	tmp->isBlack = isBlack;
	tmp->total = tmp->win = tmp->childN = tmp->expandN = 0;
	tmp->change = change;
#ifdef _INCLUDE_RAVE
	memset(tmp->totalAMAF,0,sizeof(unsigned int)*64);
	memset(tmp->winAMAF, 0, sizeof(unsigned int) * 64);
#endif
	return tmp;
}

void PoolManager::FreeAll()
{
	TreeNode *tmp = freeList;
	if (tmp == NULL)
		freeList = allocList;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = allocList;
	}
	allocList = NULL;
}

int PoolManager::getTotal()
{
	return total;
}

PoolManager::~PoolManager()
{
	TreeNode *tmp = freeList, *p;
	while (freeList != NULL)
	{
		p = freeList->next;
		free(freeList);
		freeList = p;
	}

	while (allocList != NULL)
	{
		p = allocList->next;
		free(allocList);
		allocList = p;
	}
}