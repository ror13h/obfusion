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
#include "hde32/hde32.h"

class codeinstr
{
public:
	codeinstr(void);
	virtual ~codeinstr(void);

	static codeinstr* disasm32(const void* data);
	static codeinstr* data_block(const void* data, u32 size);

	u8* data() { return m_data; };
	u32 size() { return m_size; };

	bool is_data() { return m_is_data; };
	u32 label() { return m_label; };
	u32 jmp_to_label() { return m_jmp_to_label; };
	hde32s* get_hde32() { return &m_hde32; };

	void set_data(const void* data, u32 size);

	void set_label( u32 label ) { m_label = label; };
	void set_jmp_to_label( u32 label ) { m_jmp_to_label = label; };

	bool is_rel_jmp();
	s32 get_rel_imm();
	s32 get_rel_disp();

protected:
	u8 *m_data;
	u32 m_size;

	hde32s m_hde32;
//	hde64s m_hde64;

	bool m_bits_64, m_bits_32;
	bool m_is_data;

	u32 m_label;
	u32 m_jmp_to_label;
};
