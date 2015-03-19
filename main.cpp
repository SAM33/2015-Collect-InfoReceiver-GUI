//NTCU TSCC TEAM , 2015.03
#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <vector>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "infoReceiver.h"
#include "diagram2.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "controller.h"
#define MS 1000
// collectl -A 127.0.0.1:4000 -scm -P

controller ctl;

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

/* mutex */
pthread_mutex_t mutex;
/*  this function will automatic call Draw  */
static void Repaint()
{
	glutPostRedisplay();
}
/*  opengl critical sectiong  */
static void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	pthread_mutex_lock(&mutex);
	for( int i = 0 ; i < controller::maxDgX * controller::maxDgY ; ++i )
	{
		ctl.dgptr[i]->draw(ctl.usingData[i],controller::maxItem);
	}
	pthread_mutex_unlock(&mutex);
	glutSwapBuffers();
}

// thread only connect with client and receiver their data
void* run(void* param)
{
	int sockfd, newsockfd, portno;

	//custom
	int id = *((int*)param);	
	portno = controller::startPort + id;

	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	clilen = sizeof(cli_addr);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	ctl.connect_state[id] = STE_LISTEN;
	listen(sockfd,5);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	int n;

	// get ip address
	ctl.ipAddr[id] = string(inet_ntoa(cli_addr.sin_addr))+":";

	bool isReg = false;
	cout << "connected " << ctl.ipAddr[id] << endl;
	ctl.connect_state[id] = STE_CONNECT;
	while( 1 )
	{
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) {
			 ctl.connect_state[id] = STE_DISCONNECT;
			 error("ERROR reading from socket");
		}
		string tmp( buffer , buffer+n );

		ctl.rcv[id].updateInfo(tmp);

		if( !isReg )
		{
			isReg = ctl.autoRegister(id);
			ctl.trySetTitle();
		}

		pthread_mutex_lock(&mutex);
		int comingSize = ctl.rcv[id].sync();
		pthread_mutex_unlock(&mutex);

		if( comingSize == 0 )
			usleep(200*MS);
		else
			usleep(900*MS);
	}

	close(newsockfd);
	close(sockfd);

	pthread_exit(NULL);
	return NULL;
}

void* key(void* param)
{
	int n;
	int dgID;
	int rcvID;
	int col;
	while(1)
	{
		do {
			puts("choose diagram:");
		} while( 1 != scanf(" %d",&dgID) );
		
		if( dgID >= controller::maxDgX * controller::maxDgY )
		{
			puts("error diagram ID");
			continue;
		}

		do {
			puts("choose node:");
		} while( 1 != scanf(" %d",&rcvID) );
		
		if( rcvID >= controller::nodeNum )
		{
			puts("error node ID");
			continue;
		}

		if( ctl.connect_state[rcvID] != STE_CONNECT )
		{
			puts("not connecting");
			continue;
		}

		do {
			ctl.printColumns(rcvID);
			puts("choose col:");
		} while( 1 != scanf(" %d",&col) );

		if( col >= ctl.rcv[rcvID].getColumns().size() )
		{
			puts("error column ID");
			continue;
		}

		ctl.setData( dgID/controller::maxDgX , dgID%controller::maxDgY , rcvID , col );
		ctl.trySetTitle();
	}
	pthread_exit(NULL);
	return NULL;
}

/*  main function  */
int main(int argc, char** argv)
{
	const int w = 400;
	const int h = 150;
	const int xB = 80;
	const int yB = 80;
	const int xN = controller::maxDgX;
	const int yN = controller::maxDgY;
	const int xT = 10 + xN*(xB+w);
	const int yT = 10 + yN*(yB+h);

	pthread_t datareceiver[4];
	pthread_mutex_init(&mutex, NULL);

	for( int x = 0 ; x < xN ; ++x )
		for( int y = 0 ; y < yN ; ++y )
			ctl.initDg( 40 + x*(xB+w) , 40 + y*(yB+h) ,w,h,x,y); 

	int a1=0 , a2=1 , a3=2 , a4=3;
	pthread_create(&datareceiver[0], NULL, run, (void *)&a1);
	pthread_create(&datareceiver[1], NULL, key, (void *)&a2);
	//pthread_create(&datareceiver[1], NULL, run, (void *)&a2);
	//pthread_create(&datareceiver[2], NULL, run, (void *)&a3);
	//pthread_create(&datareceiver[3], NULL, run, (void *)&a4);

	/*  init opengl (glu,glut...)  */
	glutInit(&argc, argv);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(xT,yT);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutCreateWindow("TSCC2015");
	glClearColor(0.0,0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,xT,0,yT);
	glutIdleFunc(Repaint);
	glutDisplayFunc(Draw);
	glutMainLoop(); //nerver return
	pthread_mutex_destroy(&mutex);
	return 0;
}

