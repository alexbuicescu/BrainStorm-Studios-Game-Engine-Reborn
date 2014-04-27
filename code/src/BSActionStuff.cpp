#include "BSActionStuff.h"
#include "My_Values.h"

///If you do not understand the code in this class you should be flipping burgers instead ;)


float TeleportX = 0, TeleportY = 0;


int jumpedFromLeftOrRight = 0;
long long OriginalTimeOnWall = 0, currentTime = 0, OriginalTimeToJumpUpTheWall = 0, timeToCreateParticles = 0;
bool imprimeForceOnWall = false, heroCanTeleportAgain = true, justHitTheGround = false;

b2Vec2 vel, velocity;
bool heroJustStopped = false;
float desiredVelX = 0, desiredVelY = 0;
float velChangeX, velChangeY;
float impulseX, impulseY, turnMultiplier = 1.f, turnMultiplierInAir = 5.5f, timeE = 0;

int nnrr = 0;


int sgn(float nr)
{
    if(nr > 0)
    {
        return 1;
    }
    else
        if(nr < 0)
        {
            return -1;
        }
        else
            if(nr == 0)
            {
                return 0;
            }
}

float modulo(float nr)
{
    if(nr < 0)
    {
        return -nr;
    }
    else
    {
        return nr;
    }
}



void BSActionStuff::controlAir(float T, b2Body *body)
{
	if (ButtonJump && pressedJumpSecond)
	{
		_BS_sound_device->play_sound(BS_Available_Sounds::jump());

		canJumpVertically = false;
		velocity.y = speedJumpUp;
		ButtonJump = false;
		body->SetLinearVelocity(velocity);
		return;
	}

	// Abort jump if user lets go of button
	if(worldGravity.y != 0)
	{
	    if (velocity.y > 0 && !ButtonJump && worldGravity.y < 0)
        {
            velocity.y = 0;
        }
        else
            if (velocity.y < 0 && !ButtonJump && worldGravity.y > 0)
            {
                velocity.y = 0;
            }
	}
	else
	{
	    if (velocity.x < 0 && !ButtonJump && worldGravity.x > 0)
        {
            velocity.x = 0;
        }
        else
            if (velocity.x > 0 && !ButtonJump && worldGravity.x < 0)
            {
                velocity.x = 0;
            }
	}


    if (isSpaceDown && hero->heroIsBerserk3 && isInAir && body->GetLinearVelocity().y <= 0)
    {
        body->ApplyForce(b2Vec2(0, 33), body->GetWorldCenter());
    }


	itWalksInAir(body, T * 0.2f);
}


