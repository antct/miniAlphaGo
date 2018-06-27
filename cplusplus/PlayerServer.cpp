#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "PlayerServer.h"

#include <Winsock2.h>
#include <direct.h>   
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <conio.h>
#include "util.h"
#pragma comment(lib,"Ws2_32.lib")

using namespace std;

PlayerServer::~PlayerServer()
{
	closesocket(sclient);
	WSACleanup();
}

int PlayerServer::StartGame(bool ifCreate)
{
	unsigned int retryCount = 0;
	if (ifCreate?StartWithCreate():StartWithoutCreate(this->isBlack))
	{
		do
		{
			retryCount++;
			cout << "Aborted! " << endl;
			if (retryCount > SERVER_RETRY)
			{
				cout << "Max server retry count reached, abort." << endl;
				return 1;
			}
			cout << "Retry... Your session id is " << sessionId << ", your turn is " << ((this->isBlack) ? "Black" : "White") << endl;

		} while (StartWithoutCreate(this->isBlack));
	}
	cout << "Finish" << endl;
	system("pause");
	return 0;
}


int PlayerServer::StartWithCreate()
{
	bool isBlack=true,blackWon=false,onGoing=true,illegal=false;
	unsigned long long occ, pla;
	unsigned char x, y;

	cout << "Server Start." << endl << "Creating session..." << endl;
	if (CreateSession(sessionId, isBlack))
	{
		cout << "Create Session Failed!" << endl;
		return 1;
	}
	cout << "Session created, get " << ((isBlack) ? "Black" : "White") << endl;
	this->isBlack = isBlack;
	player->setIsBlack(isBlack);
	while (true)
	{
		cout << "Check status..." << endl;
		if (Status(sessionId, onGoing, blackWon))
		{
			cout << "Status Failed!" << endl;
			return 1;
		}
		if (onGoing == false)
		{
			cout << "Finish" << endl << "Winner is " << ((blackWon == player->getIsBlack()) ? "us." : "oppoent.") << endl;
		}
		cout << "Check turn..." << endl;
		if (Turn(sessionId, isBlack))
		{
			cout << "Turn Failed!" << endl;
			return 1;
		}
		if (isBlack == player->getIsBlack())
		{
			cout << "Check board..." << endl;
			if (Board(sessionId, occ, pla))
			{
				cout << "Board Failed!" << endl;
			}
			PrintBoardWithHint(occ,pla,isBlack);
			if (player->Play(occ, pla, player->getIsBlack(), x, y))
			{
				cout << "No legal step, change color." << endl;
				if (ChangeColor(sessionId))
				{
					cout << "Change color failed." << endl;
					return 1;
				}
				continue;
			}
			cout << "Sending movement..." << endl;
			if (Move(sessionId, x, y, player->getIsBlack(),illegal))
			{
				if (illegal)
				{
					cout << "Illegal Move!!!! Potential collision...Reset" << endl;
					continue;
				}
				cout << "Move Failed!" << endl;
				return 1;
			}
			cout << "Check board..." << endl;
			if (Board(sessionId, occ, pla))
			{
				cout << "Board Failed!" << endl;
			}
			PrintBoardWithHint(occ, pla, isBlack);
			cout << "Waiting for oppoent..." << endl;
		}
		else
		{
			cout << "Waiting for oppoent..." << endl;
			Sleep(SLEEP_TIME);
		}
	}
	
	system("pause");
	return 0;
}

int PlayerServer::StartWithoutCreate(bool isBlack)
{
	bool  blackWon = false, onGoing = true, illegal = false;
	unsigned long long occ, pla;
	unsigned char x, y;

	cout << "Server Start without creating." << endl;
	//Board(sessionId, occ, pla);
	//PrintBoardWithHint(occ, pla, isBlack);
	
	player->setIsBlack(isBlack);
	while (true)
	{
		cout << "Check status..." << endl;
		if (Status(sessionId, onGoing, blackWon))
		{
			cout << "Status Failed!" << endl;
			return 1;
		}
		if (onGoing == false)
		{
			cout << "Finish" << endl << "Winner is " << ((blackWon == player->getIsBlack()) ? "us." : "oppoent.") << endl;
		}
		cout << "Check turn..." << endl;
		if (Turn(sessionId, isBlack))
		{
			cout << "Turn Failed!" << endl;
			return 1;
		}
		if (isBlack == player->getIsBlack())
		{
			cout << "Check board..." << endl;
			if (Board(sessionId, occ, pla))
			{
				cout << "Board Failed!" << endl;
			}
			PrintBoardWithHint(occ, pla, isBlack);
			if (player->Play(occ, pla, player->getIsBlack(), x, y))
			{
				cout << "No legal step, change color." << endl;
				if (ChangeColor(sessionId))
				{
					cout << "Change color failed." << endl;
					return 1;
				}
				continue;
			}
			cout << "Sending movement..." << endl;
			if (Move(sessionId, x, y, player->getIsBlack(), illegal))
			{
				if (illegal)
				{
					cout << "Illegal Move!!!! Potential collision...Reset" << endl;
					continue;
				}
				cout << "Move Failed!" << endl;
				return 1;
			}
			cout << "Check board..." << endl;
			if (Board(sessionId, occ, pla))
			{
				cout << "Board Failed!" << endl;
			}
			PrintBoardWithHint(occ, pla, isBlack);
			cout << "Waiting for oppoent..." << endl;
		}
		else
		{
			cout << "Waiting for oppoent..." << endl;
			Sleep(SLEEP_TIME);
		}
	}

	return 0;
}

