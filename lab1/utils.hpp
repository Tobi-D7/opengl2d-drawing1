//
//  utils.h
//  lab1
//
//  Created by Sam Xie on 1/2/18.
//  Copyright © 2018 University of Singapore. All rights reserved.
//

#ifndef utils_h
#define utils_h

// CS3241Lab1.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <3ds.h>
#include <GL/picaGL.h>
#include <GL/glu.h>
/* Include header files depending on platform */
#ifdef _WIN32
#include "glut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

#endif /* utils_h */
