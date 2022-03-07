#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
//#include <assert.h>

struct CPoint
{
	long m_X;
	long m_Y;
	// ====================================================================
  
  CPoint subtract ( const CPoint & other ) const {
    return { m_X - other.m_X, m_Y - other.m_Y };
  }

  long lenght_squared ( ) const {
    return m_X * m_X + m_Y * m_Y;
  }
};

class CCircle
{
	public:
		CCircle & init ( const CPoint & center, unsigned int radius )
		{
      m_Center = center;
      m_Radius_Squared = radius * radius;      
      return *this;
		}
		// ====================================================================
		CCircle & change_center ( const CPoint & newCenter )
		{
      m_Center = newCenter;
      return *this;
		}
		// ====================================================================
		bool contains_point ( const CPoint & point ) const 
		{
      long sqd = point.subtract(m_Center).lenght_squared();
      return sqd <= m_Radius_Squared;
		}
		// ====================================================================
	private:
    CPoint m_Center;
    long m_Radius_Squared;
};

struct CTriangle
{
	static bool valid_triangle ( const CPoint & a, const CPoint & b, const CPoint & c )
	{
    long x = a.m_X * (b.m_Y - c.m_Y) + b.m_X * (c.m_Y - a.m_Y) + c.m_X * (a.m_Y - b.m_Y);
    if ( x==0 )
    {
      return false;
    }
		else
    {
      return true;
    }
	}
	// ====================================================================
};

int main ( void )
{
	std::vector<CPoint> points = {{5,5}, {3,3}, {1,1}, {-6,-3}, {-2,2},
					   			              {3,1}, {-3,3}, {3,-4}, {-6,-5}, {1,2}};
	CCircle circle;
	assert ( circle.init({15,15}, 5).change_center({-2,-2}).contains_point({-2,-2}) );

	assert ( ! circle.contains_point({15,15}) );

	size_t counter = 0;
	for ( CPoint & point : points )
		counter += circle.contains_point(point);
	
	assert ( counter == 5 );

	
	// Pro spusteni tohoto testu pouze odkomentujte tuto sekci
	auto const_test = [](const CCircle & circle, const std::vector<CPoint> & points){
		size_t counter = 0;
		for ( const CPoint & point : points )
			counter += circle.contains_point(point);
		return counter;
	};
	assert ( const_test(circle, points) == 5 );
	

	assert ( ! CTriangle::valid_triangle(points[0], points[1], points[2]) );
	assert ( ! CTriangle::valid_triangle(points[4], points[9], {-8,2})    );
	assert ( ! CTriangle::valid_triangle(points[1], points[5], {3,-5})    );
	assert ( ! CTriangle::valid_triangle(points[0], points[1], points[2]) );
	assert ( ! CTriangle::valid_triangle(points[0], points[0], points[0]) );
	assert ( ! CTriangle::valid_triangle(points[0], points[0], points[1]) );
	
	assert (   CTriangle::valid_triangle(points[0], points[2], points[4]) );
	assert (   CTriangle::valid_triangle(points[0], points[1], points[4]) );
	assert (   CTriangle::valid_triangle(points[0], points[1], points[3]) );
	assert (   CTriangle::valid_triangle(points[0], points[1], points[5]) );
	assert (   CTriangle::valid_triangle(points[0], points[1], points[6]) );
	assert (   CTriangle::valid_triangle(points[0], points[1], points[7]) );
	assert (   CTriangle::valid_triangle(points[0], points[1], points[8]) );
	assert (   CTriangle::valid_triangle(points[0], points[1], points[9]) );
  
}