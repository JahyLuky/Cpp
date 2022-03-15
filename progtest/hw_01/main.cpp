#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;




/*
void createTree ( vector <bool> & data )
{
  tree * node;
  //node->left = node->right = nullptr;
  
  int len = data.size(), flag = 0, cnt = 0;
  vector <bool> znak;
  for ( int i = 0; i < 10; i++ )
  {
    if ( flag == 0 )
    {
      if ( data[i] == 0 )
      {
        //cout << "nula" << endl;
      }
      else
      {
        flag = 1;
      }
    }
    else
    {
      // gets character to save in leaf
      znak.push_back( data[i] );
      if ( cnt == 7 )
      {
        printDataBin(znak);
        char a = getChar(znak);
        flag = 0;
        cnt = 0;
      }
      else
      {
        cnt++;
      }
    }
  }
  printDataBin(znak);
}
*/

char getChar ( vector <bool> & data )
{
  char myChar;
  int val = 0, pow = 2, len = data.size()-2;
  val += data[7];
  cout << "len " << len << endl;
  for ( int i = len; i >= 0; i-- )
  {
    val += data[i] * pow;
    pow *= 2;
  }

  myChar = val;
  return myChar;
}

int main ( void )
{
    vector <bool> data = { 0,1,1,0,1,1,1,1 };
    char a = getChar ( data );
    cout << a << endl;
    return 0;
}