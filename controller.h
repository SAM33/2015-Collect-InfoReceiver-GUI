#ifndef CON_H
#define CON_H

#define DgSize maxDgX*maxDgY
#define conv(x,y) x*maxDgX+y

#include "infoReceiver.h"
#include "diagram2.h"

class controller {
	public:
		static const int maxDgX = 2;
		static const int maxDgY = 2;
		static const int nodeNum = 4;
		static const int startPort = 4000;
		static const int maxItem = 100;

		diagram2 *dgptr[DgSize];	
		string title[DgSize];
		int usingCol[DgSize];
		int usingID[DgSize];
		deque<float> *usingData[DgSize];
		
		infoReceiver rcv[nodeNum];
		string ipAddr[nodeNum];
	
		map<int,deque<float>> data[nodeNum];
		controller();
		void initDg(int,int,int,int,int,int);
		void setData(int,int,int,int);

		// call it to set title
		void trySetTitle();

		bool autoRegister(int);
	private:
};

#endif
