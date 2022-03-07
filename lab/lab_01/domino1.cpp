#include <iostream>
#include <algorithm>

using namespace std;

struct Domino {
	int dots [2];
};

struct MyArray {
	unsigned int m_used;
	unsigned int m_cap;
	Domino * m_arr;
};

bool readDomino ( std:: istream & in, Domino & tile )
{
  in >> tile.dots[0] >> tile.dots[1];
  return (bool)in;
}

bool writeDomino ( std:: ostream & out, const Domino & tile )
{
  out << "[" << tile.dots[0] << "|" << tile.dots[1] << "]";
  return (bool)out;
}

bool lessDomino ( const Domino & a, const Domino & b )
{
  if ( a.dots[0] < b.dots[0] ) return true;
  if ( a.dots[0] > b.dots[0] ) return false;
  return a.dots[1] < b.dots[1];
}

bool array_init ( MyArray & arr, unsigned int capacity )
{
	MyArray newArray = { 0, capacity, new Domino [capacity] }; 
	
	if ( newArray.m_arr == nullptr )
		return false;
	
	arr = newArray; 
	return true;
}

void array_destroy ( MyArray & arr )
{
	delete [] arr.m_arr;
	arr.m_arr = nullptr;
	arr.m_used = arr.m_cap = 0;
}

bool array_load ( MyArray & arr )
{
	for ( unsigned int i = 0; i < arr.m_cap; ++i )
		if ( !readDomino( cin, arr.m_arr[i] ) )
			return false;
	return true;
}

void array_print ( const MyArray & arr )
{
	for ( unsigned int i = 0; i < arr.m_cap; ++i )
		writeDomino( cout, arr.m_arr[i]);
	cout << endl;
}

void array_sort ( MyArray & arr )
{
  sort( arr.m_arr, arr.m_arr + arr.m_cap, lessDomino );
  // stable_sort( arr.m_arr, arr.m_arr + arr.m_cap, lessDomino );
}

int main ( )
{
	int N = 0;
	MyArray numbers;

	cout  << "Zadej pozadovanou velikost:" << endl;
	if ( !(cin >> N) || N <= 0)
	{
		cout << "Chybny vstup!" << endl;
		return 0;
	}

	if ( array_init(numbers, N) == false )
	{
		cout << "Vytvoreni pole selhalo!" << endl;
		return 0;
	}

	cout << "Zadej sva cisla:" << endl;
	if ( array_load(numbers) == false )
	{
		cout << "Nacteni cisel selhalo!" << endl;
		array_destroy(numbers);
		return 0;
	}

	cout << "Nactena vsechna cisla!" << endl;
  array_sort(numbers);
	array_print(numbers);

	array_destroy(numbers);
	return 0;
}