#include <Mega32.h> 
#include <stdio.h>
#include <delay.h>
#include "txrx.c"

#define begin {
#define end }   

#define TRUE 1
#define FALSE 0 

#define BLINKTHRESHOLD 50  //value above which a blink is detected
#define BLINKTIMER 5
#define MOVETHRESHOLD 5
#define data_length 3
#define tx_id 8

char data[data_length]; 

unsigned char X, Y, blink; 
int Xref, Yref, blinkref;
char count;

void initialize(void)
begin

   delay_ms(1000);  //wait a second to equilibrate     
   
   DDRD = 0xFF;
   PORTD.2 = 0;        

	ADMUX = 0b00100000; //ADLAR is not set
   ADCSRA = 0b11000111; //Enable & Start ADC w/ division factor of 128 
   
  	count = 0;
  	Xref = Yref = blinkref = 0;
 
  	txrx_init(1,0,249,1);//TX only - 4000 baud - led on
  	 	
	//crank up the ISRs
	#asm
		sei
	#endasm 
end 

void main(void)
begin  

	initialize();
	
	// begin real work
	while (TRUE) 
	begin       
	      			
			   ADMUX = 0b00100000; // X
			   ADCSRA.6 = 1;
			   while (ADCSRA.6 == 1);
			   X = ADCH; 
	 
			   ADMUX = 0b00100001; // Y
			   ADCSRA.6 = 1;
			   while (ADCSRA.6 == 1);
			   Y = ADCH;
         
			   ADMUX = 0b00100010; // Diode
			   ADCSRA.6 = 1;
			   while (ADCSRA.6 == 1);
			   blink = ADCH; 
			   
			//calculate the background 
			if(count < 100)
			begin 			   
			   Xref += X;
			   Yref += Y;
			   blinkref += blink;
			   count++;
			end
			if(count == 100)
			begin  
			    count++; 
			    Xref = (int)(Xref/100);
			    Yref = (int)(Yref/100);
			    blinkref = (int)(blinkref/100);
			end
			if (count > 100)	  
			begin
			
			   data[0] = 126;	//X movement
			   data[1] = 126;	//Y movement
			   data[2] = 126;	//blink
			   
			   //large X movement
	        if (X-Xref > MOVETHRESHOLD*3)
	         begin
               data[0] = 255;
		      end
		      else if (X-Xref < -MOVETHRESHOLD*3)
		      begin
		         data[0] = 0;
		      end 
			  //medium X movement
          else if (X-Xref > MOVETHRESHOLD<<2)
	         begin
               data[0] = 210;
		      end
		      else if (X-Xref < -MOVETHRESHOLD<<2)
		      begin
		         data[0] = 42;
		      end
			  //little X movement
		      else if (X-Xref > MOVETHRESHOLD)
	         begin
               data[0] = 168;
		      end
		      else if (X-Xref < -MOVETHRESHOLD)
		      begin
		         data[0] = 84;
		      end 
			  //large Y movement 
	        if (Y-Yref > MOVETHRESHOLD*3)
	         begin
               data[1] = 255;
		      end
		      else if (Y-Yref < -MOVETHRESHOLD*3)
		      begin
		         data[1] = 0;
		      end  
			  //medium Y movement
          else if (Y-Yref > MOVETHRESHOLD<<2)
	         begin
               data[1] = 210;
		      end
		      else if (Y-Yref < -MOVETHRESHOLD<<2)
		      begin
		         data[1] = 42;
		      end
			  //little Y movement
		      else if (Y-Yref > MOVETHRESHOLD)
	         begin
               data[1] = 168;
		      end
		      else if (Y-Yref < -MOVETHRESHOLD)
		      begin
		         data[1] = 84;
		      end 
			   
			  //blink detect
			   if(blink-blinkref>BLINKTHRESHOLD)
				begin
			        data[2] = 255;
			   end
            
			   //data[2] = blink;
			   
				   //send data to receiver
			      tx_me(data, data_length, tx_id);
  	
			end
			

			
 	end
end