/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//

** Application Notes:


Mini-Fractals is an OpenGL/MFC Appplication, that produces Orbital type Fractals, like the Mandelbrot Set.
It has two different Color Schemes, gradient color system and a e-Smoothing Coloring System.
The Calculations to produce the Fractal are done by your computers GPU, or Graphical Processing Unit, 
AKA a big Computer chip.

The Application can compile the User Entered code into GLSL language, using a complex number functions,
Such as ADD[x]  SUB[a,b] == a-b  ... ATANH[x] where x,a and b are complex numbers.



Help :

Getting The App to Up and Running Correctly with OpenGL :

Although using Windows Update, or a 3rd party Driver-Booster 
will have relatively new and functional drivers,
I have found that by installing The Whole, Large ATI Catalyst CCC,
About 70-120 MB in size, that Mini-Fractals will run (somewhat slow, about 10 FPS) on :
ATI 4250 Mobile, with 40 shaders. 
Before installing the whole CCC, the drivers where current to Nov. 2013, 
Although the App failed completely.


If you get a yellow square for an image, it means your hardware can run the GLSL code, 
but lacks the the GPU cards manufactures Software Package to do so correctly. 
Without the full ATI CCC software installed, This will be true on ati 4250 mobile GPU. 
This runs very fast on AMD APUs, such as the FM1 and FM2 lines. GPU calculation times are 
10 to 100 times faster than on CPU on A10-5700 (3.4 GHz Quad CPU) with ati 7660D (384 shaders).
 
This app has a known flaw with ati 4650/4670 GPUs, which shows as garbage video in 'set' (black) areas. 
This is Probably due the ATI CCC not being installed.
The app does run on ati 5450.
The app does NOT run on Geforce FX 5200.
If you can run DirectX 10, you "should" be able to run the app successfully.
The app will (hopefully) be graceful in informing you of issues, but it is a beta, so, please execuse any crashes.

It is hoped the Complex Library will be of great help to many who are using GLSL.


////////////////

** Notes on User Equations :

All Complex Mathematical Functions, like sin, cos, tan, asin, acos, asinh, ln, log, log10, exp, etc. use [...] and are lower case only, ie: sin[ZZ]  

All GLSL functions and code are availible, just enter all (...) as [...] and the translator will change them to '(' and ')' ... technical reasons = (Cheap Translator) ... :)

The use of '(' and ')' is for algebric use only, as in:  ZZ * ZZ + CC / (1+ floatG * ZZ)  ... hope this clarifies it.

[] for all functions and GLSL language use,  () for algrebra. 

Inside a pair of () must be some kind of operator: +-/*  otherwise use [] 

All lines must end with ; 

Read:   shader-code-workspace.fs   file for all the available variables and functions. There are many.
See :   gl-spec pdf for GLSL language help. I think this is using 2.1 spec, but I've included 3.3 and 1.2 also.



=========================================
=========================================

From Mini-Fractals-User-Function_code-tips+help.txt :



Install the Whole ATI or nVidia Software Package, a 70-120 MB file, more than just a driver ...

Successfully runs on ATI 4250 Mobile GPU with full ATI CCC ... OpenCL does NOT !!



Help :

On Using the 'User Entered Equations' :

How to get the 'stupid' translator to properly digest more complicated algebraic statements ...

Exactly like this :

ZZ = sin[ZZ] / CC * floatG + ((pow[ZZ,(floatG+4)] + CC)*(1-floatG)) ;


Note:
pay carefule attention to the use of whitespace (space character) 
and before and after +-*/  and before and after (,) 

In some places use no spaces ...
In others places pad with both spaces ...



Fun and interesting Tip:

Try the above example function in the 'User Entered Equation' text box, 
make sure it's just a single line in the box.
Set the 'floatG' value in it's text box to -1.0
Set the 'Animate floatG from above to:' value to 1.0 
and check the 'Animate' checkbox.
Finally, click on 'Make Fractal' Button.
Enjoy the Show.
Un-check the 'Animate' Checkbox to STOP the action.













/////////////////

** Credits:





** for helpful info and tutorials, Thanks to : 





