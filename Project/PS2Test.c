//#include "address_map_arm.h" //contains address of I/O devices 


//Goal is to check signal from keyboard, and light up led due to signal 
//Expand to interrupts

// Hex Code for Keys: Up: E075, Down: E072, Left: E06B, Right: E074
//Error: 0x00, Reset: 0xFF

int main (void){
	
	//Points to address of PS2
	volatile int * PS2_ptr = (int *)0xFF200100;

	volatile int reset = 0xFF;
	int PS2_data, RVALID;
    unsigned char byte0,byte1,byte2,byte3;
	int LED_1 = 0x1; // pattern for LED lights
	int LED_2 = 0x2;
	int LED_3 = 0x3;
	int LED_4 = 0x4; 
	int ERROR = 0x5;
	
	*(PS2_ptr) = 0xFF;
	
	
	//endless loop
	while(1){
	PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
	volatile int * LED_ptr = (int *)0xFF200000;	//Points to LED Address 
	RVALID = PS2_data & 0x8000; // extract the RVALID field

	if(RVALID!=0){ //when RVALID is 1, there is data 
	
		byte0 = PS2_data & 0xFF;//data in LSB	
		
			if(byte0 == (char) 0xE075){  //UP
				*(LED_ptr) = LED_1; //led 0
			}
			else if(byte0 == (char)0xE072){ //DOWN
				*(LED_ptr) = LED_2;  //led 2
			}
			else if(byte0 == (char)0xE06B){ //LEFT 
				*(LED_ptr) = LED_3;  //led 0 & 1 
			}
			else if(byte0 ==(char)0XE074){ //RIGHT 
				*(LED_ptr) = LED_4; // led 3
			}
			else{
				//undefined 
				*(LED_ptr) = ERROR; //101 -> led 2 & led 0	
			}
			
		}
		
	}

}