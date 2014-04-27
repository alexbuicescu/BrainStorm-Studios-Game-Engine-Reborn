#ifndef UMBRE_H_INCLUDED
#define UMBRE_H_INCLUDED

#include <Box2D/Box2D.h>



class BSUmbre
{
public:
	void calculeazaDreapta(float x1, float y1, float x2, float y2);
	float aflaX();
	float aflaY();

	//caz11 = cand jucatorul se afla in stanga, sub obiect
	//caz12 = cand jucatorul se afla in stanga, la nivelul obiectului
	//caz13 = cand jucatorul se afla in stanga, deasupra obiectului
	//caz14 = cand jucatorul se afla deasupra obiectului

	//caz21 = cand jucatorul se afla in dreapta, sub obiect
	//caz22 = cand jucatorul se afla in dreapta, la nivelul obiectului
	//caz23 = cand jucatorul se afla in dreapta, deasupra obiectului
	//caz24 = cand jucatorul se afla sub obiect

	void caz11();
	void caz12();
	void caz13();

	void caz21();
	void caz22();
	void caz23();

	void findOutTheLine();
	b2Vec2 getRayPoints(b2Vec2 p1, b2Vec2 p2);
	void drawReflectedRay( b2Vec2 p1, b2Vec2 p2 );

	void afiseazaUmbre();
};

#endif // UMBRE_H_INCLUDED
