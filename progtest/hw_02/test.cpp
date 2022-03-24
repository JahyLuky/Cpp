#ifndef __PROGTEST__
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
#endif /* __PROGTEST__ */

struct Comp
{
  string m_name;
  string m_address;
  string m_id;
  unsigned int m_invoice;

  // TRUE = aren't same, FALSE = are same
  bool operator != ( const Comp & a ) const
  {
    if ( ! strcasecmp( a.m_name.c_str(), m_name.c_str() ) )
    {
      if ( ! strcasecmp( a.m_address.c_str(), m_address.c_str() ) )
      {
        return false;
      }
    }
    if ( ! strcmp( a.m_id.c_str(), m_id.c_str() ) )
    {
      return false;
    }
    return true;
  }
};

struct compare
{
  // compare names, if equal, compare addresses
  bool operator() ( const Comp & a, const Comp & b ) const
  {
    int cmp = strcasecmp( a.m_name.c_str(), b.m_name.c_str() );
    int addr = strcasecmp( a.m_address.c_str(), b.m_address.c_str() );
    if ( cmp == 0 )
    {
      if ( addr == 0 )
        return false;
      else
        return addr < 0;
    }
    return cmp < 0;
  }
};
struct compare_id
{
  // compare IDs
  bool operator() ( const Comp & a, const Comp & b ) const
  {
    int id = strcmp( a.m_id.c_str(), b.m_id.c_str() );
    return id < 0;
  }
};

class CVATRegister
{
  public:
                  CVATRegister   ( void );
                  ~CVATRegister  ( void );
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID );
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr );
    bool          cancelCompany  ( const string    & taxID );
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount );
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount );
    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const;
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const;
    bool          firstCompany   ( string          & name,
                                   string          & addr ) const;
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const;
    unsigned int  medianInvoice  ( void ) const;


  private:
    vector <Comp> m_id_sort;
    vector <Comp> m_name_sort;
    vector <unsigned int> m_invoice;
};

CVATRegister::CVATRegister ( void )
{}
CVATRegister::~CVATRegister ( void )
{}

bool CVATRegister::cancelCompany ( const string & name, const string & addr )
{
  Comp isThere;
  isThere.m_name = name;
  isThere.m_address = addr;
  auto posName = lower_bound ( m_name_sort.begin(), m_name_sort.end(), isThere, compare() );
  
  if ( posName == m_name_sort.end() || *posName != isThere )
  {
    return false;
  }
  
  auto posID = lower_bound ( m_id_sort.begin(), m_id_sort.end(), (*posName), compare_id() );
  m_name_sort.erase( posName );
  m_id_sort.erase( posID );

  return true;
}
bool CVATRegister::cancelCompany ( const string & taxID )
{
  Comp isThere;
  isThere.m_id = taxID;
  auto posID = lower_bound ( m_id_sort.begin(), m_id_sort.end(), isThere, compare_id() );
  
  if ( posID == m_id_sort.end() || *posID != isThere )
  {
    return false;
  }

  auto posName = lower_bound ( m_name_sort.begin(), m_name_sort.end(), (*posID), compare() );
  m_id_sort.erase( posID );
  m_name_sort.erase( posName );

  return true;
}

bool CVATRegister::newCompany ( const string & name, const string & addr, const string & taxID )
{ 
  Comp isThere;
  isThere.m_name = name;
  isThere.m_address = addr;
  isThere.m_id = taxID;
  isThere.m_invoice = 0;
  auto posName = lower_bound ( m_name_sort.begin(), m_name_sort.end(), isThere, compare() ); 
  
  if ( posName == m_name_sort.end() || *posName != isThere )
  {
    auto posID = lower_bound ( m_id_sort.begin(), m_id_sort.end(), isThere, compare_id() );
    
    if ( posID == m_id_sort.end() || ( *posID != isThere ) )
    {
      m_name_sort.insert( posName, isThere );
      m_id_sort.insert( posID, isThere );
      
      return true;
    }
  } 

  return false;
}

