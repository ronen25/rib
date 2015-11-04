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

#ifndef RIB_H
#define RIB_H

/* Return codes for functions */
#define RIB_RETCODE_OK 0
#define RIB_RETCODE_MEMERR -1
#define RIB_RETCODE_RANGEERR -2
#define RIB_RETCODE_SYNTAXERR -3

/* Command symbols */
#define RIB_COMMAND_LBRACKET 	'['
#define RIB_COMMAND_RBRACKET 	']'
#define RIB_COMMAND_PLUS 		'+'
#define RIB_COMMAND_MINUS 		'-'
#define RIB_COMMAND_LARROW 		'<'
#define RIB_COMMAND_RARROW 		'>'
#define RIB_COMMAND_INPUT 		','
#define RIB_COMMAND_OUTPUT 		'.'

/* Command configuration */
#define RIB_MEMVIEW_PRINT_COUNT 8

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "mem.h"

/* RIB_RETCODE typedef */
typedef int RIB_RETCODE;

/* Booleans for configuration */
bool rib_debug;
bool rib_interactive_input;

/* Init/free functions */
void rib_init(void);
void rib_free(void);

/* BF operations */
RIB_RETCODE rib_do_rarrow(void);
RIB_RETCODE rib_do_larrow(void);
RIB_RETCODE rib_do_inc(void);
RIB_RETCODE rib_do_dec(void);
RIB_RETCODE rib_do_readbyte(void);
RIB_RETCODE rib_do_putbyte(void);
RIB_RETCODE rib_do_lbracket(void);
RIB_RETCODE rib_do_rbracket(void);

RIB_RETCODE rib_do_command(void);
RIB_RETCODE rib_do_program(void);
RIB_RETCODE rib_do_endprogram(void); /* Operations done when a program ends execution */

void rib_dohelp(void);
void rib_viewstats(void);
void rib_viewmem(void);
void rib_setinput(void);
void rib_fromfile(void);

char *rib_find_rbracket(char *scanbegin);
char *rib_find_lbracket(char *scanbegin);

#endif /* RIB_H */
