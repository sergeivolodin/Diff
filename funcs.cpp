#include <iostream>
using namespace std;
bool is_integer(const float a) {
    int temp=static_cast<int>(a);
    if(temp==a) {
        return(true);
    }
    return(false);
}
