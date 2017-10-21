#include "stdafx.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#define MAX_TILES 100

typedef enum
{
	UNMARKED,
	MARKED
} marked_type;

typedef struct _tile_type
{
	char *a;
	char *b;
	unsigned m;
	unsigned n;
	unsigned length;
	struct _tile_type *next;
} tile_type;

void Greedy_String_Tiling ( tile_type *tiles, char *A, char *B, unsigned MML )
{
	unsigned j, m, n;
	unsigned maxmatch;
	tile_type matches;
	tile_type *match;
	tile_type *p, *q;
	unsigned Al = strlen ( A );
	unsigned Bl = strlen ( B );
	marked_type *Aa = ( marked_type * ) malloc ( Al*sizeof ( marked_type ) );
	marked_type *Bb = ( marked_type * ) malloc ( Bl*sizeof ( marked_type ) );

	for ( m = 0; m < Al; m++ ) Aa[m] = UNMARKED;
	for ( n = 0; n < Bl; n++ ) Bb[n] = UNMARKED;

	/* tiles = { }; ���������������Ӵ� */

	do
	{
		maxmatch = MML; /*MML:Ҫ�󹫹��Ӵ��ﵽ����С����*/
		/* matches = { }; */
		matches.next = NULL;

		/*For all unmarked Aa in A { */
		for ( m = 0; m < Al; m++ )
		{
			if ( Aa[m] == MARKED ) continue;
			/*For all unmarked Bb in B { */
			for ( n = 0; n < Bl; n++ )
			{
				if ( Bb[n] == MARKED ) continue;
				j = 0;
				/*while (Aa+j== Bb+j&& unmarked(Aa+j) && unmarked(Bb+j)) */
				/*logicielע:�����һ��������α�������,Ӧ��A+j== B+j*/
				while ( A[m + j] == B[n + j] && Aa[m + j] == UNMARKED && Bb[n + j] == UNMARKED ) j++;

				match = ( tile_type * ) malloc ( sizeof ( tile_type ) );
				match->a = A + m;
				match->b = B + n;
				match->m = m;
				match->n = n;
				match->length = j;
				match->next = NULL;
				if ( j == maxmatch )
					/* ��һ�������Ӵ�����Ϣ���뵽������
					����������matches = matches��match(a, b , j);  */
				{
					match->next = matches.next;
					matches.next = match;
				}
				else if ( j > maxmatch )
				{
					/* matches = {match(a, b, j)}; */
					p = matches.next;
					while ( p )
					{
						q = p->next;
						free ( p );
						p = q;
					}
					matches.next = match;
					maxmatch = j;
				}
			}
		}
		/*For all match(a,b, maxmatch)��matches{ ����ƥ�䲿�����ϼǺ�
		For j = 0��(maxmatch -1) {
		����  ����mark(Aa+j); mark(Bb+j); }
			  ��  ��tiles = tiles��match(a, b , maxmatch);
				  }  */
		p = matches.next;
		while ( p )
		{
			for ( j = 0; j < maxmatch; j++ )
			{
				Aa[p->m + j] = MARKED;
				Bb[p->n + j] = MARKED;
			}
			q = p;
			p = p->next;
		}
		tiles->next = matches.next;
		tiles = q;
	} while ( maxmatch > MML );

	free ( Aa );
	free ( Bb );
}

//int main ()
//{
//	unsigned i;
//	tile_type tiles;
//	tile_type *r;
//
//	Greedy_String_Tiling ( &tiles, "abcd-efgh-ijklm", "defgh-abcij--klm", 3 );
//	r = tiles.next;
//	while ( r )
//	{
//		printf ( "str=" );
//		for ( i = 0; i < r->length; i++ ) printf ( "%c", r->a[i] );
//		printf ( "\nlen=%d\n", r->length );
//		r = r->next;
//	}
//	system ( "pause" );
//	return 0;
//}

