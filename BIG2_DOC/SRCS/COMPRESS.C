#include "LZW.H"
#include <stdlib.h>

#define FLAG_Copied 		0x80					/* */
#define FLAG_Compress		0x40					/* */
enum {FALSE,TRUE};

int GetMatch (unsigned char *Source, unsigned int X, unsigned int SourceSize,
							signed int *Hash, unsigned int *Size, signed int *Pos);

/* #[ GetMatch : */
int GetMatch (unsigned char *Source, unsigned int X, unsigned int SourceSize,
							signed int *Hash, unsigned int *Size, signed int *Pos)
{
unsigned int HashValue = (40543L* ((((Source[X] << 4) ^
												 Source[X + 1]) << 4) ^
												 Source[X + 2]) >> 4) &
												 0xfff;

	*Pos = Hash[HashValue];
	Hash[HashValue] = X;
	if ((*Pos != -1) && ((X - *Pos) < 4096))
	{
		for (*Size = 0; ((*Size < 18)
			&& (Source[X+*Size] == Source[*Pos+*Size])
			&& ((X+*Size) < SourceSize)); (*Size)++);
		return(*Size >= 3);
	}
	return (FALSE);
}
/* #] GetMatch : */ 
/* #[ Compression : */
unsigned int Compression (unsigned char *Source, unsigned char *Dest, unsigned int SourceSize)
{ 	 
signed int *Hash = calloc(4096,1), Pos;
unsigned int Key, Size;
unsigned char Bit = 0;
unsigned int Command = 0;
unsigned int X = 0;
unsigned int Y = 3;
unsigned int Z = 1;

	for (Key = 0 ; Key < 4096 ; Key++)
		Hash[Key] = -1;
	Dest[0] = FLAG_Compress;
	
	for ( ; (X < SourceSize) && (Y <= SourceSize) ; )
	{
		if (Bit > 15)
		{
			Dest[Z++] = (Command >> 8) & 0x00ff;
			Dest[Z] = Command & 0x00ff;
			Z = Y;
			Bit = 0;
			Y += 2;
		}
			
		for (Size = 1 ;
				 (Source[X] == Source[X+Size]) &&
										(Size < 0x0fff) &&
										(X+Size < SourceSize) ;
				 Size++);
						
			if (Size >= 16)
			{
				Dest[Y++] = 0;
				Dest[Y++] = ((Size - 16) >> 8) & 0x00ff;
				Dest[Y++] = (Size - 16) & 0x00ff;
				Dest[Y++] = Source[X];
				X += Size;
				Command = (Command << 1) + 1;
			}
			else if (GetMatch (Source, X, SourceSize, Hash, &Size, &Pos))
			{
				Key = ((X-Pos) << 4) + (Size - 3);
				Dest[Y++] = (Key >> 8) & 0x00ff;
				Dest[Y++] = Key & 0x00ff;
				X += Size;
				Command = (Command << 1) + 1;
			}
			else 
			{
				Dest[Y++] = Source[X++];
				Command = (Command << 1);
			}

			Bit++;
	}
	Command <<= (16-Bit);
	Dest[Z++] = (Command >> 8) & 0x00ff;
	Dest[Z] = Command & 0x00ff;
	if (Y > SourceSize)
	{
		for (Y = 0 ; Y < SourceSize ; Dest[Y+1] = Source[Y++])
			;
		Dest[0] = FLAG_Copied;
		free (Hash);
		return (SourceSize + 1);
	}
	free (Hash);	
	return (Y);
}
/* #] Compression : */ 
/* #[ Decompression : */
unsigned int	Decompression(unsigned char *Source,unsigned char *Dest,unsigned int SourceSize)
{
unsigned int X = 3;
unsigned int Y = 0;
unsigned int Pos, Size, K;
unsigned int Command = (Source[1] << 8) + Source[2];
unsigned char Bit = 16;
	
	if (Source[0] == FLAG_Copied)
	{
		for (Y = 1; Y < SourceSize; Dest[Y-1] = Source[Y++]);
		return (SourceSize-1);
	}
	for ( ; X < SourceSize ; )
	{ 
		if (Bit == 0)
		{ 
			Command = (Source[X++] << 8);
			Command += Source[X++];
			Bit = 16;
		}
		if (Command & 0x8000)
		{
			Pos = (Source[X++] << 4);
			Pos += (Source[X] >> 4);
			if (Pos)
			{
				Size = (Source[X++] & 0x0f) + 3;
				for (K = 0 ; K < Size ; K++)
					Dest[Y + K] = Dest[Y - Pos + K];
				Y += Size;
			}
			else 
			{ 
				Size = (Source[X++] << 8);
				Size += Source[X++] + 16;
				for (K = 0 ; K < Size ; Dest[Y+K++] = Source[X])
					;
				X++;
				Y += Size;
			}
		}
		else
			Dest[Y++] = Source[X++];
		Command <<= 1;
		Bit--;
	}
	return (Y);
}
/* #] Decompression : */ 
