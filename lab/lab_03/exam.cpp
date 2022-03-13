#include <string>
#include <iostream>
#include <cassert>

struct SStudent
{
	std::string m_name;
	int m_knowledge_level;

	bool passed ( int difficulty_level ) const
	{
		return m_knowledge_level >= difficulty_level;
	}
};

class CTeacher
{
	public:
		CTeacher ( const std::string & name )
		:m_name( name ),
		 m_passed_students( 0 )
		{}
		// ====================================================
		size_t passed_students ( void ) const
		{
			return m_passed_students;
		}
		// ====================================================
		void passed_students ( size_t newly_passed )
		{
			m_passed_students += newly_passed;
		}
		// ====================================================
	private:
		std::string m_name;
		size_t m_passed_students;
};

class CList
{
	public:
		// ====================================================
		// Kopirovani se bude resit na dalsich cvicenich,
		// tentokrat si ho tedy odpustime.
		// ====================================================
		CList              ( const CList & ) = delete;
		CList & operator = ( const CList & ) = delete;
		CList              ( CList && )      = delete;
		CList & operator = ( CList && )      = delete;
		// ====================================================
		CList ( void )
		: m_used( 0 ), m_capacity( 0 ), m_data( nullptr )
		{
		}
		// ====================================================
		~CList ( void )
		{
			delete [] m_data;
			m_used = m_capacity = 0;
		}
		// ====================================================
		CList & add_student ( const SStudent & student )
		{
			if ( m_used >= m_capacity )
				resize();
			m_data[ m_used++ ] = student;
			return * this;
		}
		// ====================================================
		const SStudent & get_student ( size_t idx ) const
		{
			return m_data[ idx ];
		}
		// ====================================================
		size_t size ( void ) const
		{
			return m_used;
		}
		// ====================================================
		const SStudent * begin ( void ) const;
		// ====================================================
		const SStudent * end ( void ) const;
		// ====================================================
	private:
		void resize ( void ) 
		{
			m_capacity = m_capacity * 2 + 1;
			SStudent * new_data = new SStudent[ m_capacity ];
			for ( size_t i = 0; i < m_used; i++ )
				new_data[ i ] = m_data[ i ];
			delete [] m_data;
			m_data = new_data;
		}
		size_t m_used;
		size_t m_capacity;
		SStudent * m_data;
};

class CExam
{
	public:
		CExam ( CTeacher & teacher, int difficulty_level );
		// =============================================================
		void start_exam ( const CList & student_list ) const;
		// =============================================================
	private:
		CTeacher & m_teacher;
		int m_difficulty_level;
};

CExam::CExam ( CTeacher & teacher, int difficulty_level )
	: m_teacher( teacher ), m_difficulty_level( difficulty_level )
	{}

void CExam::start_exam ( const CList & student_list ) const
{
	size_t passed_students = 0;
	for ( size_t i = 0; i < student_list.size(); i++ ) {
		//(1) m_teacher.passed_students( student_list.get_student( i ).passed( m_difficulty_level ) );
		passed_students += student_list.get_student( i ).passed( m_difficulty_level );
		//(2) if ( student_list.get_student( i ).passed( m_difficulty_level ) )
		//(2)	 passed_students++;
	}
	m_teacher.passed_students( passed_students );
}

int main ( void )
{
	CTeacher honza("Honza"), franta("Franta");
	assert( honza.passed_students() == 0 );

	CList student_list1;
	student_list1.add_student({"Tomas", 5}).add_student({"Patrik", 24}).add_student({"Pepa", 25}).add_student({"Jirka", 88});

	CExam exam1(honza, 25);
	exam1.start_exam(student_list1);
	assert( honza.passed_students() == 2 );

	exam1.start_exam(student_list1);
	assert( honza.passed_students() == 4 );

	CExam exam2(honza, 50);
	exam2.start_exam(student_list1);
	assert( honza.passed_students() == 5 );

	CList student_list2;
	CExam exam3(franta, 0);
	exam3.start_exam(student_list2);
	assert( franta.passed_students() == 0 );

	exam3.start_exam(student_list1);
	assert( franta.passed_students() == 4 );

	student_list2.add_student({"Franta", 99});
	
	CExam exam4(honza, 100), exam5(franta, 99);
	exam4.start_exam(student_list2);
	assert ( honza.passed_students() == 5 );

	exam5.start_exam(student_list2);
	assert ( franta.passed_students() == 5 );

	exam3.start_exam(student_list1);
	assert ( franta.passed_students() == 9 );
	return 0;
}
