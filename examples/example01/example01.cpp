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

#include "../../include/obfusion.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

bool read_file(const char* fname, u8** data, u32* size)
{
	bool ret = false;
	FILE *f = fopen(fname,"rb");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		u32 fsize = ftell(f);

		fseek(f, 0, SEEK_SET);

		u8 *bindata = new u8[fsize];
		if (fread(bindata, 1, fsize, f) == fsize)
		{
			*data = bindata;
			*size = fsize;
			ret = true;
		}
		else
			delete[] bindata;

		fclose(f);
	}
	return ret;
}

bool write_file(const char* fname, u8* data, u32 size)
{
	bool ret = false;
	FILE *f = fopen(fname,"wb");
	if (f)
	{
		if (fwrite(data, 1, size, f) == size)
			ret = true;

		fclose(f);
	}
	return ret;
}

int main()
{
	obfusion obf;

	u8 *shell_data = 0;
	u32 shell_size;

	if (read_file("../res/exec_calc.bin", &shell_data, &shell_size))
	{
		// shellcode binary data is present from offset 184 to 189 ("calc\0" string)
		// we need to supply this information so the obfuscator doesn't try to disassemble binary data
		std::vector<std::pair<u32,u32>> data_ranges;
		data_ranges.push_back( std::make_pair<u32,u32>(184, 189) );

		// initial setup of obfuscation parameters
		obf.set_obf_steps(5,20); // we want the obfuscated instructions to be divided into 5-20 separate instructions (approximately) (def. 3-8)
		obf.set_jmp_perc(20); // there should be 20% chance of inserting a jump at every instruction of the obfuscated output (def. 5%)

		obf.load32(shell_data, shell_size, data_ranges);

		delete[] shell_data;
		shell_data = 0;

		printf("obfuscating...\n");

		// obfuscate with random seed of 0xCAFEBABE and 3 obfuscation passes
		obf.obfuscate(0xCAFEBABE, 3);

		// dump the obfuscated shellcode into the buffer
		obf.dump(&shell_data, &shell_size);

		// save obfuscated shellcode to binary file
		if (write_file("../res/output.bin", shell_data, shell_size))
			printf("saved.\n");

#ifdef _WIN32
		DWORD prot;
		VirtualProtect(shell_data, shell_size, PAGE_EXECUTE_READWRITE, &prot);

		// execute obfuscated shellcode to test if runs properly (should execute calc.exe)
		((void(*)())shell_data)();
#endif

		delete[] shell_data;
	}

	return 0;
}