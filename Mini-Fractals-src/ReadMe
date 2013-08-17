/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//

** Application Notes:

If you get a yellow square for an image, it means your hardware can run the GLSL code, but lacks the appropriate shaders. This is true on ati 4250 mobile GPU.
This will not run on intel 3000, so I doubt it will work on i3, i5, i7 unless a discrete PCIe x16 Video card is installed. 
This runs very fast on AMD APUs, such as the FM1 and FM2 lines. GPU calculation times are 10 to 100 times faster than on CPU on A10-5700 with ati 7660D (384 shaders) 
This app has a known flaw with ati 4650/4670 GPUs, which shows as garbage video in 'set' (black) areas. Why is unknown to the author.
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



/////////////////

** Credits:





** for helpful info and tutorials, Thanks to : 





