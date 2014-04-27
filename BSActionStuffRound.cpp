
#include "BSActionStuffRound.h"

int timpulCareTrece = 0, timpulCareAtrecut = 0, jumpedFromLeftOrRight = 0;
long long OriginalTimeOnWall = 0, currentTime = 0, timeToStayOnWall = 250, OriginalTimeToJumpUpTheWall = 0, timeToCreateParticles = 0;
bool imprimeForceOnWall = false, heroCanTeleportAgain = true, justHitTheGround = false;


	b2Vec2 vel, velocity;
    float desiredVelX = 0, desiredVelY = 0;
    float velChangeX, velChangeY;
    float impulseX, impulseY, turnMultiplier = 1.f, turnMultiplierInAir = 3.5f, timeE = 0;

int nnrr = 0;

int currentTexture = 0;

				bool aCreat = false;
				int indexCurent = 0;
				int currentParticleNr = 0;
				int pozitivSauNegativ = 0;



void BSActionStuffRound::levelFinished()
{
    //iau timpul curent
    timpulCareTrece = SDL_GetTicks();

    //daca este diferit de -1 inseamna ca a trecut un anumit interval de timp
    if(timpulCareTrece != -1)
    {
        //il fac sa se micsoreze atata timp cat cercul cel mic are o anumita raza
        if(razaInitialaInner > 3)
        {
            razaInitialaInner --;
        }
        else
        {
            //aici numarul timpul in care cercul nu se micsoreaza
            if(timpulCareAtrecut != 30)
            {
                timpulCareAtrecut++;
            }
        }

        //daca dupa un anumit timp in care cercul nu s-a micsorat, incep sa il micsorez din nou
        //iar raza trebuie sa fie > 0 pentru ca altfel o sa se albeasca cercul
        //daca se albeste imaginea dupa ce se termina cercul, inseamna ca raza inner, este mai mare decat raza
        //outer, asa ca sa schimb valoarea pe care o scad la razaInitialaInner
        if(timpulCareAtrecut == 30 && razaInitialaInner > 0.1f)
        {
            razaInitialaInner -=0.1f;

            if(razaInitialaInner == 0)
            {
                timpulCareAtrecut = 0;
            }
        }
        else
            //resetez timpul in care a stat oprit
            if(razaInitialaInner == 0)
            {
                timpulCareAtrecut = 0;
            }
    }

}


void BSActionStuffRound::controlAir(float T)
{
	// Abort jump if user lets go of button
    if (velocity.y > 0 && !ButtonJump)
        velocity.y = 0;

	//velocity.y = Hero2nd->theBody->GetLinearVelocity().y;

    // Air walk
    if(!itWalksInAir(Hero2nd->theBody, T * 0.2f))
    {
    	//Hero2nd->theBody->SetLinearDamping(1);
    	/*
    	if(velocity.x > 0)
		{
			velocity.x -= 0.5f;
		}
		else
			if(velocity.x < 0)
			{
				velocity.x += 0.5f;
			}

		float velChangeX = velocity.x - Hero2nd->theBody->GetLinearVelocity().x;
		float impulseX = Hero2nd->theBody->GetMass() * velChangeX;
		//body->SetLinearVelocity(velocity);
		Hero2nd->theBody->ApplyLinearImpulse( b2Vec2(impulseX,0), Hero2nd->theBody->GetWorldCenter() );*/
    }
	/*
	desiredVelY = vel.y * 4.0f;
	vel.x = 0;
	desiredVelX = vel.x * 4.0f;*/
	//std::cout<<"                         "<<Hero2nd->theBody->GetLinearVelocity().x<<" "<<Hero2nd->theBody->GetLinearVelocity().y<<'\n';
}

