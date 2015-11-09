/*
 *  RIB - An interactive BF interpreter
 *  Copyright (C) 2015 Ronen Lapushner.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */	

#include "rib.h"
#include "config.h"

int main(int argc, char **argv) {
	rib_init();
	atexit(rib_free);

	/* Print header */
	printf("Ronen's Interactive Brainfuck (%s)", PACKAGE_STRING);
	printf("\n==================================================");
	printf("\n%d bytes memory\t\tOK\n%d bytes input buffer\t\tOK", RIB_MEM_SIZE, RIB_INPUT_SIZE);
	printf("\n%d bytes program buffer\tOK", RIB_PROGRAM_SIZE);
	printf("\n==================================================\n");

	while (1) {
		/* Print prompt and get input */
		printf("(rib) ");
		rib_util_getinput(rib_program);

		/* Check if program contains anything at all */
		if (strcmp(rib_program, "") == 0) /* Empty command */
			continue;
		else if (strcmp(rib_program, "q") == 0) /* Quit program */
			break;
		else if (strcmp(rib_program, "h") == 0) /* Help command */
			rib_dohelp();
		else if (strcmp(rib_program, "c") == 0) /* Clear buffers */
			rib_clearbuffs();
		else if (strcmp(rib_program, "v") == 0) { /* View statistics */
			rib_viewmem();
			rib_viewstats();
		}
		else if (strcmp(rib_program, "i") == 0) /* Set input string */
			rib_setinput();
		else if (strcmp(rib_program, "p") == 0) /* View positions */
			rib_viewstats();
		else if (strcmp(rib_program, "f") == 0) /* Load program from file */
			rib_fromfile();
		else if (strcmp(rib_program, "d") == 0) { /* Debug mode */
			if (rib_debug)
				rib_debug = false;
			else
				rib_debug = true;

			/* Check flag */
			printf(rib_debug ? "Debugging ON.\n" : "Debugging OFF.\n");
		}
		else {
			/* Begin executing the program */
			rib_do_program();

			/* End program execution (clear buffers, reset pointers, etc.) */
			rib_do_endprogram();
		}

		/* Flush STDIN */
		fflush(stdin);
	}

	return 0;
}
