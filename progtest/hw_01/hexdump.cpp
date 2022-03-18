#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
  ifstream a ( fileName1, ios::binary );
  ifstream b ( fileName2, ios::binary );

  if ( !a || !b )
    return false;

  a.seekg(0, ios::end);
  int file_size1 = a.tellg();

  b.seekg(0, ios::end);
  int file_size2 = b.tellg();

  if ( file_size1 != file_size2 )
    return false;

  char c_a, c_b;
  for ( ; a.get( c_a ); )
  {
    b.get( c_b );

    if ( a.fail() || b.fail() )
      return false;

    if ( c_a != c_b )
    {
      return false;
    }
  }
  
  cout << "identicalFiles" << endl;

  a.close();
  b.close();
  if ( !b.good() )
    return false;
  return true;
}


bool dumpFile ( const char * fileName )
{
  ifstream ifs ( fileName, ios::in | ios::binary );
  
  if ( ! ifs )
  {
    cout << "Cannot open " << fileName << endl;
    return false;
  }

  int offset = 0;
  char chars[11] = { 0 };
  for ( char c; ifs . get ( c ) ; )
  {
    if ( offset % 10 == 0 )
    {
      if ( offset > 0 )
        cout << " " << chars << '\n';
      cout << setw ( 8 ) << setfill ( '0' ) << offset;
    }

    cout << ' ' << hex << setw ( 2 ) << setfill ( '0' ) << (unsigned int)(unsigned char)c;
    chars [ offset % 10 ] = isalpha ( c ) ? c : '.';
    offset ++;
  }
  cout << endl;
  return true;
}


int main ( int argc, char * argv [] )
{
  for ( int i = 1; i < argc; i ++ )
  {
    cout << argv[i] << endl;
    dumpFile ( argv[i] );
  }
  return EXIT_SUCCESS;
}
  