int PlayerServer::InitSocket()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 1;
	}

	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 1;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	serAddr.sin_addr.S_un.S_addr = inet_addr(ipAddr.c_str());
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 1;
	}

	int recvTimeout = SEND_TIMEOUT_SECOND * 1000;  
	int sendTimeout = RECV_TIMEOUT_SECOND * 1000;

	if (SOCKET_ERROR == setsockopt(sclient, SOL_SOCKET, SO_RCVTIMEO, (char *)&recvTimeout, sizeof(int)))
	{
		cout << "Set recv timeout Failed!" << endl;
	}
	if (SOCKET_ERROR == setsockopt(sclient, SOL_SOCKET, SO_SNDTIMEO, (char *)&sendTimeout, sizeof(int)))
	{
		cout << "Set send timeout Failed!" << endl;
	}
	return 0;
}

int PlayerServer::ClearSocket()
{
	closesocket(sclient);
	//WSACleanup();
	return 0;
}

int PlayerServer::sendData(const char * message)
{
	unsigned int resendCount = 0;
	while (send(sclient, message, strlen(message), 0) == SOCKET_ERROR)
	{
		cout << "send failed!" << endl;
		cout << "send message:" << message<<endl;
		resendCount++;
		if (resendCount > MAX_RESEND)
		{
			cout << "Max resend count reached, abort!" << endl;
			return 1;
		}
		cout << "Resending... " << resendCount << " time(s)" << endl;
	}

	return 0;
}

int PlayerServer::recvData(char * buffer)
{
	unsigned int resendCount = 0;
	while (recv(sclient, buffer, MAX_STR_BUFFER, 0) == SOCKET_ERROR)
	{
		cout << "recv failed!" << endl;
		resendCount++;
		if (resendCount > MAX_RESEND)
		{
			cout << "Max retry count reached, abort!" << endl;
			return 1;
		}
		cout << "Retry.. " << resendCount << " time(s)" << endl;
	}
		
	return 0;
}

int PlayerServer::CreateSession(int id, bool & isBlack)
{
	InitSocket();
	sprintf(buffer,"GET /create_session/%d HTTP/1.0\r\n\r\n",id);
	if (sendData(buffer))
		return 1;
	memset(buffer, 0, sizeof(char)*MAX_STR_BUFFER);
	memset(buffer2, 0, sizeof(char)*MAX_STR_BUFFER);
	if (recvData(buffer))
		return 1;
	if (recvData(buffer2))
		return 1;
	//cout << buffer << endl;
	strcat(buffer,buffer2);
	ClearSocket();
	if (strstr(buffer, "HTTP/1.0 500 INTERNAL SERVER ERROR") != NULL)
	{
		cout << "Server Error!" << endl;
		cout << buffer << endl;
		return 1;
	}
	char *p = strstr(buffer,"\r\n\r\n");
	if (p == NULL)
		return 1;
	switch (*(p + 4))
	{
	case 'W': case 'w':isBlack = false; return 0; break;
	case 'B': case 'b':isBlack = true; return 0; break;
	default:return 1;
	}
}

int PlayerServer::Move(unsigned int sessionId, unsigned char x, unsigned char y, bool isBlack, bool &illegal)
{
	InitSocket();
	sprintf(buffer,"POST /move/%d/%d/%d/%c HTTP/1.0\r\n\r\n ",sessionId,(int)x,(int)y,((isBlack)?'B':'W'));
	if (sendData(buffer))
		return 1;
	memset(buffer, 0, sizeof(char)*MAX_STR_BUFFER);
	memset(buffer2, 0, sizeof(char)*MAX_STR_BUFFER);
	if (recvData(buffer))
		return 1;
	if (recvData(buffer2))
		return 1;
	//cout << buffer << endl;
	strcat(buffer, buffer2);
	//cout << buffer << endl;
	ClearSocket();
	illegal = false;
	if (strstr(buffer, "HTTP/1.0 500 INTERNAL SERVER ERROR") != NULL)
	{
		cout << "Server Error!" << endl;
		cout << buffer << endl;
		return 1;
	}
	char *p = strstr(buffer, "\r\n\r\n");
	if (p == NULL)
		return 1;
	if (!strcmp(p+4, "SUCCESS"))
		return 0;
	if (!strcmp(p + 4, "ERROR"))
	{
		cout << "Illegal movement!" << endl;
		illegal = true;
	}
	return 1;
}