///Magic is real, some lepricons made me write this method at 3 o'clock in the morning
void BSActionStuff::controlWall(b2Body *body)
{
	if(nnrr <= 2)
	{
		nnrr++;
	}

	if(nnrr == 3)
	{
		nnrr = 0;
		imprimeForceOnWall = false;
		jumpedFromLeftOrRight = 0;
	}

	//ii dau valoarea 0 pe orizontala cand se afla pe perete
	//body->ApplyLinearImpulse( b2Vec2(0,0), body->GetWorldCenter() );
	//velocity.x = 0;

    if(canJumpHorizontally && ButtonJump)
    {
        float speedVelocityX = 0, speedVelocityY = 0, impulseX = 0, impulseY = 0, linearVelocityX = 0, linearVelocityY = 0;
//        if(worldGravity.y != 0)
//        {
            speedVelocityX = velocity.x;
            speedVelocityY = velocity.y;
            linearVelocityX = hero->theBody->GetLinearVelocity().x;
            linearVelocityY = hero->theBody->GetLinearVelocity().y;
//        }
//        else
//        {
//            speedVelocityY = velocity.x;
//            speedVelocityX = velocity.y;
//            linearVelocityX = hero->theBody->GetLinearVelocity().y;
//            linearVelocityY = hero->theBody->GetLinearVelocity().x;
//        }

        _BS_sound_device->play_sound(BS_Available_Sounds::jump());
        ButtonJump = false;
        //verifica daca vrea sa  sara de pe perete pe alt perete
        if((ButtonRight && isOnLeftWall) || (ButtonLeft && isOnRightWall))
        {
            //*-1 pentru ca atunci cand e pe peretele din stanga (de exemplu) va trebui sa ii imprim
            //o forta pozitiva, dar variableWall are val -1 pe peretele din stanga
            speedVelocityX = speedJumpToSide * variableWall * (-1);
            float velChangeX = speedVelocityX - linearVelocityX;
            impulseX = body->GetMass() * velChangeX;

            speedVelocityY = speedJumpUp * 1.2f;// * variableWall * (-1);
            float velChangeY = speedVelocityY - linearVelocityY;
            impulseY = body->GetMass() * velChangeY;
        }
        else
            //verific daca vrea sa sara pe acelasi perete in sus
            if((ButtonLeft && isOnLeftWall) || (ButtonRight && isOnRightWall))
            {
                speedVelocityX = speedJumpToTheSameSide * variableWall * (-1);
                float velChangeX = speedVelocityX - linearVelocityX;
                impulseX = body->GetMass() * velChangeX;

                speedVelocityY = speedJumpUpOnWall;// * variableWall * (-1);
                float velChangeY = speedVelocityY - linearVelocityY;
                impulseY = body->GetMass() * velChangeY;

                imprimeForceOnWall = true;
                nnrr = 1;

                OriginalTimeToJumpUpTheWall = SDL_GetTicks();

                //salvez de pe care perete a sarit
                if(isOnLeftWall)
                {
                    jumpedFromLeftOrRight = -1;
                }
                else
                {
                    jumpedFromLeftOrRight = 1;
                }
            }
            else
                //verific daca vrea sa sara de pe perete
                {
                    speedVelocityX = (speedJumpToSide / 2.f) * variableWall * (-1);
                    float velChangeX = speedVelocityX - linearVelocityX;
                    impulseX = body->GetMass() * velChangeX;

                    speedVelocityY = speedJumpUp;// * variableWall * (-1);
                    float velChangeY = speedVelocityY - linearVelocityY;
                    impulseY = body->GetMass() * velChangeY;
                }


        body->SetLinearVelocity( b2Vec2(0, 0));
        if(worldGravity.y != 0)
        {
            speedVelocityY *= -sgn(worldGravity.y);
                velocity.x = speedVelocityX;
                velocity.y = speedVelocityY;
            body->ApplyLinearImpulse( b2Vec2(speedVelocityX, speedVelocityY), body->GetWorldCenter() );
        }
        else
        {
            speedVelocityY *= -sgn(worldGravity.x);
                velocity.y = speedVelocityX;
                velocity.x = speedVelocityY;
            body->ApplyLinearImpulse( b2Vec2(speedVelocityY, speedVelocityX), body->GetWorldCenter() );
        }
    }

	//verifica daca vrea sa se indeparteze de perete fara sa sara
	if(!ButtonJump)
	{
	    if((ButtonRight && isOnLeftWall) || (ButtonLeft && isOnRightWall))
        {
            if(OriginalTimeOnWall == 0)
            {
                OriginalTimeOnWall = SDL_GetTicks();
            }
            else
                //verifica timpul scurs pe perete cand apasa sa sara de pe el
                if(CanGoAwayFromWall(OriginalTimeOnWall))
                {
                    itWalksInAir(body, 1.1f);
                    OriginalTimeOnWall = 0;
                }
        }
        else
            //in cazul in care renunta sa sara, va reseta timpul in care sa sara
            if(	((ButtonRight == false && isOnLeftWall) || (ButtonLeft == false && isOnRightWall)) && OriginalTimeOnWall != 0)
            {
                OriginalTimeOnWall = 0;
            }
	}
}

void BSActionStuff::controlGround(b2Body *body, float T)
{
	imprimeForceOnWall = false;

	if (ButtonJump && canJumpVertically)
	{
		_BS_sound_device->play_sound(BS_Available_Sounds::jump());

		canJumpVertically = false;

	    if(worldGravity.y != 0)
		{
		    velocity.y = speedJumpUp * -sgn(worldGravity.y);
		}
		else
        {
            velocity.x = speedJumpUp * -sgn(worldGravity.x);
        }
		ButtonJump = false;
		body->SetLinearVelocity(velocity);
		return;
	}

	// Run on ground
	if (!itWalks(body, T))
	{
	    if(worldGravity.y != 0)
		{
		    velocity.x = 0;
		}
		else
        {
            velocity.y = 0;
        }
		body->SetLinearVelocity(velocity);
	}
}

void BSActionStuff::controlCorner(b2Body *body)
{
	//pun vel.x =0, pentru ca altfel ar sari la inceput putin, dupa care sare mult
	if(worldGravity.y != 0)
	{
	    velocity.x = 0;
        body->SetLinearVelocity(velocity);
        velocity.y = 0;
	}
	else
    {
        velocity.y = 0;
	    body->SetLinearVelocity(velocity);
        velocity.x = 0;
    }
	if (ButtonJump && canJumpVertically)
	{
		_BS_sound_device->play_sound(BS_Available_Sounds::jump());
		canJumpVertically = false;
		ButtonJump = false;

		if(worldGravity.y != 0)
		{
		    velocity.y = speedJumpUpOnWall * -sgn(worldGravity.y);
        std::cout<<"vel1: "<<velocity.y<<' '<<'\n';
		}
		else
        {
            velocity.x = speedJumpUpOnWall * -sgn(worldGravity.x);
        std::cout<<"vel2: "<<velocity.y<<' '<<'\n';
        }
		body->SetLinearVelocity(velocity);
		isInCorner = false;
		return;
	}

	itWalks(hero->theBody, 0.1f);
}

