#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
 
 
  /*VARIABLES*/
            char matrix;
            char column;
            char data[]={
            0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08,    /*0-7*/
            0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04,    /*8-15*/
            0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02,    /*16-23*/
            0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,    /*24-31*/
            0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,    /*32-39*/
            0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02,    /*40-47*/
            0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04,    /*48-55*/
            0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08     /*56-63*/
            };
            char datacopy[]={
            0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
            0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
            0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
            0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
            0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
            0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
            0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
            0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF
            };
            char letter[]={0x3F,0x48,0x48,0x3F};
            int AlexIsTired=1;
            int i;
            int i2;
            int mode=3; /*standard, scrolling, and blinking*/
            int cursor=1;
            int character=1;
            int modepos=1;
            char A;
            char wave[]={
            0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,    /*0-7*/
            0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02,    /*8-15*/
            0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04,    /*16-23*/
            0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08,    /*24-31*/
            0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10,    /*32-39*/
            0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,    /*40-47*/
            0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40,    /*48-55*/
            0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80     /*56-63*/
            };
            char wavecopy[64];
            char wave2[]={
            0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,    /*0-7*/
            0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02,    /*8-15*/
            0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04,    /*16-23*/
            0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08,    /*24-31*/
            0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10,    /*32-39*/
            0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,    /*40-47*/
            0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40,    /*48-55*/
            0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80     /*56-63*/
            }; 
            char wave2copy[64];
            
            /*SUBROUTINES*/

          /* putcspi function sends a char to SPI*/
          void putcspi(char cx)
           {
            char temp;
            while(!(SPISR & 0x20));  /* see page 446 HCS12 book */
            SPIDR = cx;
            while(!(SPISR & 0x80));
            temp = SPIDR;
           }
           
          /* putsspi sends a string of chars to SPI*/
          void putsspi(char*ptr)
           {
             while(*ptr) 
             {
              putcspi(*ptr);
              ptr++;
             }
           }
           
           /* sendchartomax function sends first char with address bits of data to drivers and then latches them (p479)*/
           void sendchartomax(char address, char value) 
           {
            PTT &= ~0x01;
            putcspi(address);
            putcspi(value);
            PTT |= 0x01;
           }
           
           /*sendtomax function sends string of 64 chars to the 64 matrix addresses*/
           void sendtomax(char*ptr)
           {
            if((PTT & 0x08)==0x08){
             for(column=0x01; column<0x09; column++)
             {
               PTT &= ~0x01;            /*allows data to be sent*/
               for (matrix=0x01; matrix<0x09; matrix++)
               {
                 putcspi(column);
                 putcspi(*ptr);
                 ptr++  ;
                
               }
              PTT |= 0x01;          /*latches data*/
             }
            }else{          
              for(column=0x01; column<0x09; column++)
             {
              PTT &= ~0x01;            /*allows data to be sent*/
              for (matrix=0x01; matrix<0x09; matrix++)
              {
                putcspi(column);
                putcspi(~(*ptr));
                ptr++  ;
                
              }
              PTT |= 0x01;          /*latches data*/
             }
            }
           }
            /*sendinitdata used for initmax*/
           void sendinitdata(char c1, char c2) 
           {
            PTT &= ~0x01;
            putcspi(c1);
            putcspi(c2);
            putcspi(c1);
            putcspi(c2);
            putcspi(c1);
            putcspi(c2);
            putcspi(c1);
            putcspi(c2);
            putcspi(c1);
            putcspi(c2);
            putcspi(c1);
            putcspi(c2);
            putcspi(c1);
            putcspi(c2);
            putcspi(c1);
            putcspi(c2);
            PTT |= 0x01;
           }
            
              
           /* initialize Drivers */
           void initmax(void)
           {
             DDRT |= 0x01;
             sendinitdata(0x0A, 0x0A); /* set intensity register to middle intensity */
             sendinitdata(0x0B, 0x07); /* set scan rate (8 digits)*/
             sendinitdata(0x0C, 0x01); /* set to normal mode (starts in shutdown) */
             sendinitdata(0x09, 0x00); /* set to no decode */ 
             sendinitdata(0x0F, 0x00); /* set to normal mode (starts in test mode) */ 
           }
           
             char array_index(char array[], int index) {
             return array[index];
             }

           /* scroll function moves shifts of the data one space */ 
           void scroll(void)
           {
            for(i=0;i<64;i++){
              datacopy[i]=data[i];
            }
            for(i=0;i<8;i++){
              data[i]=datacopy[i+57];
            }
            for(i=8;i<64;i++)
            {
              data[i]=datacopy[i-8];
            }
            data[7]=datacopy[56];
            for(i2=1;i2<4;i2++)
            {  
             for(i=1;i<25000;i++){};
            }
            sendtomax(data);
           }
           
           /* flash function moves shifts of the data one space */ 
           void flash(void)
           {
            for(i=0;i<64;i++)
            {
             datacopy[i]=0x00;
            }
            sendtomax(datacopy);
            for(i2=1;i2<2;i2++)
            {  
             for(i=1;i<25000;i++){};
            }
            sendtomax(data);
            for(i2=1;i2<2;i2++)
            {  
             for(i=1;i<25000;i++){};
            }
            
           }  
           
           /*assign letter function recieves a list of chars and assigns them to letter*/
           void assignletter(char c1, char c2, char c3, char c4) 
           {
           letter[0]=c1;
           letter[1]=c2;
           letter[2]=c3;
           letter[3]=c4;
           }
           
           /*getcharacter function recieves a char and manipulates the values for the letter variable*/
           void getcharacter(int c)
           {
           if(c==1) assignletter(0x3F,0x48,0x48,0x3F);        /*A*/
           else if(c==2) assignletter(0x7F,0x49,0x49,0x76);   /*B*/
           else if(c==3) assignletter(0x3E,0x41,0x41,0x22);   /*C*/
           else if(c==4) assignletter(0x7F,0x41,0x41,0x3E);   /*D*/
           else if(c==5) assignletter(0x7F,0x49,0x49,0x49);   /*E*/
           else if(c==6) assignletter(0x7F,0x48,0x48,0x40);   /*F*/
           else if(c==7) assignletter(0x3E,0x41,0x45,0x26);   /*G*/
           else if(c==8) assignletter(0x7F,0x08,0x08,0x7F);   /*H*/
           else if(c==9) assignletter(0x41,0x7F,0x41,0x00);   /*I*/
           else if(c==10) assignletter(0x06,0x01,0x01,0x7E);  /*J*/
           else if(c==11) assignletter(0x7F,0x0C,0x12,0x61);  /*K*/
           else if(c==12) assignletter(0x7F,0x01,0x01,0x01);  /*L*/
           else if(c==13) assignletter(0x7F,0x30,0x30,0x7F);  /*M*/
           else if(c==14) assignletter(0x7F,0x38,0x0E,0x7F);  /*N*/
           else if(c==15) assignletter(0x3E,0x41,0x41,0x3E);  /*O*/
           else if(c==16) assignletter(0x7F,0x48,0x48,0x30);  /*P*/
           else if(c==17) assignletter(0x3E,0x41,0x43,0x3F);  /*Q*/
           else if(c==18) assignletter(0x7F,0x4C,0x4A,0x31);  /*R*/
           else if(c==19) assignletter(0x32,0x49,0x49,0x26);  /*S*/
           else if(c==20) assignletter(0x40,0x7F,0x40,0x00);  /*T*/
           else if(c==21) assignletter(0x7E,0x01,0x01,0x7E);  /*U*/
           else if(c==22) assignletter(0x78,0x07,0x07,0x78);  /*V*/
           else if(c==23) assignletter(0x7F,0x06,0x06,0x7F);  /*W*/
           else if(c==24) assignletter(0x63,0x1C,0x1C,0x63);  /*X*/
           else if(c==25) assignletter(0x70,0x3F,0x70,0x00);  /*Y*/
           else if(c==26) assignletter(0x43,0x4D,0x59,0x61);  /*Z*/
           else if(c==27) assignletter(0x3E,0x41,0x41,0x3E);  /*0*/
           else if(c==28) assignletter(0x21,0x7F,0x01,0x00);  /*1*/
           else if(c==29) assignletter(0x23,0x43,0x45,0x39);
           else if(c==30) assignletter(0x22,0x49,0x49,0x36);
           else if(c==31) assignletter(0x78,0x08,0x08,0x7F);
           else if(c==32) assignletter(0x7A,0x49,0x49,0x46);
           else if(c==33) assignletter(0x36,0x49,0x49,0x06);
           else if(c==34) assignletter(0x40,0x40,0x40,0x7F);
           else if(c==35) assignletter(0x32,0x4D,0x4D,0x32);
           else if(c==36) assignletter(0x30,0x48,0x48,0x7F);  /*9*/
           else if(c==37) assignletter(0x00,0x01,0x00,0x00);  /*.*/
           else if(c==38) assignletter(0x00,0x7D,0x00,0x00);  /*!*/
           else if(c==39) assignletter(0x20,0x45,0x48,0x30);  /*?*/
           else if(c==40) assignletter(0x00,0x1B,0x00,0x00);  /*:*/
           else if(c==41) assignletter(0x00,0x00,0x00,0x00);  /* */
           }
           

           
           /*placeletter function recieves a int (1-16) places letter into data in the correct position*/
           void placeletter(int p)
           {
            if(p==1 |p==3 |p==5 |p==7 |p==9 |p==11 |p==13 |p==15){
              p=(p-1)/2;
             data[7-p]=letter[0];
             data[15-p]=letter[1];
             data[23-p]=letter[2];
             data[31-p]=letter[3];
            } else{
              p=p/2-1;
             data[39-p]=letter[0];
             data[47-p]=letter[1];
             data[55-p]=letter[2];
             data[63-p]=letter[3];
            }
           }
           
           /*placeinverseletter function recieves a int (1-16) places inverse letter into data in the correct position*/
           void placeinverseletter(int p)
           {
            if(p==1 |p==3 |p==5 |p==7 |p==9 |p==11 |p==13 |p==15){
              p=(p-1)/2;
             data[7-p]=~letter[0];
             data[15-p]=~letter[1];
             data[23-p]=~letter[2];
             data[31-p]=~letter[3];
            } else{
              p=p/2-1;
             data[39-p]=~letter[0];
             data[47-p]=~letter[1];
             data[55-p]=~letter[2];
             data[63-p]=~letter[3];
            }

           }
           
                      
           /* writemode displays whatever mode is represented by modepos*/
           void writemode(int n)
           {
            sendtomax(datacopy);
            for(i=0;i<64;i++){
              datacopy[i]=data[i];
            }
            for(i=0;i<64;i++){
              data[i]=0x00;
            }
            
            if (n==1)
            { 
              getcharacter(19); /*S*/
              placeletter(1);
              getcharacter(20); /*T*/
              placeletter(2);
              getcharacter(1);  /*A*/
              placeletter(3);
              getcharacter(14); /*N*/
              placeletter(4);
              getcharacter(4);  /*D*/
              placeletter(5);
              getcharacter(1);  /*A*/
              placeletter(6);
              getcharacter(18); /*R*/
              placeletter(7);
              getcharacter(4);  /*T*/
              placeletter(8);
            }
            if (n==2)
            {
              getcharacter(19); /*S*/
              placeletter(2);
              getcharacter(3);  /*C*/
              placeletter(3);
              getcharacter(18); /*R*/
              placeletter(4);
              getcharacter(15); /*O*/
              placeletter(5);
              getcharacter(12); /*L*/
              placeletter(6);
              getcharacter(12); /*L*/
              placeletter(7);
            }
            if (n==3)
            {
              getcharacter(2);  /*B*/
              placeletter(2);
              getcharacter(12); /*L*/
              placeletter(3);
              getcharacter(9);  /*I*/
              placeletter(4);
              getcharacter(14); /*N*/
              placeletter(5);
              getcharacter(11); /*K*/
              placeletter(6);
            }
            if (n==4) 
            { 
              getcharacter(23);  /*W*/
              placeletter(2);
              getcharacter(1); /*A*/
              placeletter(3);
              getcharacter(22);  /*V*/
              placeletter(4);
              getcharacter(5); /*E*/
              placeletter(5);
            }
            sendtomax(data);
            for(i=0;i<64;i++){
              data[i]=datacopy[i];
            }
           }
           
           /*wave creates a wave graphic*/
           void makewave(void){

            for(i=0;i<64;i++){
              wavecopy[i]=wave[i];
              wave2copy[i]=wave2[i];
            }
            for(i=0;i<56;i++){
              wave[i]=wavecopy[i+8];
            }
            for(i=0;i<8;i++){
              wave2[i]=wave2copy[i+57];
            }
            for(i=57;i<64;i++)
            {
              wave[i]=wavecopy[i-57];
            }
            for(i=8;i<64;i++){
              wave2[i]=wave2copy[i-8];
            }
            wave[56]=wavecopy[63];
            wave2[7]=wave2copy[1];
            sendtomax(data); 
            for(i=1;i<400;i++){};
            sendtomax(wave);
            for(i=1;i<10;i++){};
            sendtomax(wave2); 
            for(i=1;i<10;i++){};

           }
            
            
            