void BSActionStuffRound::controlWall(b2Body *body)
{
	//std::cout<<"touches the wall  "<<SDL_GetTicks()<<" "<<OriginalTimeOnWall<<'\n';
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

	//verifica daca vrea sa  sara de pe perete pe alt perete
	if(canJumpHorizontally && ((ButtonRight && isOnLeftWall) || (ButtonLeft && isOnRightWall)) && ButtonJump)
	{
		//*-1 pentru ca atunci cand e pe peretele din stanga (de exemplu) va trebui sa ii imprim
		//o forta pozitiva, dar variableWall are val -1 pe peretele din stanga
		velocity.x = speedJumpToSide * variableWall * (-1);
		float velChangeX = velocity.x - Hero2nd->theBody->GetLinearVelocity().x;
		float impulseX = body->GetMass() * velChangeX;


		velocity.y = speedJumpUp;// * variableWall * (-1);
		float velChangeY = velocity.y - Hero2nd->theBody->GetLinearVelocity().y;
		float impulseY = body->GetMass() * velChangeY;


		//body->SetLinearVelocity(velocity);
		body->ApplyLinearImpulse( b2Vec2(impulseX,impulseY), body->GetWorldCenter() );
	}
	else
		//verific daca vrea sa sara pe acelasi perete in sus
		if(canJumpHorizontally && ((ButtonLeft && isOnLeftWall) || (ButtonRight && isOnRightWall)) && ButtonJump)
		{
			velocity.x = speedJumpToTheSameSide * variableWall * (-1);
			float velChangeX = velocity.x - Hero2nd->theBody->GetLinearVelocity().x;
			float impulseX = body->GetMass() * velChangeX;


			velocity.y = speedJumpUp * 1.2f;// * variableWall * (-1);
			float velChangeY = velocity.y - Hero2nd->theBody->GetLinearVelocity().y;
			float impulseY = body->GetMass() * velChangeY;


			//body->SetLinearVelocity(velocity);
			body->ApplyLinearImpulse( b2Vec2(impulseX,impulseY), body->GetWorldCenter() );
			imprimeForceOnWall = true;
			//std::cout<<"jumped"<<'\n';
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
			if(canJumpHorizontally && ButtonJump)
			{
				velocity.x = (speedJumpToSide / 2.f) * variableWall * (-1);
				float velChangeX = velocity.x - Hero2nd->theBody->GetLinearVelocity().x;
				float impulseX = body->GetMass() * velChangeX;


				velocity.y = speedJumpUp;// * variableWall * (-1);
				float velChangeY = velocity.y - Hero2nd->theBody->GetLinearVelocity().y;
				float impulseY = body->GetMass() * velChangeY;


				//body->SetLinearVelocity(velocity);
				body->ApplyLinearImpulse( b2Vec2(impulseX,impulseY), body->GetWorldCenter() );
			}
	//CanGoAwayFromWall(12);

	//verifica daca vrea sa se indeparteze de perete fara sa sara
	if(((ButtonRight && isOnLeftWall) || (ButtonLeft && isOnRightWall)) && OriginalTimeOnWall == 0)
	{
		//float verifyTime = timp->getTime();
		//if(verifyTime != -1)
		{
			OriginalTimeOnWall = SDL_GetTicks();
		}
	}
	else
		//verifica timpul scurs pe perete cand apasa sa sara de pe el
		if(((ButtonRight && isOnLeftWall) || (ButtonLeft && isOnRightWall)) && CanGoAwayFromWall(OriginalTimeOnWall))
		{
			std::cout<<"goes off\n";
			itWalksInAir(body, 1.1f);
			OriginalTimeOnWall = 0;
		}
		else
		//in cazul in care renunta sa sara, va reseta timpul in care sa sara
			if(	((ButtonRight == false && isOnLeftWall) || (ButtonLeft == false && isOnRightWall))
				&& OriginalTimeOnWall != 0)
			{
				OriginalTimeOnWall = 0;
			}
		//std::cout<<"        is on wall\n";
}

void BSActionStuffRound::controlGround(b2Body *body, float T)
{
	imprimeForceOnWall = false;
	//if(body->GetLinearVelocity().x || body->GetLinearVelocity().y)
	//std::cout<<body->GetLinearVelocity().x<<" "<<body->GetLinearVelocity().y<<'\n';
	if (ButtonJump && canJumpVertically)
	{
		canJumpVertically = false;
		velocity.y = speedJumpUp;
		body->SetLinearVelocity(velocity);
		return;
	}

	// Run on ground
	if (!itWalks(body, T))
	{
		velocity.x = 0;
		body->SetLinearVelocity(velocity);
	}




















	/*
//		//std::cout<<jumpsUpTheWall<<" " << ButtonLeft << " " << ButtonRight << " " << ButtonJump<<'\n';
	b2Vec2 velc;// = new b2Vec2();
	if (ButtonJump == true && canJumpVertically == true)
	{
		//timeStart = getTime();
		canJumpVertically = false;
		//timeStop = 0;
		velc.y = speedJumpUp;
		Hero2nd->theBody->SetLinearVelocity(velc);
		return;
	}
	else
		if(ButtonJump == false && canJumpVertically == true)
		{
			velc.y = 0;
			Hero2nd->theBody->SetLinearVelocity(velc);
		}


	float sign = 0;
	if (ButtonRight == true && ButtonLeft == false)
	{
		sign = 1;
	}
	else
		if (ButtonRight == false && ButtonLeft == true)
		{
			sign = -1;
		}

		//std::cout<<velc.x<<" " << velc.y << " " << sign << '\n';

	if(sign != 0)
	{
		////std::cout<<velc.x<<'\n';
		float currSign = 0;
		if(Hero2nd->theBody->GetLinearVelocity().x > 0)
		{
			currSign = 1;
		}
		else
			if(Hero2nd->theBody->GetLinearVelocity().x < 0)
			{
				currSign = -1;
			}

		//this is to check if AccelerationButton is pressed
		//float v = walkAccel();
		float v = speedWalk;
		float turnMul = 1;
		float T = 0.1f;

		if ((currSign != 0) && (currSign != sign))
		{
			v *= turnMul;
		}
		velc.x += v * sign * T;
		Hero2nd->theBody->SetLinearVelocity(velc);
	}
	else
	{
		velc.x = 0;
		Hero2nd->theBody->SetLinearVelocity(velc);
	}*/
}

void BSActionStuffRound::controlCorner(b2Body *body)
{
	//std::cout<<"corner\n";

	//pun vel.x =0, pentru ca altfel ar sari la inceput putin, dupa care sare mult
	velocity.x = 0;
	body->SetLinearVelocity(velocity);

	velocity.y = 0;
	if (ButtonJump && canJumpVertically)
	{
		canJumpVertically = false;
		velocity.y = speedJumpUpOnWall;
		body->SetLinearVelocity(velocity);
		return;
	}
	itWalks(Hero2nd->theBody, 0.1f);
}

inline bool BSActionStuffRound::itWalks(b2Body *body, float T)
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
	if(body->GetLinearVelocity().x < 0)
	{
		currentSign = -1;
	}
	else
		if(body->GetLinearVelocity().x > 0)
		{
			currentSign = 1;
		}

	float v = speedWalk;
	if (currentSign != 0 && currentSign != sign)
	{
		//iau pozitia ca sa stiu cum misc camera
		startPositionCameraX = Hero2nd->theBody->GetPosition().x;

		v *= turnMultiplier;
		//body->SetLinearVelocity(b2Vec2(0, 0));
		//std::cout<<v<<'\n';
	}

	//verific daca viteza curenta este mai mica decat viteza maxima, altfel nu mai maresc viteza
	if(sign < 0)
	{
		if(velocity.x > 0)
		{
			velocity *= -1 * T;
		}
		if(velocity.x > -speedWalk)
		{
			velocity.x += v * sign * T;
		}
		else
		{
			velocity.x = -speedWalkInAir;
		}
	}
	else
		if(sign > 0)
		{
			if(velocity.x < 0)
			{
				velocity *= -1 * 0.1f;
			}
			if(velocity.x < speedWalk)
			{
				velocity.x += v * sign * T;
			}
			else
			{
				velocity.x = speedWalkInAir;
			}
		}
    float velChangeX = velocity.x - Hero2nd->theBody->GetLinearVelocity().x;
    float impulseX = body->GetMass() * velChangeX;
	//body->SetLinearVelocity(velocity);
    body->ApplyLinearImpulse( b2Vec2(impulseX,0), body->GetWorldCenter() );

	//std::cout<<" "<<Hero2nd->theBody->GetLinearVelocity().x<<'\n';

	return true;
}