///Jesus told me how to write the next method
inline bool BSActionStuff::itWalks(b2Body *body, float T)
{
	float sign = 0;

	if (ButtonRight && ButtonLeft == false)
	{
		sign = 1;
	}
	else
		if (ButtonRight == false && ButtonLeft)
		{
			sign = -1;
		}
		else
			{
				return false;
			}
	float currentSign = 0;

    float bodyVelocity = 0;
    if(worldGravity.y != 0)
    {
        bodyVelocity = body->GetLinearVelocity().x;
    }
    else
    {
        bodyVelocity = body->GetLinearVelocity().y;
    }

	if(bodyVelocity < 0)
	{
		currentSign = -1;
	}
	else
		if(bodyVelocity > 0)
		{
			currentSign = 1;
		}

	float v = speedWalk;

	if (currentSign != 0 && currentSign != sign)
	{
		//iau pozitia ca sa stiu cum misc camera
		startPositionCameraX = hero->theBody->GetPosition().x;

		v *= turnMultiplier;
	}

    float speedVelocity = 0;
    if(worldGravity.y != 0)
    {
        speedVelocity = velocity.x;
    }
    else
    {
        speedVelocity = velocity.y;
    }
	//verific daca viteza curenta este mai mica decat viteza maxima, altfel nu mai maresc viteza
	if(sign < 0)
	{
		if(speedVelocity > 0)
		{
			speedVelocity *= -1 * T;
		}

		if(speedVelocity > -speedWalk)
		{
			speedVelocity += v * sign * T;
		}
		else
		{
			speedVelocity = -speedWalk;
		}
	}
	else
		if(sign > 0)
		{
			if(speedVelocity < 0)
			{
				speedVelocity *= -1 * 0.1f;
			}

			if(speedVelocity < speedWalk)
			{
				speedVelocity += v * sign * T;
			}
			else
			{
				speedVelocity = speedWalk;
			}
		}

    if(worldGravity.y != 0)
    {
        float velChangeX = speedVelocity - hero->theBody->GetLinearVelocity().x;
        float impulseX = body->GetMass() * velChangeX;
        body->ApplyLinearImpulse( b2Vec2(impulseX, 0), body->GetWorldCenter() );
        velocity.x = speedVelocity;
    }
    else
    {
        float velChangeX = speedVelocity - hero->theBody->GetLinearVelocity().y;
        float impulseX = body->GetMass() * velChangeX;
        body->ApplyLinearImpulse( b2Vec2(0, impulseX), body->GetWorldCenter() );
        velocity.y = speedVelocity;
    }

	return true;
}

