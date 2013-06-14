// File Name: Exceptions.h
// Description:
//    This fils includes several common exceptions may happen in bitgen process.
//    Also we define some macros here to simplify the exceptional coding.
// Author: Chi Wang
// Date: 2013-5-30

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <exception>
#include <string>

#define MY_THROW(ExClass, args)                             \
	do                                                         \
	{                                                          \
	ExClass e(args);                                       \
	e.Init(__FILE__, __FUNCTION__, __LINE__);              \
	throw e;                                               \
	}                                                          \
	while (false)     

#define MY_DEFINE_EXCEPTION(ExClass, Base)                     \
	ExClass(const std::string& msg = "") throw()               \
	: Base(msg)                                            \
	{}                                                         \
	\
	~ExClass() throw() {}                                        \
	\
	/* override */ std::string GetClassName() const            \
	{                                                          \
	return #ExClass;                                       \
	}

class ExceptionBase : public std::exception
{
public:
	ExceptionBase(const std::string& msg = "") throw();

	virtual ~ExceptionBase() throw();

	void Init(const char* file, const char* func, int line);

	virtual std::string GetClassName() const;

	virtual std::string GetMessage() const;

	const char* what() const throw();

	const std::string& ToString() const;

	//std::string GetStackTrace() const;

protected:
	std::string mMsg;
	const char* mFile;
	const char* mFunc;
	int mLine;

private:
	//enum { MAX_STACK_TRACE_SIZE = 50 };
	//void* mStackTrace[MAX_STACK_TRACE_SIZE];
	//size_t mStackTraceSize;
	mutable std::string mWhat;
};


//! CilInfoMissException
/*!
	CilInfoMissException denotes that there is some configuration information
	missed in the cil. The cil developer should notice these exceptions and
	fill in the missing info.
*/
class CilInfoMissException : public ExceptionBase 
{
public:
	MY_DEFINE_EXCEPTION( CilInfoMissException, ExceptionBase )
};

class UnknownAttrTypeException : public ExceptionBase
{
public:
	MY_DEFINE_EXCEPTION( UnknownAttrTypeException, ExceptionBase )
};

#define CONDITIONAL_THROW( cond, expc, msg ) \
	if (!cond) \
		MY_THROW(expc, msg);

#endif