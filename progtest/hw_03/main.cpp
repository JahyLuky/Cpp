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


/*
Pouzij time_t z ctime knihovny
Tam zadas den, mesic, rok
Existuje i diff(datum1,datum2) a vypocitani ti rozdíl hned
Ne prave to dela funkce ctime
Jen v konstruktoru kontruluju jestli jsou cisla v rozsahu
A bacha asi jen na unor 28/29
me to proslo pres CDate + int jenom
*/
int main ()
{
    tm tm = {0};  // zero initialise
    tm.tm_year = 2020-1900; // 2020
    tm.tm_mon = 2-1; // February
    tm.tm_mday = 15; // 15th
    tm.tm_hour = 10;
    tm.tm_min = 15;
    tm.tm_isdst = 0; // Not daylight saving
    std::time_t t = std::mktime(&tm); 
    std::tm local = *std::localtime(&t);
 
    std::cout << "local: " << std::put_time(&local, "%c %Z") << '\n';
}
/*

CFraction::CFraction ( long long int numerator, long long int denominator )
:m_numerator(numerator),
 m_denominator(denominator)
{
	if ( m_denominator < 0 )
	{
		m_numerator *= -1;
		m_denominator *= -1;
	}
	shrink();
}

CFraction operator + ( const CFraction & lhs, const CFraction & rhs )
{
	auto lcm = std::lcm(lhs.m_denominator, rhs.m_denominator);

	return {lhs.m_numerator*(lcm/lhs.m_denominator) + rhs.m_numerator*(lcm/rhs.m_denominator), lcm};
}

CFraction operator - ( const CFraction & lhs, const CFraction & rhs )
{
	return lhs + (-rhs);
}

CFraction operator * ( const CFraction & lhs, const CFraction & rhs )
{
	auto	gcd1 = std::gcd(std::abs(lhs.m_numerator), rhs.m_denominator),
		gcd2 = std::gcd(lhs.m_denominator, std::abs(rhs.m_numerator));

	return {(lhs.m_numerator/gcd1)*(rhs.m_numerator/gcd2), (lhs.m_denominator/gcd2)*(rhs.m_denominator/gcd1)};
}

CFraction operator / ( const CFraction & lhs, const CFraction & rhs )
{
	return lhs * CFraction(rhs.m_denominator, rhs.m_numerator);
}

CFraction & CFraction::operator += ( const CFraction & oth )
{
	return *this = *this + oth;
}

CFraction & CFraction::operator -= ( const CFraction & oth )
{
	return *this += (-oth);
}

CFraction & CFraction::operator *= ( const CFraction & oth )
{
	return *this = *this * oth;
}

CFraction & CFraction::operator /= ( const CFraction & oth )
{
	return *this *= CFraction(oth.m_denominator, oth.m_numerator);
}

CFraction CFraction::operator ++ ( int n )
{
	auto old = *this;
	if ( n == 0 )
		*this += 1;
	else
		*this += n;
	return old;
}

CFraction CFraction::operator -- ( int n )
{
	return operator++(n==0?-1:-n);
}

CFraction & CFraction::operator ++ ( void )
{
	return *this += 1;
}

CFraction & CFraction::operator -- ( void )
{
	return *this -= 1;
}

CFraction CFraction::operator - ( void ) const
{
	return {-m_numerator, m_denominator};
}

bool operator <  ( const CFraction & lhs, const CFraction & rhs )
{
	auto lcm = std::lcm(lhs.m_denominator, rhs.m_denominator);

	return lhs.m_numerator * (lcm/rhs.m_denominator) < rhs.m_numerator * (lcm/lhs.m_denominator);
}

bool operator <= ( const CFraction & lhs, const CFraction & rhs )
{
	return lhs < rhs || lhs == rhs;
}

bool operator == ( const CFraction & lhs, const CFraction & rhs )
{
	auto lcm = std::lcm(lhs.m_denominator, rhs.m_denominator);

	return lhs.m_numerator * (lcm/rhs.m_denominator) == rhs.m_numerator * (lcm/lhs.m_denominator);
}

bool operator != ( const CFraction & lhs, const CFraction & rhs )
{
	return !(lhs == rhs);
}

bool operator >= ( const CFraction & lhs, const CFraction & rhs )
{
	return rhs <= lhs;
}

bool operator >  ( const CFraction & lhs, const CFraction & rhs )
{
	return rhs < lhs;
}

std::ostream & operator << ( std::ostream & os, const CFraction & fraction )
{
	return os << fraction.m_numerator << "/" << fraction.m_denominator;
}

std::istream & operator >> ( std::istream & is, CFraction & fraction )
{
	decltype(CFraction::m_numerator) numerator = 0, denominator = 1;

	is >> numerator;

	if ( is && is.peek() == '/' )
		is.ignore(1) >> denominator;

	if ( is && denominator != 0 )
		fraction = CFraction(numerator, denominator);
	else
		is.setstate(std::ios::failbit);
	return is;
}

void CFraction::shrink ( void )
{
	auto gcd = std::gcd(std::abs(m_numerator), m_denominator);
	if ( gcd == 0 )
	{
		m_denominator = 1;
		return;
	}
	m_numerator   /= gcd;
	m_denominator /= gcd;
*/