bool BSActionStuff::itWalksInAir(b2Body *body, float T)
{
        float speedVelocity = 0;
        int worldSign = 0;
		if(worldGravity.y != 0)
        {
            speedVelocity = velocity.x;/// * -sgn(worldGravity.y);
            worldSign = sgn(worldGravity.y);
        }
        else
        {
            speedVelocity = velocity.y;
            worldSign = -sgn(worldGravity.x);
        }
        float linearVelocity = 0;
		if(worldGravity.y != 0)
        {
            linearVelocity = body->GetLinearVelocity().x * -sgn(worldGravity.y);
        }
        else
        {
            linearVelocity = body->GetLinearVelocity().y * sgn(worldGravity.x);
        }

	if(imprimeForceOnWall == false)
	{
		float sign = 0;

		if (ButtonRight && ButtonLeft == false)
		{
			sign = 1;
		}
		else
			if (ButtonRight == false && ButtonLeft)
			{
				sign = -1;
			}
			else
				return false;

		float currentSign = 0;

		if(linearVelocity < 0)
		{
			currentSign = -1;
		}
		else
			if(linearVelocity > 0)
			{
				currentSign = 1;
			}

		float v = speedWalkInAir;

		if (currentSign != 0 && currentSign != sign)
		{
			//iau pozitia ca sa stiu cum misc camera
			startPositionCameraX = hero->theBody->GetPosition().x;
			v *= turnMultiplierInAir;
		}

		//verific daca viteza curenta este mai mica decat viteza maxima, altfel nu mai maresc viteza
		if(worldSign < 0)
		{
		    if(sign < 0)
            {
                if(speedVelocity > -speedWalkInAir)
                {
                    speedVelocity += v * sign * T;
                }
                else
                {
                    speedVelocity = -speedWalkInAir;
                }
            }
            else
            {
                if(speedVelocity < speedWalkInAir)
                {
                    speedVelocity += v * sign * T;
                }
                else
                {
                    speedVelocity = speedWalkInAir;
                }
            }
		}
        else
        {
            if(sign < 0)
            {
                if(speedVelocity < -speedWalkInAir)
                {
                    speedVelocity += v * T * (-1);
                }
                else
                {
                    speedVelocity = -speedWalkInAir;
                }
            }
            else
            {
                if(speedVelocity > speedWalkInAir)
                {
                    speedVelocity -= v * T * (-1);
                }
                else
                {
                    speedVelocity = speedWalkInAir;
                }
            }
        }


        if(worldGravity.y != 0)
		{
		    float velChangeX = speedVelocity - hero->theBody->GetLinearVelocity().x;
            float impulseX = body->GetMass() * velChangeX;
            body->ApplyLinearImpulse( b2Vec2(impulseX, 0), body->GetWorldCenter() );
            velocity.x = speedVelocity;
		}
		else
        {
            float velChangeX = speedVelocity - hero->theBody->GetLinearVelocity().y;
            float impulseX = body->GetMass() * velChangeX;
            body->ApplyLinearImpulse( b2Vec2(0, impulseX), body->GetWorldCenter() );
            velocity.y = speedVelocity;
		}
		return true;
	}
	else
	{
		if( (jumpedFromLeftOrRight < 0 && ButtonRight) || (jumpedFromLeftOrRight > 0 && ButtonLeft))
		{
			imprimeForceOnWall = false;
		}

		if(SDL_GetTicks() - OriginalTimeToJumpUpTheWall > timeToStayInAirWhenJumpsOnTheSameWall * (theTimeHowFast / 60))
		{
			if(jumpedFromLeftOrRight < 0)
			{
				speedVelocity = -speedJumpToTheSameSide;
			}
			else
			{
				speedVelocity = speedJumpToTheSameSide;
			}

            if(worldGravity.y != 0)
			{
			    float velChangeX = speedVelocity - hero->theBody->GetLinearVelocity().x;
                float impulseX = body->GetMass() * velChangeX * 0.07 * 1.2f;
                body->ApplyLinearImpulse( b2Vec2(impulseX, 0), body->GetWorldCenter() );
                velocity.x = speedVelocity;
            }
            else
			{
			    float velChangeX = speedVelocity - hero->theBody->GetLinearVelocity().y;
                float impulseX = body->GetMass() * velChangeX * 0.07 * 1.2f;
                body->ApplyLinearImpulse( b2Vec2(0, impulseX), body->GetWorldCenter() );
                velocity.y = speedVelocity;
            }
		}
		else
		{
			if(jumpedFromLeftOrRight < 0)
			{
				speedVelocity = -speedJumpToTheSameSide;
			}
			else
			{
				speedVelocity = speedJumpToTheSameSide;
			}

			if(worldGravity.y != 0)
			{
			    float velChangeX = speedVelocity - hero->theBody->GetLinearVelocity().x;
                float impulseX = body->GetMass() * velChangeX * 0.07 * 0.7f;
                body->ApplyLinearImpulse( b2Vec2(impulseX, 0), body->GetWorldCenter() );
                velocity.x = speedVelocity;
            }
            else
			{
			    float velChangeX = speedVelocity - hero->theBody->GetLinearVelocity().y;
                float impulseX = body->GetMass() * velChangeX * 0.07 * 0.7f;
                body->ApplyLinearImpulse( b2Vec2(0, impulseX), body->GetWorldCenter() );
                velocity.y = speedVelocity;
            }
		}
	}
	return true;
}





