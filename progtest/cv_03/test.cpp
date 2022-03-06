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

bool findThem ( const string & query, vector <string> & data, int i )
{
  //cout << query << " q" << endl;
  //cout << data[i] << " da" << endl;
  stringstream toWord( data[i] );
  string word;

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

bool checkData ( vector <string> & data, int i )
{
  stringstream toWord ( data[i] );
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

string getLine ( vector <string> & data, int i )
{
  stringstream toWord( data[i] );
  string words;
  string line;
  int cnt = 0;
  while( toWord >> words )
  {
    int len = words.size();
    if ( cnt == 2 )
    {
      words.insert( len, "\n" );
      line += words;
      cnt = 0;
    }
    else
    {
      words.insert( len, " " );
      line += words;
      cnt++;
    }
    
  }
  //cout << line;
  return data[i];
}

bool report ( const string & fileName, ostream & out )
{
  ifstream ifs;
  ifs.open( fileName );
  if ( ifs.fail() )
    return false;

  vector <string> data {};
  string toString;
  const int sizeLine = 4096;
  char str[sizeLine];
  int flag = 0, cnt = 0;

  while ( ifs.getline( str, sizeLine ) )
  {
    if ( str[0] == '\0' )
    {
      flag = 1;
    }
    if ( !flag )
    {
      toString = str;
      data.push_back( toString );
      
      if ( !checkData( data, cnt ) )
      {
        cout << "checkData" << endl;
        return false;
      }
      cnt++;
    }
    else
    {
      string query = str;
      int found = 0;
      for ( int j = 0; j < cnt; j++ )
      {
        if ( findThem( query, data, j ) == true )
        {
          found++;
          
          string line = getLine( data, j );
          out << line << endl;
          
        }
      }
      if ( found > 0 )
        out << "-> " << found << endl;  
      //cout << "-> " << found << endl;
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
  //report ( "tests/test0_in.txt", oss );
  //report ( "tests/test1_in.txt", oss );
  
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
  
  return 0;
}
#endif /* __PROGTEST__ */