bool BSActionStuffRound::itWalksInAir(b2Body *body, float T)
{
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
		if(body->GetLinearVelocity().x < 0)
		{
			currentSign = -1;
		}
		else
			if(body->GetLinearVelocity().x > 0)
			{
				currentSign = 1;
			}

		float v = speedWalkInAir;
		if (currentSign != 0 && currentSign != sign)
		{
			//iau pozitia ca sa stiu cum misc camera
			startPositionCameraX = Hero2nd->theBody->GetPosition().x;
			v *= turnMultiplierInAir;
		}

		//verific daca viteza curenta este mai mica decat viteza maxima, altfel nu mai maresc viteza
		if(sign < 0)
		{
			if(velocity.x > -speedWalkInAir)
			{
				velocity.x += v * sign * T;
			}
			else
			{
				velocity.x = -speedWalkInAir;
			}
		}
		else
		{
			if(velocity.x < speedWalkInAir)
			{
				velocity.x += v * sign * T;
			}
			else
			{
				velocity.x = speedWalkInAir;
			}
		}

		float velChangeX = velocity.x - Hero2nd->theBody->GetLinearVelocity().x;
		float impulseX = body->GetMass() * velChangeX;
		//body->SetLinearVelocity(velocity);
		body->ApplyLinearImpulse( b2Vec2(impulseX,0), body->GetWorldCenter() );

		//std::cout<<Hero2nd->theBody->GetLinearVelocity().x<<"     "<<Hero2nd->theBody->GetLinearVelocity().y<<"   "<<sign<<'\n';

		return true;
	}
	else
	{
		if( (jumpedFromLeftOrRight < 0 && ButtonRight) || (jumpedFromLeftOrRight > 0 && ButtonLeft))
		{
			imprimeForceOnWall = false;
			std::cout<<"a apasat\n";
		}
			//std::cout<<"jumps us"<<'\n';
		//if(body->GetLinearVelocity().x > 0)

		if(SDL_GetTicks() - OriginalTimeToJumpUpTheWall > 250)
		{
			if(jumpedFromLeftOrRight < 0)
			{
				velocity.x = -speedJumpToTheSameSide;
			}
			else
			{
				velocity.x = speedJumpToTheSameSide;
				//body->ApplyForce(b2Vec2(100 , 0), body->GetWorldCenter());
			}
			float velChangeX = velocity.x - Hero2nd->theBody->GetLinearVelocity().x;
			float impulseX = body->GetMass() * velChangeX;
			//body->SetLinearVelocity(velocity);


			body->ApplyLinearImpulse( b2Vec2(impulseX,0), body->GetWorldCenter() );
			//body->ApplyForce(b2Vec2(-500 , 0), body->GetWorldCenter());
		}
	}
}