void BSActionStuff::moveThePlayer()
{
    bool canPlayIt = false, finish = false;
	///Kill the hero if goes over the boundaries
	if(!hero->heroIsDead && (hero->theBody->GetPosition().x < gameBoundLeft - 1 ||
       hero->theBody->GetPosition().x > gameBoundRight + 1 ||
       hero->theBody->GetPosition().y < gameBoundDown - 1 ||
       hero->theBody->GetPosition().y > gameBoundUp + 1))
	{
	    _BS_sound_device->play_sound(BS_Available_Sounds::player_dieing());

        hero->heroIsBerserk1 = false;
        hero->heroIsBerserk2 = false;
        hero->heroIsBerserk3 = false;

		hero->heroIsDead = true;
		std::cout<<"dead out of boundary"<<'\n';
		hero->texture = playerTextureDead;
		hero->heroWasDead = true;
	}

	desiredVelX = 0;

	///Also Set the animation index for when it stops
	if(hero->theBody->GetPosition().x != lastPositionOfHeroX)
	{
		hero->heroStopped = false;
		heroJustStopped = false;
		itMovedX = true;

		///See which direction is the hero facing
		if(hero->theBody->GetLinearVelocity().x > 0)
		{
			heroFacesLeftOrRight = 1;
		}
		else
			if(hero->theBody->GetLinearVelocity().x < 0)
			{
				heroFacesLeftOrRight = -1;
			}
	}
	else
	{
		if(itMovedX)
		{
			hero->heroStopped = true;
			heroJustStopped = true;
			timer = SDL_GetTicks();
		}

		itMovedX = false;
	}

	if(hero->theBody->GetLinearVelocity().y != 0)
	{
		hero->heroStopped = false;
		heroJustStopped = false;
		itMovedY = true;
	}
	else
	{
		if(itMovedY)
		{
			hero->heroStopped = true;
			heroJustStopped = true;
			timer = SDL_GetTicks();
		}

		itMovedY = false;
	}

	///Set the animation index
	if(!hero->heroIsDead && !isTeleporting)
	{
        if(itMovedX && !itMovedY)
        {
            if(hero->theBody->GetLinearVelocity().x > 0)
            {
                ///Verific daca e mai mic sau mai mare decat animatia, pentru ca daca ar fi mai mare decat animatia de miscare la dreapta
                ///dar mai mica decat miscarea de animatie la dreapta + 1, inseamna ca face ceva intermediar, cum ar fi: incepe
                ///miscare, este in miscare continua, sau termina miscarea

                if(hero->animationIndexCurrentAnimation != hero->animationIndexPlayerMoveRight)
                {
                    canPlayIt = true;
                    hero->animationIndexCurrentAnimation = hero->animationIndexPlayerMoveRight;
                }

                if(itMovedXLeft)
                {
                    itTurnedX = true;
                }

                itMovedXRight = true;
                itMovedXLeft = false;
            }
            else
            {
                if(hero->animationIndexCurrentAnimation != hero->animationIndexPlayerMoveLeft)
                {
                    canPlayIt = true;
                    hero->animationIndexCurrentAnimation = hero->animationIndexPlayerMoveLeft;
                }

                if(itMovedXRight)
                {
                    itTurnedX = true;
                }

                itMovedXRight = false;
                itMovedXLeft = true;
            }
        }

        if(itMovedY && isInAir)
        {
            if(hero->theBody->GetLinearVelocity().y > 0)
            {
                ///Daca se misca spre dreapta
                if(heroFacesLeftOrRight == 1)
                {
                    if(hero->animationIndexCurrentAnimation != hero->animationIndexPlayerMoveUpRight)
                    {
                        canPlayIt = true;
                        hero->animationIndexCurrentAnimation = hero->animationIndexPlayerMoveUpRight;
                        listOfActiveAnimations[0]->setPlayableStatus(canPlayIt);
                    }
                }
                else
				///Daca se misca spre stanga
                {
                    if(hero->animationIndexCurrentAnimation != hero->animationIndexPlayerMoveUpLeft)
                    {
                        canPlayIt = true;
                        hero->animationIndexCurrentAnimation = hero->animationIndexPlayerMoveUpLeft;
                        listOfActiveAnimations[0]->setPlayableStatus(canPlayIt);
                    }
                }
            }
            else
            {
                ///Daca se misca spre dreapta
                if(heroFacesLeftOrRight == 1)
                {
                    if(hero->animationIndexCurrentAnimation != hero->animationIndexPlayerMoveDownRight)
                    {
                        canPlayIt = true;
                        hero->animationIndexCurrentAnimation = hero->animationIndexPlayerMoveDownRight;
                        listOfActiveAnimations[0]->setPlayableStatus(canPlayIt);
                    }
                }
                else
				///Daca se misca spre stanga
                {
                    if(hero->animationIndexCurrentAnimation != hero->animationIndexPlayerMoveDownLeft)
                    {
                        canPlayIt = true;
                        hero->animationIndexCurrentAnimation = hero->animationIndexPlayerMoveDownLeft;
                        listOfActiveAnimations[0]->setPlayableStatus(canPlayIt);
                    }
                }
            }
        }

        if(!itMovedX && !itMovedY)
        {
            itMovedXRight = false;
            itMovedXLeft = false;
            if(hero->heroStopped && heroJustStopped)
            {
                listOfActiveAnimations[0]->finish = true;
                finish = true;
                heroJustStopped = false;
            }

            if(hero->animationIndexCurrentAnimation != hero->animationIndexPlayerStand && !listOfActiveAnimations[0]->finish)///!hero->heroStopped)
            {
                canPlayIt = true;
                hero->animationIndexCurrentAnimation = hero->animationIndexPlayerStand;
            }
        }
	}
	else
        if(hero->heroIsDead && !isTeleporting &&
           hero->animationIndexCurrentAnimation != hero->animationIndexPlayerDieRight &&
           hero->animationIndexCurrentAnimation != hero->animationIndexPlayerDieLeft)
        {
            if(lastPositionOfHeroX < hero->theBody->GetPosition().x)
			{
			    hero->animationIndexCurrentAnimation = hero->animationIndexPlayerDieRight;
			}
			else
            {
			    hero->animationIndexCurrentAnimation = hero->animationIndexPlayerDieLeft;
            }
            listOfActiveAnimations[0]->canPlayIt = false;
            canPlayIt = true;
        }
        else
            ///daca se teleporteaza
            if(isTeleporting)
            {
                ///daca a iesit din worm hole
                if(SDL_GetTicks() - timeWhenTeleported >  12 * 20 && SDL_GetTicks() - timeWhenTeleported <  24 * 20)
                {
                    if(hero->animationIndexCurrentAnimation != hero->animationIndexPlayerTeleportedOut)
                    {
                        listOfActiveAnimations[0]->canPlayIt = false;
                        canPlayIt = true;
                        hero->animationIndexCurrentAnimation = hero->animationIndexPlayerTeleportedOut;
                        TeleportHero(TeleportX, TeleportY);
                    }
                }
                else
                    ///daca a intrat in worm hole
                    if(SDL_GetTicks() - timeWhenTeleported <=  12 * 20)
                    {
                        if(hero->animationIndexCurrentAnimation != hero->animationIndexPlayerTeleportedIn)
                        {
                            listOfActiveAnimations[0]->canPlayIt = false;
                            canPlayIt = true;
                            hero->animationIndexCurrentAnimation = hero->animationIndexPlayerTeleportedIn;
                        }
                    }
                    else
                        ///daca a iesit cu totul din worm hole
                        if(SDL_GetTicks() - timeWhenTeleported >= 24 * 20)
                        {
                            isTeleporting = false;
                        }
            }

	if(abs(startPositionCameraX - hero->theBody->GetPosition().x) > 2)
	{
		startPositionCameraX = hero->theBody->GetPosition().x - 2;
		changeInPosition = 2;
	}


	lastPositionOfHeroX = hero->theBody->GetPosition().x;
	lastPositionOfHeroY = hero->theBody->GetPosition().y;



    listOfActiveAnimations[0]->finish = finish;
    listOfActiveAnimations[0]->indexOfAnimation = hero->animationIndexCurrentAnimation;
    if(!listOfActiveAnimations[0]->canPlayIt && canPlayIt)
    {
        listOfActiveAnimations[0]->setPlayableStatus(canPlayIt);
    }
    listOfActiveAnimations[3]->indexOfAnimation = hero->animationIndexPlayerWormHole;

    listOfActiveAnimations[0]->xCord = hero->theBody->GetPosition().x;
    listOfActiveAnimations[0]->yCord = hero->theBody->GetPosition().y;
    listOfActiveAnimations[0]->zCord = hero->zCord;

    listOfActiveAnimations[1]->xCord = hero->theBody->GetPosition().x;
    listOfActiveAnimations[1]->yCord = hero->theBody->GetPosition().y + 0.45f;
    listOfActiveAnimations[1]->zCord = hero->zCord;

    listOfActiveAnimations[2]->xCord = hero->theBody->GetPosition().x;
    listOfActiveAnimations[2]->yCord = hero->theBody->GetPosition().y + 0.45f;
    listOfActiveAnimations[2]->zCord = hero->zCord;

    listOfActiveAnimations[4]->xCord = hero->theBody->GetPosition().x;
    listOfActiveAnimations[4]->yCord = hero->theBody->GetPosition().y + 0.45f;
    listOfActiveAnimations[4]->zCord = hero->zCord;


	if(isInAir)
	{
		controlAir(0.1f, hero->theBody);
	}
	else
		if(isInCorner)
		{
			controlCorner(hero->theBody);
		}
		else
			if(isOnGround)
			{
				controlGround(hero->theBody, 0.05f);
			}
			else
				if((isOnLeftWall || isOnRightWall) && !hero->heroIsDead)
				{
					controlWall(hero->theBody);
					if(isOnLeftWall)
					{
					    hero->animationIndexCurrentAnimation = hero->animationIndexPlayerWallLeft;
					}
					else
                        if(isOnRightWall)
                        {
                            hero->animationIndexCurrentAnimation = hero->animationIndexPlayerWallRight;
                        }
                    canPlayIt = true;
                    listOfActiveAnimations[0]->setPlayableStatus(canPlayIt);
				}
}





