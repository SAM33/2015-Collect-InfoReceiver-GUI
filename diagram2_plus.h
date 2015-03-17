#ifndef DIAGRAM2_H_PLUS
#define DIAGRAM2_H_PLUS
#include "diagram2.h"
#include "infoReceiver.h"
#include <map>
using namespace std;
class diagram2_plus : public diagram2 , public infoReceiver
{
    public:
    string address;
    diagram2_plus(int _x,int _y,int _width,int _height);
    void update(string tmp);
    void setRange(int _maxSize_X);
    bool selectcolumn(int _chosen);
    int updateInfo( string str );
    int getselectcolumn();

    private:
    int maxSize_X;
    map<int, deque<string>> ques;
    int chosen;
    bool hasAutoRegister;
    void registerdiagram(deque<string> *que,int _maxitem);
    void registerInfoReceiver(deque<string> *que ,int col ,int max);
    void autoregister();
};

#endif // DIAGRAM2_H_PLUS
