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

#include "gm.h"

gmexport double buffer_create() {
	Buffer *b = NULL; // GCC is complaining for some reason ...
	b = new Buffer();
	try {
		gmdata.buffers.insert(std::pair<unsigned int, Buffer*>(++gmdata.idcounter_buffers, b));
	}
	catch(...) {
		delete b;
		throw;
	}
	return gmdata.idcounter_buffers;
}

gmexport double buffer_destroy(double id) {
	std::map<unsigned int, Buffer*>::iterator it = gmdata.buffers.find(gm_cast<unsigned int>(id));
	if(it == gmdata.buffers.end()) {
		return 0;
	}
	delete it->second;
	gmdata.buffers.erase(it);
	return 1;
}

gmexport double buffer_exists(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	return (b == NULL)? 0 : 1;
}

gmexport const char* buffer_to_string(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ToString();
}

gmexport double buffer_get_pos(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->GetPos();
}

gmexport double buffer_get_length(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->GetLength();
}

gmexport double buffer_at_end(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return (b->IsAtEnd())? 1 : 0;
}

gmexport double buffer_get_error(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return (b->GetError())? 1 : 0;
}

gmexport double buffer_clear_error(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->ClearError();
	return 1;
}

gmexport double buffer_clear(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->Clear();
	return 1;
}

gmexport double buffer_set_pos(double id, double pos) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->SetPos(gm_cast<unsigned int>(pos));
	return 1;
}

gmexport double buffer_read_from_file(double id, const char* filename) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return (b->ReadFromFile(filename))? 1 : 0;
}

gmexport double buffer_read_from_file_part(double id, const char* filename, double pos, double len) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return (b->ReadFromFilePart(filename, gm_cast<unsigned int>(pos), gm_cast<unsigned int>(len)))? 1 : 0;
}

gmexport double buffer_write_to_file(double id, const char* filename) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return (b->WriteToFile(filename))? 1 : 0;
}

gmexport double buffer_append_to_file(double id, const char* filename) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return (b->AppendToFile(filename))? 1 : 0;
}

gmexport double buffer_rc4_crypt(double id, const char* key) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->RC4Crypt(key, strlen(key));
	return 1;
}

gmexport double buffer_rc4_crypt_buffer(double id, double id2) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	Buffer *b2 = gmdata.FindBuffer(gm_cast<unsigned int>(id2));
	if(b2 == NULL) return 0;
	b->RC4Crypt(b2->GetData(), b2->GetLength());
	return 1;
}

gmexport double buffer_zlib_compress(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->ZLibCompress();
	return 1;
}

gmexport double buffer_zlib_uncompress(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return (b->ZLibUncompress())? 1 : 0;
}

gmexport double buffer_read_int8(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<int8_t>();
}

gmexport double buffer_read_uint8(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<uint8_t>();
}

gmexport double buffer_read_int16(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<int16_t>();
}

gmexport double buffer_read_uint16(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<uint16_t>();
}

gmexport double buffer_read_int32(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<int32_t>();
}

gmexport double buffer_read_uint32(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<uint32_t>();
}

gmexport double buffer_read_int64(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<int64_t>();
}

gmexport double buffer_read_uint64(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<uint64_t>();
}

gmexport double buffer_read_intv(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadIntV();
}

gmexport double buffer_read_uintv(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadUIntV();
}

gmexport double buffer_read_float32(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<float>();
}

gmexport double buffer_read_float64(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	return b->ReadType<double>();
}

gmexport double buffer_write_int8(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<int8_t>(gm_cast<int8_t>(value));
	return 1;
}

gmexport double buffer_write_uint8(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<uint8_t>(gm_cast<uint8_t>(value));
	return 1;
}

gmexport double buffer_write_int16(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<int16_t>(gm_cast<int16_t>(value));
	return 1;
}

gmexport double buffer_write_uint16(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<uint16_t>(gm_cast<uint16_t>(value));
	return 1;
}

gmexport double buffer_write_int32(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<int32_t>(gm_cast<int32_t>(value));
	return 1;
}

gmexport double buffer_write_uint32(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<uint32_t>(gm_cast<uint32_t>(value));
	return 1;
}

gmexport double buffer_write_int64(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<int64_t>(gm_cast<int64_t>(value));
	return 1;
}

gmexport double buffer_write_uint64(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<uint64_t>(gm_cast<uint64_t>(value));
	return 1;
}

gmexport double buffer_write_intv(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteIntV(gm_cast<int32_t>(value));
	return 1;
}

gmexport double buffer_write_uintv(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteUIntV(gm_cast<uint32_t>(value));
	return 1;
}

gmexport double buffer_write_float32(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<float>(gm_cast<float>(value));
	return 1;
}

gmexport double buffer_write_float64(double id, double value) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteType<double>(gm_cast<double>(value));
	return 1;
}

gmexport const char* buffer_read_string(double id) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return "";
	return b->ReadString();
}

gmexport double buffer_write_string(double id, const char* string) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteString(string);
	return 1;
}

gmexport const char* buffer_read_data(double id, double len) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return "";
	unsigned int l = gm_cast<unsigned int>(len);
	gmreturnstring.SetLength(l);
	b->ReadData(gmreturnstring.GetData(), l);
	return gmreturnstring.ToString();
}

gmexport double buffer_write_data(double id, const char* string) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteData(string, strlen(string));
	return 1;
}

gmexport const char* buffer_read_hex(double id, double len) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return "";
	unsigned int l = gm_cast<unsigned int>(len);
	gmreturnstring.SetLength(l * 2);
	b->ReadHex(gmreturnstring.GetData(), l);
	return gmreturnstring.ToString();
}

gmexport double buffer_write_hex(double id, const char* string) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	b->WriteHex(string, strlen(string) / 2);
	return 1;
}

gmexport double buffer_write_buffer(double id, double id2) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	Buffer *b2 = gmdata.FindBuffer(gm_cast<unsigned int>(id2));
	if(b2 == NULL) return 0;
	b->WriteBuffer(b2);
	return 1;
}

gmexport double buffer_write_buffer_part(double id, double id2, double pos, double len) {
	Buffer *b = gmdata.FindBuffer(gm_cast<unsigned int>(id));
	if(b == NULL) return 0;
	Buffer *b2 = gmdata.FindBuffer(gm_cast<unsigned int>(id2));
	if(b2 == NULL) return 0;
	b->WriteBufferPart(b2, gm_cast<unsigned int>(pos), gm_cast<unsigned int>(len));
	return 1;
}

