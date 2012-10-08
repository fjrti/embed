#include <Mega32.h>
#include <delay.h>
#include <stdio.h>
#include "txrx.c" 

#define begin {
#define end }

//define some useful maps to bits
#define MOUSE_CLK_OUT PORTC.1
#define MOUSE_CLK_IN PINC.0
#define MOUSE_DATA_OUT PORTC.5
#define MOUSE_DATA_IN PINC.4
#define KEYBOARD_CLK_OUT PORTC.3
#define KEYBOARD_CLK_IN PINC.2
#define KEYBOARD_DATA_OUT PORTC.7
#define KEYBOARD_DATA_IN PINC.6  

//define PS/2 data line states
#define IDLE 0
#define INHIBIT 1
#define BUSY 2
#define WAIT_FOR_REQUEST 3
#define REQUEST 4
#define POWEROFF 5

// boolean defines
#define TRUE 1
#define FALSE 0 

// RX defines
#define tx_id 163
#define MAX_RX_LENGTH 32   

char k;
unsigned char my_rx_data[MAX_RX_LENGTH];

#define MOUSE 1
#define KEYBOARD 2

#define LowLetter 0
#define CapLetter 1
#define Number 2
#define Punct 3

//an ascii list of the english letters sorted by frequency (sub 32 to get upper case)
//the letters are: e t a i o n s h r d l c u m w f g y p b v k j x q z
flash char letter[26]
={101,116,97,111,105,110,115,104,114,100,108,99,117,109,119,102,103,121,112,98,118,107,106,120,113,122};
//numbers : 0 1 2 3 4 5 6 7 8 9
flash char numbers[10] =
{48,49,50,51,52,53,54,55,56,57};
//other characters (punctuation)
//sorted for frequency:  . , : ; ? ! " ' / ( ) * + - = # $ % &
flash char punct[19] = 
{46,44,58,59,63,33,34,39,47,40,41,42,43,45,61,35,36,37,38};
char userMode;  //user mode 0 = mouse; 1 = keyboard;
char charMode;  //charater mode, 0:low lett  1:cap lett  2:numb   3:punc  5:backspace
char charLen[4] = {26,26,10,19};  //number of characters available in current mode
signed char nextChar;
char newChar;

signed char stepX, stepY, btnClick;
int blinkCount;
char bsCount;

char key, prev_key;

// byte declarations
unsigned char mouse_byte_index;
unsigned char mouse_transmit_byte;
unsigned char mouse_transmit_parity;
unsigned char mouse_receive_byte;
unsigned char mouse_receive_bit;
unsigned char mouse_previous_byte;
unsigned char mouse_resend_byte;

// clock state machine declarations
unsigned char mouse_clock_state;
unsigned char mouse_next_clock_state;
unsigned char mouse_state;
unsigned char mouse_transmit;
unsigned char mouse_receive;


// mouse packet declarations
unsigned char gen_mouse_packet;
unsigned int mouse_count;
unsigned char mouse_delay;

// mouse packet
unsigned char mouse_byte;
// keyboard packet
unsigned char key_byte;

// byte declarations
unsigned char keyboard_byte_index;
unsigned char keyboard_transmit_byte;
unsigned char keyboard_transmit_parity;
unsigned char keyboard_receive_byte;
unsigned char keyboard_receive_bit;
unsigned char keyboard_previous_byte;
unsigned char keyboard_resend_byte;

// clock state machine declarations
unsigned char keyboard_clock_state;
unsigned char keyboard_next_clock_state;
unsigned char keyboard_state;
unsigned char keyboard_transmit;
unsigned char keyboard_receive;


// keyboard packet declarations
unsigned char gen_keyboard_packet;
unsigned int keyboard_count;
unsigned char keyboard_delay;

#define RISE 0
#define HIGH 1
#define FALL 2
#define LOW 3

// queue declarations
#define QUEUELEN 100
unsigned char mouse_queue[QUEUELEN]; //queue of data sent by keyboard. (first in, first out)
unsigned char mouse_queueFull;    //indicates if queue is full
unsigned char mouse_queueEmpty;//indicates if queue is empty
unsigned char mouse_queueIn;       //indicates where to put data into queue
unsigned char mouse_queueOut;    //indicates where to take data out of queue

unsigned char keyboard_queue[QUEUELEN]; //queue of data sent by keyboard. (first in, first out)
unsigned char keyboard_queueFull;    //indicates if queue is full
unsigned char keyboard_queueEmpty;//indicates if queue is empty
unsigned char keyboard_queueIn;       //indicates where to put data into queue
unsigned char keyboard_queueOut;    //indicates where to take data out of queue