void BSActionStuffRound::moveThePlayer()
{
	ActionFuncTeleportHero();
	//std::cout<<easeOut(1, 1, 1, 2)<<'\n';
	//float timpul = timp->getTime();
	//if(timpul != -1)
	{
		//std::cout<<SDL_GetTicks()<<'\n';
	}
	//vel = Hero2nd->theBody->GetLinearVelocity();
    desiredVelX = 0;

    if(Hero2nd->theBody->GetPosition().x != lastPositionOfHeroX)
	{
			itMovedX = true;
	}
	else
	{
		itMovedX = false;
	}

    if(Hero2nd->theBody->GetPosition().y != lastPositionOfHeroY)
	{
		itMovedY = true;
	}
	else
	{
		itMovedY = false;
	}
	lastPositionOfHeroX = Hero2nd->theBody->GetPosition().x;
	lastPositionOfHeroY = Hero2nd->theBody->GetPosition().y;



	//********************************************************************************************************
	//********************************************************************************************************
	//********************************************************************************************************
	//std::cout<<sinX<<" "<<sin2<<'\n';
	//daca este in jumatatea dreapta a ecranului
	if(Hero2nd->theBody->GetPosition().x > SCREEN_WIDTH / scaleSize / 2)
	{
		//daca s-a miscat atunci cresc sinusurile
		if(itMovedX)
		{
			//iterez sinusul pentru cand se misca spre dreapta
			if(Hero2nd->theBody->GetLinearVelocity().x > 0)
			{
				if(sinX < 90)
				{
					//iterez sinX pana la maxim
					sinX +=iteratieSin;
				}
				else
					if(sinX > 90)
					{
						sinX = 90;
					}
			}
			//if the body is moving to the left
			else
			{
				if(sinX > -90)
				{
					sinX -=iteratieSin;
				}
				else
					if(sinX < -90)
					{
						sinX = -90;
					}
			}
		}
	}


	if(itMovedX == false || Hero2nd->theBody->GetPosition().x <= SCREEN_WIDTH / scaleSize / 2)
	{
		//if the body is moving to the right
		if(sinX > 0)//Hero2nd->theBody->GetLinearVelocity().x > 0)
		{
			sinX -= iteratieSin;
			//scad primul sin

			if(sinX <0)
			{
				sinX = 0;
			}
		}
		else
			//if the body is moving to the left
			if(sinX < 0)//Hero2nd->theBody->GetLinearVelocity().x > 0)
			{
					sinX += iteratieSin;
					//scad primul sin

					if(sinX > 0)
					{
						sinX = 0;
					}
			}
	}


	if(Hero2nd->theBody->GetPosition().y > SCREEN_HEIGHT / scaleSize / 2)
	{
		//daca s-a miscat atunci cresc sinusurile
		if(itMovedY)
		{
			//iterez sinusul pentru cand se misca spre dreapta
			if(Hero2nd->theBody->GetLinearVelocity().y > 0)
			{
				if(sinY < 90)
				{
					//iterez sinX pana la maxim
					sinY +=iteratieSin;
				}
				else
					if(sinY > 90)
					{
						sinY = 90;
					}
			}
			//if the body is moving to the left
			else
			{
				if(sinY > -90)
				{
					sinY -=iteratieSin;
				}
				else
					if(sinY < -90)
					{
						sinY = -90;
					}
			}
		}
	}


	if(itMovedY == false || Hero2nd->theBody->GetPosition().y <= SCREEN_HEIGHT / scaleSize / 2)
	{
		//if the body is moving to the right
		if(sinY > 0)//Hero2nd->theBody->GetLinearVelocity().x > 0)
		{
			sinY -= iteratieSin;
			//scad primul sin

			if(sinY <0)
			{
				sinY = 0;
			}
		}
		else
			//if the body is moving to the left
			if(sinY < 0)//Hero2nd->theBody->GetLinearVelocity().x > 0)
			{
				sinY += iteratieSin;
				//scad primul sin

				if(sinY > 0)
				{
					sinY = 0;
				}
			}
	}


/*		std::cout<<"misca "<<startPositionCameraX<<" "<<Hero2nd->theBody->GetPosition().x<<
		abs(startPositionCameraX - Hero2nd->theBody->GetPosition().x)<<"\n";
*/
	if(abs(startPositionCameraX - Hero2nd->theBody->GetPosition().x) > 2)
	{
		//std::cout<<"misca "<<startPositionCameraX<<" "<<Hero2nd->theBody->GetPosition().x<<
		//abs(startPositionCameraX - Hero2nd->theBody->GetPosition().x)<<"\n";
		startPositionCameraX = Hero2nd->theBody->GetPosition().x - 2;
		changeInPosition = 2;
	}







	if(isInAir)
	{
		controlAir(0.1f);
	}
	else
		if(isInCorner)
		{
			controlCorner(Hero2nd->theBody);
		}
		else
			if(isOnGround)
			{
				 controlGround(Hero2nd->theBody, 0.05f);
			}
				else
					if(isOnLeftWall || isOnRightWall)
					{
						controlWall(Hero2nd->theBody);
					}

	//daca este pe pamant poate sa sara
/*	if(ButtonJump)
	{
		if(ButtonJump == true && canJumpVertically == true && canJumpHorizontally == false)
		{
			desiredVelY = speedJumpUp;
			canJumpVertically = false;
			ButtonJump = false;
		}
		else
		if(isOnLeftWall || isOnRightWall)
		{//std::cout<<"is in air"<<'\n';
			//daca este pe perete
			if(ButtonJump == true && canJumpHorizontally == true)
			{
				//daca este pe peretele din stanga, si apasa pe stanga, ca sa urce
				if(ButtonLeft == true && isOnLeftWall == true)
				{
					desiredVelY = speedJumpUp;// b2Min( vel.y + 3.1f,  speedJumpUp );
					desiredVelX = speedJumpToSide;
					canJumpHorizontally = false;
					jumpsUpTheWall = true;
				}
				else
					if(ButtonRight == true && isOnRightWall == true)
					{
						desiredVelY = speedJumpUp;// b2Min( vel.y + 3.1f,  speedJumpUp );
						desiredVelX = -speedJumpToSide;
						canJumpHorizontally = false;
						jumpsUpTheWall = true;
					}
			}
			else
				if(canJumpHorizontally == false)
				{
					desiredVelY = vel.y * 4.0f;
					vel.x = 0;
					desiredVelX = vel.x * 4.0f;
				}
		}
		else
		{
			desiredVelY = vel.y * 4.0f;
			vel.x = 0;
			desiredVelX = vel.x * 4.0f;
		}
	}
	else
	{
		desiredVelY = vel.y * 4.0f;
		vel.x = 0;
		desiredVelX = vel.x * 4.0f;
	}

	if(jumpsUpTheWall == true && ButtonLeft == true)
	{
		Hero2nd->theBody->ApplyLinearImpulse(b2Vec2(-2, 0), Hero2nd->theBody->GetWorldCenter());
	}
	else
		if(jumpsUpTheWall == true && ButtonRight == true)
		{
			Hero2nd->theBody->ApplyLinearImpulse(b2Vec2(2, 0), Hero2nd->theBody->GetWorldCenter());
		}*/
/*
	if(ButtonRight == true && ButtonLeft == false && (isOnLeftWall == false && isOnRightWall == false))
    {
		desiredVelX = b2Min( vel.x + 3.1f,  speedWalk );
    }
    else
        if(ButtonLeft == true && ButtonRight == false  && (isOnLeftWall == false && isOnRightWall == false))
        {
			desiredVelX = b2Max( vel.x - 3.1f, -speedWalk );
        }
		else
			if(ButtonLeft == false && ButtonRight == false && isOnLeftWall == false && isOnRightWall == false)
		{
			vel.x = 0;
			desiredVelX = vel.x * 4.0f;
			Hero2nd->theBody->SetLinearVelocity(b2Vec2(0,vel.y));
		}
    velChangeX = desiredVelX - vel.x;
    velChangeY = desiredVelY - vel.y;

    impulseX = Hero2nd->theBody->GetMass() * velChangeX; //disregard time factor
    impulseY = Hero2nd->theBody->GetMass() * velChangeY; //disregard time factor

    Hero2nd->theBody->ApplyLinearImpulse( b2Vec2(impulseX, impulseY), Hero2nd->theBody->GetWorldCenter() );*/
}

