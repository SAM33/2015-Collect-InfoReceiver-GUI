#include "diagram2_plus.h"

    void  diagram2_plus::autoregister()
    {
        //立即依照Headers中記載的colums數量來把相等數量的deque註冊到infoReceiver中
        int csize = columnName.size();
        for(int i=0 ; i<csize ; i++)
        {
                registerInfoReceiver( &ques[i] , i , maxSize_X );
        }
        //將chosen註冊到圖表
       selectcolumn(chosen);
        //Success
        hasAutoRegister=true;
        //cout<<"autoregister success"<<endl;
    }
    void  diagram2_plus::registerInfoReceiver(deque<string> *que ,int col ,int max)
    {
        maxSize[col] = max;
        monitorVec[col] = que;
    }
    void  diagram2_plus::registerdiagram(deque<string> * _dequeptr,int _maxitem)
    {
        dequeptr = _dequeptr;
        maxitem = _maxitem;
    }

    int  diagram2_plus::getselectcolumn()
    {
        return chosen;
    }

    bool  diagram2_plus::selectcolumn(int _chosen)
    {
        int csize = columnName.size();
        if(_chosen<csize && _chosen>=3)
        {
            chosen = _chosen;
            //將chosen註冊到圖表
            registerdiagram( &ques[chosen],maxSize_X);
            settitle(address+columnName.at(chosen));
            return true;
        }
        return false;
    }


    diagram2_plus::diagram2_plus(int _x,int _y,int _width,int _height) :diagram2( _x, _y, _width, _height)
    {
        chosen = 3;  //Default
        hasHeader = false;
        hasAutoRegister = false;
        maxSize_X = 10;
    }

    void diagram2_plus::update(string tmp)
    {
		updateInfo(tmp);
		sync();
        //cout<<"update success"<<endl;
    }
    void diagram2_plus::setRange(int _maxSize_X)
    {
        maxSize_X = _maxSize_X;
        //cout<<"setRange( success"<<endl;
    }

    int diagram2_plus::updateInfo( string str )
    {
        if(hasHeader && !hasAutoRegister)
        {
            autoregister();
        }
        infoReceiver::updateInfo(str);
        //cout<<"updateInfo success"<<endl;
    }
