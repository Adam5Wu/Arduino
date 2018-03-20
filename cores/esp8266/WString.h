/*
 WString.h - String library for Wiring & Arduino
 ...mostly rewritten by Paul Stoffregen...
 Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
 Copyright 2011, Paul Stoffregen, paul@pjrc.com
 Modified by Zhenyu Wu <Adam_5Wu@hotmail.com> for VFATFS, 2017.01

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef String_class_h
#define String_class_h
#ifdef __cplusplus

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pgmspace.h>

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
class StringSumHelper;

// an abstract class used as a means to proide a unique pointer type
// but really has no body
class __FlashStringHelper;
#define FPSTR(pstr_pointer) \
	(reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#define F(string_literal) (FPSTR(PSTR(string_literal)))

// The string class
class String {
		// use a function pointer to allow for "if (s)" without the
		// complications of an operator bool(). for more information, see:
		// http://www.artima.com/cppsource/safebool.html
		typedef void (String::*StringIfHelperType)() const;
		void StringIfHelper() const {}

	public:
		// constructors
		// creates a copy of the initial value.
		// if the initial value is null or invalid, or if memory allocation
		// fails, the string will be marked as invalid (i.e. "if (s)" will
		// be false).
		String(const char *cstr = "", size_t len = 0);
		String(String const &str);
		String(const __FlashStringHelper *str, size_t len = 0);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
		String(String &&rval);
		String(StringSumHelper &&rval);
#endif
		explicit String(char c, size_t count = 1);
		explicit String(unsigned char, unsigned char base = 10);
		explicit String(int, unsigned char base = 10);
		explicit String(unsigned int, unsigned char base = 10);
		explicit String(long, unsigned char base = 10);
		explicit String(unsigned long, unsigned char base = 10);
		explicit String(float, unsigned char decimalPlaces = 2);
		explicit String(double, unsigned char decimalPlaces = 2);
		~String(void);

		static String const EMPTY;

		// memory management
		// return true on success, false on failure (in which case, the string
		// is left unchanged).
		unsigned char reserve(unsigned int size);
		inline unsigned int length(void) const { return buffer? len: 0; }

		// creates a copy of the assigned value.  if the value is null or
		// invalid, or if the memory allocation fails, the string will be
		// marked as invalid ("if (s)" will be false).
		String & operator = (String const &rhs);
		String & operator = (const char *cstr);
		String & operator = (const __FlashStringHelper *str);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
		String & operator = (String &&rval);
		String & operator = (StringSumHelper &&rval);
#endif

		// concatenate (works w/ built-in types)

		// returns true on success, false on failure (in which case, the string
		// is left unchanged).  if the argument is null or invalid, the
		// concatenation is considered unsucessful.
		unsigned char concat(String const &str);
		unsigned char concat(const char *cstr);
		unsigned char concat(char c, size_t count = 1);
		unsigned char concat(unsigned char num, unsigned char base = 10);
		unsigned char concat(int num, unsigned char base = 10);
		unsigned char concat(unsigned int num, unsigned char base = 10);
		unsigned char concat(long num, unsigned char base = 10);
		unsigned char concat(unsigned long num, unsigned char base = 10);
		unsigned char concat(float num, unsigned char decimalPlaces = 2);
		unsigned char concat(double num, unsigned char decimalPlaces = 2);

		unsigned char concat(const __FlashStringHelper *str);
		unsigned char concat(const char *cstr, unsigned int length);

		// if there's not enough memory for the concatenated value, the string
		// will be left unchanged (but this isn't signalled in any way)
		String & operator +=(String const &rhs) { return concat(rhs), *this; }
		String & operator +=(const char *cstr) { return concat(cstr), *this; }
		String & operator +=(char c) { return concat(c), *this; }
		String & operator +=(unsigned char num){ return concat(num), *this; }
		String & operator +=(int num) { return concat(num), *this; }
		String & operator +=(unsigned int num) { return concat(num), *this; }
		String & operator +=(long num) { return concat(num), *this; }
		String & operator +=(unsigned long num) { return concat(num), *this; }
		String & operator +=(float num) { return concat(num), *this; }
		String & operator +=(double num) { return concat(num), *this; }
		String & operator += (const __FlashStringHelper *str)
		{ return concat(str), *this; }

		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			String const &rhs);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			const char *cstr);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			char c);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			unsigned char num);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			int num);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			unsigned int num);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			long num);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			unsigned long num);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			float num);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			double num);
		friend StringSumHelper & operator +(const StringSumHelper &lhs,
			const __FlashStringHelper *rhs);

		// comparison (only works w/ Strings and "strings")
		int compareTo(String const &s, bool ignoreCase = false) const;
		int compareTo(const char *cstr, bool ignoreCase = false) const;
		int compareTo(const __FlashStringHelper *str,
			bool ignoreCase = false) const;

		bool equals(String const &s, bool ignoreCase = false) const;
		bool equals(const char *cstr, bool ignoreCase = false) const;
		bool equals(const __FlashStringHelper *str,
			bool ignoreCase = false) const;

		bool equalsIgnoreCase(String const &s) const;
		bool equalsIgnoreCase(const char *cstr) const;
		bool equalsIgnoreCase(const __FlashStringHelper *str) const;

		bool equalsConstantTime(String const &s) const;

		bool operator ==(String const &rhs) const { return equals(rhs); }
		bool operator ==(const char *cstr) const { return equals(cstr); }
		bool operator ==(const __FlashStringHelper *str) const
		{ return equals(str); }
		bool operator !=(String const &rhs) const { return !equals(rhs); }
		bool operator !=(const char *cstr) const { return !equals(cstr); }
		bool operator !=(const __FlashStringHelper *str) const
		{ return !equals(str); }

		bool operator <(String const &rhs) const;
		bool operator >(String const &rhs) const;
		bool operator <=(String const &rhs) const;
		bool operator >=(String const &rhs) const;

		bool startsWith(String const &prefix, bool ignoreCase = false) const;
		bool startsWith(String const &prefix, unsigned int offset,
			bool ignoreCase = false) const;
		bool startsWith(const char *cstr, unsigned int offset,
			bool ignoreCase = false) const;
		bool startsWith(const char *cstr, unsigned int bLen,
			unsigned int offset, bool ignoreCase = false) const;
		bool startsWith(const __FlashStringHelper *str, unsigned int offset = 0,
			bool ignoreCase = false) const;
		bool startsWith(const __FlashStringHelper *str, unsigned int bLen,
			unsigned int offset, bool ignoreCase = false) const;

		bool endsWith(String const &suffix, bool ignoreCase = false) const;
		bool endsWith(String const &prefix, unsigned int offset,
			bool ignoreCase = false) const;
		bool endsWith(const char *cstr, unsigned int offset,
			bool ignoreCase = false) const;
		bool endsWith(const char *cstr, unsigned int bLen,
			unsigned int offset, bool ignoreCase = false) const;
		bool endsWith(const __FlashStringHelper *str,
			unsigned int offset = 0, bool ignoreCase = false) const;
		bool endsWith(const __FlashStringHelper *str,
			unsigned int bLen, unsigned int offset,
			bool ignoreCase = false) const;

		// character acccess
		char charAt(unsigned int index) const;
		void setCharAt(unsigned int index, char c);
		char operator [](unsigned int index) const;
		char& operator [](unsigned int index);
		void getBytes(unsigned char *buf, unsigned int bufsize,
			unsigned int index = 0) const;
		void toCharArray(char *buf, unsigned int bufsize,
			unsigned int index = 0) const
		{ getBytes((unsigned char *) buf, bufsize, index); }

		const char* c_str() const
		{ return buffer? buffer : EMPTY.begin(); }
		char* begin() { return &(*this)[0]; }
		char* end() { return begin() + length(); }
		const char* begin() const { return c_str(); }
		const char* end() const { return begin() + length(); }

		// search
		int indexOf(char ch) const;
		int indexOf(char ch, unsigned int fromIndex) const;
		int indexOf(String const &str) const;
		int indexOf(String const &str, unsigned int fromIndex) const;
		int lastIndexOf(char ch) const;
		int lastIndexOf(char ch, unsigned int fromIndex) const;
		int lastIndexOf(String const &str) const;
		int lastIndexOf(String const &str, unsigned int fromIndex) const;

		String substring(unsigned int beginIndex) const
		{ return substring(beginIndex, len); }
		String substring(unsigned int beginIndex, unsigned int endIndex) const;

		// modification
		void replace(char find, char replace);
		void replace(String const& find, String const& replace);
		void remove(unsigned int index);
		void remove(unsigned int index, unsigned int count);
		void toLowerCase(void);
		void toUpperCase(void);
		void trim(void);

		// parsing/conversion
		int toInt(void) const;
		float toFloat(void) const;

		bool toInt(int &val, unsigned char base = 10) const;
		bool toUInt(unsigned int &val, unsigned char base = 10) const;
		bool toLong(long &val, unsigned char base = 10) const;
		bool toULong(unsigned long &val, unsigned char base = 10) const;
		bool toLLong(long long &val, unsigned char base = 10) const;
		bool toULLong(unsigned long long &val, unsigned char base = 10) const;
		bool toFloat(float &val) const;
		bool toDouble(double &val) const;

		bool empty(void) const;
		void clear(bool free = false);

		operator StringIfHelperType() const
		{ return empty() ? 0 : &String::StringIfHelper; }

	protected:
		char *buffer;           // the actual char array
		unsigned int capacity;  // the array length minus one (for the '\0')
		unsigned int len;       // the String length (not counting the '\0')

		void init(void);
		void invalidate(void);
		unsigned char changeBuffer(unsigned int maxStrLen);

		// copy and move
		String & copy(const char *cstr, unsigned int length);
		String & copy(const __FlashStringHelper *pstr, unsigned int length);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
		void move(String &rhs);
#endif
};

class StringSumHelper: public String {
	public:
		StringSumHelper(String const &s) : String(s) {}
		StringSumHelper(const char *p) : String(p) {}
		StringSumHelper(char c) : String(c) {}
		StringSumHelper(unsigned char num) : String(num) {}
		StringSumHelper(int num) : String(num) {}
		StringSumHelper(unsigned int num) : String(num) {}
		StringSumHelper(long num) : String(num) {}
		StringSumHelper(unsigned long num) : String(num) {}
		StringSumHelper(float num) : String(num) {}
		StringSumHelper(double num) : String(num) {}
};

#endif  // __cplusplus
#endif  // String_class_h