void BSActionStuffRound::moveRightOnGround(b2Body *body)
{
	//Hero2nd->theBody->SetLinearVelocity(b2Vec2(20 * 30, 0));

	vel = body->GetLinearVelocity();
    desiredVelX = 0;

	if(ButtonRight == true && ButtonLeft == false)
    {
		desiredVelX = b2Min( vel.x + 0.1f,  5.0f );
    }
    else
        if(ButtonLeft == true && ButtonRight == false)
        {
			desiredVelX = b2Max( vel.x - 0.1f, -5.0f );
        }
		else
		{
			desiredVelX = vel.x * 4.0f;
		}
		/*
    switch ( //moveState )
    {
      case MS_LEFT:  desiredVel = b2Max( vel.x - 0.1f, -5.0f ); break;
      case MS_STOP:  desiredVel = vel.x * 4.0f; break;
      case MS_RIGHT: desiredVel = b2Min( vel.x + 0.1f,  5.0f ); break;
    }*/
    velChangeX = desiredVelX - vel.x;
    impulseX = body->GetMass() * velChangeX; //disregard time factor
    body->ApplyLinearImpulse( b2Vec2(impulseX,0), body->GetWorldCenter() );
}



void BSActionStuffRound::destroyTheParticles()
{

}

void BSActionStuffRound::generateTheParticles()
{

}

