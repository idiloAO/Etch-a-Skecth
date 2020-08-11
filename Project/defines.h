
#define		CPU0						0x01	// bit-mask; bit 0 represents cpu0
#define		ENABLE					     0x1




#define		USER_MODE				0b10000
#define		FIQ_MODE			    0b10001
#define		IRQ_MODE			    0b10010
#define		SVC_MODE			    0b10011
#define		ABORT_MODE				0b10111
#define		UNDEF_MODE				0b11011
#define		SYS_MODE			    0b11111

#define		INT_ENABLE				0b01000000
#define		INT_DISABLE				0b11000000