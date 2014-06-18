// Author: Jesse Cheun, jcheun@ucsc.edu
// $Id: debugf.c,v 1.2 2013-06-06 03:47:33-07 - - $

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "debugf.h"

char *Exec_Name = NULL;
int Exit_Status = EXIT_SUCCESS;

static char *debugflags = "";
static bool alldebugflags = false;

void __errorprintf (char *filename, int line, const char *func,
                    char *format, ...) {
   va_list args;
   fflush (NULL);
   fprintf (stdout, "Error: %s[%d] %s(): ",
            filename, line, func);
   va_start (args, format);
   vfprintf (stdout, format, args);
   va_end (args);
   fflush (NULL);
}

void set_debugflags (char *flags) {
   debugflags = flags;
   if (strchr (debugflags, '@') != NULL) alldebugflags = true;
   //DEBUGF ('a', "Debugflags = \"%s\"\n", debugflags);
}

void __debugprintf (char flag, char *file, int line,
                    const char *func, char *format, ...) {
   va_list args;
   if (alldebugflags || strchr (debugflags, flag) != NULL) {
      fflush (NULL);
      fprintf (stderr, "%s: DEBUGF(%c) %s[%d] %s(): ",
               Exec_Name, flag, file, line, func);
      va_start (args, format);
      vfprintf (stderr, format, args);
      va_end (args);
      fflush (NULL);
   }
}

