#ifndef UTILS_H
#define UTILS_H

#include <vector>

using namespace std;

class Utils
{
public:
    Utils();

    template<typename T>
    static void permut(T& x, T& y){
        T tmpX;
        tmpX = x;
        T tmpY;
        tmpY = y;
        x = tmpY;
        y = tmpX;
    }

    static int randInt(int low, int high);
};

#endif // UTILS_H