void BSActionStuffRound::generateWhenHitsGroundParticles()
{

}



void BSActionStuffRound::ClickOnBlock()
{
	if(MouseLeft && canClickLeft)
	{
		if(canAddObstacles)
		{
			float variabilaAdunareX = 0, variabilaAdunareY = 0;
			if(LittleBigMoveTheObstacle == 0)
			{
				variabilaAdunareX = Hero_size / 2.f;
				variabilaAdunareY = Hero_size / 2.f;
			}
			else
				if(LittleBigMoveTheObstacle == 1)
				{
					variabilaAdunareX = widthOfObstacle / 2.f;
					variabilaAdunareY = heightOfObstacle / 2.f;
				}
				else
					if(LittleBigMoveTheObstacle == -1)
					{
						variabilaAdunareX = Hero_size / 4.f;
						variabilaAdunareY = Hero_size / 4.f;
					}

			bool dejaExista = false;
			int indexOfBody = 0;



			if(dejaExista == false)
			{
			}
			else
			{
				//selectedBody = bodies[indexOfBody];
				//indexOfSelectedBody = indexOfBody;
				std::cout<<"deja exista un obiect la acele coordonate"<<'\n';
			}
			canClickLeft = false;
		}
		else
		{
			selectedSquareBody = NULL;
		}
	}

	if(MouseRight && canClickRight)
	{
		if(canAddObstacles)
		{
			float variabilaAdunareX = 0, variabilaAdunareY = 0;
			if(LittleBigMoveTheObstacle == 0)
			{
				variabilaAdunareX = Hero_size / 2.f;
				variabilaAdunareY = Hero_size / 2.f;
			}
			else
				if(LittleBigMoveTheObstacle == 1)
				{
					variabilaAdunareX = widthOfObstacle / 2.f;
					variabilaAdunareY = heightOfObstacle / 2.f;
				}
				else
					if(LittleBigMoveTheObstacle == -1)
					{
						//variabilaAdunareX = Hero_size / 4.f;
						//variabilaAdunareY = Hero_size / 4.f;
					}

			bool dejaExista = false;
			int indexSters = 0;



			if(dejaExista)
			{
			}
			canClickRight = false;
		}
		else
		{
			selectedSquareBody = NULL;
			selectedRoundBody = NULL;
		}
	}


	/*
	//if(MouseMoved)
    {
        //pozitia butonului din pauza
        //300,300 - pozitia butonului
        //100 width of the button
        //480 este height of the window
        //if(MouseX > 300 - 100/2 && MouseX < 300 + 100/2 &&
           //MouseY > 480 - 300 - 30/2 && MouseY < 480 - 300 + 30/2)
		if(MouseX / scaleSize > (float) SCREEN_WIDTH / 2.f / scaleSize - 2 - buttonMenuWidth &&// -1 pentru ca
			//sa zicem ca butonul e format din 4 patrate, iar coordonatele butonului este dat in al 3lea patrat
			//butonul: [][]|aici se afla coordonatele lui, care defapt sunt ale urmatorului patrat|[][],
			//si anume patratul 3. Asadar atunci cand verific la stanga daca mouseul se afla in buton mai scad
			//inca un patrat
			MouseX / scaleSize < (float) SCREEN_WIDTH / 2.f / scaleSize - 2 + buttonMenuWidth &&
			MouseY / scaleSize > (float) SCREEN_HEIGHT / scaleSize - SCREEN_HEIGHT / 2 / scaleSize + 2 - buttonMenuHeight &&
			MouseY / scaleSize < (float) SCREEN_HEIGHT / scaleSize - SCREEN_HEIGHT / 2 / scaleSize + 2 + buttonMenuHeight)

        {
            canClick = true;
            theString = "este pe buton";
            //std::cout<<"este pe buton " << MouseX / scaleSize<< "  "  << MouseY / scaleSize<<'\n';
        }
        else
		{
			theString = "nu este pe buton " + toString(SCREEN_WIDTH / 2 / scaleSize - 10) + " " +
									toString(SCREEN_HEIGHT / 2 / scaleSize - 10);
			//std::cout<<"   "<< MouseX / scaleSize<< "  "  << MouseY / scaleSize<<'\n';
		}
    }

    if(MouseLeft == true && canClick == true && aDatClick == false)
    {
		//std::cout<<"CLICK!!!"<<'\n';
		aDatClick = true;
    }*/
}



