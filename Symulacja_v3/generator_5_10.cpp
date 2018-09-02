#include "stdafx.h"
#include <iostream>
#include "generator_5_10.h"
#include "generator_01.h"
#include <math.h>

generator_5_10::generator_5_10(double seed) {
	gen_01 = new generator_01(seed);
}

int generator_5_10::newNumber() {
	return 5.5 + 5.0 * gen_01->newNumber();
}