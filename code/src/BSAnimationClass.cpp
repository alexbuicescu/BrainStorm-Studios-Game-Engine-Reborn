#include "BSAnimationClass.h"

void BSAnimationClass::addAnimation(int nrOfAnimations1, int nrOfAnimations2, int nrOfAnimations3, int AnimationTime1, int AnimationTime2, int AnimationTime3,
                                    int loop1, int loop2, int loop3, int AnimationIndex, std::string name)
{
    BSAnimList *p = new BSAnimList;
    p->nr1 = nrOfAnimations1;
    p->nr2 = nrOfAnimations2;
    p->nr3 = nrOfAnimations3;
    p->loop1 = loop1;
    p->loop2 = loop2;
    p->loop3 = loop3;

	for(int i = 0 ; i < nrOfAnimations1 + nrOfAnimations2 + nrOfAnimations3; i++)
	{
	    BSAnimation *q = new BSAnimation;

		///Setez timpul dintre fiecare animatie
		if(i < nrOfAnimations1)
		{
			q->timeUntilNext = AnimationTime1;
		}
		else
			if(i >= nrOfAnimations1 && i < nrOfAnimations1 + nrOfAnimations2)
			{
				q->timeUntilNext = AnimationTime2;
			}
			else
				if(i >= nrOfAnimations1 + nrOfAnimations2)
				{
					q->timeUntilNext = AnimationTime3;
				}

    	///Numele cu care voi incarca textura
		///Numele texturii + nr + extensie
		std::string nameOfAnimation = name + BStoString(i) + ".png";
		///Incarca textura
    	loadATexture(nameOfAnimation.c_str(), q->texture);

    	///if it is the berserk, I make the animation bigger
    	if(AnimationIndex == 11 || AnimationIndex == 12 || AnimationIndex == 13 || AnimationIndex == 14)
    	{
    	    q->width = 2;
            q->height = 2;
    	}
    	else
        {
            q->width = 1;
            q->height = 1;
    	}
    	q->vao = originalBody->vao;
    	q->vbo = originalBody->vbo;
    	q->ibo = originalBody->ibo;

    	q->color.setColorsTo1();
        p->animation.push_back(q);
    }
    listOfLoadedAnimations.push_back(p);
}