void BSActionStuffRound::ActionFuncTeleportHero()
{
	float TeleportX = 0, TeleportY = 0;


	if(Hero2nd->theBody->GetPosition().x <= CameraExtremeLeft)
	{
		TeleportX = MouseX / scaleSize;
	}
	else
		if(Hero2nd->theBody->GetPosition().x > CameraExtremeLeft && Hero2nd->theBody->GetPosition().x < CameraExtremeRight)
		{
			TeleportX = MouseX / scaleSize + Hero2nd->theBody->GetPosition().x - CameraExtremeLeft;
		}
		else
			if(Hero2nd->theBody->GetPosition().x > CameraExtremeRight)
			{
				TeleportX = MouseX / scaleSize + CameraExtremeRight - CameraExtremeLeft;
			}

	if(Hero2nd->theBody->GetPosition().y <= CameraExtremeDown)
	{
		TeleportY = (float)(SCREEN_HEIGHT - MouseY) / scaleSize;
	}
	else
		if(Hero2nd->theBody->GetPosition().y > CameraExtremeDown && Hero2nd->theBody->GetPosition().y < CameraExtremeUp)
		{
			TeleportY = (float)Hero2nd->theBody->GetPosition().y + (SCREEN_HEIGHT - MouseY) / scaleSize - CameraExtremeDown;
		}
		else
			if(Hero2nd->theBody->GetPosition().y > CameraExtremeUp)
			{
				TeleportY = (SCREEN_HEIGHT - MouseY) / scaleSize + CameraExtremeUp - CameraExtremeDown;
			}

	TeleportHero(TeleportX, TeleportY);
}