int PlayerServer::Board(unsigned int sessionId, unsigned long long & occ, unsigned long long & pla)
{
	occ = pla = 0LL;
	InitSocket();
	sprintf(buffer, "GET /board_string/%d HTTP/1.0\r\n\r\n", sessionId);
	if (sendData(buffer))
		return 1;
	memset(buffer, 0, sizeof(char)*MAX_STR_BUFFER);
	memset(buffer2, 0, sizeof(char)*MAX_STR_BUFFER);
	if (recvData(buffer))
		return 1;
	if (recvData(buffer2))
		return 1;
	//cout << buffer << endl;
	strcat(buffer, buffer2);
	
	ClearSocket();
	if (strstr(buffer, "HTTP/1.0 500 INTERNAL SERVER ERROR") != NULL)
	{
		cout << "Server Error!" << endl;
		cout << buffer << endl;
		return 1;
	}
	char *p = strstr(buffer, "\r\n\r\n");
	if (p == NULL)
		return 1;
	p += 4;
	for (int pos = 0; pos < 64; pos++)
	{
		switch (*p)
		{
		case '0':p+=2; break;
		case 'W':p+=2; SetPosition(occ, pla, pos / 8, pos % 8, false); break;
		case 'B':p+=2; SetPosition(occ, pla, pos / 8, pos % 8, true); break;
		default:p+=2; break;
		}
	}
	//PrintBoard(occ,pla);
	return 0;
}

int PlayerServer::Turn(unsigned int sessionId, bool &isBlack)
{
	InitSocket();
	sprintf(buffer, "GET /turn/%d HTTP/1.0\r\n\r\n", sessionId);
	if (sendData(buffer))
		return 1;
	memset(buffer, 0, sizeof(char)*MAX_STR_BUFFER);
	memset(buffer2, 0, sizeof(char)*MAX_STR_BUFFER);
	if (recvData(buffer))
		return 1;
	if (recvData(buffer2))
		return 1;
	//cout << buffer << endl;
	strcat(buffer, buffer2);
	ClearSocket();
	if (strstr(buffer, "HTTP/1.0 500 INTERNAL SERVER ERROR") != NULL)
	{
		cout << "Server Error!" << endl;
		cout << buffer << endl;
		return 1;
	}
	char *p = strstr(buffer, "\r\n\r\n");
	if (p == NULL)
		return 1;
	switch (*(p + 4))
	{
	case 'W': case 'w':isBlack = false; return 0; break;
	case 'B': case 'b':isBlack = true; return 0; break;
	default:return 1;
	}
}

int PlayerServer::Status(unsigned int sessionId, bool & onGoing, bool & blackWon)
{
	InitSocket();
	sprintf(buffer, "GET /status/%d HTTP/1.0\r\n\r\n", sessionId);
	if (sendData(buffer))
		return 1;
	memset(buffer, 0, sizeof(char)*MAX_STR_BUFFER);
	memset(buffer2, 0, sizeof(char)*MAX_STR_BUFFER);
	if (recvData(buffer))
		return 1;
	if (recvData(buffer2))
		return 1;
	//cout << buffer << endl;
	strcat(buffer, buffer2);
	ClearSocket();
	if (strstr(buffer, "HTTP/1.0 500 INTERNAL SERVER ERROR") != NULL)
	{
		cout << "Server Error!" << endl;
		cout << buffer << endl;
		return 1;
	}
	char *p = strstr(buffer, "\r\n\r\n");
	if (p == NULL)
		return 1;
	onGoing = false;
	if (!strcmp(p + 4, "ongoing"))
	{
		onGoing = true;
		return 0;
	}
	if (!strcmp(p + 4, "Black has won"))
	{
		blackWon = true;
		return 0;
	}
	if (!strcmp(p + 4, "White has won"))
	{
		blackWon = false;
		return 0;
	}
		
	return 1;
}

int PlayerServer::ChangeColor(unsigned int sessionId)
{
	InitSocket();
	sprintf(buffer, "GET /change_color/%d HTTP/1.0\r\n\r\n", sessionId);
	if (sendData(buffer))
		return 1;
	memset(buffer, 0, sizeof(char)*MAX_STR_BUFFER);
	memset(buffer2, 0, sizeof(char)*MAX_STR_BUFFER);
	if (recvData(buffer))
		return 1;
	if (recvData(buffer2))
		return 1;
	//cout << buffer << endl;
	strcat(buffer, buffer2);
	ClearSocket();
	if (strstr(buffer, "HTTP/1.0 500 INTERNAL SERVER ERROR") != NULL)
	{
		cout << "Server Error!" << endl;
		cout << buffer << endl;
		return 1;
	}
	/*
	char *p = strstr(buffer, "\r\n\r\n");
	if (!strcmp(p + 4, "SUCCESS"))
		return 0;
	return 1;
	*/
	return 0;
}