void BSActionStuff::destroyTheParticles()
{

}

void BSActionStuff::generateTheParticles()
{
	for(int i = 0 ; i < 10; i++)
	{
        //		BSCloudyParticles *part = new BSCloudyParticles;
        //		part->xCord = hero->theBody->GetPosition().x - i / 10.0f;
        //		part->yCord = hero->theBody->GetPosition().y - 1 / 2.0f;
        //		part->radius = 0.4f;
        //		part->colorOfParticle = {1, 1, 1, 1};
        //		part->sinValue = 0;
        //		part->yPower = 1;
        //		part->xPower = 1;
	}
}

void BSActionStuff::generateWhenHitsGroundParticles()
{

}

void BSActionStuff::ActionFuncTeleportHero(float xCord, float yCord)
{
    TeleportX = xCord;
    TeleportY = yCord;
    isTeleporting = true;
    timeWhenTeleported = SDL_GetTicks();
}

void BSActionStuff::TeleportHero(float coordX, float coordY)
{
	std::cout<<"teleported"<<'\n';
	hero->theBody->SetTransform(b2Vec2(coordX, coordY), hero->theBody->GetAngle());
	heroCanTeleportAgain = false;
	hero->theBody->ApplyLinearImpulse(b2Vec2(0, 0.1f), hero->theBody->GetWorldCenter());
}

