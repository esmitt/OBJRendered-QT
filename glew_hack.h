/**
 * This file contains a little trick that allows the use of the GLEW library
 * with Qt, since both library are normally mutually exclusive.
 **/

/*
 * Include the GLEW header only once.
 */
#ifndef  GL_H
    #define GL_H
    #include <GL/glew.h>
#endif // GL_H

/*
 * Define a glew_hack function to initialize GLEW, but define it only once, on
 * whichever file defines the symbol GLEW_HACK_IMPLEMENTATION
 */
#ifdef GLEW_HACK_IMPLEMENTATION
#include <iostream>
#include <cstdlib>

static void glew_hack() {
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization fail" << std::endl;
        exit(EXIT_FAILURE);
    }
}
#endif // GLEW_HACK_IMPLEMENTATION
