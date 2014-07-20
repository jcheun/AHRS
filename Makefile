# Author: Jesse Cheun, jcheun@ucsc.edu
# $Id: Makefile,v 1.3 2013-06-06 05:28:53-07 - - $

MKFILE    = Makefile
GMAKE     = gmake --no-print-directory

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99
VPATH     = ${shell ls -d */ | sed 's/\///'}
#find . -not -path '*/\.*' -type d \( ! -iname ".*" \) | sed 's|^\./||g'

INC_DIR   = $(VPATH:%=-I %)
CSOURCE   = ${shell find . -type f -name "*.c" -printf '%f\n'}
OBJECTS   = ${CSOURCE:.c=.o} 
EXECBIN   = AHRS

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
		${GCC} -lm -o $@ ${OBJECTS}

%.o : %.c
		${GCC} ${INC_DIR} -c $<

clean :
		- rm ${OBJECTS} ${EXECBIN}

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

