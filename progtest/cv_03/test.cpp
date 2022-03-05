#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */


bool findThem ( const string & query, const string & data, ostream & out )
{
  stringstream toWord ( data );
  string buffer;

  while ( toWord >> buffer )
  {
    if ( query == buffer )
    {
      //cout << buffer << endl;
    }
  }


  return false;
}

bool isPhoneNum ( const string & words )
{
  int len = words.size();
  if ( len != 9 )
    return false;
  
  for ( int i = 0; i < len; i++ )
  {
    if ( words[i] >= '0' && words[i] <= '9' )
      continue;
    else if ( words[0] == '0' )
      return false;
    else
      return false;
  }
  return true;
}

bool checkData ( const string & line, string & data )
{
  stringstream toWord ( line );
  string words;
  int len, cnt = 0;

  while( toWord >> words )
  {
    len = words.size();
    if ( cnt == 2 )
    {
      if ( isPhoneNum( words ) == 0 )
        return false;

      words.insert( len, "\n" );
      data += words;
      cnt = 0;
    }
    else
    {
      words.insert( len, " " );
      data += words;
      cnt++;
    }
    //cout << data;
  }
  
  return true;
}

bool report ( const string & fileName, ostream & out )
{
  ifstream ifs;
  ifs.open( fileName );

  string buffer, data;
  int flag = 0;
  const int sizeLine = 4096;
  char str[sizeLine];
  while ( ifs.getline( str, sizeLine ) )
  {
    cout << str << endl;
  }

  ifs.close();
  return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  report ( "tests/test0_in.txt", oss );
  //report ( "tests/test1_in.txt", oss );
  /*
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  */
  return 0;
}
#endif /* __PROGTEST__ */
