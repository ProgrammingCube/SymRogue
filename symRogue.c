#include "symRogue.h"

/* constants */
#define	ROW_LEN		16
#define	COL_HEIGHT	10
#define	MAP_SIZE	ROW_LEN * COL_HEIGHT
#define	ROW_MASK	( ROW_LEN - 1 )
#define	MONS_NUM	3
#define	MONS_XP		10
#define	MONS_ATK	5
#define	PLYR_ATK	10
#define	P_MAX_HP	50
#define	M_MAX_HP	30

/* globals */
char t_str[4];
char stat_str[20];
unsigned char map[ MAP_SIZE ];
#if SYM==1
#pragma bss-name (push,"ZEROPAGE")
unsigned char k_input = 0x00;
unsigned char plyr_pos,
	      temp_pos,
	      plyr_hp;
#pragma bss-name (pop)
#else
unsigned char k_input = 0x00;
unsigned char plyr_pos,
	      temp_pos,
	      plyr_hp;
#endif

unsigned char mons_xy[ MONS_NUM ],
	      mons_hp[ MONS_NUM ],
	      mons_ch[ MONS_NUM ];

/* function declarations */
void parsInpt();
void updtPlyr();
void updtMons();
void mtpCmbt();
void ptmCmbt();
void printMap();
void prntStat();

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
	plyr_hp  = P_MAX_HP;

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
		mons_hp[ i ] = M_MAX_HP;
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

		/* update player position AFTER all updates */
		map[ plyr_pos ] = '@';

		/* draw screen */
		clrscr();
		printMap();

		/* print status bar */
		prntStat();

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

/* void updtPlyr()
 *
 * updates player based upon temp_pos
 */
void updtPlyr()
{
	unsigned char i;
	/* draw floor at current player */
	map[ plyr_pos ] = '.';
	/* check for walls */
	if ( map[ temp_pos ] == '#' )
		temp_pos = plyr_pos;
	/* check for monsters */
	for ( i = 0; i < MONS_NUM; ++i)
	{
		if ( ( temp_pos == mons_xy[ i ] ) && ( mons_ch[ i ] != '%' ) )
		{
			/* combat */
			ptmCmbt( i );
			temp_pos = plyr_pos;
		}
	}

	/* update player position to floor */
	plyr_pos = temp_pos;
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
		/* check to see if monster is alive even */
		if ( mons_ch[ i ] == '%' )
			goto endDMons;
		
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

		if ( map[ mtmp_pos ] == '#' )
		{
			mtmp_pos = mons_xy[ i ];
		}
		if ( map[ mtmp_pos ] == 'M' )
		{
			mtmp_pos = mons_xy[ i ];
		}
		if ( mtmp_pos == plyr_pos )
		{
			/* combat */
			mtpCmbt();
			mtmp_pos = mons_xy[ i ];
		}
		map[ mons_xy [ i ] ] = '.';
		mons_xy[ i ] = mtmp_pos;
endDMons:
		map[ mons_xy[ i ] ] = mons_ch[ i ];
	}
}

/* void ptmCmbt( i )
 *
 * enters player to monster combat for mons[ i ]
 */
void ptmCmbt( i )
unsigned char i;
{
	if ( ( mons_hp[ i ] -= PLYR_ATK ) <= 0 )
	{
		mons_ch[i] = '%';
	}
}

/* void mtpCmbt()
 *
 * enters monster to player combat
 */
void mtpCmbt()
{
	if ( ( plyr_hp -= MONS_ATK ) <= 0 )
	{
		/* quit */
		puts("You died...");
		k_input = 'q';
	}
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

/* void prntStat()
 *
 * prints status message
 */
void prntStat()
{
	itoa( plyr_hp, t_str, 10 );
	memcpy( stat_str, "HP:\t", 4 );
	memcpy( stat_str + 4, t_str, 4 );
	puts( stat_str );
}