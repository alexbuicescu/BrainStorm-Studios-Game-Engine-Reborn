#pragma once
#ifndef BSANIMATIONCLASS_H_INCLUDED
#define BSANIMATIONCLASS_H_INCLUDED

#include "BSValues.h"
#include "BSRenderer.h"

class BSAnimationClass
{
public:

    void addAnimation(int nrOfAnimations1, int nrOfAnimations2, int nrOfAnimations3, int AnimationTime1, int AnimationTime2, int AnimationTime3,
                                    int loop1, int loop2, int loop3, int AnimationIndex, std::string name);

};


#endif // BSANIMATIONCLASS_H_INCLUDED
