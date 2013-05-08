# GLOffView

GLOffView is a simple command line tool for viewing 3D images in .off files.

It was written in 2003 for a University assignment.

## Requirements

 * Linux, Mac OS X (requires X Code and CLI tools).
 * OpenGL, GLUT
 * GCC or CLANG

## Setup

#### 1. Grab the source

    $ git clone git://github.com/calebbrown/gloffview.git

#### 2. Make the code

    $ make

#### 3. Run!

    $ ./gloffview -t examples/harley.off


## Options

    -r {x|y|z}  - rotation axis. the axis to rotate about
    -f [n]      - total frames to render, where 'n' is the number of frames
    -w [n]      - window width & height, where 'n' is the number of pixels
    -a [n]      - angle (degrees) to rotate the model per frame
    -b          - turn ON back face culling
    -o {n|d|v}  - optimisation mode. n = normal, no optimisation
                                     d = use display lists
                                     v = use vertex arrays
    -t          - track ball mode. Interactive rotation of the model.
                  '-r','-f','-w','-a' parameters have no effect when '-t'
                  is specified as a parameter.
    -c [n]      - clocked mode. Run for 'n' seconds and quit, displaying
                  fps information.
    -d [n]      - fps dump mode. Dump the fps every 'n' seconds.
