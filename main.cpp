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
#include "diagram2_plus.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "controlpanel.h"
// collectl -A 127.0.0.1:4000 -scm -P


void error(const char *msg)
{
	perror(msg);
	exit(1);
}


/*  diagrams dataptr  */
std::vector<diagram2_plus *> diagrams_plus;
controlpanel panel;
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
    for(int i=0 ; i<diagrams_plus.size() ; i++)
        diagrams_plus.at(i)->draw();
    panel.draw();
    pthread_mutex_unlock(&mutex);
    glutSwapBuffers();

}
/*  critical sectiong of comuuter1 input  */
void* run(void* Diagram)
{
	diagram2_plus *diagram = (diagram2_plus*)Diagram;
	int sockfd, newsockfd, portno;
	printf("set port:%d\n",diagram->port);
	portno = diagram->port;
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
	puts("binding");
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	puts("listening");
	listen(sockfd,5);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
    int n;
    diagram->setRange(100);
    diagram->setvistiable(true);
	diagram->address = string(inet_ntoa(cli_addr.sin_addr))+":";
	while( 1 )
	{
        pthread_mutex_lock(&mutex);
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		string tmp( buffer , buffer+n );
        diagram->update(tmp);
		pthread_mutex_unlock(&mutex);
		usleep(50000);
	}

	close(newsockfd);
	close(sockfd);

	pthread_exit(NULL);
	return NULL;
}

void keyPressed (unsigned char key, int x, int y)
{
    int s;
    float f;
    diagram2_plus *diagrams_plus_ptr = panel.SELECT_Diagram;
    if(diagrams_plus_ptr==NULL)
    {
        return;
    }
    if(key==panel.HOTKEY_NextDiagram)
    {
        int index = panel.getindex();
        cout<<"set index "<<index<<" to "<<(index+1)<<endl;
        index++;
        if(index<diagrams_plus.size())
        {
            cout<<"success to "<<index<<endl;
            panel.setindex(index);
            panel.SELECT_Diagram = diagrams_plus.at(index);
            cout<<"x="<<panel.SELECT_Diagram->getx()<<"y="<<panel.SELECT_Diagram->gety()<<endl;
        }
        else
        {
            cout<<"unsuccess"<<endl;
        }
    }
    if(key==panel.HOTKEY_LastDiagram)
    {
        int index = panel.getindex();
        cout<<"set index "<<index<<" to "<<(index-1)<<endl;
        index--;
        if(index>=0)
        {
            cout<<"success to "<<index<<endl;
            panel.setindex(index);
            panel.SELECT_Diagram = diagrams_plus.at(index);
            cout<<"x="<<panel.SELECT_Diagram->getx()<<"y="<<panel.SELECT_Diagram->gety()<<endl;
        }
        else
        {
            cout<<"unsuccess"<<endl;
        }
    }
    if(key==panel.HOTKEY_NextColume)
    {
        s = diagrams_plus_ptr ->getselectcolumn();
        cout<<"set chosen "<<s<<" to "<<(s+1)<<endl;
        if( diagrams_plus_ptr ->selectcolumn(s+1) )
            cout<<"success"<<endl;
        else
            cout<<"unsuccess"<<endl;
    }
    if(key==panel.HOTKEY_LastColume)
    {
        s = diagrams_plus_ptr ->getselectcolumn();
        cout<<"set chosen "<<s<<" to "<<(s-1)<<endl;
        if( diagrams_plus_ptr ->selectcolumn(s-1) )
            cout<<"success"<<endl;
        else
            cout<<"unsuccess"<<endl;
    }
    if(key==panel.HOTKEY_IncreaseMaxvalue)
    {
        f = diagrams_plus_ptr ->getmaxvalue();
        cout<<"set maxvalue "<<f<<" to "<<(f*10)<<endl;
        diagrams_plus_ptr ->setmaxvalue(f*10);
    }
    if(key==panel.HOTKEY_DecreaseMaxvalue)
    {
        f = diagrams_plus_ptr ->getmaxvalue();
        cout<<"set maxvalue "<<f<<" to "<<(f/10)<<endl;
        diagrams_plus_ptr ->setmaxvalue(f/10);
    }
}

/*  main function  */
int main(int argc, char** argv)
{
    	pthread_t datareceiver[4];
    	pthread_mutex_init(&mutex, NULL);
    	int r;
    	/*  create diagram  */
        diagram2_plus *diagramptr1 = new diagram2_plus(50,50,400,150); //x=50 y=50 width=400 height=150
        diagramptr1->port=4000;
        r = pthread_create(&datareceiver[0], NULL, run, (void *)diagramptr1);
        diagrams_plus.push_back(diagramptr1);
    	/*  create diagram  */
        diagram2_plus *diagramptr2 = new diagram2_plus(50,300,400,150); //x=50 y=50 width=400 height=150
        diagramptr2->port=4001;
        r = pthread_create(&datareceiver[1], NULL, run, (void *)diagramptr2);
        diagrams_plus.push_back(diagramptr2);
    	/*  create diagram  */
        diagram2_plus *diagramptr3 = new diagram2_plus(550,50,400,150); //x=50 y=50 width=400 height=150
        diagramptr3->port=4002;
        r = pthread_create(&datareceiver[2], NULL, run, (void *)diagramptr3);
        diagrams_plus.push_back(diagramptr3);
    	/*  create diagram  */
        diagram2_plus *diagramptr4 = new diagram2_plus(550,300,400,150); //x=50 y=50 width=400 height=150
        diagramptr4->port=4003;
        r = pthread_create(&datareceiver[3], NULL, run, (void *)diagramptr4);
        diagrams_plus.push_back(diagramptr4);
        //default selected
        panel.setindex(0);
        panel.SELECT_Diagram = diagrams_plus.at(0);
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
    	glutKeyboardFunc(keyPressed);
    	glutMainLoop(); //nerver return
    	pthread_mutex_destroy(&mutex);
	return 0;
}

