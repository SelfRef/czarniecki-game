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

#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef>
#include <stdint.h>

class Buffer {
	
	private:
	char *data;
	unsigned int length, capacity;
	unsigned int pos;
	bool error;
	
	public:
	Buffer();
	~Buffer();
	void Swap(Buffer& other);
	
	void Clear();
	void SetPos(unsigned int newpos);
	void SetLength(unsigned int newlength);
	
	bool ReadFromFile(const char* filename);
	bool ReadFromFilePart(const char* filename, unsigned int _pos, unsigned int _len);
	bool WriteToFile(const char* filename);
	bool AppendToFile(const char* filename);
	
	void RC4Crypt(const char* key, unsigned int keylen);
	
	void ZLibCompress();
	bool ZLibUncompress();
	
	template<typename A> A ReadType();
	template<typename A> void WriteType(A x);
	uint32_t ReadUIntV();
	void WriteUIntV(uint32_t x);
	int32_t ReadIntV();
	void WriteIntV(int32_t x);
	const char* ReadString();
	void WriteString(const char* string);
	void ReadData(void* ptr, unsigned int len);
	void WriteData(const void* ptr, unsigned int len);
	void ReadHex(char* ptr, unsigned int len);
	void WriteHex(const char* ptr, unsigned int len);
	
	void WriteBuffer(const Buffer* b);
	void WriteBufferPart(const Buffer* b, unsigned int _pos, unsigned int _len);
	
	public:
	inline const char* ToString() { return (data == NULL)? "" : data; }
	inline char* GetData() { return data; }
	inline unsigned int GetPos() { return pos; }
	inline unsigned int GetLength() { return length; }
	inline bool IsAtEnd() { return (pos>=length); }
	inline bool GetError() { return error; }
	inline void ClearError() { error = false; }
	
};

template<typename A>
A Buffer::ReadType() {
	if(error) return A();
	if(pos + sizeof(A) > length) {
		error = true;
		return A();
	}
	A *a = (A*)(data + pos);
	pos += sizeof(A);
	return *a;
}
template<typename A>
void Buffer::WriteType(A x) {
	if(sizeof(A) != 0) {
		unsigned int p = length;
		SetLength(length + sizeof(A));
		*(A*)(data + p) = x;
	}
}

#endif // BUFFER_H

