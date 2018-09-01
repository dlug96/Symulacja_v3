#pragma once
#include "stdafx.h"
#include "Header.h"

class generator_01
{
private:
	int X;
	int m;
	int a;
	int q;
	int r;
	double h;
	//X=a*(X_poprzednie - q*h) - r*h
	//Jeśli X<0 => X=X+m
	//Liczba losowa = X/m

public:
	generator_01(double lastValue);

	double newNumber();
};