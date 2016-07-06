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

#include "include.h"
#include "codeinstr.h"

codeinstr::codeinstr(void)
{
	m_bits_32 = false;
	m_bits_64 = false;
	m_size = 0;
	m_is_data = false;
	m_label = 0;
	m_jmp_to_label = 0;

	m_data = 0;

	memset(&m_hde32, 0, sizeof(hde32s));
//	memset(&m_hde64, 0, sizeof(hde64s));
}

codeinstr::~codeinstr(void)
{
	if (m_data)
	{
		delete[] m_data;
		m_data = 0;
	}
}

codeinstr* codeinstr::disasm32(const void* data)
{
	hde32s _hde32s;
	u32 size = hde32_disasm(data, &_hde32s);

	if (size > 0)
	{
		codeinstr *cinstr = new codeinstr();
		if (cinstr)
		{
			cinstr->m_bits_32 = true;
			cinstr->m_bits_64 = false;
			cinstr->m_size = size;
			cinstr->m_is_data = false;

			cinstr->m_data = new u8[size];
			memcpy(cinstr->m_data, data, size);

			memcpy( reinterpret_cast<void*>(&cinstr->m_hde32), reinterpret_cast<const void*>(&_hde32s), sizeof(hde32s) );
			return cinstr;
		}
	}
	return 0;
}

codeinstr* codeinstr::data_block(const void* data, u32 size)
{
	if (size > 0)
	{
		codeinstr *cinstr = new codeinstr();
		if (cinstr)
		{
			cinstr->m_bits_32 = false;
			cinstr->m_bits_64 = false;
			cinstr->m_size = size;
			cinstr->m_is_data = true;

			cinstr->m_data = new u8[size];
			memcpy(cinstr->m_data, data, size);

			return cinstr;
		}
	}
	return 0;
}

void codeinstr::set_data(const void* data, u32 size)
{
	if (size > 0)
	{
		if (m_data)
		{
			delete[] m_data;
			m_data = 0;
		}

		if (m_is_data == false)
			m_size = hde32_disasm(data, &m_hde32);
		else
			m_size = size;

		m_data = new u8[m_size];
		memcpy(m_data, data, m_size);
	}
}

bool codeinstr::is_rel_jmp()
{
	if ( m_size >= 2 )
	{
		u8 op1 = m_hde32.opcode;
		u8 op2 = m_hde32.opcode2;

		if ( (op1 & 0xf0) == 0x70 || (op1 >= 0xe0 && op1 <= 0xe3) || op1 == 0xe8 || op1 == 0xe9 || op1 == 0xeb || (op1 == 0x0f && (op2 & 0xf0) == 0x80) )
			return true;
	}
	return false;
}

s32 codeinstr::get_rel_imm()
{
	u32 flags = m_hde32.flags;

	if ( (flags & F_IMM8) == F_IMM8 )
		return (s32)((s8)m_hde32.imm.imm8);
	else if ( (flags & F_IMM16) == F_IMM16 )
		return (s32)((s16)m_hde32.imm.imm16);
	else if ( (flags & F_IMM32) == F_IMM32 )
		return (s32)m_hde32.imm.imm32;
	return 0;
}

s32 codeinstr::get_rel_disp()
{
	u32 flags = m_hde32.flags;

	if ( (flags & F_DISP8) == F_DISP8 )
		return (s32)((s8)m_hde32.disp.disp8);
	else if ( (flags & F_DISP16) == F_DISP16 )
		return (s32)((s16)m_hde32.disp.disp16);
	else if ( (flags & F_DISP32) == F_DISP32 )
		return (s32)m_hde32.disp.disp32;
	return 0;
}
