#include "drawing.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <thread>
#include "snake_game.h"

const int SERVER_PORT = 10000;
const int NOTIFICATION_PORT = 10001;
const char* SERVER_ADDRESS = "127.0.0.1";
const int EULA_LEN = 1024;

static char eula[EULA_LEN];
static void(*packetHandler)();
static int randomSeed;

bool shouldQuit = false;

void handleNotificationPacket();
void notificationThread_Main(int socketHandle);
void inputThread_Main();

SnakeGame* currentGame;

int main()
{	
    memset(eula,0,EULA_LEN);
    randomSeed = 42;
    packetHandler = handleNotificationPacket;

	int eulaSocket;
	int notificationSocket;

	{
		struct sockaddr_in sockAddr = {0};
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(SERVER_PORT);
		inet_pton(AF_INET, SERVER_ADDRESS, &sockAddr.sin_addr);
		eulaSocket = socket(AF_INET, SOCK_STREAM, 0);

		int err = connect(eulaSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
		if (err< 0)
		{
			printf("Error connecting to eula server: %s\n", strerror(errno));
			exit(1);
		}
	}

	{
		struct sockaddr_in sockAddr = {0};
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(NOTIFICATION_PORT);
		inet_pton(AF_INET, SERVER_ADDRESS, &sockAddr.sin_addr);
		notificationSocket = socket(AF_INET, SOCK_STREAM, 0);

		int err = connect(notificationSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
		if (err< 0)
		{
			printf("Error connecting to notification server: %s\n", strerror(errno));
			exit(1);
		}
	}

    char buff[EULA_LEN*2];
    recv(eulaSocket, &buff, EULA_LEN*2, 0);
    strcpy(eula, buff);

	//once we have eula, start notification thread, and start rendering the game (eula screen first)
	std::thread notificationThread(&notificationThread_Main, notificationSocket);
	notificationThread.detach();
	
	printf("Welcome To Snake. Press any key to continue.\n");
	getc(stdin);

	printf("%s\n", &eula[0]);
	printf("Press any key to accept and continue.\n");
	getc(stdin);

	WindowSize winSize = initializeWindow();
	currentGame = new SnakeGame(winSize.w,winSize.h, randomSeed);

	//has to be below the initWindow call since that handles calling nodelay()
	std::thread inputThread(&inputThread_Main);
	inputThread.detach();


	while (!shouldQuit)
	{
		currentGame->tick();
		drawGame(currentGame);
	}

	shutdownDrawing();
	

	return 0;
}

void inputThread_Main()
{
	while(1)
	{
		char input = getc(stdin);
		if (currentGame)
		{
			currentGame->handleInput(input);
		}

		if (input == ':' || currentGame->currentMode == GAMEOVER) 
		{
			shouldQuit = true;
			break;
		}
	}
}

void notificationThread_Main(int socketHandle)
{
	while(1)
	{       
		char buff[1];
        recv(socketHandle, &buff, 1, 0);
        (*packetHandler)();
        break;
	}
}

void handleNotificationPacket()
{
    printf("Notification Received\n");
}