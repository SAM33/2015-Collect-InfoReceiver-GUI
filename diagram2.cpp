#include "diagram2.h"
//#define ENABLE_DEBUG
void diagram2 :: drawtext(float x, float y,string text, int size)
{
	glRasterPos2f(x, y);
	switch(size)
	{
		case TEXT_SMALL:
			glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char *)text.c_str());
			break;

		case TEXT_MIDDLE:
			glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)text.c_str());
			break;

		case TEXT_LARGE:
			glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)text.c_str());
			break;

		default:
			glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)text.c_str());
	}
}

void diagram2 :: drawline(float X1, float Y1, float X2, float Y2)
{
	glBegin(GL_LINES);
	glVertex2d(X1, Y1);
	glVertex2d(X2, Y2);
	glEnd();
}

diagram2 :: diagram2(int _x,int _y,int _width,int _height)
{
	x = _x;
	y = _y;
	width = _width;
	height = _height;
	vistiable = false;
	maxvalue = 100;
	maxitem = 10;
	title = NULL;
}

void diagram2 :: setvistiable(bool _vistiable)
{
	vistiable = _vistiable;
}

void diagram2 :: settitle(string *_title)
{
	title = _title;
}

void diagram2 :: setmaxvalue(float _maxvalue)
{
	if( _maxvalue>=1)
	{
		maxvalue = _maxvalue;
	}
}

float diagram2 :: getmaxvalue()
{
	return maxvalue;
}

int diagram2 ::getx()
{
	return x;
}

int diagram2 ::gety()
{
	return y;
}

int diagram2 ::getheight()
{
	return height;
}

int diagram2 ::getwidth()
{
	return width;
}

void diagram2 :: draw(deque<float> *dequeptr , int _maxitem)
{
	maxitem = _maxitem;
	glColor3f(0,1,0);
	if( title!=NULL )drawtext(x,y+height+20,*title,TEXT_LARGE);
	drawGrid();
	/* 畫出資料 */
	if(dequeptr==NULL)
	{
		//cout<<"no data to draw"<<endl;
		return;
	}

	glColor3f(1,0,0); //R G B
	int size = dequeptr->size();
	if(size<2) //無資料時,或是資料不足連成一條線時
	{
		//cout<<"too few data to draw"<<endl;
		return;
	}

	if(size<=maxitem && size>=2) //一般情況
	{

		maxvalue = *std::max_element( dequeptr->begin() , dequeptr->end() );
		float X1,X2,Y1,Y2;
		float Value1,Value2;
		for(int index=0 ; index<size-1 ; index++)
		{
			Value1=dequeptr->at(index);
			Value2=dequeptr->at(index+1);
			/* 第一個繪圖點 */
			float X1 = x+(float)((float)index/(float)maxitem)*(float)width;
			float Y1 = y+(float)((float)Value1/(float)maxvalue)*(float)height;
			/* 第二個繪圖點 */
			float X2 = x+(float)((float)(index+1)/(float)maxitem)*(float)width;
			float Y2 = y+(float)((float)Value2/(float)maxvalue)*(float)height;
			/* 畫出相連的線 */
			drawline(X1,Y1,X2,Y2);
		}
	}
	else	//發生錯誤,註冊近來的deque和x軸的顯示框對不上
	{
		cout<<"Error on diagram2::draw"<<endl;
		return;
	}
}

int diagram2 :: drawGrid()
{
	char temp[20];
	/* 畫出X軸 */
	glColor3f(1,1,1); //R G B
	drawline(x,y,x+width,y);
	for(int i=1 ; i<=maxitem ; i++)
	{
		if(maxitem>=20)
		{
			if(i%((int)maxitem/20)==0)
			{
				float X = x+(float)((float)i/(float)maxitem)*(float)width;
				drawline(X,y+5,X,y-5);
				bzero(temp,20);
				sprintf(temp,"%d",i);
				drawtext(X-5,y-20,string(temp),TEXT_SMALL);
			}
		}
		else
		{
			float X = x+(float)((float)i/(float)maxitem)*(float)width;
			drawline(X,y+5,X,y-5);
			bzero(temp,20);
			sprintf(temp,"%d",i);
			drawtext(X-5,y-20,string(temp),TEXT_SMALL);
		}
	}

	/* 畫出Y軸 */
	glColor3f(1,1,1); //R G B
	drawline(x,y,x,y+height);
	for(int i=1 ; i<=maxvalue ; i++)
	{
		if(maxvalue>10)
		{
			if(i%((int)maxvalue/10)==0)
			{
				float Y = y+(float)((float)i/(float)maxvalue)*(float)height;
				drawline(x+5,Y,x-5,Y);
				bzero(temp,20);
				sprintf(temp,"%d",i);
				drawtext(x-20,Y-5,string(temp),TEXT_SMALL);
			}
		}
		else
		{
			float Y = y+(float)((float)i/(float)maxvalue)*(float)height;
			drawline(x+5,Y,x-5,Y);
			bzero(temp,20);
			sprintf(temp,"%d",i);
			drawtext(x-20,Y-5,string(temp),TEXT_SMALL);
		}
	}

	return 0;
}

