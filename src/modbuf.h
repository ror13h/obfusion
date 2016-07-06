/*

Obfusion - C++ X86 Code Obfuscation Library
Copyright (c) 2016 Kuba Udymowski-Grecki

Contact: kuba@breakdev.org

This file is part of Obfusion.

Obfusion is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Obfusion is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Obfusion.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

#define CHUNK_SIZE 8192

class modbuf
{
public:
	modbuf(void);
	modbuf(int chunk_size);
	virtual ~modbuf(void);

	void clear();

	template<typename T>
	void add( T n )
	{
		add_data( (unsigned char*)&n, sizeof(T) );
	}

	void add_data( void *data, int len );
	void add_size( int len );

	unsigned char* data() { return m_buf; };
	int size() { return m_size; };

protected:
	unsigned char *m_buf;
	int m_size;
	int m_alloc_size;

	int m_chunk_size;
};
