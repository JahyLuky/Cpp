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
    CTimeStamp ( int year, int month, int day, 
                 int hour, int min, int sec )
    : m_year ( year ),
      m_month ( month ),
      m_day ( day ),
      m_hour ( hour ),
      m_min ( min ),
      m_sec ( sec )
    {}
  private:
    int m_year;
    int m_month;
    int m_day;
    int m_hour;
    int m_min;
    int m_sec;
}

class CContact
{
  public:
    CContact ( CTimeStamp getTimeStamp, int a_phone, int b_phone )
    : m_TimeStamp ( getTimeStamp ), m_a ( a_phone ), m_b ( b_phone )
    {}
  private:
    CTimeStamp & m_TimeStamp;
    int m_a;
    int m_b;
}

class CEFaceMask
{
  public:
    CEFaceMask & addContact ( CTimeStamp contact, CContact a, CContact b )
    {
      m_list[a] = b;
      return * this;
    }
    const CContact listContacts ( CContact a, CTimeStamp b , CTimeStamp c ) const
    {

    }
  private:
    CContact & m_a;
    CContact & m_b;
    CTimeStamp & m_contact;
    vector <int> m_list;
}

#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
  return 0;
}
#endif /* __PROGTEST__ */
