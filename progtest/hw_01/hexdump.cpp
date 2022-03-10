#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
using namespace std;


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
  