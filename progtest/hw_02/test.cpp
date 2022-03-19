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
};

// pole pointeru na pole
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

    bool findComp ( const string & name, const string & addr );
    bool findComp ( const string & taxID );
    bool compareString ( const string & a, const string & b );
  private:
    vector <Comp> m_data;
    /*
    vector <CVATRegister> m_id_sort;
    vector <CVATRegister> m_name_sort;
    vector <unsigned int> m_invoice;
    */
};

bool CVATRegister::compareString ( const string & a, const string & b )
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

CVATRegister::CVATRegister ( void )
{}
CVATRegister::~CVATRegister ( void )
{}

bool CVATRegister::findComp ( const string & name, const string & addr )
{
  size_t count = m_data.size();
  for ( size_t i = 0; i < count; i++ )
  {
    if ( compareString( m_data[i].m_name, name ) 
    && compareString( m_data[i].m_address, addr ) )
    {
      cout << m_data[i].m_name << " " << name << endl;
      cout << m_data[i].m_address << " " << addr << endl;
      return false;
    }
  }
  return true;
}
bool CVATRegister::findComp ( const string & taxID )
{
  //binary_search(haystack.begin(), haystack.end(), needle);
  size_t count = m_data.size();
  for ( size_t i = 0; i < count; i++ )
  {
    if ( taxID == m_data[i].m_id )
    {
      return false;
    }
  }
  return true;
}

bool CVATRegister::cancelCompany ( const string & name, const string & addr )
{
  size_t count = m_data.size();
  for ( size_t i = 0; i < count; i++ )
  {
    if ( compareString( m_data[i].m_name, name ) 
    && compareString( m_data[i].m_address, addr ) )
    {
      m_data.erase( m_data.begin() + i );
      return true;
    }
  }
  return false;
}
bool CVATRegister::cancelCompany ( const string & taxID )
{
  size_t count = m_data.size();
  for ( size_t i = 0; i < count; i++ )
  {
    if ( taxID == m_data[i].m_id )
    {
      m_data.erase( m_data.begin() + i );
      return true;
    }
  }
  return false;
}

bool CVATRegister::newCompany ( const string & name, const string & addr, const string & taxID )
{
  if ( !findComp( name, addr ) || !findComp( taxID ) )
    return false;
  
  Comp tmp;
  tmp.m_name = name;
  tmp.m_address = addr;
  tmp.m_id = taxID;
  tmp.m_invoice = 0;
  m_data.push_back( tmp );

  return true;
}

bool CVATRegister::invoice ( const string & name, const string & addr, unsigned int amount )
{
  size_t count = m_data.size();
  for ( size_t i = 0; i < count; i++ )
  {
    if ( compareString( m_data[i].m_name, name ) 
    && compareString( m_data[i].m_address, addr ) )
    {
      m_data[i].m_invoice = amount;
      return true;
    }
  }
  return false;
}
bool CVATRegister::invoice ( const string & taxID, unsigned int amount )
{
  size_t count = m_data.size();
  for ( size_t i = 0; i < count; i++ )
  {
    if ( taxID == m_data[i].m_id )
    {
      m_data[i].m_invoice += amount;
      return true;
    }
  }
  return false;
}

bool CVATRegister::audit ( const string & name, const string & addr, unsigned int & sumIncome ) const
{
  size_t count = m_data.size();
  for ( size_t i = 0; i < count; i++ )
  {
    //if ( compareString( m_data[i].m_name, name ) 
    //&&   compareString( m_data[i].m_address, addr ) )
    //if ( m_data[i].m_name == name 
    if ( m_data[i].m_address == addr )
    {
      sumIncome = m_data[i].m_invoice;
      return true;
    }
  }
  return false;
}
bool CVATRegister::audit ( const string & taxID, unsigned int & sumIncome ) const
{
  size_t count = m_data.size();
  for ( size_t i = 0; i < count; i++ )
  {
    if ( taxID == m_data[i].m_id )
    {
      sumIncome = m_data[i].m_invoice;
      return true;
    }
  }
  return false;
}
    
bool CVATRegister::firstCompany ( string & name, string & addr ) const
{
  return false;
}
bool CVATRegister::nextCompany ( string & name, string & addr ) const
{
  return false;
}

unsigned int CVATRegister::medianInvoice ( void ) const
{
  /*
  vkládat při invoice
  sortit pole při median
  udělat kopii a sortit pole při mediam
  hledání indexu vždy binsearchem, vkládání do pole lineárně
  volat qsort při medianu projde
  */
 // kopie -> sesortit -> zahodit
  return -69;
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
  //assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  //assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  //assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  //assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  /*
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
  */
  /*
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
  */

  return 0;
}
#endif /* __PROGTEST__ */
