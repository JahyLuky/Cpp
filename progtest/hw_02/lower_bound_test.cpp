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
#include <memory>
using namespace std;

bool compareString ( const string & a, const string & b )
{
  size_t a_len = a.size();
  size_t b_len = b.size();
  if ( a_len != b_len )
    return false;
  
  for ( size_t i = 0; i < a_len; i++ )
  {
    if ( tolower( a[i] ) != tolower( b[i] ) )
      return false;
  }
  return true;
}

struct Comp
{
  string m_name;
  string m_address;
  string m_id;
  unsigned int m_invoice;

  bool operator != ( const Comp & a )
  {
    if (  compareString( a.m_name, m_name )  )
    {
      if ( compareString( a.m_address, m_address )  )
      {
        return false;
      }
    }
    return true;
  }
};

struct comp
{
    bool operator() ( const Comp & a, const Comp & b ) const
    {
      if ( compareString( a.m_name, b.m_name ) ) {
      //if ( a.m_name == b.m_name ) {
        return a.m_address < b.m_address;
      }
      else
      {
        return a.m_name < b.m_name;
      }
    }
};

int main ( void )
{
  vector <Comp> data;
  Comp a, b;

  a.m_name = "name";
  a.m_address = "b_zde";
  a.m_id = "123";

  auto pos = lower_bound( data.begin(), data.end(), a, comp() );


  if ( pos == data.end() || *pos != a )
  {
    cout << "a" << endl;
    data.insert( pos, a );
  }

  b.m_name = "aname";
  b.m_address = "b_zde";
  b.m_id = "234";

  pos = lower_bound( data.begin(), data.end(), b, comp() );

  if ( *pos != a )
  {
    cout << "zde";
  }

  if ( pos == data.end() || *pos != b )
  {
    cout << "b" << endl;
    data.insert( pos, b );
  }

  Comp c;
  c.m_name = "aname";
  c.m_address = "a_zde";
  c.m_id = "345";
  
  pos = lower_bound( data.begin(), data.end(), c, comp() );

  if ( pos == data.end() || *pos != c )
  {
    cout << "c" << endl;
    data.insert( pos, c );
  }


  for ( size_t i = 0; i < (size_t)data.size(); i++ )
  {
    cout << data[i].m_name << " " << data[i].m_address << " - ";
  }
  cout << endl;

  return 0;
}