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
		ctl.dgptr[i]->draw(ctl.usingData[i],controller::maxItem);
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
	listen(sockfd,5);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	int n;

	// get ip address
	ctl.ipAddr[id] = string(inet_ntoa(cli_addr.sin_addr))+":";

	bool isReg = false;
	cout << "connected " << ctl.ipAddr[id] << endl;
	while( 1 )
	{
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		string tmp( buffer , buffer+n );

		pthread_mutex_lock(&mutex);
		ctl.rcv[id].updateInfo(tmp);

		if( !isReg )
		{
			isReg = ctl.autoRegister(id);
			ctl.trySetTitle();
		}

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

/*  main function  */
int main(int argc, char** argv)
{
	const int width = 400;
	const int h = 150;
	pthread_t datareceiver[4];
	pthread_mutex_init(&mutex, NULL);

	ctl.initDg( 50, 50,width,h,0,0); //x=50 y=50 width=400 height=150
	ctl.initDg( 50,300,width,h,0,1); 
	ctl.initDg(550, 50,width,h,1,0); 
	ctl.initDg(550,300,width,h,1,1); 
	ctl.setData(0,1,0,3);
	ctl.setData(0,0,0,6);
	ctl.setData(1,0,0,9);

	int a1=0 , a2=1 , a3=2 , a4=3;
	pthread_create(&datareceiver[0], NULL, run, (void *)&a1);
	//pthread_create(&datareceiver[1], NULL, run, (void *)&a2);
	//pthread_create(&datareceiver[2], NULL, run, (void *)&a3);
	//pthread_create(&datareceiver[3], NULL, run, (void *)&a4);

	/*  init opengl (glu,glut...)  */
	glutInit(&argc, argv);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(1000,500);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutCreateWindow("TSCC2015");
	glClearColor(0.0,0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,1000,0,500);
	glutIdleFunc(Repaint);
	glutDisplayFunc(Draw);
	glutMainLoop(); //nerver return
	pthread_mutex_destroy(&mutex);
	return 0;
}

