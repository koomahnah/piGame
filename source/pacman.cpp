extern "C"{
#include "gpio.h"
}
#include "pacman.h"
test *p;
int main(){
	test moj;
	moj.a();
	p = &moj;
	return 0;
}
