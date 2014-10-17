/*
		LZW.H
		Compression de donn‚es
*/

#if  !defined( __LZW__ )
#define __LZW__

unsigned int Compression (unsigned char *Source, unsigned char *Dest, unsigned int SourceSize);
unsigned int Decompression (unsigned char *Source, unsigned char *Dest, unsigned int SourceSize);

#endif

