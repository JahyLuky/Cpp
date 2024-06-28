#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <istream>
#include <sstream>
#include <vector>


using namespace std;

struct Image {
  int width;
  int height;
  vector<char> data;
  int size;
};

// načte obrázek do paměti
Image imageInit( const char * fileName )
{
  Image init;
  fstream ifs (fileName, ios::in | ios::binary);
  
  int i = 0;
  char c;
  while ( 1 )
  {
    if ( ifs.eof() )
      break;
    
    ifs.get( c );
    init.data.push_back( c );
    i++;
  }
  init.size = i;
  return init;
}

// zruší data, uvolní paměť...
void imageDestroy( Image image )
{
  image.data.clear();
  image.data.shrink_to_fit();
}

// vrátí šířku obrázku
int imageWidth( Image image )
{
  image.width = (int)(unsigned char)image.data[12];
  return image.width;
}

// vrátí výšku obrázku
int imageHeight( Image image )
{
  image.height = (int)(unsigned char)image.data[14];
  return image.height;
}

// vytvoří obrázek kruhu zadané barvy (r, g, b) na bílém pozadí o zadaném poloměru radius.
Image imageCircle( int radius, int r, int g, int b )
{
  Image circle;
  
  

  return circle;
}

// uloží obrázek do souboru
void imageSave( const char *image, const char *fileName )
{
  fstream ifs(image, ios::in | ios::binary);
  fstream ofs(fileName, ios::out | ios::binary);
  
  const int len = 4096;
  char buf[4096];
  while ( 1 )
  {
    if ( ifs.eof() )
      break;
    ifs.read(buf, len);
    int nBytesRead = ifs.gcount();
    if (nBytesRead <= 0)
      break;
    ofs.write(buf, nBytesRead);
  }

  ifs.close();
  ofs.close();
}

int main()
{
  const char *srcFile = "image/vagner.tga";
  Image filebuf;
  filebuf = imageInit( srcFile );

  int width = imageWidth( filebuf );
  int height = imageHeight( filebuf );
  cout << "width: " << width << " height: " << height << endl;
  cout << "size: " << filebuf.size << endl;

  //Image circle = imageCircle(  );

  //imageSave(srcFile, "image/newVagner.tga");

  imageDestroy( filebuf );
  return 0;
}