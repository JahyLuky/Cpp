#ifndef __PROGTEST__
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
#endif /* __PROGTEST__ */


struct Data
{
  vector <bool> m_data;
};

struct tree
{
  struct tree * left;
  struct tree * right;
  char character;
};


void printDataBin ( const vector <bool> & data )
{
  int len = data.size();
  for ( int i = 0; i < len; i++)
  {
    cout << data[i];
  }
  cout << '\n' << "----------------" << endl;
}

tree huffCode ( tree code ,vector <bool> & data, int i = 0 )
{
  code.left = code.right = nullptr;
  
  int len = data.size();
  for ( ; i < len; i++ )
  {
    if ( data[i] == 0 )
    {
      code.left = huffCode( code.left->left, data, i );
    }
    else
    {
      //8bits to char
    }
  }
  
}


bool binDump( const char * fileName )
{
  ifstream ifs ( fileName, ios::in | ios::binary );
  
  if ( !ifs )
    return false;


  Data data;
  //read every char from stream and convert it to bits
  for ( char c; ifs.get( c ); )
  {
    if ( ifs.fail() )
      return false;
    
    //c = (unsigned char)c;
    //converting to bits
    for( int i = 7; i >= 0; i-- )
    {
      data.m_data.push_back(( ( c >> i ) & 1 ));
    }
  }

  printDataBin( data.m_data );

  tree code;
  code = huffCode( code, data.m_data );

  return true;
}

bool decompressFile ( const char * inFileName, const char * outFileName )
{
  // todo
  return false;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
  // keep this dummy implementation (no bonus) or implement the compression (bonus)
  return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
  ifstream a ( fileName1, ios::binary );
  ifstream b ( fileName2, ios::binary );

  if ( !a || !b )
    return false;

  a.seekg(0, ios::end);
  int file_size1 = a.tellg();

  b.seekg(0, ios::end);
  int file_size2 = b.tellg();

  if ( file_size1 != file_size2 )
    return false;

  char c_a, c_b;
  for ( ; a.get( c_a ); )
  {
    b.get( c_b );

    if ( a.fail() || b.fail() )
      return false;

    if ( c_a != c_b )
    {
      return false;
    }
  }
  
  cout << "identicalFiles" << endl;
  return true;
}

int main ( void )
{
  binDump("tests/test0.huf");
  //assert ( identicalFiles ( "bbb.txt", "aaa.txt" ) );
  /*
  assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );

  */

  return 0;
}
#endif /* __PROGTEST__ */
