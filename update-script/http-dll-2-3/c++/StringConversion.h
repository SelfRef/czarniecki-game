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

#ifndef STRINGCONVERSION_H
#define STRINGCONVERSION_H

inline std::string UIntToString(unsigned int x) {
	char buffer[20];
	char *ptr = buffer + sizeof(buffer);
	do {
		*(--ptr) = '0' + x % 10;
	} while((x /= 10) != 0);
	return std::string(ptr, buffer + sizeof(buffer) - ptr);
}

inline unsigned int StringToUInt(const std::string& str) {
	unsigned int value = 0;
	for(unsigned int i = 0; i < str.length(); ++i) {
		char c = str[i];
		if(c < '0' || c > '9') break;
		if(value > UINT_MAX / 10) return UINT_MAX;
		unsigned int value10 = value*10;
		if(value10 >= UINT_MAX - (unsigned int)(c - '0')) return UINT_MAX;
		value = value10 + (unsigned int)(c - '0');
	}
	return value;
}

#endif // STRINGCONVERSION_H

