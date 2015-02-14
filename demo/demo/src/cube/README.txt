______________________________________
PROGRAM:

"Cube", Linux g++ build, Nathan Sema (CSc 433)

______________________________________
OVERVIEW:

/*
 * Program:
 *		CSc 533 - Cube by Nathan Sema.
 *
 * Overview:
 * 		The Cube program loads 6 images as a "ImageData" jpeg image structures
 *		where each image corresponds to a given face of a cube.
 *
 *		The a opengl mode is implemented in which the cube is displayed using
 *		the hardware-accelerated GL pipeline depicted by gluPerspective().
 *		Notice this mode is much more efficient granted the images are textured
 *		on the sides of the cube.
 *
 *		Rotations are implemented accordingly:
 *
 *			Rotation around the Y axis : 'y'
 *			Rotation around the X axis : 'x'
 *			Rotation around the Z axis : 'z'
 *			Rotation around the diagonal axis : 'd'
 *
 *		Pressing the 's' key switches the direction in which cube is rotating.
 *
 *		Pressing the '-'/'+' decreases/increases the degree of rotation at which
 *		the cube is rotating.
 *
 *		Pressing the 'r' key resets the cube to its initial front face. The 'q'
 *		key exits the program, first destroying the loaded textured images and rgb 
 *		pixel data accordingly.
 *
 * Acknowledgements:
 *		Modified from a demonstrator program by Leonard D. Brown,
 *		University of Arizona.
 *
 *		Portions of demo code provided by Kobus Barnard,
 *		University of Arizona.
 *
 *		ByteRaster class and methods modified from Michael Garland's
 *		LibGFX library in accordance with granted terms and conditions
 *		(see http://graphics.cs.uiuc.edu/~garland/software/libgfx.html)
 *
 * To make this code relatively independent of build paths and install
 *      	paths, I've based directory paths in the VS project on environment
 *		variables GLUT_PATH and LIBJPEG_PATH.  These environment variables
 *		must be defined by the OS should point to the root directories of
 *		the GLUT and LIBJPEG libraries, respectively.
 *
 * If your system is lacking these APIs, you can find them here:
 *		GLUT: http://www.xmission.com/~nate/glut.html
 *		LibJPEG: http://www.ijg.org/
 *		Windows: Use the library provided on the CSc433 website OpenGL page.
 *					Other versions may cause jpeg_read_header to crash due
 *					to standard C library incompatibility issue with MSVC
 *					unless you rebuild the entire library from source.
 */
______________________________________
DEPENDENCIES & INSTALLATION:

To build the program, you'll need the GLUT and LibJPEG library binaries, in addition to the header files.  Links to these libraries are available on the course website. The Makefile assumes that these libraries are part of your platform's standard installation, and makes no explicit reference to [alternative] include or library paths.  If this is not the case, you'll need to adjust the build file settings accordingly.

Simply type "make" to make all relevant object files and to produce an executable.	
______________________________________
ACKNOWLEDGEMENTS:

This program is initially modified from a previous application: "paintdemo", Linux g++ build, Leonard D Brown.

This program uses source code, with citation, from Kobus Barnard, PhD (UA) and Michael Garland, PhD (UIUC, NVidia, Inc).
