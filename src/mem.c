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

#include "mem.h"

void rib_initbuffs(void) {
	/* Initialize main memory buffer */
	rib_mem = calloc(RIB_MEM_SIZE, sizeof(char));
	if (!rib_mem) {
		fprintf(stderr, "\nError initializing main memory buffer.");
		exit(EXIT_FAILURE);
	}

	/* Initialize positions */
	rib_pos = 0;
	rib_p = &rib_mem[rib_pos];

	/* Initialize program buffer */
	rib_program = calloc(RIB_PROGRAM_SIZE, sizeof(char));
	if (!rib_program) {
		fprintf(stderr, "\nError initializing program buffer.");
		exit(EXIT_FAILURE);
	}
	rib_progptr = rib_program;

	/* Initialize input buffer */
	rib_input = calloc(RIB_INPUT_SIZE, sizeof(char));
	if (!rib_input) {
		fprintf(stderr, "\nError initializing input buffer.");
		exit(EXIT_FAILURE);
	}
}

void rib_freebuffs(void) {
	free(rib_mem);
	free(rib_program);
}
