#pragma once
#include <string>
#include "PoolManager.h"

class Player
{
protected:
	bool isBlack;
	std::string name;
public:
	Player(bool isBlack) :isBlack(isBlack) { name = (isBlack) ? "Black Player" : "White Player"; }
	Player(bool isBlack,std::string name) :isBlack(isBlack),name(name) {}
	std::string getName() { return name; }
	void setIsBlack(bool isBlack) { this->isBlack = isBlack; }
	bool getIsBlack() { return isBlack; }
	virtual int NotifySkip() = 0;
	virtual int NotifyWin() = 0;
	virtual int NotifyLose() = 0;
	virtual int NotifyDraw() = 0;
	virtual int NotifyIllegal(unsigned char x,unsigned char y) = 0;
	virtual int Play(const unsigned long long &occ,const unsigned long long &pla,bool isBlack,unsigned char &x,unsigned char &y) = 0;
};

class AIPlayer: public Player
{
	long times;
	PoolManager &manager;
	float timeBudget;
	bool useRAVE,ifPause;
public:
	AIPlayer(bool isBlack, float timeBudget, PoolManager &manager,bool useRAVE=false) :Player(isBlack), timeBudget(timeBudget), manager(manager),times(0L),useRAVE(useRAVE) {}
	AIPlayer(bool isBlack,long times,PoolManager &manager, bool useRAVE = true):Player(isBlack),times(times),manager(manager), useRAVE(useRAVE) {}
	AIPlayer(bool isBlack,std::string name, long times, PoolManager &manager, bool useRAVE = true) :Player(isBlack,name), times(times), manager(manager), useRAVE(useRAVE) {}
	virtual int NotifySkip();
	virtual int NotifyWin();
	virtual int NotifyLose();
	virtual int NotifyDraw();
	virtual int NotifyIllegal(unsigned char x, unsigned char y);
	virtual int Play(const unsigned long long &occ, const unsigned long long &pla, bool isBlack, unsigned char &x, unsigned char &y);
};

class LocalPlayer : public Player
{
public:
	LocalPlayer(bool isBlack) :Player(isBlack) {}
	LocalPlayer(bool isBlack, std::string name) :Player(isBlack, name) {}
	virtual int NotifySkip();
	virtual int NotifyWin();
	virtual int NotifyLose();
	virtual int NotifyDraw();
	virtual int NotifyIllegal(unsigned char x, unsigned char y);
	virtual int Play(const unsigned long long &occ, const unsigned long long &pla, bool isBlack, unsigned char &x, unsigned char &y);
};

class RemotePlayer : public Player
{
public:
	RemotePlayer(bool isBlack) :Player(isBlack) {}
	RemotePlayer(bool isBlack, std::string name) :Player(isBlack, name) {}
	virtual int NotifySkip();
	virtual int NotifyWin();
	virtual int NotifyLose();
	virtual int NotifyDraw();
	virtual int NotifyIllegal(unsigned char x, unsigned char y);
	virtual int Play(const unsigned long long &occ, const unsigned long long &pla, bool isBlack, unsigned char &x, unsigned char &y);
};