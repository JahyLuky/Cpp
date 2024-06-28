#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;


void foo ( const char * a )
{
    a += 2;
}
void bar ( const char * & a )
{
    a ++;
    foo ( a );
    a ++;
}


int main(void) {
    const char * x = "I am C++. Resistance Is Futile. You Will Be Assimilated.";
    bar ( x );
    cout << x << endl;

    return 1;
}