//deseneaza stelute in spatele lui
void BSActionStuff::PathColoring( int LeftOrRight)
{

}

void BSActionStuff::heroTouchesObjects()
{
	//get all the objects the hero is contacting with
	b2ContactEdge *edge = hero->theBody->GetContactList();

	///retin daca se afla in aer ca sa stiu daca creez particule pentru cand atinge pamantul
	bool wasInAir = isInAir;

	canJumpVertically = false;
	canJumpHorizontally = false;
	isOnGround = false;
	isInAir = true;
	isInCorner = false;
	isOnLeftWall = false;
	isOnRightWall = false;
	jumpsUpTheWall = false;
	variableWall = 0;

	while (edge != NULL)
	{
		b2WorldManifold *a = new b2WorldManifold();
		edge->contact->GetWorldManifold(a);
		b2Vec2 normal1 = a->normal;

		//vad cu care obiecte intra in contact
		if (edge->contact->IsTouching())
		{
		    ///Shallow copy
			//            USERDATA *us = (USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData();

			///Deep copy
			//            USERDATA *us = new USERDATA;
			//			us->BumTime = ((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->BumTime;
			//			us->canKillHero = ((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->canKillHero;
			//			us->height = ((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->height;
			//			us->isDust = ((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->isDust;
			//			us->isDustAble = ((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->isDustAble;
			//			us->isToNextLevel = ((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->isToNextLevel;
			//			us->name = ((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->name;
			//			us->timeUntilDestroy = ((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->timeUntilDestroy;
			//			us->width = ((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->width;

			///Iau userdataul din corpul pe care il atinge jucatorul, ca sa vad daca il poate omori pe jucator
			//			USERDATA *us2 = (USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData();
			//            USERDATA *us2 = new USERDATA;
			//			us2->BumTime = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->BumTime;
			//			us2->canKillHero = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->canKillHero;
			//			us2->height = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->height;
			//			us2->isDust = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->isDust;
			//			us2->isDustAble = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->isDustAble;
			//			us2->isToNextLevel = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->isToNextLevel;
			//			us2->name = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->name;
			//			us2->timeUntilDestroy = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->timeUntilDestroy;
			//			us2->width = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->width;

			if(((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->isDustAble)
			{
                ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->isDust = 1;
			}

			///Daca este 1, inseamna ca a atins un obiect care il omoara, asa ca este mort, altfel nu este mort
			if(((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->canKillHero && !hero->heroWasDead)
			{
				//				alSourcePlay(SourceSoundDies);

                if(hero->heroIsBerserk1 || hero->heroIsBerserk2 || hero->heroIsBerserk3)
				{
				    ///listOfActiveAnimations[1]->indexOfAnimation = hero->animationIndexCurrentAnimationBerserk;
                    listOfActiveAnimations[1]->canPlayIt = true;
                    listOfActiveAnimations[1]->finish = true;
				}

				if(hero->heroIsBerserk2)
				{
				    ///listOfActiveAnimations[2]->indexOfAnimation = hero->animationIndexPlayerAuraElectricity;
                    listOfActiveAnimations[2]->canPlayIt = false;
                }
				hero->heroIsBerserk1 = false;
				hero->heroIsBerserk2 = false;
				hero->heroIsBerserk3 = false;

				hero->heroIsDead = ((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->canKillHero;
				hero->texture = playerTextureDead;
				hero->heroWasDead = true;
				std::cout<<"dead, killed by object"<<'\n';

				deathsThisLevel++;
				deathsTotalNo++;
			}
			else
				if(((USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->isToNextLevel && !hero->heroWasToNextLevel &&
				   levelChoserPlayer->indexOfLevelThatItIsOn + 1 < chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->nrOfLevels &&
				   chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter[levelChoserPlayer->indexOfLevelThatItIsOn + 1]->canPlayIt)
				{
					levelChoserPlayer->justFinished = true;
					hero->heroWasToNextLevel = true;
					chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter[levelChoserPlayer->indexOfLevelThatItIsOn]->isFinished = true;
				}

			if(((USERDATA*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->name == "hero")
			{
				//if it is touching the ground
//				if (normal1.y != 0  || normal1.x != 0 )
//				{
//				    canJumpVertically = true;
//					isOnGround = true;
//					pressedJumpSecond = false;
//					isInAir = false;
//
//					///retin daca tocmai a atins pamantul
//					if(wasInAir)
//					{
//						justHitTheGround = true;
//					}
//				}

				if(normal1.y > 0 && worldGravity.y > 0)
                {
                    isOnGround = true;
//					std::cout<<"touches ground"<<'\n';
				}
				else
                    if(normal1.y < 0 && worldGravity.y < 0)
                    {
                        isOnGround = true;
//                        std::cout<<"touches ground"<<'\n';
                    }
                    else
                        if(normal1.x > 0 && worldGravity.x > 0)
                        {
                            isOnGround = true;
//                            std::cout<<"touches ground"<<'\n';
                        }
                        else
                            if(normal1.x < 0 && worldGravity.x < 0)
                            {
                                isOnGround = true;
//                                std::cout<<"touches ground"<<'\n';
                            }
                if(isOnGround)
                {
				    canJumpVertically = true;
					isOnGround = true;
					pressedJumpSecond = false;
					isInAir = false;

					///retin daca tocmai a atins pamantul
					if(wasInAir)
					{
						justHitTheGround = true;
					}
                }

                if(worldGravity.y < 0)
				{
				    if(normal1.x > 0)
                    {
						isOnLeftWall = false;
						isOnRightWall = true;
//                        std::cout<<"touches wall right 1"<<'\n';
                    }
                    else
                        if(normal1.x < 0)
                        {
                            isOnLeftWall = true;
                            isOnRightWall = false;
//                            std::cout<<"touches wall left 1"<<'\n';
                        }
				}
                else
                    if(worldGravity.y > 0)
                    {
                        if(normal1.x > 0)
                        {
                            isOnLeftWall = true;
                            isOnRightWall = false;
//                            std::cout<<"touches wall left 3"<<'\n';
                        }
                        else
                            if(normal1.x < 0)
                            {
                                isOnLeftWall = false;
                                isOnRightWall = true;
//                                std::cout<<"touches wall right 3"<<'\n';
                            }
                    }
                    else
                        if(worldGravity.x > 0)
                        {
                            if(normal1.y > 0)
                            {
                                isOnLeftWall = false;
                                isOnRightWall = true;
//                                std::cout<<"touches wall right 4"<<'\n';
                            }
                            else
                                if(normal1.y < 0)
                                {
                                    isOnLeftWall = true;
                                    isOnRightWall = false;
//                                    std::cout<<"touches wall left 4"<<'\n';
                                }
                        }
                        else
                            if(worldGravity.x < 0)
                            {
                                if(normal1.y > 0)
                                {
                                    isOnLeftWall = true;
                                    isOnRightWall = false;
//                                    std::cout<<"touches wall left 2"<<'\n';
                                }
                                else
                                    if(normal1.y < 0)
                                    {
                                        isOnLeftWall = false;
                                        isOnRightWall = true;
//                                        std::cout<<"touches wall right 2"<<'\n';
                                    }
                            }
                if(isOnLeftWall || isOnRightWall)
                {
                    pressedJumpSecond = false;

					canJumpHorizontally = true;
					//-1 for left, 1 for right

                    variableWall = 1;
					if(isOnLeftWall)
					{
					    variableWall = -1;
					}
					isInAir = false;

					///retin daca tocmai a atins pamantul
					if(wasInAir)
					{
						justHitTheGround = true;
					}
                }
//                    else
//                    {
//                        std::cout<<"touches nothing"<<'\n';
//                    }

//				//if it touches a wall
//				if(normal1.x != 0)
//				{
//				    std::cout<<"touches walls"<<'\n';
//					//if it touches the left wall
//					if(normal1.x < 0)
//					{
//						isOnLeftWall = true;
//						isOnRightWall = false;
//					}
//					else
//					//if it touches the right wall
//					{
//						isOnLeftWall = false;
//						isOnRightWall = true;
//					}
//					pressedJumpSecond = false;
//
//					canJumpHorizontally = true;
//					//-1 for left, 1 for right
//					variableWall = normal1.x;
//					isInAir = false;
//
//
//					///retin daca tocmai a atins pamantul
//					if(wasInAir)
//					{
//						justHitTheGround = true;
//					}
//				}

			}
		}


		delete a;
		edge = edge->next;
	}

	//verfic daca se afla in colt (si pe pamant si pe perete)
	if(isOnGround && (isOnLeftWall || isOnRightWall))
	{
		isInCorner = true;
		isOnLeftWall = false;
		isOnRightWall = false;
	}

	delete edge;
}

bool BSActionStuff::CanGoAwayFromWall(float timeToMatch)
{
	currentTime = SDL_GetTicks();
	if(currentTime - timeToMatch > timeToStayOnWall * (theTimeHowFast / 60) )
	{
		return true;
	}
	return false;
}



//float BSActionStuff::easeIn (float t,float b , float c, float d)
//{
//	return c*(t/=d)*t*t*t*t + b;
//}
//float BSActionStuff::easeOut(float t,float b , float c, float d)
//{
//	return c*((t=t/d-1)*t*t*t*t + 1) + b;
//}
//
//float BSActionStuff::easeInOut(float t,float b , float c, float d)
//{
//	if ((t/=d/2) < 1)
//		return c/2*t*t*t*t*t + b;
//
//	return c/2*((t-=2)*t*t*t*t + 2) + b;
//}
