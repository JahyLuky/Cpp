#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
  public:
    InvalidDateException ( )
      : invalid_argument ( "invalid date or format" )
    {
    }
};
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
  return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================


class CDate
{
  public:
    tm m_date;

    CDate ( int year, int month, int day )
    { 
      tm date = {1};
      date.tm_mday = day;
      date.tm_mon = month - 1;
      date.tm_year = year - 1900;

      //cout << date.tm_year +1900 << " " << date.tm_mon+1 << " " << date.tm_mday << "--1" << endl;
      time_t time = mktime( &date );
      //cout << date.tm_year+1900 << " " << date.tm_mon +1<< " " << date.tm_mday << "--2" << endl;
      if ( time == -1 ||
           date.tm_mday != day ||
           (date.tm_mon + 1) != month ||
           (date.tm_year + 1900) != year )
      {
        cout << year << " " << month << " " << day << endl;
        //cout << date.tm_year + 1900 << " " << date.tm_mon + 1 << " " << date.tm_mday << endl;
        throw InvalidDateException();
      }
      m_date = {1};
      m_date.tm_mday = day;
      m_date.tm_mon = month - 1;
      m_date.tm_year = year - 1900;
    }

    CDate operator + ( int num )
    {
      m_date.tm_mday += num;
      mktime( &m_date );
      return *this;
    }
    
    CDate operator - ( int num )
    {
      m_date.tm_mday -= num;
      mktime( &m_date );
      return *this;
    }
    
    int operator - ( const CDate & date )
    {
      tm date_copy = {1};
      date_copy.tm_mday = date.m_date.tm_mday;
      date_copy.tm_mon = date.m_date.tm_mon ;
      date_copy.tm_year = date.m_date.tm_year;

      time_t mkDate = mktime ( &date_copy );
      time_t class_Date = mktime ( &m_date );

      int tmp = difftime( class_Date, mkDate );
      if ( tmp < 0 )
      {
        tmp *= (-1);
      }

      tmp /= toSec;
      return tmp;
    }

    CDate operator ++ ( int num )
    {
      CDate tmp(0,0,0);
      tmp.m_date = {1};
      tmp.m_date.tm_mday = m_date.tm_mday;
      tmp.m_date.tm_mon = m_date.tm_mon;
      tmp.m_date.tm_year = m_date.tm_year;
      mktime( &tmp.m_date );
      m_date.tm_mday += 1;
      return tmp;
    }
    CDate operator ++ ( void )
    {
      m_date.tm_mday += 1;
      mktime( &m_date );
      return *this;
    }

    CDate operator -- ( int num )
    {
      CDate tmp(0,0,0);
      tmp.m_date = {1};
      tmp.m_date.tm_mday = m_date.tm_mday;
      tmp.m_date.tm_mon = m_date.tm_mon;
      tmp.m_date.tm_year = m_date.tm_year;
      mktime( &tmp.m_date );
      m_date.tm_mday -= 1;
      return tmp;
    }
    CDate operator -- ( void )
    {
      m_date.tm_mday -= 1;
      mktime( &m_date );
      return *this;
    }
    
    bool operator == ( const CDate & date )
    {
      if ( m_date.tm_mday != date.m_date.tm_mday ||
           m_date.tm_mon != date.m_date.tm_mon ||
           m_date.tm_year != date.m_date.tm_year )
      {
        return false;
      }
      return true;
    }

    bool operator != ( const CDate & date )
    {
      if ( *this == date )
      {
        return false;
      }
      return true;
    }
    
    bool operator > ( const CDate & date )
    {
      tm date_copy = {1};
      date_copy.tm_mday = date.m_date.tm_mday;
      date_copy.tm_mon = date.m_date.tm_mon;
      date_copy.tm_year = date.m_date.tm_year;
          
      time_t mkDate = mktime ( &date_copy );
      time_t class_Date = mktime ( &m_date );
      
      if ( mkDate == class_Date )
      {
        return false;
      }
      return true;
    }
    
    bool operator >= ( const CDate & date )
    {
      if ( *this == date || *this > date )
      {
        return true;
      }
      return false;
    }
    
    bool operator < ( const CDate & date )
    {
      if ( *this >= date )
      {
        return false;
      }
      return true;
    }

    bool operator <= ( const CDate & date )
    {
      if ( *this == date || *this < date )
      {
        return true;
      }
      return false;
    }

    friend ostream & operator << ( ostream & out, const CDate & date )
    {
      out << date.m_date.tm_year + 1900 << "-" << setfill( '0' ) << setw( 2 ) 
          << date.m_date.tm_mon + 1 << "-" << setw( 2 ) 
          << date.m_date.tm_mday << flush;
      return out;
    }
    
    friend istream & operator >> ( istream & in, CDate & date )
    {
      tm date_copy = {};
      in >> get_time( &date_copy, "%Y-%m-%d" );

      date.m_date.tm_mday = date_copy.tm_mday;
      date.m_date.tm_mon = date_copy.tm_mon;
      date.m_date.tm_year = date_copy.tm_year;

      if ( 0 )
      {
        in.setstate( ios::failbit );
      }
      
      return in;
    }
    
  private:
    // 24 * 60 * 60
    int toSec = 86400;
};


void printTM ( tm & a )
{
  cout << a.tm_year + 1900 << "-" << setfill( '0' ) << setw( 2 ) 
          << a.tm_mon + 1 << "-" << setw( 2 ) 
          << a.tm_mday << endl;
}

void printPLS ( const CDate & a )
{
  cout << a.m_date.tm_year + 1900 << "-" << setfill( '0' ) << setw( 2 ) 
          << a.m_date.tm_mon + 1 << "-" << setw( 2 ) 
          << a.m_date.tm_mday << endl;
}

#ifndef __PROGTEST__
int main ( void )
{
  ostringstream oss;
  istringstream iss;

  CDate a ( 2000, 1, 2 );
  CDate b ( 2010, 2, 3 );
  CDate c ( 2004, 2, 10 );
  
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2000-01-02" );
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2010-02-03" );
  oss . str ("");
  oss << c;
  assert ( oss . str () == "2004-02-10" );
  a = a + 1500;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2004-02-10" );
  b = b - 2000;
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2004-08-13" );
  assert ( b - a == 185 );
  assert ( ( b == a ) == false );
  assert ( ( b != a ) == true );
  assert ( ( b <= a ) == false );
  assert ( ( b < a ) == false );
  assert ( ( b >= a ) == true );
  assert ( ( b > a ) == true );
  assert ( ( c == a ) == true );
  assert ( ( c != a ) == false );
  assert ( ( c <= a ) == true );
  assert ( ( c < a ) == false );
  assert ( ( c >= a ) == true );
  assert ( ( c > a ) == false );
  a = ++c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-11" );
  a = --c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-10" );
  a = c++;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-11" );
  a = c--;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-10" );
  iss . clear ();
  iss . str ( "2015-09-03" );
  assert ( ( iss >> a ) );
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-09-03" );
  a = a + 70;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-11-12" );

  CDate d ( 2000, 1, 1 );
  try
  {
    CDate e ( 2000, 32, 1 );
    assert ( "No exception thrown!" == nullptr );
  }
  catch ( ... )
  {
  }
  iss . clear ();
  iss . str ( "2000-12-33" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-11-31" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-02-29" );
  assert ( ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );
  iss . clear ();
  iss . str ( "2001-02-29" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );

  

  return 0;
}
#endif /* __PROGTEST__ */
