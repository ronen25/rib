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

#include "util.h"

void rib_util_printheader(const char *title, bool isBold) {
	/* Print title and newline */
	puts(title);
	putc('\n', stdout);

	/* Print the title line */
	for (size_t i = 0; i < strlen(title) + 1; i++)
		putc(isBold ? '=' : '-', stdout);

	/* To finish, print another newline */
	putc('\n', stdout);
}

int rib_util_getinput(void) {
	/* Check if there is any input at all */
	if (fgets(rib_program, RIB_INPUT_SIZE, stdin) != NULL) {
		size_t actualLength = strlen(rib_program);

		/* Sanitize newline if needed */
		if (actualLength > 0 && rib_program[actualLength - 1] == '\n')
			rib_program[actualLength - 1] = '\0';

		return 1;
	}

	return 0;
}