void main(void) {
  /* put your own code here */
  
    /*SPI CONFIGURATION*/
          SPIBR = 0x00; /* baud rate 8MHz*/
          SPICR1 = 0x50; /*enable SPI, data shift SCK's rising edge, master mode */
          SPICR2 = 0x02; /*disable bidirectional, CLK stops in wait mode */
          WOMS = 0x00;
          
   

           
          /*MAIN CODE*/ 
          
          /*initialization*/       
          DDRT |= 0xFE;      /* 1111111x configure PTT 1,2,3,4,5,6,7 for input */
          DDRT = DDRT-0xFE;
          DDRE = 0x00; /* configure port E for input(only need PTE 1)*/
          initmax();
          for(i=0;i<64;i++){
            data[i]=0x00;
          }
          
          /*MAIN LOOP*/
         while(AlexIsTired==1)
         {
         
          sendtomax(data);
          
          /*check each input and adjust control variables*/
          
          /*display (s1) selected*/
          if ((PTT & 0x02)==0x02)
          {
          
               for(i=1;i<5000;i++);
      
               while((PTT & 0x02)==0x02)
               {
                if(mode==1){
                  sendtomax(data);
                }
                if(mode ==3) {
                  flash();  
                }
                if(mode==2){
                  scroll();
                }
                if(mode==4){
                  makewave();
                }
               }

          
          /*edit (s1) selected*/
          } else if ((PTT & 0x02)!=0x02)
           {
             for(i=1;i<5000;i++);
             
             /* text(s2) selected*/
             if ((PTT & 0x04)==0x04) 
             {
               for(i=1;i<15000;i++);
               
               cursor=1;
               
               while((PTT & 0x02)!=0x02 & (PTT & 0x04)==0x04)
               {
               getcharacter(41);
               placeinverseletter(cursor);
               sendtomax(data);
               for(i=1;i<25000;i++);
               getcharacter(character);
               placeletter(cursor);
               sendtomax(data);
               for(i=1;i<20;i++){
                
               
                if ((PTT & 0x10)!=0x10) 
                { 
                 for(i=1;i<20000;i++);
                 placeletter(cursor);
                 if (cursor == 16){
                   cursor=1;
                 }else cursor++;
                 placeletter(cursor);
                 sendtomax(data);
                 
                 while ((PTT & 0x10)!=0x10);
                } else if ((PTT & 0x20)!=0x20) 
                { 
                 for(i=1;i<20000;i++);
                 
                 placeletter(cursor);
                 if (cursor == 1){
                   cursor=16;
                 }else cursor--;
                 placeletter(cursor);
                 sendtomax(data);
                 
                 while ((PTT & 0x20)!=0x20);
                } else if ((PTT & 0x40)!=0x40)  
                { 
                 for(i=1;i<20000;i++);
                 
                 if (character == 41){
                   character=1;
                 }else character++;
                 getcharacter(character);
                 placeletter(cursor);
                 sendtomax(data);
                 while ((PTT & 0x40)!=0x40);
                } else if ((PTT & 0x80)!=0x80)  
                { 
                 for(i=1;i<20000;i++);
                 
                 if (character == 1){
                   character=41;
                 }else character--;
                 getcharacter(character);
                 placeletter(cursor);
                 sendtomax(data);
                 while ((PTT & 0x80)!=0x80);
                } else if ((PORTE & 0x02)!=0x02) 
                { 
                 for(i=1;i<20000;i++);
                 placeletter(cursor);
                 if (cursor == 16){
                   cursor=1;
                 }else cursor++;
                 placeletter(cursor);
                 sendtomax(data);
                 
                 while ((PORTE & 0x02)!=0x02);
                }else{
                  for(i=1;i<20000;i++);
                }
               }
               }
             }else if ((PTT & 0x04)!=0x04){  /*mode (s2) selected*/
               modepos=1;
               writemode(modepos);

               while((PTT & 0x02)!=0x02 & (PTT & 0x04)!=0x04)
               {
                if ((PTT & 0x10)!=0x10) 
                { 
                 for(i=1;i<25000;i++);
                 if (modepos == 4){
                   modepos=1;
                 }else modepos++;
                 writemode(modepos);
                 while ((PTT & 0x10)!=0x10);
                }

                if ((PTT & 0x20)!=0x20) 
                { 
                 for(i=1;i<25000;i++);
                 if (modepos == 1){
                   modepos=4;
                 }else modepos=modepos-1;
                 writemode(modepos);
                 while ((PTT & 0x20)!=0x20);
                }
                if ((PORTE & 0x02)!=0x02) 
                { 
                 for(i=1;i<25000;i++);
                 mode=modepos;
                 while ((PORTE & 0x02)!=0x02);
                }
               }
             }
           }
  
          
         }
          
         

	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
