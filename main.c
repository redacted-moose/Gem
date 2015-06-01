//#include <os.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "gem.h"
#include "gpu.h"
#include "mmu.h"

void reset();

char buf[1024];

int main(int argc, char **argv)
{

	unsigned int breakpoint;
	unsigned int memaddress;
	unsigned int val;
	unsigned int watchpointb;
	unsigned int watchpointw;
	word watchw;
	byte watchb;

	int xcoord;
	int ycoord;
	int color;


	// assert_ndless_rev(801);
//	INFO("Testing INFO works");
//	WARN("Testing WARN works");
//	ERROR("Testing ERROR works");
	// enable_relative_paths(argv);
	// lcd_ingray();
	// clrscr();

	reset();
//	load_rom(getcwd(NULL, 0));

//	INFO("Writing 0xdead to 0x8000\n");
//	write_word(0x8000, 0xdead);
//	INFO("Got 0x%x from 0x8000\n", read_word(0x8000));

	if (argc == 2) {
		load_rom(argv[1]);
		INFO("Successfully loaded rom file %s\n", argv[1]);
	} else {
		load_rom("adjustris.gb.tns");
		INFO("Successfully loaded rom file %s\n", "adjustris.gb.tns");
	}

//	char * tok;

	while(true) {
		printf(">");
		scanf("%s", &buf);
//		printf("%s\n", buf);
//		tok = strtok(buf, " \n");
//		printf("%s\n", tok);
		if (strlen(buf) == 1) {
			switch (buf[0]) {
			case 'r':
				printf("pc: %04x\tsp: %04x\n", cpu.pc, cpu.sp);
				printf("af: %04x\tbc: %04x\n", cpu.af, cpu.bc);
				printf("de: %04x\thl: %04x\n", cpu.de, cpu.hl);
				break;
			case 'b':
//				tok = strtok(buf, " ");
				scanf("%x", &breakpoint);
				printf("Set breakpoint at 0x%04x\n", breakpoint);
				break;
			case 'c':
				do {
					execute();
				} while (cpu.pc != (word)breakpoint);
				printf("Breakpoint at 0x%04x\n", cpu.pc);
				break;
			case 's':
				execute();
				if (cpu.pc == (word)breakpoint) printf("Breakpoint at 0x%04x\n", cpu.pc);
				break;
			case 'x':
//				tok = strtok(buf, " ");
//				printf("%s\n", tok);
				scanf("%x", &memaddress);
				printf("0x%04x:\nbyte: %02x\nword: %04x\n", memaddress, read_byte((word)memaddress), read_word((word)memaddress));
				break;
			case 'w':
//				tok = strtok(buf, " ");
				scanf("%x", &memaddress);
//				tok = strtok(buf, " ");
				scanf("%x", &val);
				write_byte((word)memaddress, (byte)val);
				printf("Wrote byte 0x%02x to 0x%04x", val, memaddress);
				break;
			case 'q':
				printf("Bye!\n");
				exit(0);
			}
		} else if (strlen(buf) == 2) {
			if (strcmp(buf, "wb")) {
				scanf("%x", &watchpointb);
				watchb = read_byte((word)watchpointb);
				printf("Set byte watchpoint at 0x%04x", watchpointb);
			} else if (strcmp(buf, "ww")) {
				scanf("%x", &watchpointw);
				watchw = read_word((word)watchpointw);
				printf("Set word watchpoint at 0x%04x", watchpointw);
//			}
			} else if (strcmp(buf, "sp")) {
				scanf("%x", &xcoord);
				scanf("%x", &ycoord);
				scanf("%x", &color);


			}
		}
	}
	return 0;
}

void reset()
{
	reset_cpu();
	reset_mmu();
	reset_gpu();
}
