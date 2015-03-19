#include "controller.h"

controller :: controller()
{
	for( int i = 0 ; i < DgSize ; ++i )
	{
		title[i] = "Not Set";
		usingCol[i] = -1;
		usingID[i] = -1;
		usingData[i] = NULL;
	}
}

void controller :: initDg(int boundx,int boundy,int width,int height,int x,int y)
{
	if( !( 0 <= x && x < maxDgX && 0 <= y && maxDgY ) )
	{
		puts("init diagram error at x,y pos");
		return ;
	}
	
	dgptr[conv(x,y)] = new diagram2(boundx,boundy,width,height); //x=50 y=50 width=400 height=150
	dgptr[conv(x,y)]->setvistiable(true);
	dgptr[conv(x,y)]->settitle( &title[conv(x,y)] );
		
}

void controller :: setData(int x,int y,int id,int col)
{
	if( col == 0 )
	{
		cout << "error:col 0 is machine name" << endl;
		return;
	}
	int index = conv(x,y);
	usingData[index] = &(data[id][col]);
	usingCol[index] = col;
	usingID[index] = id;
	char chs[100] = {};
	sprintf( chs , "Set to ID:%d\tcol:%d\n" , id , col );
	string tmp(chs);
	title[index] = tmp;
}

void controller :: trySetTitle()
{
	for( int i = 0 ; i < DgSize ; ++i )
	{
		if( usingCol[i] == -1 || usingID[i] == -1 )
			continue;

		auto cols = rcv[usingID[i]].getColumns();
		if( usingCol[i] < cols.size() )
			title[i] = ipAddr[usingID[i]] + " " + cols[usingCol[i]];
		else
			title[i] = ipAddr[usingID[i]] + " Listening";
	}
}

bool controller :: autoRegister(int id) 
{
	auto cols = rcv[id].getColumns();
	bool res = false;
	for( int i = 1 ; i < cols.size() ; ++i )
	{
		res = true;
		rcv[id].registerItem( &data[id][i] , i , 100 );	
	}

	return res;
}
