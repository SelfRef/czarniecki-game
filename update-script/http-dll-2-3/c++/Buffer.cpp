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

#include "Buffer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <new>
#include <algorithm>

#include "Hash.h"
#include "zlib.h"

Buffer::Buffer() {
	data = NULL;
	length = 0;
	capacity = 0;
	pos = 0;
	error = false;
}

Buffer::~Buffer() {
	free(data);
}

void Buffer::Swap(Buffer& other) {
	std::swap(data, other.data);
	std::swap(length, other.length);
	std::swap(capacity, other.capacity);
	std::swap(pos, other.pos);
	std::swap(error, other.error);
}

void Buffer::Clear() {
	free(data);
	data = NULL;
	length = 0;
	capacity = 0;
	pos = 0;
	error = false;
}

void Buffer::SetPos(unsigned int newpos) {
	pos = (newpos > length)? length : newpos;
}

void Buffer::SetLength(unsigned int newlength) {
	unsigned int required = newlength + 1;
	if(required < 16) required = 16;
	unsigned int new_capacity;
	if(required > capacity) {
		new_capacity = required + required / 2;
	} else if(required <= capacity / 2) {
		new_capacity = required + required / 4;
	} else {
		// no reallocation
		length = newlength;
		if(pos > length) {
			pos = length;
		}
		data[length] = '\0';
		return;
	}
	void *temp = realloc(data, new_capacity);
	if(temp == NULL) {
		throw std::bad_alloc();
	}
	data = (char*)(temp);
	length = newlength;
	capacity = new_capacity;
	if(pos > length) {
		pos = length;
	}
	data[length] = '\0';
}

bool Buffer::ReadFromFile(const char* filename) {
	Clear();
	FILE *f = fopen(filename, "rb");
	if(f == NULL) return false;
	try {
		fseek(f, 0, SEEK_END);
		unsigned int size = ftell(f);
		fseek(f, 0, SEEK_SET);
		if(size == (unsigned int)(-1)) {
			fclose(f);
			return false;
		}
		SetLength(size);
		if(fread(data, 1, size, f) != size) {
			Clear();
			fclose(f);
			return false;
		}
	}
	catch(...) {
		fclose(f);
		throw;
	}
	fclose(f);
	return true;
}

bool Buffer::ReadFromFilePart(const char* filename, unsigned int _pos, unsigned int _len) {
	Clear();
	FILE *f = fopen(filename, "rb");
	if(f == NULL) return false;
	try {
		if(fseek(f, _pos, SEEK_SET) != 0) {
			fclose(f);
			return false;
		}
		SetLength(_len);
		if(fread(data, 1, _len, f) != _len) {
			Clear();
			fclose(f);
			return false;
		}
	}
	catch(...) {
		fclose(f);
		throw;
	}
	fclose(f);
	return true;
}

bool Buffer::WriteToFile(const char* filename) {
	FILE *f;
	f = fopen(filename, "wb");
	if(f == NULL) return false;
	if(fwrite(data, 1, length, f) != length) {
		fclose(f);
		return false;
	}
	fclose(f);
	return true;
}

bool Buffer::AppendToFile(const char* filename) {
	FILE *f;
	f = fopen(filename, "ab");
	if(f == NULL) return false;
	if(fwrite(data, 1, length, f) != length) {
		fclose(f);
		return false;
	}
	fclose(f);
	return true;
}

void Buffer::RC4Crypt(const char* key, unsigned int keylen) {
	// 'key' may point to data in this buffer - this is not a problem but it's pretty pointless
	unsigned char table[256];
	for(unsigned int k = 0; k < 256; ++k) {
		table[k] = k;
	}
	unsigned char i = 0, j = 0;
	for(unsigned int k = 0; k < 256; ++k) {
		j = j + table[k] + key[k % keylen];
		std::swap(table[k], table[j]);
	}
	j = 0;
	for(unsigned int k = 0; k < length; ++k) {
		i += 1;
		j += table[i];
		std::swap(table[i], table[j]);
		data[k] ^= table[(unsigned char) (table[i] + table[j])];
	}
}

void Buffer::ZLibCompress() {
	Buffer out;
	unsigned int blocksize = length + 1000;
	out.SetLength(blocksize);
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.next_in = (Bytef*) data;
	strm.avail_in = length;
	strm.next_out = (Bytef*) out.GetData();
	strm.avail_out = blocksize;
	int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	if(ret != Z_OK) {
		throw std::bad_alloc();
	}
	try {
		for( ; ; ) {
			ret = deflate(&strm, Z_FINISH);
			if(ret == Z_STREAM_ERROR || ret == Z_MEM_ERROR) {
				throw std::bad_alloc();
			}
			if(ret == Z_STREAM_END) {
				deflateEnd(&strm);
				out.SetLength(out.GetLength() - strm.avail_out);
				break;
			}
			if(out.GetLength() > (1 << 30) || blocksize > (1 << 30)) {
				throw std::bad_alloc();
			}
			blocksize += blocksize / 2;
			out.SetLength(out.GetLength() + blocksize);
			strm.next_out = (Bytef*) (out.GetData() + out.GetLength() - blocksize);
			strm.avail_out = blocksize;
		}
	} catch(...) {
		deflateEnd(&strm);
		throw;
	}
	Swap(out);
}

