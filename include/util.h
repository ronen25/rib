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

#ifndef RIB_UTIL_H
#define RIB_UTIL_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "mem.h"

/* Printing utillity methods */
void rib_util_printheader(const char *title, bool isBold);
void rib_util_printseparator(bool isBold, size_t len);
int rib_util_getinput(void);

#endif /* RIB_UTIL_H */
