#include "address_map_arm.h" //contains address of I/O devices 
#define bit_24_pattern 0x01000000  //pattern for LED'S , LED 6 LIGHTS UP 


// Goal is to receive signals from sensors(Arduino) then light up an led on FPGA 
//Test which values LED correspond to which pin 

int main (void){
	
	volatile int * JP1_ptr = (int *)JP1_BASE ;  //Pointers to JP1 address
	volatile int * LED_ptr = (int *)LED_BASE;	//Points to LED Address 
    int LED_0 = 0x1; // pattern for LED lights
	int LED_1 = 0x2;
	int LED_2 = 0x3;
	int LED_3 = 0x4;
	
	
    int JP1_value;
	
	while (1){
   //Dereference to get value 
	JP1_value = *(JP1_ptr);
	  

	  if(JP1_value == 0x1) //pin 0, received signal
	    *(LED_ptr) = LED_bits;  //pattern to display on LED 

	  else if (JP1_value == 0x2)  //pin 1, received signal
		 *(LED_ptr) = LED_1;  
	  
	 else if (JP1_value  == 0x3)
		 *(LED_ptr) = LED_2;
	 
	 else if (JP1_value == 0x4)
		*(LED_ptr) = LED_3;

	
	}
}

