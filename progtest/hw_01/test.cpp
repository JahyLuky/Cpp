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


struct tree
{
  struct tree * left;
  struct tree * right;
  char data;
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


// convert 8 bits to int to char
char getChar ( vector <bool> & data )
{
  char myChar;
  int val = 0, pow = 2, len = data.size()-2;
  val += data[7];
  for ( int i = len; i >= 0; i-- )
  {
    val += data[i] * pow;
    pow *= 2;
  }
  myChar = val;
  //cout << myChar;
  return myChar;
}

struct tree * newNode( void )
{
	struct tree*  tmp = ( struct tree * ) malloc( sizeof(struct tree) );
	tmp->right = NULL;
	tmp->left = NULL;
  return tmp;
}

// tree*&
struct tree * createTree ( vector <bool> & data, int getPos, struct tree * node )
{
  int pos = getPos;
  vector <bool> oneChar;
  int len = data.size();
  if ( pos >= len )
    return node;
  
  if ( data[pos] == 0 )
  {
    pos++;
    node->left = newNode();
    node->right = newNode();
    node->left = createTree( data, pos, node->left );
    node->right = createTree( data, pos, node->right );
  }
  else
  {
    pos++;
    int newPos = pos + 7;
    cout << pos << " " << newPos << endl;
    if ( newPos >= len )
      return node;
    // gets next 8 bit for ASCII
    for ( int j = pos; j <= newPos; j++ )
    {
      oneChar.push_back( data[j] );
    }
    node->data = getChar ( oneChar );
    cout << node->data << endl;
    pos = newPos + 1;
  }

  return node;
}

bool binDump ( const char * fileName )
{
  ifstream ifs ( fileName, ios::in | ios::binary );
  
  if ( !ifs || !ifs.is_open() )
    return false;

  vector <bool> data;
  //read every char from stream and convert it to bits
  for ( char c; ifs.get( c ); )
  {
    if ( ifs.fail() )
      return false;
    
    //c = (unsigned char)c;
    //converting to bits
    for( int i = 7; i >= 0; i-- )
    {
      data.push_back(( ( c >> i ) & 1 ));
    }
  }

  printDataBin( data );

  struct tree * head = NULL;
  head = newNode();
  int pos = 0;
  head = createTree( data, pos, head );
  
  ifs.close();
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

  a.close();
  b.close();
  if ( !b.good() )
    return false;
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