void BSActionStuffRound::TeleportHero(float coordX, float coordY)
{
	//daca se poate teleporta

		if(MouseLeft && canClickLeft)
		{
			if(canTeleportHero && heroCanTeleportAgain)
			{
				std::cout<<"teleported"<<'\n';
				Hero2nd->theBody->SetTransform(b2Vec2(coordX, coordY), Hero2nd->theBody->GetAngle());
				heroCanTeleportAgain = false;
				Hero2nd->theBody->ApplyLinearImpulse(b2Vec2(0, 0.1f), Hero2nd->theBody->GetWorldCenter());
			}

		}
		else
			//daca a luat mana de pe mouse atunci il poate teleporta din nou
		{
			heroCanTeleportAgain = true;
		}


}



//deseneaza stelute in spatele lui
void BSActionStuffRound::PathColoring( int LeftOrRight)
{

}

void BSActionStuffRound::heroTouchesObjects()
{
    b2ContactEdge *edge = Hero2nd->theBody->GetContactList(); //get all the objects the hero is contacting with
    //for (b2ContactEdge* edge = Hero2nd->theBody->GetContactList(); edge; edge = edge->next)
	//{

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
			USERDATA* us = new USERDATA;
			us = (USERDATA*)edge->contact->GetFixtureB()->GetBody()->GetUserData();
			anotherString = us->name;

			if(us->name == "Hero2nd")
			{
				std::cout<<normal1.y<<'\n';
				//if it is touching the ground
				if (normal1.y > 0 )//&& isOnLeftWall == false && isOnRightWall == false)
				//if the normal is negative/ the hero is being pushed up/ sitting on top of another object
				{
					anotherString = "is on the ground";
					canJumpVertically = true;
					isOnGround = true;
					isInAir = false;

					///retin daca tocmai a atins pamantul
					if(wasInAir)
					{
						justHitTheGround = true;
					}
				}

				//if it touches a wall
				if(normal1.x != 0)
				{
					//anotherString = "is touching " + toString(normal1.x) + " wall";
					//if it touches the left wall
					if(normal1.x > 0)
					{
						isOnLeftWall = true;
						isOnRightWall = false;
					}
					else
					//if it touches the right wall
					{
						isOnLeftWall = false;
						isOnRightWall = true;
					}
					canJumpHorizontally = true;
					variableWall = normal1.x;//-1 for left, 1 for right
					isInAir = false;


					///retin daca tocmai a atins pamantul
					if(wasInAir)
					{
						justHitTheGround = true;
					}
				}
				//anotherString = toString(normal1.x) + "  " + toString(normal1.y);
			}
        }

        //anotherString = toString(normal1.x) + "  " + toString(normal1.y);

        delete a;
        a = NULL;

			//std::cout<<normal1.x<<" "<<normal1.y<<"   "<<' ';

        edge = edge->next;
    }

    //verfic daca se afla in colt (si pe pamant si pe perete)
	if(isOnGround && (isOnLeftWall || isOnRightWall))
	{
		isInCorner = true;
		isOnLeftWall = false;
		isOnRightWall = false;
		//std::cout<<"is in corner"<<'\n';
	}

	delete edge;
	edge = NULL;
}








bool BSActionStuffRound::CanGoAwayFromWall(float timeToMatch)
{
	//currentTime = timp->getTime();
	//if(currentTime != -1)
	currentTime = SDL_GetTicks();
	{
		//std::cout<<currentTime<<" "<<OriginalTimeOnWall<<'\n';
		if(currentTime - timeToMatch > timeToStayOnWall)
		{
			return true;
		}
	}
	return false;
}



float BSActionStuffRound::easeIn (float t,float b , float c, float d) {
	return c*(t/=d)*t*t*t*t + b;
}
float BSActionStuffRound::easeOut(float t,float b , float c, float d) {
	return c*((t=t/d-1)*t*t*t*t + 1) + b;
}

float BSActionStuffRound::easeInOut(float t,float b , float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
	return c/2*((t-=2)*t*t*t*t + 2) + b;
}

float BSActionStuffRound::radical(float x)
{
	return x - 1/2.f;
	//return pow(x,(1/2.f));
}
