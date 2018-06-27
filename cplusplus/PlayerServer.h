#ifndef PLAYERSERVER_H
#define PLAYERSERVER_H

#include "Player.h"
#include <string>



class PlayerServer
{
	Player *player;
	std::string ipAddr;
	unsigned int port;
	unsigned long long sclient;
	char buffer[MAX_STR_BUFFER],buffer2[MAX_STR_BUFFER];
	unsigned int sessionId;
	bool isBlack;
public:
	PlayerServer(Player *player,std::string ipAddr,unsigned int port,unsigned int sessionId) :player(player),ipAddr(ipAddr),port(port),sessionId(sessionId) {}
	~PlayerServer();
	void setIsBlack(bool isBlack) {
		this->isBlack = isBlack;
	}
	int StartGame(bool ifCreate);
	int StartWithCreate();
	int StartWithoutCreate(bool isBlack);
	int InitSocket();
	int ClearSocket();
	int sendData(const char *message);
	int recvData(char *buffer);
	int CreateSession(int id,bool &isBlack);
	int Move(unsigned int sessionId,unsigned char x,unsigned char y,bool isBlack,bool &illegal);
	int Board(unsigned int sessionId, unsigned long long &occ,unsigned long long &pla);
	int Turn(unsigned int sessionId, bool &isBlack);
	int Status(unsigned int sessionId, bool &onGoing, bool &blackWon);
	int ChangeColor(unsigned int sessionId);
};

#endif