bool Buffer::ZLibUncompress() {
	Buffer out;
	unsigned int blocksize = length + 1000;
	out.SetLength(blocksize);
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.next_in = (Bytef*) data;
	strm.avail_in = length;
	strm.next_out = (Bytef*) out.GetData();
	strm.avail_out = blocksize;
	int ret = inflateInit(&strm);
	if(ret == Z_MEM_ERROR) {
		throw std::bad_alloc();
	}
	if(ret != Z_OK) {
		return false;
	}
	try {
		for( ; ; ) {
			ret = inflate(&strm, Z_FINISH);
			if(ret == Z_STREAM_ERROR || ret == Z_MEM_ERROR) {
				throw std::bad_alloc();
			}
			if(ret == Z_NEED_DICT || ret == Z_DATA_ERROR) {
				inflateEnd(&strm);
				return false;
			}
			if(ret == Z_STREAM_END) {
				inflateEnd(&strm);
				out.SetLength(out.GetLength() - strm.avail_out);
				break;
			}
			if(out.GetLength() > (1 << 30) || blocksize > (1 << 30)) {
				throw std::bad_alloc();
			}
			blocksize += blocksize / 2;
			out.SetLength(out.GetLength() + blocksize);
			strm.next_out = (Bytef*) (out.GetData() + out.GetLength() - blocksize);
			strm.avail_out = blocksize;
		}
	} catch(...) {
		inflateEnd(&strm);
		throw;
	}
	Swap(out);
	return true;
}

uint32_t Buffer::ReadUIntV() {
	if(error) return 0;
	if(pos + 1 > length) {
		error = true;
		return 0;
	}
	uint32_t a = *(uint8_t*)(data + pos);
	if(a & 1) {
		++pos;
		return a >> 1;
	} else if(a & 2) {
		if(pos + 2 > length) {
			error = true;
			return 0;
		}
		a = *(uint16_t*)(data + pos);
		pos += 2;
		return (a >> 2) + 0x80;
	} else if(a & 4) {
		if(pos + 3 > length) {
			error = true;
			return 0;
		}
		a |= (uint32_t)(*(uint16_t*)(data + pos + 1)) << 8;
		pos += 3;
		return (a >> 3) + 0x4080;
	} else {
		if(pos + 4 > length) {
			error = true;
			return 0;
		}
		a = *(uint32_t*)(data + pos);
		pos += 4;
		return (a >> 3) + 0x204080;
	}
}

void Buffer::WriteUIntV(uint32_t x) {
	if(x < 0x80) {
		uint32_t a = (x << 1) | 1;
		unsigned int p = length;
		SetLength(length + 1);
		*(uint8_t*)(data + p) = a;
	} else if(x < 0x4080) {
		uint32_t a = ((x - 0x80) << 2) | 2;
		unsigned int p = length;
		SetLength(length + 2);
		*(uint16_t*)(data + p) = a;
	} else if(x < 0x204080) {
		uint32_t a = ((x - 0x4080) << 3) | 4;
		unsigned int p = length;
		SetLength(length + 3);
		*(uint8_t*)(data + p) = a;
		*(uint16_t*)(data + p+1) = a >> 8;
	} else {
		uint32_t a = (x - 0x204080) << 3;
		unsigned int p = length;
		SetLength(length + 4);
		*(uint32_t*)(data + p) = a;
	}
}

int32_t Buffer::ReadIntV() {
	uint32_t xx = ReadUIntV();
	if(xx & 1) {
		return (int32_t)((xx >> 1) ^ 0xffffffff);
	} else {
		return (int32_t)(xx >> 1);
	}
}

void Buffer::WriteIntV(int32_t x) {
	if(x < 0) {
		WriteUIntV((((uint32_t)(x) ^ 0xffffffff) << 1) | 1);
	} else {
		WriteUIntV((uint32_t)(x) << 1);
	}
}

const char* Buffer::ReadString() {
	if(error) return "";
	for(unsigned int i = pos; i < length; ++i) {
		if(data[i] == '\0') {
			const char *str = data+pos;
			pos = i + 1;
			return str;
		}
	}
	error = true;
	return "";
}

void Buffer::WriteString(const char* string) {
	unsigned int len = strlen(string) + 1;
	if(len != 0) {
		unsigned int p = length;
		SetLength(length + len);
		memcpy(data + p, string, len);
	}
}

void Buffer::ReadData(void* ptr, unsigned int len) {
	if(error) {
		memset(ptr, 0, len);
		return;
	}
	if(pos + len > length) {
		error = true;
		memset(ptr, 0, len);
		return;
	}
	if(len != 0) {
		memcpy(ptr, data+pos, len);
		pos += len;
	}
}

void Buffer::WriteData(const void* ptr, unsigned int len) {
	if(len != 0) {
		unsigned int p = length;
		SetLength(length + len);
		memcpy(data + p, ptr, len);
	}
}

void Buffer::ReadHex(char* ptr, unsigned int len) {
	if(error) {
		memset(ptr, '0', len * 2);
		return;
	}
	if(pos+len>length) {
		error = true;
		memset(ptr, '0', len * 2);
		return;
	}
	if(len != 0) {
		bintohex(data + pos, ptr, len);
		pos += len;
	}
}

void Buffer::WriteHex(const char* ptr, unsigned int len) {
	if(len != 0) {
		unsigned int p = length;
		SetLength(length + len);
		hextobin(ptr, data + p, len);
	}
}

void Buffer::WriteBuffer(const Buffer* b) {
	unsigned int len = b->length; // in case b = this
	if(len != 0) {
		unsigned int p = length;
		SetLength(length + len);
		memcpy(data + p, b->data, len);
	}
}

void Buffer::WriteBufferPart(const Buffer* b, unsigned int _pos, unsigned int _len) {
	if(_pos > b->length) _pos = b->length;
	if(_len > b->length-_pos) _len = b->length-_pos;
	if(_len != 0) {
		unsigned int p = length;
		SetLength(length + _len);
		memcpy(data + p, b->data+_pos, _len);
	}
}

