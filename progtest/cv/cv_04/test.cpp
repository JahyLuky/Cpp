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

class CTimeStamp
{
  public:
    //CTimeStamp();
    CTimeStamp( int year, int month, int day,
                int hour, int min, int sec )
    {
      m_data.push_back(year);
      m_data.push_back(month);
      m_data.push_back(day);
      m_data.push_back(hour);
      m_data.push_back(min);
      m_data.push_back(sec);
      //m_data[0] = year;
      //m_data[1] = month;
      //m_data[2] = day;
      //m_data[3] = hour;
      //m_data[4] = min;
      //m_data[5] = sec;
    }
    const CTimeStamp & getDate ( void )
    {
      return *this;
    }
  private:
    vector <int> m_data;
};

class CContact
{
  public:
    //CContact();
    CContact( const CTimeStamp getTime, const int a_phone,
     const int b_phone )
    : m_date( getTime ), m_a( a_phone ), m_b( b_phone )
    {}
    const CTimeStamp & getTimeStamp ( void )
    {
      return m_date;
    }
    const int getA ( void )
    {
      return m_a;
    }
    const int getB ( void )
    {
      return m_b;
    }
  private:
    CTimeStamp m_date;
    int m_a;
    int m_b;
};

class CEFaceMask
{
  public:
    CEFaceMask();
    CEFaceMask( CContact cont )
    : m_cont ( cont )
    {}
    CEFaceMask & addContact( CContact a )
    {
      m_data.push_back( a );
      return *this;
    }
    vector <int> & listContacts( int a_phone, CTimeStamp & start,
    CTimeStamp & end );
  private:
    CContact m_cont;
    vector <CContact> m_data;
};


#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;
  CContact(CTimeStamp(1,2,3,4,5,6),123,321);
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
  /*
  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
  */
  

  return 0;
}
#endif /* __PROGTEST__ */
