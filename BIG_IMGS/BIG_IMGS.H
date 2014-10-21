/*! @file BIG_IMGS.H
@brief declaration of functions for loading images

BIG_IMGS library allow to load images in the context of the BIG library.
To use this library in your program you need to include the BIG_IMGS.H 
include file. The program will also need to be linked with the BIG_IMGS.LIB
library. The library does not define any new type it just offers 6 new
functions described below.

@verbatim
BIG = "BIG Is GEM" - A high level GEM library. 
Initial Development by Claude ATTARD, Maintenance by Jean LOUIS-GUERIN
Copyright (c) 1993-2014 Claude ATTARD
Copyright (c) 2010-2014 Jean LOUIS-GUERIN

website: http://info-coach.fr/atari/software/system/big.php
forum:   http://www.atari-forum.com/viewtopic.php?f=16&t=27060

The BIG library may be used and distributed without restriction provided that 
this copyright statement is not removed from the file and that any derivative 
work contains the original copyright notice and the associated disclaimer.

The BIG library  is free software; you can redistribute it and/or modify  it 
under the terms of the GNU General Public License as published by the Free 
Software Foundation; either version 3 of the License, or (at your option) any 
later version.

The BIG library is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
HxCFloppyEmulator; if not, write to the Free Software Foundation, Inc., 51 
Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
@endverbatim

*/

#ifndef _BIG_IMGS_H_
#define _BIG_IMGS_H_
#include <BIG2.H>

/*! @addtogroup image_group
@{ */

/*! @brief Loads (and unpack if necessary) an image
@param	file	Name of the image file (may include the complete path)
@param	img		Pointer to a MFDB in which the parameters for the image will be
				filled by the load_image function.
@return			true if success; false if failure (wrong image format, could not
				allocate memory, image resolution does not match current 
				resolution ...

This function loads and unpack if necessary images of type IMG, PAC, PI?, or PC?

@note The function determine the format of the specified file and works accordingly.
It is the <b>responsibility of the caller</b> to release the memory pointed by
img.fd_addr before leaving the application. */
int load_image (char *file, MFDB *img);

/*! @brief Loads (and unpack if necessary) an image of type IMG
@param	file	Name of the image file (may include the complete path)
@param	img		Pointer to a MFDB in which the parameters for the image will be
				filled by the load_image function.
@return			true if success; false if failure (wrong image format, could not
				allocate memory, image resolution does not match current 
				resolution ...

This function loads and unpack if necessary images of type IMG

@note The function determine the format of the specified file and works accordingly.
It is the <b>responsibility of the caller</b> to release the memory pointed by
img.fd_addr before leaving the application. */
int load_img (char *file, MFDB *img);

/*! @brief Loads (and unpack if necessary) an image of type PAC
@param	file	Name of the image file (may include the complete path)
@param	img		Pointer to a MFDB in which the parameters for the image will be
				filled by the load_image function.
@return			true if success; false if failure (wrong image format, could not
				allocate memory, image resolution does not match current 
				resolution ...

This function loads and unpack if necessary images of type PAC (Stad)

@note The function determine the format of the specified file and works accordingly.
It is the <b>responsibility of the caller</b> to release the memory pointed by
img.fd_addr before leaving the application. */
int load_pac (char *file, MFDB *img);

/*! @brief Loads (and unpack if necessary) an image of type PI?
@param	file	Name of the image file (may include the complete path)
@param	img		Pointer to a MFDB in which the parameters for the image will be
				filled by the load_image function.
@return			true if success; false if failure (wrong image format, could not
				allocate memory, image resolution does not match current 
				resolution ...

This function loads and unpack if necessary images of type PI1, PI2, or PI3
(unpacked Degas images) depending on the resolution

@note The function determine the format of the specified file and works accordingly.
It is the <b>responsibility of the caller</b> to release the memory pointed by
img.fd_addr before leaving the application. */
int load_pix (char *file, MFDB *img);

/*! @brief Loads (and unpack if necessary) an image of type PC?
@param	file	Name of the image file (may include the complete path)
@param	img		Pointer to a MFDB in which the parameters for the image will be
				filled by the load_image function.
@return			true if success; false if failure (wrong image format, could not
				allocate memory, image resolution does not match current 
				resolution ...

This function loads and unpack if necessary images of type PC1, PC2, or PC3
(packed Degas images) depending on the resolution.

@note The function determine the format of the specified file and works accordingly.
It is the <b>responsibility of the caller</b> to release the memory pointed by
img.fd_addr before leaving the application. */
int load_pcx (char *file, MFDB *img);


/*! @brief Loads and unpack an IMG file
@param	bimg	Pointer to a t_image_dial structure (as defined in BIG) that is
				used to describe an image for the display routine of this image 
				object.
@param	file	Image file name including if necessary the path

@param	tree	Number of the forms that contains the object.
@param	obj		Number of the object in the form.
@param	fcenterw Flag to indicate if the object must be width centered in the
				parent object
@param	fcenterh Flag to indicate if the object must be height centered in the
				parent object
@return			true if success; false if failure (wrong image format, could not
				allocate memory, image resolution does not match current 
				resolution ...

Loads and unpack an IMG file to an B_PICT object (image in a form)

@note The width and Height of the object are adapted by the function to match the
width and height of the image. The  position is eventually modified according to
the setting of the flags. Normally it is not required to free the memory reserved
for the image as the BIG end() function take care of it. The function only handle
standard IMG format without color palette. */
int img_attach (t_image_dial *bimg, char *file, int tree, int obj, int fcenterw, int fcenterh);

/*! @} */

#endif

