#include "fraction.hpp"
#include <numeric>

CFraction::CFraction ( int num, int denom )
: m_numerator( num ), m_denominator( denom )
{
    normalize();
}

CFraction CFraction::operator + ( const CFraction & lhs, const CFraction & rhs )
{
    return { lhs.m_numerator * rhs.m_denominator + rhs.m_numerator * lhs.m_denominator,
             lhs.m_denominator * rhs.m_denominator };
}
CFraction CFraction::operator - ( const CFraction & lhs, const CFraction & rhs )
{
    return lhs + (-rhs);
}
CFraction CFraction::operator * ( const CFraction & lhs, const CFraction & rhs )
{
    return { lhs.m_numerator * rhs.m_numerator, 
             lhs.m_denominator * rhs.m_denominator };
}
CFraction CFraction::operator / ( const CFraction & lhs, const CFraction & rhs )
{
    return lhs * CFraction( rhs.m_denominator, rhs.m_numerator );
}

CFraction & CFraction::operator += ( const CFraction & rhs )
{
    return *this = *this + rhs;
}
CFraction & CFraction::operator -= ( const CFraction & rhs )
{
    return *this += (-rhs);
}
CFraction & CFraction::operator *= ( const CFraction & rhs )
{
    return *this = *this * rhs;
}
CFraction & CFraction::operator /= ( const CFraction & rhs )
{
    return *this = *this / rhs;
}

//postfix
CFraction CFraction::operator ++ ( int n )
{
    CFraction old = *this;
    *this += n == 0 ? 1 : n;
    return old;
}
CFraction CFraction::operator -- ( int n )
{
    CFraction old = *this;
    *this -= n == 0 ? 1 : n;
    return old;
}
//prefix
CFraction & CFraction::operator ++ ( void )
{
    return *this += 1;
}
CFraction & CFraction::operator -- ( void )
{
    return *this -= 1;
}

//unarni -
CFraction CFraction::operator - ( void ) const
{
    return { - m_numerator, m_denominator };
}

bool operator <  ( const CFraction & lhs, const CFraction & rhs )
{
    return lhs.m_numerator * rhs.m_denominator < rhs.m_numerator * lhs.m_denominator;
}
bool operator <= ( const CFraction & lhs, const CFraction & rhs )
{
    return lhs < rhs || lhs == rhs;
    //return !( rhs < lhs );
}
bool operator == ( const CFraction & lhs, const CFraction & rhs )
{
    return lhs.m_numerator * rhs.m_denominator == rhs.m_numerator * lhs.m_denominator;
}
bool operator != ( const CFraction & lhs, const CFraction & rhs )
{
    return !( lhs == rhs );
}
bool operator >= ( const CFraction & lhs, const CFraction & rhs )
{
    return  rhs <= lhs;
}
bool operator >  ( const CFraction & lhs, const CFraction & rhs )
{
    return lhs < rhs;
}

std::ostream & operator << ( std::ostream & os, const CFraction & rhs )
{
    return os << rhs.m_numerator << "/" << rhs.m_denominator;
}
std::istream & operator >> ( std::istream & is, CFraction & rhs )
{
    int num, denom;
    char x;
    //is >> num >> x >> denom;
    is >> num;
    if ( !is )
    {

    }
    if ( is.peek() == '/' )
    {

    }
    is >> x;
    if ( !is || x != '/' )
    rhs = CFraction( num, denom );
    return is;
}