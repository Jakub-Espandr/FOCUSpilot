/*
EEPROMTyped.h - EEPROM Typed Library

The EEPROM Typed Library is a lightweight static library for reading and
writing standard Arduino data types to the EEPROM.  The library provides a
write and read method call that takes a memory address and standard data 
type variable which is deconstructed into or reconstructed from byte values
written or read from the EEPROM memory.  The library also provides a sizeof
used to return the variable size in bytes which can be used to calculate 
memory addresses.  The library works with array, bool, byte, char, double, 
float, int, long, short, size_t, string, unsigned char, unsigned int, 
unsigned long and word data types.

MIT License

Copyright 2014-2021 Terence F. Golla

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef EEPROMTyped_h
#define EEPROMTyped_h

#include <Arduino.h>
#include <EEPROM.h>

struct EEPROMTypedClass
{
	// Note: Implementation of templated functions must reside in the header file for the compiler
	// to create the correct type instantiations when building main, hense preventing error messages
	// like... undefined reference to `int EEPROMTyped::read<int>(int, int&)'
	// ref: https://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor

	// Returns the byte size of the variable.
	template <class T>
	int sizeOf(T &variable)
	{
		return sizeof(variable);
	}

	// Writes the variable value to the EEPROM memory address.
	template <class T>
	int write(int address, const T &variable)
	{
		const byte *p = (const byte *)(const void *)&variable;
		unsigned int i;
		for (i = 0; i < sizeof(variable); i++)
		{
			const byte b = *p;
			if (EEPROM.read(address) != b)
				EEPROM.write(address++, b), ++p;
			else
				address++, p++;
		}
		return i;
	}

	// Reads the variables value from the EEPROM memory address.
	template <class T>
	int read(int address, T &variable)
	{
		byte *p = (byte *)(void *)&variable;
		unsigned int i;
		for (i = 0; i < sizeof(variable); i++)
			*p++ = EEPROM.read(address++);
		return i; // Returns the variable size in bytes.
	}
};

static EEPROMTypedClass EEPROMTyped;

#endif
