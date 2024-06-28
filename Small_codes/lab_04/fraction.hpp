#pragma once

#include <ostream>
#include <istream>

class CFraction
{
	public:
		// ==============================================
		// ====Constructors, destructors=================
		// vychozi konstruktor,
		CFraction ( int num = 0, int denom = 1 );
        // konstruktor s citatelem,
        // konstruktor s citatelem i jmenovatelem
		// destruktor
		// neni potreba

		// ==============================================
		// ==============Operatory+-*/===================
		//CFraction operator + ( /*const CFraction & lhs,*/ const CFraction & rhs ) const;
								 // this
		// op +
		friend CFraction operator + ( const CFraction & lhs, const CFraction & rhs );
		// op -
		friend CFraction operator - ( const CFraction & lhs, const CFraction & rhs );
		// op *
		friend CFraction operator * ( const CFraction & lhs, const CFraction & rhs );
		// op /
		friend CFraction operator / ( const CFraction & lhs, const CFraction & rhs );

		// ==============================================
		// ============== Operatory@= ===================
		// op +=
		CFraction & operator += ( const CFraction & rhs );
		// op -=
		CFraction & operator -= ( const CFraction & rhs );
		// op *=
		CFraction & operator *= ( const CFraction & rhs );
		// op /=
		CFraction & operator /= ( const CFraction & rhs );

		// ==============================================
		// ============= Operatory++-- ==================
		// op postfix ++
		CFraction operator ++ ( int n );
		// op postfix --
		CFraction operator -- ( int n );
		// op prefix ++
		CFraction & operator ++ ( void );
		// op prefix --
		CFraction & operator -- ( void );

		// ==============================================
		// ================ Unarni - ====================
		// op unarni - (*-1)
		CFraction operator - ( void ) const;

		// ==============================================
		// ============== Compare =======================
		// op <  
		friend bool operator <  ( const CFraction & lhs, const CFraction & rhs );
		// op <= 
		friend bool operator <= ( const CFraction & lhs, const CFraction & rhs );
		// op == 
		friend bool operator == ( const CFraction & lhs, const CFraction & rhs );
		// op != 
		friend bool operator != ( const CFraction & lhs, const CFraction & rhs );
		// op >= 
		friend bool operator >= ( const CFraction & lhs, const CFraction & rhs );
		// op >  
		friend bool operator >  ( const CFraction & lhs, const CFraction & rhs );

		// ==============================================
		// ============= IO operators ===================
		// op << na vypis do nejakeho streamu
		friend std::ostream & operator << ( std::ostream & os, const CFraction & rhs );
		// op >> na cteni z nejakeho streamu
		friend std::istream & operator >> ( std::istream & is, CFraction & rhs );

	private:
		// ==============================================
		// ===============private vars===================
		void normalize ( void );
		int m_numerator;
		int m_denominator;
};