// PS/2 Host Command set.
#define ERROR			              0xFC
#define RESET 			            0xFF
#define RESEND 			            0xFE
#define SET_MAKE                0xFD
#define SET_MAKE_BREAK          0xFC
#define SET_TYPE                0xFB
#define SET_ALL_TYPE_MAKE_BREAK 0xFA
#define SET_ALL_MAKE            0xF9
#define SET_ALL_MAKE_BREAK      0xF8
#define SET_ALL_TYPE            0xF7
#define SET_DEFAULT 	          0xF6
#define DIS_DATA_REPORT         0xF5
#define EN_DATA_REPORT 	        0xF4
#define SET_SAMPLE_RATE         0xF3
#define GET_DEVICE_ID 	        0xF2
#define SET_REMOTE_MODE         0xF0      // for mouse
#define SET_SCAN_CODE           0xF0      // for keyboard
#define SET_WRAP_MODE 	        0xEE      // for mouse
#define ECHO                    0xEE      // for keyboard
#define SET_LED                 0xED
#define RESET_WRAP_MODE         0xEC
#define READ_DATA 		          0xEB
#define SET_STREAM_MODE         0xEA
#define STATUS_REQ 		          0xE9
#define SET_RES 		            0xE8
#define SET_SCALING21	          0xE7
#define SET_SCALING11	          0xE6

#define ACK 			          0xFA
#define SELF_TEST_PASS	    0xAA
#define INIT			          0x00
#define SCALING21		        0x02

// mouse declarations
unsigned char mouse_deviceID;
unsigned char mouse_data_reporting;
unsigned char mouse_sample_rate;
unsigned char mouse_scaling;	// 0 = 1:1, 1 = 2:1
unsigned char mouse_resolution;
unsigned char mouse_waiting_for_sample_rate;
unsigned char mouse_waiting_for_resolution;

// keyboard declarations
unsigned char keyboard_deviceID[2];
unsigned char keyboard_data_reporting;
unsigned char keyboard_waiting_for_sample_rate;
unsigned char keyboard_waiting_for_led;

// Function prototypes
void initialize(void); //all the usual mcu stuff

// function prototypes
unsigned char mouse_queuePut(unsigned char);
unsigned char mouse_queueGet(void);
void mouse_process_command(unsigned char);

unsigned char keyboard_queuePut(unsigned char);
unsigned char keyboard_queueGet(void);
void keyboard_process_command(unsigned char);

unsigned char getParity(unsigned char);

// mouse function
void mouse_init(void);
void mouse_send(signed char, signed char, unsigned char, unsigned char, unsigned char);

//keyboard function
void keyboard_init(void);
void keyboard_send(char,char);

void setMode(char);

