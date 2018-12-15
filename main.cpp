#include "main.hpp"

int main(void)
{
	unsigned int time=0;

	setup();

	while(1){

		if(millis()-time >= 500){
			time = millis();
			led0.toggle();
			s.printf("yes\n");
		}
	}

}
