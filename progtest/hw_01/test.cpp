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
  int m_size;
};

struct tree
{
  tree * left;
  tree * right;
  char data;
};

tree * newNode( void )
{
	tree * node = new tree;
  node->left = node->right = nullptr;
  return node;
}

void deleteTree ( tree * node )
{
	if ( !node )
    return;
  if ( node->left)
	  deleteTree( node->left );
  if ( node->right)
    deleteTree( node->right );

	delete node;
}

// convert 8 bits (int) to char
char getChar ( vector <bool> & data, int pos )
{
  char myChar;
  int val = 0, pow = 2;
  for ( int i = (pos+8); i >= pos; i-- )
  {
    if ( i == pos+8 )
    {
      val += data[i];
      continue;
    }
    val += data[i] * pow;
    pow *= 2;
  }
  myChar = val;
  return myChar;
}

tree * createTree ( vector <bool> & data, int & pos )
{
  tree * node = newNode(  );

  if ( data[pos] == 1 )
  {
    node->left = node->right = nullptr;
    node->data = getChar( data, pos );
    pos += 8;
  }
  else
  {
    node->left = createTree( data, ++pos );
    node->right = createTree( data, ++pos );
  }
  return node;
}

void printDataBin ( const Data & data )
{
  for ( int i = 0; i < data.m_size; i++ )
  {
    cout << data.m_data[i];
  }
  cout << '\n' << "----------------" << endl;
}

// convert 12 bits to int
int convert12bits ( Data & data, int & pos )
{
  int val = 0, pow = 2;
  for ( int i = (pos+12); i >= pos; i-- )
  {
    //cout << data.m_data[i];
    if ( i == pos+12 )
    {
      val += data.m_data[i];
      continue;
    }
    val += data.m_data[i] * pow;
    pow *= 2;
  }
  //cout << endl;
  pos += 12;
  return val;
}

// save decompressed data to file
bool saveFile ( Data & data, int & pos, tree * node, const char * outFileName )
{
  ofstream ofs( outFileName, ios::out );
  
  if ( !ofs || ofs.fail() )
    return false;


  int leftData = 4096, cntChars = 0;
  bool checkChunk = 1, wasCheckChunk = 0, isLastChunk = 0;
  
  tree * head = node;
  
  for ( int i = pos; i < data.m_size; i++ )
  {
    if ( checkChunk == 1 )
    {
      if ( data.m_data[i] == 0 )
      {
        leftData = convert12bits( data, pos );
        if ( leftData == 0 && wasCheckChunk == 1 )
        {
          break;
        }
        if ( leftData == 0 && wasCheckChunk == 0 )
        {
          return false;
        }
        if ( pos >= data.m_size )
        {
          return false;
        }
        isLastChunk = 1;
      }
      cntChars = 0;
      checkChunk = 0;
      i++;
    }



    if ( data.m_data[i] == 0 )
    {  
      if ( node->left )
        node = node->left;                                   
      if ( !node->left && !node->right )
      { 
        ofs << node->data;
        node = head;
        cntChars++;
        if ( isLastChunk )
        {
          if ( cntChars == leftData )
            break;
        }
        if ( cntChars == leftData )
        {
          wasCheckChunk = 1;
          checkChunk = 1;
        }
      }
    }
    else
    {  
      if ( node->right )
        node = node->right;
      if ( !node->left && !node->right )
      {
        ofs << node->data;
        node = head;
        cntChars++;
        if ( isLastChunk )
        {
          if ( cntChars == leftData )
            break;
        }
        if ( cntChars == leftData )
        {
          wasCheckChunk = 1;
          checkChunk = 1;
        }
      }
    }
  }
  

  deleteTree ( head );
  ofs.close();
  if ( !ofs.good() )
    return false;
  return true;
}

bool binDump ( const char * fileName, const char * outFileName )
{
  ifstream ifs ( fileName, ios::in | ios::binary );
  
  if ( !ifs || !ifs.is_open() || ifs.fail() )
    return false;

  Data data;
  // read every char from stream and convert it to bits
  for ( char c; ifs.get( c ); )
  {
    if ( ifs.fail() )
      return false;
    // converting to bits
    for( int i = 7; i >= 0; i-- )
    {
      data.m_data.push_back(( ( c >> i ) & 1 ));
    }
  }
  data.m_size = data.m_data.size();
  
  //printDataBin( data );

  // creates bin tree with coded characters
  tree * head = nullptr;
  int pos = 0;
  head = createTree( data.m_data, pos );

  if ( saveFile( data, ++pos, head, outFileName ) == false )
    return false;

  ifs.close();
  return true;
}

bool decompressFile ( const char * inFileName, const char * outFileName )
{
  if ( binDump( inFileName, outFileName ) == false )
    return false;
  return true;
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
  
  assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );
  
  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );
  
  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );
  
  assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );
 
  cout << "---------------------------------------------------"
  << endl;

  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );
  

  return 0;
}
#endif /* __PROGTEST__ */
