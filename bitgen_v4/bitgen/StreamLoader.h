#ifndef __STREAM_LOADER_H__
#define __STREAM_LOADER_H__


#include "Cil.h"

#include "SramBit.h"
#include "utils.h"
class BitStream;


class StreamLoader
{
public:
	StreamLoader( const Cil &, BitStream &);
	~StreamLoader(void);

	void toStream( const SramBit & );

private:
	BitStream* _stream;
	const Cil* _cil;

	size_t getTileOrder( const Point & ) const;
	Point getSramScale( size_t tileOrder ) const;
	size_t getStreamOrder( const Point &, const Point & ) const ;
};


#endif