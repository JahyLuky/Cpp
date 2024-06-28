#include <iostream>

using namespace std;

struct MyArray {
	unsigned int m_used;
	unsigned int m_cap;
	double * m_arr;
};

bool array_init ( MyArray & arr, unsigned int capacity )
{
	MyArray newArray = { 0, capacity, new double [capacity] }; 
	
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
		if ( !( cin >> arr.m_arr[i] ) )
			return false;
	return true;
}

void array_print ( const MyArray & arr )
{
	for ( unsigned int i = 0; i < arr.m_cap; ++i )
		cout << arr.m_arr[i] << " ";
	cout << endl;
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
	array_print(numbers);

	array_destroy(numbers);
	return 0;
}