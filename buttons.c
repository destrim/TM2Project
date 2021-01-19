
#include "MKL05Z4.h"                    /* Device header */
#include "buttons.h"
#include "fsm.h"
#include "pmusic.h"

#define COL 4
#define ROW 4


/* Buttons connected to port B */
typedef enum{ BUT1 = 1, BUT2 = 2, BUT3 = 3 } ButtonType;

/*Number of IRQ generated by PORTC_PORTD interrupt*/
static const IRQn_Type myPORT_IRQn = PORTB_IRQn;

/*----------------------------------------------------------------------------
	Interrupt service routine
	Button will cause PORTC_PORTD interrupt 
 *----------------------------------------------------------------------------*/
void PORTB_IRQHandler(void) {

	if( PORTB->ISFR & (1 << BUT1) ){	/* Check in ISFR register if button BUT1 is pressed */
		
    while((FPTB->PDIR&(1<<BUT1))==0);
		PORTB->PCR[BUT1] |= PORT_PCR_ISF_MASK;
  }
	
	if( PORTB->ISFR & (1 << BUT2) ){	/* Check in ISFR register if button BUT2 is pressed */
		
    while((FPTB->PDIR&(1<<BUT2))==0);
		PORTB->PCR[BUT2] |= PORT_PCR_ISF_MASK;
	}
		
	if( PORTB->ISFR & (1 << BUT3) ){ /* Check in ISFR register if button BUT3 is pressed */
		
    while((FPTB->PDIR&(1<<BUT3))==0);
		PORTB->PCR[BUT3] |= PORT_PCR_ISF_MASK;
  }
}

/*----------------------------------------------------------------------------
	Function initializes port C pin for switch 1 (SW1) handling
	and enables PORT_C_D interrupts
 *----------------------------------------------------------------------------*/
void buttonsInitialize(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK; 				/* Enable clock for port B */
	
	PORTB->PCR[BUT1] |= PORT_PCR_MUX(1);      	/* Pin PTB0 is GPIO */
	PORTB->PCR[BUT2] |= PORT_PCR_MUX(1);      	/* Pin PTB1 is GPIO */
	PORTB->PCR[BUT3] |= PORT_PCR_MUX(1);      	/* Pin PTB2 is GPIO */
	
	/* Port control register for bit 3 of port C configuration. Activate pull up and interrupt */
	PORTB->PCR[BUT1] |=  PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* Button SW1 initialize */
	PORTB->PCR[BUT1] |=  PORT_PCR_IRQC(10);
	
	PORTB->PCR[BUT2] |=  PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* Button SW5 initialize */
	PORTB->PCR[BUT2] |=  PORT_PCR_IRQC(10);
	
	PORTB->PCR[BUT3] |=  PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* Button SW9 initialize */
	PORTB->PCR[BUT3] |=  PORT_PCR_IRQC(10);
	
		
	/* ARM's Nested Vector Interrupt Controller configuration */
	NVIC_ClearPendingIRQ(myPORT_IRQn);				/* Clear NVIC any pending interrupts on PORTC_B */
	NVIC_EnableIRQ(myPORT_IRQn);							/* Enable NVIC interrupts source for PORTC_B module */
	NVIC_SetPriority (myPORT_IRQn, 3);			/* Set PORT_B interrupt priority level  */ 

}


void delay_ms(int n) {
	volatile int i;
	volatile int j;
	
	for( i = 0 ; i < n; i++) for(j = 0; j < 3500; j++) {}
}

void buttonsInit(void) { /* initialization */
	
	/* Enable clock for port A */
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK;
	
	/* initializing columns */
	PORTA->PCR[12] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* set MUX to GPIO and enable pull-up resitor on Pin 12*/
	PORTA->PCR[11] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* set MUX to GPIO and enable pull-up resitor on Pin 11*/
	PORTA->PCR[10] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* set MUX to GPIO and enable pull-up resitor on Pin 10*/
	PORTA->PCR[9] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* set MUX to GPIO and enable pull-up resitor on Pin 9*/
	
	/* initializing rows */
	PORTA->PCR[8] |= PORT_PCR_MUX(1); /* set MUX to GPIO on Pin 8*/
	PTA->PSOR |= MASK(8); /* set Pin 8 to off */
	PORTA->PCR[7] |= PORT_PCR_MUX(1); /* set MUX to GPIO on Pin 7*/
	PTA->PSOR |= MASK(7); /* set Pin 8 to off */
	PORTA->PCR[6] |= PORT_PCR_MUX(1); /* set MUX to GPIO on Pin 6*/
	PTA->PSOR |= MASK(6); /* set Pin 8 to off */
	PORTA->PCR[5] |= PORT_PCR_MUX(1); /* set MUX to GPIO on Pin 5*/
	PTA->PSOR |= MASK(5); /* set Pin 8 to off */
	
}

int buttonsGet(void) {  /* get single sample of clicked button */
	int whichbut[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  /* array for checking which button was activated */
	int count = 0;  /* variable for counting buttons that are checked */
	volatile int k;
	volatile int m;
	volatile int n;
	for (k = 0; k < ROW; k++) {  /* iterating on rows */
		PTA->PDDR |= MASK(8 - k);  /* set Pin(8 - k) as output */
		PTA->PCOR |= MASK(8 - k);  /* activating Pin(8-k) */
		for (m = 0; m < COL; m++) {  /* iterating on columns */
			whichbut[count] = (int)(PTA->PDIR & (1<<(12-m)));  /* if 1 then write it to array */
			count++;
		}
		PTA->PSOR |= MASK(8 - k); /* set Pin(8 - k) to off */
		PTA->PDDR ^= MASK(8 - k); /* set Pin(8 - k) as input */
	}
	for (n = 0; n < 16; n++) if(~whichbut[n] == 1) return n+1; /* return number of activate button (1 to 16) */
	return 0;
}

int getButtonNumber(void) {  /* debouncing */
	int prev, next;
	prev = buttonsGet();  /* get 1st sample */
	delay_ms(5);  /* wait for 5 ms */
	next = buttonsGet();  /* get 2nd sample */
	if( next == prev) return prev;  /* if both samples are the same, return it */
	else return 0;  /* else return 0 */
}