bool CVATRegister::invoice ( const string & name, const string & addr, unsigned int amount )
{
  Comp isThere;
  isThere.m_name = name;
  isThere.m_address = addr;
  auto posName = lower_bound ( m_name_sort.begin(), m_name_sort.end(), isThere, compare() );

  if ( posName == m_name_sort.end() || *posName != isThere )
  {
    return false;
  }

  m_invoice.push_back( amount );
  auto posID = lower_bound ( m_id_sort.begin(), m_id_sort.end(), (*posName), compare_id() );
  (*posName).m_invoice += amount;
  (*posID).m_invoice += amount;

  return true;
}
bool CVATRegister::invoice ( const string & taxID, unsigned int amount )
{
  Comp isThere;
  isThere.m_id = taxID;
  auto posID = lower_bound ( m_id_sort.begin(), m_id_sort.end(), isThere, compare_id() );
  
  if ( posID == m_id_sort.end() || *posID != isThere )
  {
    return false;
  }
  
  m_invoice.push_back( amount );
  auto posName = lower_bound ( m_name_sort.begin(), m_name_sort.end(), (*posID), compare() );
  (*posName).m_invoice += amount;
  (*posID).m_invoice += amount;
  
  return true;
}

bool CVATRegister::audit ( const string & name, const string & addr, unsigned int & sumIncome ) const
{
  Comp isThere;
  isThere.m_name = name;
  isThere.m_address = addr;
  auto posName = lower_bound ( m_name_sort.begin(), m_name_sort.end(), isThere, compare() );
  
  if ( posName == m_name_sort.end() || *posName != isThere )
  {
    return false;
  }
  
  sumIncome = (*posName).m_invoice;
  
  return true;
}
bool CVATRegister::audit ( const string & taxID, unsigned int & sumIncome ) const
{
  Comp isThere;
  isThere.m_id = taxID;
  auto posID = lower_bound ( m_id_sort.begin(), m_id_sort.end(), isThere, compare_id() );
  
  if ( posID == m_id_sort.end() || *posID != isThere )
  {
    return false;
  }
  
  sumIncome = (*posID).m_invoice;
  
  return true;
}
    
bool CVATRegister::firstCompany ( string & name, string & addr ) const
{
  if ( m_name_sort.empty() )
    return false;

  name = m_name_sort[0].m_name;
  addr = m_name_sort[0].m_address;
  
  return true;
}
bool CVATRegister::nextCompany ( string & name, string & addr ) const
{
  Comp tmp;
  tmp.m_name = name;
  tmp.m_address = addr;
  auto pos = lower_bound( m_name_sort.begin(), m_name_sort.end(), tmp, compare() );
  
  if ( pos == m_name_sort.end() )
  {
    return false;
  }

  ++pos;

  if ( pos == m_name_sort.end() )
  {
    return false;
  }

  name = (*pos).m_name;
  addr = (*pos).m_address;

  return true;
}

unsigned int CVATRegister::medianInvoice ( void ) const
{
  if ( m_invoice.empty() )
    return 0;
  
  vector <unsigned int> tmp;
  size_t count = m_invoice.size();

  for ( size_t i = 0; i < count; i++ )
  {
    tmp.push_back( m_invoice[i] );
  }

  sort ( tmp.begin(), tmp.end() );

  unsigned int median = count / 2;

  return tmp[ median ];
}

#ifndef __PROGTEST__
int main ( void )
{
  string name, addr;
  unsigned int sumIncome;

  CVATRegister b1;

  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . invoice ( "666/666", 2000 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . cancelCompany ( "666/666" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . invoice ( "123456", 100 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 300 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 230 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 830 ) );
  assert ( b1 . medianInvoice () == 830 );
  assert ( b1 . invoice ( "123456", 1830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 3200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "123456" ) );
  assert ( ! b1 . firstCompany ( name, addr ) );

  CVATRegister b2;
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . medianInvoice () == 0 );
  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . medianInvoice () == 1000 );
  assert ( b2 . invoice ( "abcdef", 2000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . invoice ( "1234567", 100 ) );
  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . audit ( "1234567", sumIncome ) );
  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . cancelCompany ( "1234567" ) );
  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );
  

  return 0;
}
#endif /* __PROGTEST__ */
