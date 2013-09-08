#ifndef RECOGNITIONEXCEPTION_H 
#define RECOGNITIONEXCEPTION_H  

#include <exception>
#include <string>

class RecognitionException : public std::exception
{
protected:
	std::string error;
public:
	RecognitionException(std::string error = "")
	{
		this->error = error;
	}

	~RecognitionException() throw()
	{

	}

	std::string what()
	{
		return this->error;
	}
};

#endif 