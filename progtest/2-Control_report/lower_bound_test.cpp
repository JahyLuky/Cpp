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
  if ( ! strcasecmp( a.c_str(), b.c_str() ) )
    return false;
  return true;
}

struct Comp
{
  string m_name;
  string m_address;
  string m_id;
};

int main ( void )
{
  Comp a;
  a.m_name = "ACME";
  a.m_address = "kolejni";
  Comp b;
  b.m_name = "aCME";
  b.m_address = "KoLEjni";
  
  if ( ! compareString( a.m_name, b.m_name )  )
  {
    if ( ! compareString( a.m_address, b.m_address )  )
    {
      cout << "stejne\n";
      return 0;
    }
  }
  cout << "ruzne\n";
  return 1;
}