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

#ifndef RIB_MEM_H
#define RIB_MEM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RIB_MEM_SIZE 30000
#define RIB_INPUT_SIZE 512
#define RIB_PROGRAM_SIZE 8192

/* Memory buffer */
char *rib_mem;

/* Memory pointer and position index
 * We need both because we allow the pointer to be looped back */
char *rib_p;
int rib_pos;

/* Program and input buffers */
char *rib_program, *rib_progptr;
char *rib_input, *rib_inputptr;

void rib_initbuffs(void);
void rib_freebuffs(void);
void rib_clearbuffs(void);

#endif /* RIB_MEM_H */