// this interrupt should run every 20us or 1/4 period of the clock
interrupt [TIM0_COMP] void timer0_overflow(void)
begin

	if (mouse_queueEmpty == TRUE)
		mouse_count++;

	if (mouse_count >= 500)	// 100 samples a second
	begin
		gen_mouse_packet = TRUE;
		mouse_count = 0;
	end

	if (mouse_delay > 0)
	   mouse_delay--;

	// state machine for transmitting a byte to Host
	if (mouse_transmit && mouse_delay == 0)
	begin
		// update the next clock state
		mouse_clock_state = mouse_next_clock_state;

		// do the appropriate command based on the state of the clock
		switch (mouse_clock_state)
		begin
			case RISE:	// start or end a byte transmission
				switch (mouse_byte_index)
				begin
					case 0:
						// get a new byte to be xmitted
						if (mouse_queueEmpty == FALSE) // get data off the queue to be xmitted
						begin
				 			mouse_transmit_byte = mouse_queueGet();
				 			mouse_transmit_parity = getParity(mouse_transmit_byte);
				 			//delay_us(150);			// used to wait until host is ready
				 			mouse_delay = 7;
				 		end
				 		break;

				 	case 11:
				 		// finished transmission
				 		mouse_transmit = FALSE; 	// transmit finished
				 		mouse_byte_index = 0;	// reset byte index
				 		break;

				 	default:
				 		break;
				end

				if (mouse_delay == 0)
				begin
				   // set the new clock state
				   MOUSE_CLK_OUT = 1;
				   mouse_next_clock_state = HIGH;
				   break;	// end of RISE
				end

			case HIGH:	// can begin writing data to host

				// state machine ensure we transmit the correct bit from the PS/2 byte
				switch (mouse_byte_index)
				begin
					case 0:
						MOUSE_DATA_OUT = 0;
						break;
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
						MOUSE_DATA_OUT = mouse_transmit_byte & 0x01;
						mouse_transmit_byte = mouse_transmit_byte >> 1;
						break;
					case 9:
						MOUSE_DATA_OUT = mouse_transmit_parity;
						break;
					case 10:
						MOUSE_DATA_OUT = 1;
						break;
					default:
						break;
				end

				// update the byte index
				mouse_byte_index++;

				// update the clock state
				mouse_next_clock_state = FALL;
				break;	// end of HIGH

			case FALL:   // clock transitions to low
				MOUSE_CLK_OUT=0;				// pull the clock to 0
				mouse_next_clock_state = LOW;	// set the next clock state to low
				break;	// end of FALL

			case LOW:   // clock is low - nothing happens here
				mouse_next_clock_state = RISE;	// next clock state is rising
				break;	// end of RISE
		end
	end


	if (mouse_receive)
	begin
		// get the next clock state
		mouse_clock_state = mouse_next_clock_state;

		switch (mouse_clock_state)
		begin
			case RISE:	// all receiving done on the rising edge of the clock
				MOUSE_CLK_OUT = 1;

				// get the bit off the data line
			 	// and place into the receive_byte
				if (mouse_byte_index >=0 && mouse_byte_index < 8)
				begin
					mouse_receive_bit = MOUSE_DATA_IN;
					mouse_receive_byte = ((mouse_receive_bit & 0x01) << mouse_byte_index) | mouse_receive_byte;
				end

				// update variables for the next state
				if (mouse_byte_index < 11)
				begin
					mouse_next_clock_state = HIGH;
					mouse_byte_index++;
				end

				break;

			case HIGH:
				switch (mouse_byte_index)
				begin
					case 10:
						MOUSE_DATA_OUT = 0;
						break;

					case 11:
			        	MOUSE_DATA_OUT = 1; 	// send ACK

			        	// terminate receive operation and reset
			        	mouse_receive = FALSE;
			        	mouse_byte_index = 0;

			        	// process the received command
			        	mouse_process_command(mouse_receive_byte);

			        	mouse_receive_byte = 0;
			        	mouse_receive_bit = 0;
			        	break;
			  		default:
			  			break;
			  	end

			  	mouse_next_clock_state = FALL;
			  	break;

			case FALL:	// hosts transmits on the falling edge
				MOUSE_CLK_OUT = 0;
				mouse_next_clock_state = LOW;
				break;

			case LOW:
				mouse_next_clock_state = RISE;
				break;
		end
	end

	switch (mouse_state)
	begin
		case IDLE:
			// enter inhibt state if clock is trying to be pulled
			// low by host when device is trying to make high
			if ((mouse_clock_state == HIGH || mouse_clock_state == RISE) && MOUSE_CLK_IN == 0)	// host trying to pull CLK low!
			begin
				mouse_state = INHIBIT;  // go to the INHIBIT state
				mouse_next_clock_state = LOW;		// turn off the clock
				mouse_transmit = FALSE;			// mouse can no longer transmit
				mouse_receive = FALSE;			// mouse can no longer receive
			end
			else  // check if mouse needs to be in BUSY state -- anything on queue?
			begin
				if(mouse_queueEmpty == FALSE)
				begin
					mouse_state = BUSY;			// transition to busy state
					mouse_transmit = TRUE;				// allow mouse to transmit
					mouse_receive = FALSE;				// make sure we don't receive!
					mouse_next_clock_state = RISE;	// turn on the clock
					mouse_byte_index = 0;				// reset the byte index for transmit
				end
				else // otherwise we should just stay in IDLE
				begin
					mouse_state = IDLE;
				end
			end

			break;

		case BUSY:    
			// again check if host is trying to inhibt
			// by pulling the clock low when it should be high
			if ((mouse_clock_state == HIGH || mouse_clock_state == RISE) && MOUSE_CLK_IN == 0)
			begin
				mouse_state = INHIBIT;  // go to the INHIBIT state
				mouse_next_clock_state = LOW;		// turn off the clock
				mouse_transmit = FALSE;			// mouse can no longer transmit
				mouse_receive = FALSE;			// mouse can no longer receive
			end
			else // go back to IDLE if mouse has nothing to send or receive
			begin
				if ( !(mouse_transmit|| mouse_receive) )
					mouse_state = IDLE;
				else
					mouse_state = BUSY;		// otherwise stay BUSY
			end
			break;

	   	case INHIBIT:
			// if we enter the INHIBIT state before we finish sending
			// a byte, we should save the byte & resend!
			if(mouse_byte_index > 0 && mouse_byte_index < 10)
			begin
				mouse_resend_byte = TRUE;
			end
			// reset the byte index
			mouse_byte_index = 0;

			// check for REQUEST state
			// first, the host must release the CLK
			if (MOUSE_CLK_IN == 1)
			begin
				mouse_state = WAIT_FOR_REQUEST;	// next, we check for DATA line to go low
			end
			else // stay in INHIBIT state
			begin
				mouse_state = INHIBIT;
				mouse_transmit = FALSE;
				mouse_receive = FALSE;
				mouse_next_clock_state = LOW;
			end

			break;

		case WAIT_FOR_REQUEST:
			// check if the data line has gone low
			if(MOUSE_DATA_IN == 0)
			begin
				mouse_state = REQUEST;	// go to REQUEST state
				mouse_next_clock_state = RISE;	// turn on the clock
			end
			else
			begin
				// we must be in the IDLE state because both
				// CLK & DATA are high
				mouse_state = IDLE;