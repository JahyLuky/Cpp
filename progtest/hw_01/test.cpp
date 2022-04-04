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
char getChar ( Data & data, int pos )
{
  if ( data.m_size < pos )
    return -69;
  char myChar;
  int val = 0, pow = 2;
  for ( int i = (pos+8); i >= pos; i-- )
  {
    if ( i == pos+8 )
    {
      val += data.m_data[i];
      continue;
    }
    val += data.m_data[i] * pow;
    pow *= 2;
  }
  myChar = val;
  return myChar;
}

tree * createTree ( Data & data, int & pos )
{
  tree * node = newNode(  );
  char tmp;
  if ( data.m_size < pos )
    return nullptr;  

  if ( data.m_data[pos] == 1 )
  {
    tmp = getChar( data, pos );
    node->left = node->right = nullptr;
    node->data = tmp;
    if ( tmp == -69 )
      return nullptr;
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
  if ( data.m_size < pos )
    return -1;
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
  
  if ( !ofs || !ofs.good() || pos > data.m_size )
  {
    cout << "savefile" << endl;
    return false;
  }

  int leftData = 4096, cntChars = 0;
  bool checkChunk = 1, isLastChunk = 0;
  
  tree * head = node;
  //cout << "data: " << endl;
  for ( int i = pos; i < data.m_size; i++ )
  {
    //cout << data.m_data[i];
    if ( checkChunk == 1 )
    {
      if ( data.m_data[i] == 0 )
      {
        leftData = convert12bits( data, i );
        if ( leftData == 0 )
        {
          //cout << "lefdat 0" << endl;
          break;
        }
        if ( pos >= data.m_size || leftData < 0 )
        {
          //cout << "pos > data < 0" << endl;
          deleteTree ( head );
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
        if ( ofs.fail() )
        {
          //cout << "false ofs" << endl;
          return false;
        }
        node = head;
        cntChars++;
        if ( isLastChunk )
        {
          //cout << data.m_data[i];
          if ( cntChars == leftData )
            break;
        }
        if ( cntChars == leftData )
        {
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
        if ( ofs.fail() )
        {
          //cout << "false ofs" << endl;
          return false;
        }
        node = head;
        cntChars++;
        if ( isLastChunk )
        {
          //cout << data.m_data[i];
          if ( cntChars == leftData )
            break;
        }
        if ( cntChars == leftData )
        {
          checkChunk = 1;
        }
      }
    }
  }
  if ( cntChars != leftData && (cntChars != 4096 && leftData != 0) )
  {
    
    //cout << cntChars << " " << leftData << endl;
    deleteTree ( head );
    //cout << "neshoda znaku a 12bit cisla" << endl;
    return false;
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
  
  if ( !ifs || !ifs.is_open() || !ifs.good() )
    return false;//cout << "open" << endl;

  if ( ifs.peek() == EOF )
  {
   return false;
  }

  Data data;
  // read every char from stream and convert it to bits
  for ( char c; ifs.get( c ); )
  {
    if ( ifs.fail() )
      return false;//cout << "fail" << endl;
    // converting to bits
    for( int i = 7; i >= 0; i-- )
    {
      data.m_data.push_back(( ( c >> i ) & 1 ));
    }
  }
  bool allZero = all_of(data.m_data.begin(), data.m_data.end(), [](bool i) { return i == 0; });
  if ( allZero )
    return false;

  data.m_size = data.m_data.size();
  
  printDataBin( data );

  // creates bin tree with coded characters
  tree * head = nullptr;
  int pos = 0;
  head = createTree( data, pos );
  if ( head == nullptr )
  {
    return false;
    cout << "head" << endl;
  }
  
/*
  cout << "12bits: " << endl;
  int aa = pos + 1;
  int con = convert12bits(data,aa);
  cout << endl << endl;
  for (int i = aa; i < data.m_size; i++)
  {
    cout << data.m_data[i];
  }
  cout << endl;  
  */


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

  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );


/*
  if ( decompressFile ( "tests/last.bin", "tempfile" ) )
  {
    cout << "true" << endl;
  }
  else{
    //cout << "false" << endl;
  }*/

  return 0;
}
#endif /* __PROGTEST__ */