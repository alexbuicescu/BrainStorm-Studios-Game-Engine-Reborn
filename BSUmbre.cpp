#include "BSUmbre.h"
#include "BSOpenGL.h"
#include "BSValues.h"
#include <Box2D/Box2D.h>
#include "BSSetUpEverything.h"
#include "BSVBORendering.h"
#include <iostream>


float panta = 0, termenLiber = 0;

// Ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
/*  struct b2RayCastInput
  {
      b2Vec2 p1, p2;
      float32 maxFraction;
  };
  struct b2RayCastOutput
  {
      b2Vec2 normal;
      float32 fraction;
  };
*/
void BSUmbre::calculeazaDreapta(float x1, float y1, float x2, float y2)
{
	panta = (float) (y2 - y1) / (x2 - x1);
	termenLiber = (float) panta * x1 - y1;
}

float BSUmbre::aflaX()
{
	float X = (float) ((SCREEN_HEIGHT / scaleSize + hero->theBody->GetPosition().y) +termenLiber) / panta;
	float Y = (float) -termenLiber;

	if(X > 0)
	{
		return X;
	}
	if(X == 0)
	{
		return X;
	}
	if(X < 0)
	{
		return -1;
	}
}

float BSUmbre::aflaY()
{
	float X = (float) ((SCREEN_HEIGHT / scaleSize - Hero_size) +termenLiber) / panta;
	float Y = (float) panta * (SCREEN_WIDTH / scaleSize - Hero_size) - termenLiber;

	if(Y > 0)
	{
		return Y;
	}
	if(Y == 0)
	{
		return X;
	}
	if(Y < 0)
	{
		return -1;
	}
}

void BSUmbre::caz11()
{

}
void BSUmbre::caz12()
{

}
void BSUmbre::caz13()
{

}
void BSUmbre::caz21()
{

}
void BSUmbre::caz22()
{

}
void BSUmbre::caz23()
{

}

void BSUmbre::findOutTheLine()
{
	/*calculeazaDreapta(hero->theBody->GetPosition().x, hero->theBody->GetPosition().y,
					bodies[4]->GetPosition().x - BodySize[4][0],
						bodies[4]->GetPosition().y + BodySize[4][1]);*/

	float triangX1 = aflaX();

	if(aflaX() > 0)
	{
				glVertex2f(SursaDeLuminaX, SursaDeLuminaY);

				glVertex2f(aflaX(), (SCREEN_HEIGHT / scaleSize + hero->theBody->GetPosition().y));
	}

	float rayLength = 35;
	b2Vec2 p1( hero->theBody->GetPosition().x - 1/100.f, hero->theBody->GetPosition().y ); //center of scene
	b2Vec2 p2 (triangX1, (SCREEN_HEIGHT / scaleSize + hero->theBody->GetPosition().y));
			//= p1 + rayLength * b2Vec2( sinf(60), cosf(60) );

	//std::cout<<getRayPoints(p1, p2).x<<' '<<getRayPoints(p1, p2).y<<'\n';
	glPushMatrix();
	drawReflectedRay(p1, p2);
	glPopMatrix();


	glPopMatrix();
}

b2Vec2 BSUmbre::getRayPoints(b2Vec2 p1, b2Vec2 p2)
{
	b2RayCastInput input;
	input.p1 = p1;
	input.p2 = p2;
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	b2Vec2 intersectionNormal(0,0);
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{

			b2RayCastOutput output;

			if ( ! f->RayCast( &output, input, 0 ) )
			{
				continue;
			}
			if ( output.fraction < closestFraction )
			{
				closestFraction = output.fraction;
				intersectionNormal = output.normal;
			}
		}
	}
	b2Vec2 intersectionPoint = p1 + closestFraction * (p2 - p1);

	return intersectionPoint;
}

void BSUmbre::drawReflectedRay( b2Vec2 p1, b2Vec2 p2 )
{
	//set up input
	b2RayCastInput input;
	input.p1 = p1;
	input.p2 = p2;
	input.maxFraction = 1;

	//check every fixture of every body to find closest
	float closestFraction = 1; //start with end of line as p2
	b2Vec2 intersectionNormal(0,0);
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{

			b2RayCastOutput output;
			if ( ! f->RayCast( &output, input, 0 ) )
			{
				continue;
			}
			if ( output.fraction < closestFraction )
			{
				closestFraction = output.fraction;
				intersectionNormal = output.normal;
				USERDATA *uuss = (USERDATA*)f->GetBody()->GetUserData();
				std::cout<<uuss->name<<'\n';
			}
		}
	}
	b2Vec2 intersectionPoint = p1 + closestFraction * (p2 - p1);

	//draw this part of the ray
	glBegin(GL_LINES);
	glVertex2f( p1.x, p1.y );
	glVertex2f( intersectionPoint.x, intersectionPoint.y );
	glEnd();

	if ( closestFraction == 1 )
		return; //ray hit nothing so we can finish here
	if ( closestFraction == 0 )
		return;

	//still some ray left to reflect
	b2Vec2 remainingRay = (p2 - intersectionPoint);
	b2Vec2 projectedOntoNormal = b2Dot(remainingRay, intersectionNormal) * intersectionNormal;
	b2Vec2 nextp2 = p2 - 2 * projectedOntoNormal;

	//recurse
	drawReflectedRay(intersectionPoint, nextp2);
}

void BSUmbre::afiseazaUmbre()
{

}
