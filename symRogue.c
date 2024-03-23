#include "symRogue.h"

/* constants */
#define	ROW_LEN		16
#define	COL_HEIGHT	10
#define	MAP_SIZE	ROW_LEN * COL_HEIGHT
#define	ROW_MASK	( ROW_LEN - 1 )
#define	MONS_NUM	6
#define	MONS_XP		10
#define	MONS_ATK	5
#define	PLYR_ATK	10
#define	P_MAX_HP	50
#define	M_MAX_HP	30

/* globals */
char t_str[4];
unsigned char map[ MAP_SIZE ];
#if SYM==1
#pragma bss-name (push,"ZEROPAGE")
unsigned char k_input = 0x00;
unsigned char plyr_pos,
	      temp_pos,
	      mtmp_pos,
	      plyr_hp,
	      g_i;
#pragma bss-name (pop)
#else
unsigned char k_input = 0x00;
unsigned char plyr_pos,
	      temp_pos,
	      mtmp_pos,
	      plyr_hp,
	      g_i;
#endif

unsigned char mons_xy[ MONS_NUM ],
	      mons_hp[ MONS_NUM ],
	      mons_ch[ MONS_NUM ];

/* function declarations */
void parsInpt();
void updtPlyr();
void updtMons();
void drawMons();
void mtpCmbt();
void ptmCmbt();
void printMap();
void prntStat();

/* main func */
void main()
{
	/* create map */
	memset( map, '#', MAP_SIZE );
	for ( g_i = ROW_LEN + 1; g_i < MAP_SIZE - ROW_LEN; g_i += ROW_LEN )
		memset( map + g_i, '.', ROW_LEN - 2 );
	
	/* clear screen once first */
	clrdraw();

	/* set up vars */
	plyr_pos = 25;
	plyr_hp  = P_MAX_HP;

	/* initialize monsters */
	for ( g_i = 0; g_i < MONS_NUM; ++g_i)
	{
		do
		{
			mons_xy[ g_i ] = rand() % MAP_SIZE;
		}
		while ( map[ mons_xy[ g_i ] ] != '.' );
		mons_ch[ g_i ] = 'M';
		map[ mons_xy[ g_i ] ] = mons_ch[ g_i ];
		mons_hp[ g_i ] = M_MAX_HP;
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
		
		for ( g_i = 0; g_i < MONS_NUM; ++g_i )
		{
			if ( mons_ch[ g_i ] == '%' )
			{
				if ( map[ mons_xy[ g_i ] ] == '.' )
				{
					map[ mons_xy[ g_i ] ] = '%';
				}
			}
		}

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
	/* draw floor at current player */
	map[ plyr_pos ] = '.';
	/* check for walls */
	if ( map[ temp_pos ] == '#' )
		temp_pos = plyr_pos;
	/* check for monsters */
	for ( g_i = 0; g_i < MONS_NUM; ++g_i)
	{
		if ( ( temp_pos == mons_xy[ g_i ] )
			&& ( mons_ch[ g_i ] != '%' ) )
		{
			/* combat */
			ptmCmbt( g_i );
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
	for ( g_i = 0; g_i < MONS_NUM; ++g_i )
	{
		char dir;
		unsigned char mtmp_pos;

		unsigned char px = plyr_pos % ROW_LEN;  // Player X position
		unsigned char py = plyr_pos / ROW_LEN;  // Player Y position
		unsigned char mx = mons_xy[g_i] % ROW_LEN;  // Monster X position
		unsigned char my = mons_xy[g_i] / ROW_LEN;  // Monster Y position

		mtmp_pos = mons_xy[ g_i ];
		
		/* check to see if monster is alive even */
		if ( mons_ch[ g_i ] == '%' )
			continue;
		/* 50% chance of moving toward you if < 5 in square */
		if ( ( abs( mx - px ) < 2 ) && ( abs( my - py ) < 2 ) )
		{
			if ( ( rand() & 1 ) )
				goto m_randMv;
			if ( py > my )
				mtmp_pos += ROW_LEN;
			else if ( px > mx )
				mtmp_pos++;
			else if ( py < my )
				mtmp_pos -= ROW_LEN;
			else if ( px < mx )
				mtmp_pos--;
		}
		else
		{
m_randMv:
			/* generate random position */
			dir = rand() & 3;
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
		}
		if ( map[ mtmp_pos ] == '#' )
		{
			mtmp_pos = mons_xy[ g_i ];
		}
		if ( map[ mtmp_pos ] == 'M' )
		{
			mtmp_pos = mons_xy[ g_i ];
		}
		if ( mtmp_pos == plyr_pos )
		{
			/* combat */
			mtpCmbt();
			mtmp_pos = mons_xy[ g_i ];
		}
		map[ mons_xy [ g_i ] ] = '.';
		mons_xy[ g_i ] = mtmp_pos;
		map[ mons_xy[ g_i ] ] = mons_ch[ g_i ];
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
		mons_ch[ i ] = '%';
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
	for ( g_i = 0; g_i < MAP_SIZE; ++g_i )
	{
		putchar( map[ g_i ] );
		if ( ! ( ( g_i + 1 ) & ( ROW_MASK ) ) )
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
	puts("HP:\t");
	puts( t_str );
}