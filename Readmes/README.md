# More informations about SANDAL2

Tests: [![tests status](https://gitlab.com/BaptistePR/SANDAL2/badges/master/pipeline.svg)](https://gitlab.com/BaptistePR/SANDAL2/commits/master)
  
SANDAL2 is a SDL2 wrapper which purpose is to make object managment and
graphic display easier.  

To generate the Doxygen documentation, use the command:  
`doxygen Doxyfile`  
<br/>

## Contributors

Huge thanks to dabaldassi for the contribution he had for this project : biggest tester, user and promoter of SANDAL2.  

[dabaldassi](https://github.com/dabaldassi) : version 1.2.3  

## Useful links

From this "read-me", you can either go to :
* the [main read-me](../README.md) ;
* a [small tutorial](Tutorial.md) explaining how to use it ;
* or the page to report an issue about a tutorial lacking things, an update that could be done or anything that comes to your mind [here](https://github.com/Klevh/SANDAL2/issues/new).

## Versions

0.0.0 : SDL2TK<br/>
0.1.0 : adding rotations<br/>
0.2.0 : clickable areas<br/>
0.3.0 : animation's managment<br/>
1.0.0 : project renamed SANDAL2<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;hide window objects from the user<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;hide element's lists from the user<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;going from an element's list structure to a list of list (display code) of list (plan) of elements<br/>
1.0.1 : possibility to hide event managment from the user<br/>
1.0.2 : adding shaded and blended text rendering<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;adding and correcting error's functions return<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;wrapping of TTF's font style<br/>
1.0.3 : adding identifier to sprites<br/>
1.0.4 : adding the possibility to set the window's icon<br/>
1.0.5 : adding the getter for the text of an element<br/>
1.0.6 : correcting the isDisplaied function<br/>
1.1.0 : correcting initIteratorElementSDL2 function<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;adding a freeing function to element's datas<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;adding a function to free an element's data without getting it<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;adding a structure that contains every function pointer (event handler)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;adding an event handler to windows so that the user can add window events<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;adding functions to allow the user to add window event functions to a window<br/>
1.1.1 : adding functions so that windows can be closed in binded functions<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;changing error return type from int to unsigned long for event functions on more than one window<br/>
1.1.2 : changing functions not meant for the user to static in their source file, removing them from the header<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;adding the cast on all mallocs for C++ portability<br/>
1.1.3 : optimizing window managment<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;renaming HitBox by Clickable<br/>
1.1.4 : adding the possibility to change the correspondance between the origin of the window and the origin of all the elements placed<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;correcting C++ compatibility by renaming delete attribute of WindowSDL2 deleted<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;passing char * to const char * when possible<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;adding clearElementToElementSDL2, clearElementWindowSDL2, clearDisplayCode and clearPlanDisplayCode<br/>
1.2.0 : removing SDL2 from all the names (structures, functions, ...)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;changing all functions name beginning with change to make them begin with set instead<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;changing all functions name beginning with remove to make them begin with del instead<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;rennaming initWindow to createWindow<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;rennaming setDisplayElement to setDisplayCodeElement<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;rennaming isDisplaied to isDisplaiedElement<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;rennaming createAnimationElement to addAnimationElement<br/>
1.2.1 : adding flipping on images<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;rennaming resizeElement to setDimensionElement<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;adding functions getCoordXElement, getCoordYElement, getWidthElement, getHeightElement, setCoordXElement, setCoordYElement, setWidthElement and setHeightElement<br/>
1.2.2 : improving C++ compatibility <br/>
1.2.3 : adding mouse wheel events and mouse motion events support <br/>
1.2.4 : bug fix on setPlanElement<br/>
1.2.5 : bug fix on Clickable and adding iterator for blocking elements of ListClickable<br/>
1.3.0 : changing the way windows are handled. You now have a user current and a currently displayed.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Bug corrected when closing a window (multi window mode)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Updating makefile to hide some command executed (copy of documentation in package)<br/>
1.4.0 : adding Action.h/.c to help handle basic actions on elements<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Adding pre made actions<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Adding a 'forever' mode for an action list<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Adding add/del an action on an element<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Removing useless window event functions (clickAll, unClickAll, keyReleasedAll and keyPressedAll)<br/>

## Project's size

Action.c      : 727 lines<br/>
Clickable.c   : 349 lines<br/>
DisplayCode.c : 80 lines<br/>
Element.c     : 2,097 lines<br/>
Font.c        : 319 lines<br/>
SANDAL2.c     : 1,215 lines<br/>
Sprite.c      : 344 lines<br/>
Window.c      : 446 lines<br/>
<br/>
Total for source files : 5,577 lines<br/>
<br/>
Action.h      : 301 lines<br/>
Clickable.h   : 230 lines<br/>
DisplayCode.h : 78 lines<br/>
Element.h     : 992 lines<br/>
Font.h        : 115 lines<br/>
SANDAL2.h     : 217 lines<br/>
Sprite.h      : 195 lines<br/>
Window.h      : 334 lines<br/>
<br/>
Total for header files : 2,462 lines<br/>
<br/>
Total without tests : 8,039 lines<br/>
<br/>
tests for Action      : 279 lines<br/>
tests for Clickable   : 274 lines<br/>
tests for DisplayCode : 67 lines<br/>
tests for Element     : 795 lines<br/>
tests for Font        : 115 lines<br/>
tests for SANDAL2     : 91 lines<br/>
tests for Sprite      : 216 lines<br/>
tests for Window      : 182 lines<br/>
<br/>
Total for test files : 2,019 lines<br/>
<br/>
<br/>
Total with tests : 10,058 lines<br/>
