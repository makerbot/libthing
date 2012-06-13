/**
   MiracleGrue - Model Generator for toolpathing. <http://www.grue.makerbot.com>
   Copyright (C) 2011 Far McKon <Far@makerbot.com>, Hugo Boyer (hugo@makerbot.com)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

*/
#ifndef _PLATFORM_TOOLS_H_
#define _PLATFORM_TOOLS_H_ (1)


#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>

#include <fstream>
#include <stdint.h>

#include "Exception.h"
#include "Scalar.h"


#ifdef __BYTE_ORDER
# if __BYTE_ORDER == __LITTLE_ENDIAN
#  define I_AM_LITTLE_ENDIAN
# else
#  if __BYTE_ORDER == __BIG_ENDIAN
#   define I_AM_BIG_ENDIAN
#  else
#error "Unknown byte order!"
#  endif
# endif
#endif /* __BYTE_ORDER */

#ifdef I_AM_BIG_ENDIAN
static inline void convertFromLittleEndian32(uint8_t* bytes)
{
    uint8_t tmp = bytes[0];
    bytes[0] = bytes[3];
    bytes[3] = tmp;
    tmp = bytes[1];
    bytes[1] = bytes[2];
    bytes[2] = tmp;
}
static inline void convertFromLittleEndian16(uint8_t* bytes)
{
    uint8_t tmp = bytes[0];
    bytes[0] = bytes[1];
    bytes[1] = tmp;
}
#else
static inline void convertFromLittleEndian32(uint8_t* bytes)
{
}

static inline void convertFromLittleEndian16(uint8_t* bytes)
{
}
#endif

// WIN32 compatibility stuff
#ifdef WIN32

#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define M_PI 3.14159265358979323846

#endif // WIN32

namespace libthing {

/// This file contains Platform specific functions and classes
///



/// Abstraction for a clock/tick object
class ClockAbstractor
{
public:
	::std::string now() const
	{
		time_t t = time(0);   // get time now
#if defined(WIN32) && defined(_MSVC_VER)
#pragma warning(disable:4996)
#endif
		struct tm * now = localtime( & t );
		::std::stringstream ss;
		ss << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<  now->tm_mday << " "
		 <<  now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec;
		return ss.str();
	}
};

/// Abstraction for file system tools and access

class FileSystemAbstractor
{
public:

	char getPathSeparatorCharacter() const;

	::std::string ExtractDirectory(const char *directoryPath) const;
	::std::string ExtractFilename(const char* filename) const;
	::std::string ChangeExtension(const char* filename, const char* extension) const;
	::std::string removeExtension(const char *filename) const;

	::std::string pathJoin(std::string path, std::string filename) const;

    int guarenteeDirectoryExists(const char* dirPath);

};


/// Abstraction for file system tools and access
class OperatingSystem
{
public:
	ClockAbstractor clock;
	FileSystemAbstractor fileSystem;

    static std::ostream &log();

};



std::string stringify(Scalar x)
{
  std::ostringstream o;
  if (!(o << x))
    throw Exception("stringify(Scalar)");
  return o.str();
}

std::string stringify(size_t x)
{
  std::ostringstream o;
  if (!(o << x))
    throw Exception("stringify(double)");
  return o.str();
}

#if defined(WIN32) && defined(_MSVC_VER)
#pragma warning(disable:4996)
#endif


} //end namespace


#endif /*_PLATFORM_TOOLS_H_*/
