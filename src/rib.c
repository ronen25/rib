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
	rib_verbose = 1;
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
	putc(*rib_p, stdout);
	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_lbracket(void) {
	/* Skip to the next command after the appropriate ] if current cell is 0 */
	if (*rib_p == 0) {
		/* Find ] */
		rib_progptr = strchr(rib_program, ']');
		if (!rib_progptr)
			return RIB_RETCODE_SYNTAXERR; /* Missing ] */
	}

	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_rbracket(void) {
	/* Skip back to the next command after the appropriate [ if the current cell is not 0 */
	if (*rib_p != 0) {
		/* Find [ */
		rib_progptr = strchr(rib_program, '[');
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
			cmdflag = RIB_RETCODE_SYNTAXERR;
			break;
	}

	/* Return flag */
	return cmdflag;
}

RIB_RETCODE rib_do_program(void) {
	RIB_RETCODE currentStatus;
	int errorFlag = 0;

	if (rib_verbose)
		printf("\nProgram loaded (%zd bytes)\n", strlen(rib_program));

	/* While not at EOF, read program */
	while (*rib_progptr != '\0') {
		/* Do command */
		currentStatus = rib_do_command();

		switch (currentStatus) {
			case RIB_RETCODE_OK:
				/* Print if in verbose mode */
				if (rib_verbose)
					printf("\n\tCommand: %c retcode %d\tp = %d\tpos = %d\n", *rib_progptr,
							currentStatus, *rib_p, rib_pos);
				break;
			case RIB_RETCODE_SYNTAXERR:
				/* Print if in verbose mode */
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

	return RIB_RETCODE_OK;
}

RIB_RETCODE rib_do_endprogram(void) {
	/* Clear program buffer */
	memset(rib_program, 0, sizeof(char) * RIB_PROGRAM_SIZE);

	/* Clear command pointer */
	rib_progptr = rib_program;
}

void rib_clearbuffs(void) {
	memset(rib_program, 0, sizeof(char) * RIB_PROGRAM_SIZE);
	memset(rib_mem, 0, sizeof(char) * RIB_MEM_SIZE);
}

// TODO: Replace this with a better version
void rib_viewmem(void) {
	int i, count;

	/* Print pointer */
	printf("\t\t VVV\n");

	/* Print header */
	printf("\t\t");
	for (i = rib_pos, count = 0; count != RIB_MEMVIEW_PRINT_COUNT; i++, count++) {
		printf("| %d |", i);

		/* Check if we have reached the end */
		if (i == RIB_MEM_SIZE - 1)
			i = -1;
	}

	printf("\n\t\t------------------------------------------------------\n");

	/* Print RIB_MEMVIEW_PRINT_COUNT cells */
	printf("\t\t");
	for (i = rib_pos, count = 0; count != RIB_MEMVIEW_PRINT_COUNT; i++, count++) {
		/* If it's an ASCII letter display it as such */
		printf(isascii(rib_mem[i]) ? "| %c |" : "| %d |", rib_mem[i]);

		/* Check if we have reached the end */
		if (i == RIB_MEM_SIZE - 1)
			i = -1;
	}

	/* Print newline */
	putc('\n', stdout);
}

void rib_viewstats(void) {
	printf("\nCurrent cell = '%d'\tPosition = %d\n", *rib_p, rib_pos);
}

void rib_setinput(void) {
	
	printf("\nCurrent input string is:\n'%s'", rib_input);
	printf("\nSet new input string (or press ENTER to leave current):\n");

	/* Get input string */
	/* TODO: Problem if input is INPUT_SIZE-length. Probably need to create a function
	 * to properly handle input, for example "getinput" */
	rib_input = fgets(rib_input, RIB_INPUT_SIZE, stdin);
	rib_input[strlen(rib_input) - 1] = '\0';

	/* Check if a new input string has been entered */
	if (strcmp(rib_input, "") != 0)
		printf("\nNew input string set.\n");
	else
		printf("\nInput string remains.");

	/* Nullify the input pointer so that rib_readbyte() could re-set it properly */
	rib_inputptr = NULL;
}

void rib_dohelp(void) {
	printf("\nRIB help\n===================================================\n");
	printf("\np - View pointer position\nv - View complete memory status");
	printf("\nc - Clear all memory buffers\ni - Set input string");
	printf("\nl - Load program from file");
	printf("\nq - Quit the program");
}

void rib_fromfile(void) {
	// TODO: Implement this
	printf("\nTODO\n");
}
