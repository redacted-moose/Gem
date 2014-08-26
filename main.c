#include <os.h>
#include <stdlib.h>
#include "gem.h"
#include "cpu.h"
#include "mmu.h"
#include "gpu.h"

void reset();

int main(int argc, char **argv)
{
	assert_ndless_rev(801);
//	INFO("Testing INFO works");
//	WARN("Testing WARN works");
//	ERROR("Testing ERROR works");
	enable_relative_paths(argv);
	lcd_ingray();
	clrscr();

	reset();
//	load_rom(getcwd(NULL, 0));
	load_rom("adjustris.gb.tns");
	INFO("Successfully loaded rom file\n");
	while(!any_key_pressed()) {
		byte instruction = read_byte(cpu.pc);
		INFO("%04x: Instruction was: 0x%02x\n", instruction);
		cpu.pc++;
		cpu.opcodes[instruction]();
		step_gpu();
	}
	return 0;
}

void reset()
{
	reset_cpu();
	reset_mmu();
	reset_gpu();
}
