#include "address_map_arm.h"
#include "defines.h"
#include <stdio.h>

void disable_A9_interrupts(void);
void set_A9_IRQ_stack(void);
void config_GIC(void);
void config_KEYs(void);
void enable_A9_interrupts(void);
void pushbutton_ISR(void);
void config_interrupt(int, int);
void plot_pixel(int x, int y, short int line_color);



int main(void) {
disable_A9_interrupts(); // disable interrupts in the A9 processor
set_A9_IRQ_stack(); // initialize the stack pointer for IRQ mode
config_GIC(); // configure the general interrupt controller
config_KEYs(); // configure pushbutton KEYs to generate interrupts
enable_A9_interrupts(); // enable interrupts in the A9 processor
while (1) // wait for an interrupt
;
}
/* setup the KEY interrupts in the FPGA */
void config_KEYs() {
volatile int * PS2_ptr = (int *) PS2_BASE; // pushbutton KEY base address
*(PS2_ptr + 1) = 0x00000001; // enable interrupts for the two KEYs
}

// Define the IRQ exception handler
void __attribute__((interrupt)) __cs3_isr_irq(void) {
// Read the ICCIAR from the CPU Interface in the GIC
int interrupt_ID = *((int *)0xFFFEC10C);
if (interrupt_ID == 79) // check if interrupt is from the KEYs
pushbutton_ISR();
else
while (1); // if unexpected, then stay here
// Write to the End of Interrupt Register (ICCEOIR)
*((int *)0xFFFEC110) = interrupt_ID;
}
// Define the remaining exception handlers
void __attribute__((interrupt)) __cs3_reset(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_undef(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_swi(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_pabort(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_dabort(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_fiq(void) {
while (1);
}
// Turn off interrupts in the ARM processor
void disable_A9_interrupts(void) {
int status = 0b11010011;
asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}

//Initialize the banked stack pointer register for IRQ mode
void set_A9_IRQ_stack(void) {
int stack, mode;
stack = 0xFFFFFFFF - 7; // top of A9 onchip memory, aligned to 8 bytes
/* change processor to IRQ mode with interrupts disabled */
mode = 0b11010010;
asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
/* set banked stack pointer */
asm("mov sp, %[ps]" : : [ps] "r"(stack));
/* go back to SVC mode before executing subroutine return! */
mode = 0b11010011;
asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
}
/*
* Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void) {
int status = 0b01010011;
asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}
/*
* Configure the Generic Interrupt Controller (GIC)
*/
void config_GIC(void) {
	config_interrupt (79, 1); // configure the FPGA KEYs interrupt (73)
	// Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of all
	// priorities
	*((int *) 0xFFFEC104) = 0xFFFF;
	// Set CPU Interface Control Register (ICCICR). Enable signaling of
	// interrupts
	*((int *) 0xFFFEC100) = 1;
	// Configure the Distributor Control Register (ICDDCR) to send pending
	// interrupts to CPUs
	*((int *) 0xFFFED000) = 1;
}
/*
* Configure Set Enable Registers (ICDISERn) and Interrupt Processor Target
* Registers (ICDIPTRn). The default (reset) values are used for other registers
* in the GIC.
*/
void config_interrupt(int N, int CPU_target) {
	int reg_offset, index, value, address;
	/* Configure the Interrupt Set-Enable Registers (ICDISERn).
	* reg_offset = (integer_div(N / 32) * 4
	* value = 1 << (N mod 32) */
	reg_offset = (N >> 3) & 0xFFFFFFFC;
	index = N & 0x1F;
	value = 0x1 << index;
	address = 0xFFFED100 + reg_offset;
	/* Now that we know the register address and value, set the appropriate bit */
	*(int *)address |= value;

	/* Configure the Interrupt Processor Targets Register (ICDIPTRn)
	* reg_offset = integer_div(N / 4) * 4
	* index = N mod 4 */
	reg_offset = (N & 0xFFFFFFFC);
	index = N & 0x3;
	address = 0xFFFED800 + reg_offset + index;
	/* Now that we know the register address and value, write to (only) the
	* appropriate byte */
	*(char *)address = (char)CPU_target;
}
// Plots a pixel 
void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void pushbutton_ISR(void) {
	
	volatile int * PS2_ptr = (int *)PS2_BASE;
	volatile int * LED_ptr = (int *)LEDR_BASE;	//Points to LED Address 
	
	int PS2_data = *(PS2_ptr);
	int RVALID = PS2_data & 0x8000;
	int i = 0;
    unsigned char byte0;
	unsigned char key [4] = {0,0,0,0};
	int readInterruptReg;
	
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

			}
		
			if(byte0 == 0x1D){  //UP, W
				*(LED_ptr) = 0x01; //led 0
				key[0] = 1;
			}
			if(byte0 == 0x1B){ //DOWN, S
				*(LED_ptr) = 0x02;  //led 2
				key[1] = 1;
			}
			if(byte0 == 0x1C){ //LEFT, A
				*(LED_ptr) = 0x04;  //led 0 & 1 
				key[2] = 1;
			}
			if(byte0 == 0x23){ //RIGHT, D
				*(LED_ptr) = 0x08; // led 3
				key[3] = 1;
			}
		
	} 
	return;
}







