#ifndef	SYMROGUE_H
#define	SYMROGUE_H

#if ( SYM == 1 )

/* headers */
#include <symio.h>
#include <string.h>
#include <stdlib.h>
/* core func overrides */
#define input()		getchar()
#define	puts(s)		_puts(s)
/* misc defines */
#define	newline()	putchar('\n');putchar('\r')
#define	clrdraw()	puts("\033[2J")
#define clrscr()	puts("\033[H")

#else

/* headers */
#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/* core func overrides */
#define input()		getch()
#define	newline()	putchar('\n');putchar('\r')
#define	clrdraw()	puts("\033[2J")
#define	clrscr()	puts("\033[H")

#endif

/* declare overridden funcs */
#if ( SYM == 1 )
void _puts( s )
char* s;
{
	register char* p = s;
	while ( *p )
		_putchar( *p++ );
}

#endif


#endif
