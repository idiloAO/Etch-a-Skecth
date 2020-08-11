
void pushbutton_ISR(void) {
	
	volatile int * PS2_ptr = (int *)PS2_BASE;
	volatile int * LED_ptr = (int *)LEDR_BASE;	//Points to LED Address 
	
	int PS2_data = *(PS2_ptr);
	int RVALID = PS2_data & 0x8000;
	int i = 0;
    unsigned char byte0;
	unsigned char key [4] = {0,0,0,0};
	int readInterruptReg;
	int x= 0,y=0;
	short int color = 0xF81F; //pink for now 
	
	//int readInterruptReg = *(PS2_ptr + 1 );
	
	//Read Interrupt Register
	  //readInterruptReg = *(PS2_ptr + 1 ); 
	  
	//Clear Interrupt 
	//*(PS2_ptr+1) = 0x00;
	
	/*
	interruptReg = *(PS2_ptr + 1 )
	*(PS2_ptr + 1 ) = interruptReg;
	
	
	*/
	int LED_1 = 0x1; // pattern for LED lights
	int LED_2 = 0x2;
	int LED_3 = 0x3;
	int LED_4 = 0x4; 
	int ERROR = 0x5;
	
	//reset 
	//*(PS2_ptr) = 0xFF; //CAUSES ERROR 
	*(LED_ptr) = 0x40;



	if(RVALID != 0){
			byte0 = (PS2_data & 0xFF);//data in LSB	
			*(LED_ptr) = 0x20;
		
			if(byte0 == 0xF0) {
				// going to check for break;
				
				for(i = 0; i < 32; ++i)
					*(LED_ptr) ^= 0x80;
				switch (PS2_data & 0xFF) {
					case 0x1D:
						key[0] = 0;
						break;
					case 0x1B:
						key[1] = 0;
						break;
					case 0x1C:
						key[2]=0;
						break;
					case 0x23:
						key[3]=0;
						break;
					default:
						break;
				}

			} //Checks for breaks above 
			
			
		////////////GOES HERE IF BYTE0!=0XF0 (BREAK)
		
			if(byte0 == 0x1D){  //UP, W
				*(LED_ptr) = 0x01; 
				key[0] = 1;
			}
			if(byte0 == 0x1B){ //DOWN, S
				*(LED_ptr) = 0x02;  
				key[1] = 1;
			}
			if(byte0 == 0x1C){ //LEFT, A
				*(LED_ptr) = 0x04;   
				key[2] = 1;
			}
			if(byte0 == 0x23){ //RIGHT, D
				*(LED_ptr) = 0x08; 
				key[3] = 1;
			}
		
	 /////////////////////
	    
	    //Draw Initial Pixel in Middle of screen 
		x=160;
		y=120;
		plot_pixel(x,y,0xF81F);
		
		if(key[0]==1){ //Go Up
		x=x++;
		y=y++;
		plot_pixel(x,y,0xF81F);
		}
		
		if(key[1]==1){ //DOWN
		y=y--;
		plot_pixel(x,y,0xF81F);
		}
		
		if(key[2] == 1){ //LEFT
		 x= x--;
	     plot_pixel(x,y,0xF81F);
		}
		
		if(key[3]==1){//Right
		x= x++;
		plot_pixel(x,y,0xF81F);
		}
	
	} 

	return;
}



//Check if it hits bounds at end of screen later 



