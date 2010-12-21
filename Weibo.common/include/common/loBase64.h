/*
Copyright (c) 2005-2007 by Jakob Schroeter <js@camaya.net>
This file is part of the gloox library. http://camaya.net/gloox

This software is distributed under a license. The full license
agreement can be found in the file LICENSE in this distribution.
This software may not be copied, modified, sold or distributed
other than expressed in the named license agreement.

This software is distributed without any warranty.
*/


#ifndef __BASE64_H__
#define __BASE64_H__

#include <common/loCommon.h>
#include <string>

DEFINE_NAMESPACE(locom)

/**
* @brief An implementation of the CloBase64 data encoding (RFC 3548)
*
* @author Jakob Schroeter <js@camaya.net>
* @since 0.8
*/
class loCOMMON_API CloBase64
{

public:
	/**
	* CloBase64-encodes the input according to RFC 3548.
	* @param input The data to encode.
	* @return The encoded string.
	*/
	static const std::string encode64( const std::string& input );

	/**
	* CloBase64-decodes the input according to RFC 3548.
	* @param input The encoded data.
	* @return The decoded data.
	*/
	static const std::string decode64( const std::string& input );

	//Base64Encod
	//Encode string
	//Param: buftoenc,input string to encode
	//       bufsize,input string length
	//       encbug, output string encoded
	//Return value:Encoded string length??? if fail return -1???
	static int Base64Encod(const char *buftoenc,int bufsize,char *encbuf);

	//Base64Decod
	//Encode string
	//Param: buftodec,input string to decode
	//       bufsize,input string length
	//       decbug, output string decoded
	//Return value:Decoded string length??? if fail return -1???
	static int Base64Decod(const char *buftodec,int bufsize,char *decbuf);

private:
	static const char alphabet64[];
	static const std::string::size_type table64[];
	static const char Base64Table[];
	static const char pad;
	static const std::string::size_type np;
};

END_NAMESPACE(locom)

#endif // BASE64_H__
