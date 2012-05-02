# Overview
Due to many projects reading/writing STL files, we are creating a base collection of reading/writing tools and classes so that we can quickly integrate various object file formats into our workflow.

And since we, the MakerBot development community, love you (unnamed hacker) we are going to share this awesome toolkit with you.


## Building

NOTE: We are combining codebases here. There is know duplication between functions and formats

A SConstruct file exists for the simple build for raw C+++

A Libthing.pro.inc file is included for use with qt creator builds,
and assumes QT classes are accessable, etc

## Layout

Due to multi-language, mulit-target goals of this projkect, source is laied out 
based on main vs test code by language.   Also, for clear reading includes in C, 
we have a 'libthing' directory in the include directory.

This allows, for Example, 

"""
#include <libthing/ObjectThingy.h>
#using namespace libthing

Class foo: public ObjectThingy 
{
...
}
"""