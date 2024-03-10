#define	SYM	0

#include "symRogue.h"

/* constants */
#define	ROW_LEN		16
#define	COL_HEIGHT	10
#define	MAP_SIZE	ROW_LEN * COL_HEIGHT
#define	ROW_MASK	( ROW_LEN - 1 )
#define	MONS_NUM	3

/* globals */
unsigned char map[ MAP_SIZE ];
unsigned char k_input = 0x00;
unsigned char plyr_pos,
	      temp_pos;
unsigned char mons_xy[ MONS_NUM ],
	      mons_hp[ MONS_NUM ],
	      mons_ch[ MONS_NUM ];

/* function declarations */
void parsInpt();
void updtPlyr();
void updtMons();
void printMap();

/* main func */
void main()
{
	unsigned char i;
	/* create map */
	memset( map, '#', MAP_SIZE );
	for ( i = ROW_LEN + 1; i < MAP_SIZE - ROW_LEN; i += ROW_LEN )
		memset( map + i, '.', ROW_LEN - 2 );
	
	/* clear screen once first */
	clrdraw();

	/* set up vars */
	plyr_pos = 25;

	/* initialize monsters */
	for ( i = 0; i < MONS_NUM; i++)
	{
		do
		{
			mons_xy[ i ] = rand() % MAP_SIZE;
		}
		while ( map[ mons_xy[i] ] != '.' );
		mons_ch[ i ] = 'M';
		map[ mons_xy[ i ] ] = mons_ch[ i ];
	}

	/* game loop */
	do
	{
		/* read key input */
		parsInpt();

		/* player routines */
		updtPlyr();

		/* monster routines */
		updtMons();

		/* draw screen */
		clrscr();
		printMap();

		/* get input */
		k_input = input();
	}
	while ( k_input != 'q' );
}

/* function definitions */
/* void parsInpt()
 *
 * parses k_input
 */
void parsInpt()
{
	temp_pos = plyr_pos;
	/* parsing table */
	switch( k_input )
	{
		case 'w':
			temp_pos -= ROW_LEN;
			break;
		case 'a':
			temp_pos--;
			break;
		case 's':
			temp_pos += ROW_LEN;
			break;
		case 'd':
			temp_pos++;
			break;
		default:
			break;
	}
}

/* void updtMons()
 *
 * updates monster
 */
void updtMons()
{
	unsigned char i;

	for ( i = 0; i < MONS_NUM; ++i )
	{
		/* generate random position */
		unsigned char dir = rand() & 3;
		unsigned char mtmp_pos;
		mtmp_pos = mons_xy[ i ];
		switch ( dir )
		{
			case 0:
				mtmp_pos -= ROW_LEN;
				break;
			case 1:
				mtmp_pos--;
				break;
			case 2:
				mtmp_pos += ROW_LEN;
				break;
			case 3:
				mtmp_pos++;
				break;
			default:
				break;
		}

		if ( map[ mtmp_pos ] == '#' || mtmp_pos == plyr_pos )
		{
			mtmp_pos = mons_xy[ i ];
		}
		map[ mons_xy [ i ] ] = '.';
		mons_xy[ i ] = mtmp_pos;
		map[ mons_xy[ i ] ] = mons_ch[ i ];
	}

	/* print monsters to map */
	for ( i = 0; i < MONS_NUM; ++i )
	{

		map[ mons_xy[ i ] ] = mons_ch[ i ];
	}
}

/* void updtPlyr()
 *
 * updates player based upon temp_pos
 */
void updtPlyr()
{
	/* draw floor at current player */
	map[ plyr_pos ] = '.';
	/* check for walls */
	if ( map[ temp_pos ] == '#' )
		temp_pos = plyr_pos;
	/* update player position to floor */
	map[ temp_pos ] = '@';
	plyr_pos = temp_pos;
}

/* void printMap()
 *
 * prints map contents. Auto-formats new line
 */
void printMap()
{
	unsigned char i;
	for ( i = 0; i < MAP_SIZE; ++i )
	{
		putchar( map[ i ] );
		if ( ! ( ( i + 1 ) & ( ROW_MASK ) ) )
		{
			newline();
		}
	}
}
