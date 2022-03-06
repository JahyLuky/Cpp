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



bool findThem ( const string & query, const string & data )
{
  stringstream toWord( data );
  string word;
  int found = 0;

  while ( toWord >> word )
  {
    if ( query == word )
    {
      return true;
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

bool checkData ( const string & data )
{
  stringstream toWord ( data );
  string word;
  int cnt = 0;

  while ( toWord >> word )
  {
    cnt++;
    if ( cnt == 3 )
    {
      if ( !isPhoneNum( word ) )
        return false;

      cnt = 0;
    }
  }

  if ( cnt != 0 )
  {
    //cout << "neco chybi nebo prebyva" << endl;
    return false;
  }

  return true;
}

bool report ( const string & fileName, ostream & out )
{
  ifstream ifs;
  ifs.open( fileName );
  if ( ifs.fail() )
    return false;

  vector<string> data {};
  string s;
  const int sizeLine = 4096;
  char str[sizeLine];
  int flag = 0, i = 0;

  while ( ifs.getline( str, sizeLine ) )
  {
    if ( str[0] == '\0' )
    {
      flag = 1;
    }
    if ( !flag )
    {
      s = str;
      cout << s;
      //data.push_back( s );
      //i++;
      //cout << data[i] << endl;
      //if ( !checkData( data[i] ) )
      //{
      //  cout << "checkData" << endl;
      //  return false;
      //}
    }
    else
    {
      string query = str;
      for ( int j = 0; j < i; j++ )
      {
        //if ( findThem( query, data[i] ) )
        //{
        //  cout << data[i] << endl;
        //}
      }
    }
  }

 // cout << data[1] << endl;
  
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
