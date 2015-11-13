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

void rib_init(void) {
	/* For debugging only */
	rib_debug = false;
	rib_pos = 0;

	rib_initbuffs();
}

void rib_free(void) {
	rib_freebuffs();
}

RIB_RETCODE rib_do_rarrow(void) {
	/* Update position */
	if (++rib_pos > RIB_MEM_SIZE - 1)
		rib_pos = 0;

	/* Update pointer */
	rib_p = &rib_mem[rib_pos];

	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_larrow(void) {
	/* Update position */
	if (--rib_pos < 0)
		rib_pos = RIB_MEM_SIZE - 1;

	/* Update buffer pointer */
	rib_p = &rib_mem[rib_pos];

	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_inc(void) {
	(*rib_p)++;
	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_dec(void) {
	(*rib_p)--;
	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_readbyte(void) {
	/* If pointer is null and input is available, the pointer is not set up.
	 * So set it up! */
	if (!rib_inputptr && rib_input) {
		rib_inputptr = rib_input;
		*rib_p = *rib_inputptr;
	}
	else if (*rib_inputptr == '\0' && rib_input) /* If pointer is '\0', EOS reached */
		*rib_p = 0;
	else {
		rib_inputptr++;
		*rib_p = *rib_inputptr;
	}

	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_putbyte(void) {
	/* If we're in verbose mode, the output will be at the end of the debug output,
	 * so we don't need to actually output anything. */
	if (!rib_debug)
		putc(*rib_p, stdout);

	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_lbracket(void) { /* [ */
	/* Skip to the next command after the appropriate ] if current cell is 0 */
	if (*rib_p == 0) {
		/* Find ] */
		rib_progptr = rib_find_rbracket(rib_progptr);
		if (!rib_progptr)
			return RIB_RETCODE_SYNTAXERR; /* Missing ] */
	}

	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_rbracket(void) { /* ] */
	/* Skip back to the next command after the appropriate [ if the current cell is not 0 */
	if (*rib_p != 0) {
		/* Find [ */
		rib_progptr = rib_find_lbracket(rib_progptr);
		if (!rib_progptr)
			return RIB_RETCODE_SYNTAXERR; /* Missing [ */
	}

	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_command(void) {
	RIB_RETCODE cmdflag;

	/* Check command type */
	switch (*rib_progptr) {
		case RIB_COMMAND_LBRACKET: /* Loop begin */
			cmdflag = rib_do_lbracket();
			break;
		case RIB_COMMAND_RBRACKET: /* Loop end */
			cmdflag = rib_do_rbracket();
			break;
		case RIB_COMMAND_PLUS: /* Increase */
			cmdflag = rib_do_inc();
			break;
		case RIB_COMMAND_MINUS: /* Decrease */
			cmdflag = rib_do_dec();
			break;
		case RIB_COMMAND_LARROW: /* Left arrow (shift left) */
			cmdflag = rib_do_larrow();
			break;
		case RIB_COMMAND_RARROW: /* Right arrow (shift right) */
			cmdflag = rib_do_rarrow();
			break;
		case RIB_COMMAND_INPUT: /* Input */
			cmdflag = rib_do_readbyte();
			break;
		case RIB_COMMAND_OUTPUT: /* Output */
			cmdflag = rib_do_putbyte();
			break;
		default:
			cmdflag = RIB_RETCODE_OK; /* Skip any unknown characters */
			break;
	}

	/* Return flag */
	return cmdflag;
}

RIB_RETCODE rib_do_program(void) {
	RIB_RETCODE currentStatus;
	int errorFlag = 0;
	int instructionCount = 0;
	char tempOutput[RIB_INPUT_SIZE] = { 0 };
	int tempOutputLen = 0;

	if (rib_debug)
		printf("\nProgram loaded (%zd bytes)\n", strlen(rib_program));

	/* While not at EOF, read program */
	while (*rib_progptr != '\0') {
		/* Do command */
		currentStatus = rib_do_command();

		switch (currentStatus) {
			case RIB_RETCODE_OK:
				/* Print if in verbose mode */
				if (rib_debug) {
					printf("\tCommand: %c retcode %d\tp = %d\tpos = %d\n", *rib_progptr,
							currentStatus, *rib_p, rib_pos);

					/* Advance instruction count and copy output to temporary buffer */
					instructionCount++;

					/* If requested output, put it in the temporary buffer */
					if (*rib_progptr == RIB_COMMAND_OUTPUT)
						tempOutput[tempOutputLen++] = *rib_p;
				}
				break;
			case RIB_RETCODE_SYNTAXERR:
				printf("\nSyntax error:\n%s\n at position %zd\n", rib_program,
						(strlen(rib_program) - (rib_progptr ? strlen(rib_progptr) : 0)));

				/* Set error flag */
				errorFlag = 1;

				break;
		}

		if (errorFlag) /* Break from the loop in case of error */
			break;
		else {
			/* Advance command pointer */
			rib_progptr++;
		}
	}

	/* If in verbose mode, print out the output */
	if (rib_debug) {
		printf("\nOutput:\n-------------------\n%s\n", tempOutput);
		printf("\nTotal %d instructions.\n", instructionCount);
	}

	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_endprogram(void) {
	/* Clear program buffer */
	memset(rib_program, 0, sizeof(char) * RIB_PROGRAM_SIZE);

	/* Clear command pointer */
	rib_progptr = rib_program;
	rib_inputptr = NULL;

	return RIB_RETCODE_OK;
}

void rib_clearbuffs(void) {
	memset(rib_program, 0, sizeof(char) * RIB_PROGRAM_SIZE);
	memset(rib_mem, 0, sizeof(char) * RIB_MEM_SIZE);
}

void rib_viewmem(void) {
	int temp, count = 0;
	int cursorPosition = 9 * (RIB_MEMVIEW_PRINT_COUNT / 2) - (RIB_MEMVIEW_PRINT_COUNT / 2);

	/* Calculatge from where to start printing */
	int begin = rib_pos - (RIB_MEMVIEW_PRINT_COUNT / 2);
	if (begin < 0)
		begin = RIB_MEM_SIZE + begin;

	/* Print current position pointer */
	for (int i = 0; i < cursorPosition; i++)
		putc(' ', stdout);
	printf("VV PTR VV\n");

	/* Print header */
	for (temp = begin, count = 0; count < RIB_MEMVIEW_PRINT_COUNT; count++, temp++) {
		/* Check bounds */
		if (temp == RIB_MEM_SIZE)
			temp = 0;

		/* Print it */
		printf("| %-5d ", temp);
	}

	/* Put | */
	putc('|', stdout);

	/* Print separator */
	rib_util_printseparator(false, cursorPosition * 2 + RIB_MEMVIEW_PRINT_COUNT);

	/* Print values */
	for (temp = begin, count = 0; count < RIB_MEMVIEW_PRINT_COUNT; count++, temp++) {
		/* Check bounds */
		if (temp == RIB_MEM_SIZE)
			temp = 0;

		/* Print it accordingly */
		printf(isprint(rib_mem[temp]) ? "| %-5c " : "| %-5d ", rib_mem[temp]);
	}

	/* Put pipe and newline to finish */
	printf("|\n");
}

void rib_viewstats(void) {
	printf("\nCurrent cell = '%d'\tPosition = %d\n", *rib_p, rib_pos);
}

void rib_setinput(void) {
	printf("\nCurrent input string is:\n'%s'", rib_input);
	printf("\nSet new input string (leave current by pressing ENTER, Ctrl+D to clear):\n");

	/* Get input string */
	if (rib_util_getinput(rib_input)) {
		/* Check if a new input string has been entered */
		if (strcmp(rib_input, "") != 0)
			printf("\nNew input string set.\n");
		else
			printf("\nInput string remains.\n");
	}
	else {
		/* Clear input string */
		memset(rib_input, 0, sizeof(char) * RIB_INPUT_SIZE);

		/* Print message */
		printf("\nInput string cleared.\n");
	}

	/* Nullify the input pointer so that rib_readbyte() could re-set it properly */
	rib_inputptr = NULL;
}

void rib_dohelp(void) {
	printf("\nRIB help\n===================================================\n");
	printf("\np - View pointer position\nv - View complete memory status");
	printf("\nc - Clear all memory buffers\ni - Set input string");
	printf("\nl - Load program from file");
	printf("\nq - Quit the program\n");
}

void rib_fromfile(void) {
	// TODO: Implement this
	printf("\nTODO\n");
}

char *rib_find_rbracket(char *scanbegin) {
	int skipcount = 0;
	char *p = scanbegin;

	/* Scan while not at end of loop */
	while (*p != '\0') {
		if (*p == '[')
			skipcount++;
		else if (*p == ']') {
			skipcount--;

			/* Check if skip count is 0 - means we found a match */
			if (skipcount == 0)
				return p;
		}

		p++;
	}

	return NULL;
}

char *rib_find_lbracket(char *scanbegin) {
	int skipcount = 0;
	char *p = scanbegin;

	/* Scan while not at the beginning of the string */
	for (size_t i = 0; i <= strlen(rib_program) - strlen(scanbegin); i++) {
		if (*p == ']')
			skipcount++;
		else if (*p == '[') {
			skipcount--;

			/* Check if skipcount is 0 - means we found a match */
			if (skipcount == 0)
				return p;
		}

		p--;
	}

	return NULL;
}
