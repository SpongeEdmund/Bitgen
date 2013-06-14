#ifndef _UTILS_HPP_
#define _UTILS_HPP_
	
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <ctime>
#include <cctype>
#include <iosfwd>
#include <algorithm>

using std::istream;
using std::ostream;
using std::string;
// 2-D point for bitgen
// 只支持加减运算
class Point
{
public:
	// 只能通过x，y来初始化
	Point( int x = -1, int y = -1 ) : _x(x), _y(y) {}

	Point& operator = (const Point& p)
	{
		if ( this == &p ) return *this;
		_x = p._x;
		_y = p._y;
		return *this;
	}

	friend const Point operator+ ( const Point & lhs, const Point & rhs );
	friend ostream & operator<< ( ostream & os, const Point & pt );
	friend istream & operator>> ( istream & is, Point & pt );
	friend bool operator== ( const Point & lhs, const Point & rhs );

	int getX() const { return _x; }
	int getY() const { return _y; }
private:
	int _x;
	int _y;
};

inline
const Point operator+ (const Point &lhs, const Point &rhs)
{
	return Point(lhs._x + rhs._x, lhs._y + rhs._y);
}

inline
ostream& operator<< ( ostream &os, const Point &pt )
{
	os << pt._x << "," << pt._y;
	return os;
}

inline
	istream& operator>> ( istream &is, Point &pt )
{
	//There are two expressions of point
	//	1. m,n or mXn
	//	2. XmYn or RmCn or whatever in this format
	//	We can determine which type of expression is from the first char
	//	As we want to roll back to the start point of the stream, we use seekg(0) to implement this
	//char fc;
	std::istream::iostate oldState = is.rdstate();

	char noUse;
	int x, y ;
	if ( is >> x >> noUse >> y ) {
		pt = Point( x, y );
		return is;
	}

	is.clear(oldState);

	if ( is >> noUse >> x >> noUse >> y )
		pt = Point( x, y );
	return is;
}

inline
bool operator==( const Point& lhs, const Point& rhs )
{
	return lhs._x == rhs._x && lhs._y == rhs._y;
}

// convert string to upper case
inline
void toUpper( std::string & str )
{
	std::transform( str.begin(), str.end(), str.begin(), ::toupper );
}
// convert string to lower case
inline
void toLower( std::string & str )
{
	std::transform( str.begin(), str.end(), str.begin(), ::tolower );
}
// convert hex str to bit vector
void Hex2BitVec( std::vector<int>& bin, const std::string& hexExp );
// convert bin str to bit vector
void Bin2BitVec( std::vector<int>& bin, const std::string & binExp );

const std::string GetCurrentTime();

void ShowRunningInfo( std::ostream& os, const std::string& info, int indent = 0 );

unsigned BitVec2U( const std::vector<int>& bitVec );

#endif