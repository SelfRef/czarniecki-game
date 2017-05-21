/*
 * Copyright 2010-2012 Maarten Baert
 * maarten-baert@hotmail.com
 * http://www.maartenbaert.be/
 * 
 * This file is part of Http Dll 2.
 * 
 * Http Dll 2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Http Dll 2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Http Dll 2. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HASH_H
#define HASH_H

#include <stdint.h>

class MD5 {
	
	private:
	unsigned int blocks, used; // 0 <= used < 64
	union {
		uint32_t w[4]; // 4 words
		uint8_t x[16]; // 16 bytes
	} result;
	union {
		uint32_t w[16]; // 16 words
		uint8_t x[64]; // 64 bytes
	} block;
	
	public:
	void Begin();
	bool ReadFile(const char* fname);
	void ReadMem(const void* data, unsigned int datalen);
	void End();
	inline const uint8_t* Result() { return result.x; }
	
};

class SHA1 {
	
	private:
	unsigned int blocks, used; // 0 <= used < 64
	union {
		uint32_t w[5]; // 5 words
		uint8_t x[20]; // 20 bytes
	} result;
	union {
		uint32_t w[16]; // 16 words
		uint8_t x[64]; // 64 bytes
	} block;
	
	public:
	void Begin();
	bool ReadFile(const char* fname);
	void ReadMem(const void* data, unsigned int datalen);
	void End();
	inline const uint8_t* Result() { return result.x; }
	
};

void bintohex(const void* input, char* output, unsigned int inputlen);
void hextobin(const char* input, void* output, unsigned int outputlen);

#endif // HASH_H

