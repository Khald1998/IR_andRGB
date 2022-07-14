
#include "mbed.h"
#include <string>

 
#define MAXPULSE    10000                               // the maximum pulse we'll listen for - 10 milliseconds 
#define NUMPULSES    200                                // max IR pulse pairs to sample
#define RESOLUTION     1                                // time between IR measurements ~1uS
#define ld1     p20
#define ld2     p19
#define ld3     p18
// we will store up to 200 pulse pairs (this is -a lot-)
uint16_t pulses[200][2];                                // pair is high and low pulse
uint16_t currentpulse = 0;                              // index for pulses we're storing
uint32_t irCode = 0;
DigitalIn IRpin (p26);
DigitalOut ledR(p20);                                   // Listen to IR receiver on Trinket/Gemma pin D2
DigitalOut ledG(p19);
DigitalOut ledB(p18); 

int listenForIR();                                  // IR receive code
void printcode();                                   // Print IR code in HEX
string translations(int IRCode);
void IRFinal ();

int main()
{
    ledR = 0;
    ledG = 0;
    ledB = 0;
    string temporary2 = "";
    printf("Ready to decode IR!\n");
    while (1) {
        
        IRFinal ();
        temporary2 = translations(irCode);
        if (temporary2 == "1") 
        ledR = 1^ledR;
        else if(temporary2 == "2") 
        ledG = 1^ledG;
        else if(temporary2 == "3") 
        ledB = 1^ledB;
        printcode();  
        //printf("Translation: %s\n",temporary2);                                  // Print IR code 
        printf("R: %d\n",ledR.read());
        printf("G: %d\n",ledG.read());
        printf("B: %d\n",ledB.read());
        
        
        
        wait_ms(100);
 
    }  //end of main loop
}
 
 
int listenForIR(){                                  // IR receive code
    currentpulse = 0;
    while (1) {
        unsigned int highpulse, lowpulse;               // temporary storage timing
        highpulse = lowpulse = 0;                       // start out with no pulse length
 
        while (IRpin==1) {                              // got a high pulse
            highpulse++;
            wait_us(RESOLUTION);
            if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
                return currentpulse;
            }
        }
        pulses[currentpulse][0] = highpulse;
 
        while (IRpin==0) {                              // got a low pulse
            lowpulse++;
            wait_us(RESOLUTION);
            if (((lowpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
                return currentpulse;
            }
        }
        pulses[currentpulse][1] = lowpulse;
        currentpulse++;
    }
}
void printcode()
{
    string temp = translations(irCode);
    uint16_t half;
    half=irCode>>16;                                    // Get first 16 bits of code
    printf("0x%x",half);                             // Print upper 16 bits in hex
    printf("%x\n",(irCode & 0xFFFF));                // print lower 16 bits in hex
    printf("Translation: %s\n",temp);
}
string translations(int IRCode)
{ 
    string temporary = "";
   switch(IRCode) {
      case 0xc03fe897 :
         temporary = "OFF";
         //ledR = ~ledR; 
         
         break;
      case 0xc03fcc33 :
         temporary = "1";
         break;
      case 0xc03fd8a7 :
         temporary = "VOL+";
         break;
      case 0xc03ff887 :
         temporary = "fun/stop";
         break;
      case 0xc03fc8b7 :
         temporary = "<<";
         break;
      case 0xc03fc0bf :
         temporary = ">|";
         break;
      case 0xc03ff08f :
         temporary = ">>";
         break;
      case 0xc03ff807 :
         temporary = "V";
         break;
      case 0xc03fea15 :
         temporary = "VOL-";
         break;
      case 0xc03fe41b :
         temporary = "^";
         break;
      case 0xc03fda25 :
         temporary = "0";
         break;
      case 0xc03fc639 :
         temporary = "2";
         break;
      case 0xc03fdea1 :
         temporary = "3";
         break;                                                                                                   
      case 0xc03fc43b :
         temporary = "4";
         break;                                                                                                   
      case 0xc03fce31 :
         temporary = "5";
         break;                                                                                                   
      case 0xc03fd6a9 :
         temporary = "6";
         break;                                                                                                   
      case 0xc03fd0af :
         temporary = "7";
         break;                                                                                                   
      case 0xc03fd2ad :
         temporary = "8";
         break;                                                                                                   
      case 0xc03fd4ab :
         temporary = "9";
         break;                                                                                                   
      case 0xc03fe619 :
         temporary = "EQ";
         break;
      case 0xc03fec13 :
         temporary = "ST/RESET";
         break;         
      default :
         temporary = "???";
   }
   return temporary;
}  
void IRFinal ()
{
        int numpulse=listenForIR();                     // Wait for an IR Code
 
        // Process the pulses to get a single number representing code
        for (int i = 0; i < 32; i++) {
            irCode=irCode<<1;
            if((pulses[i][0] * RESOLUTION)>0&&(pulses[i][0] * RESOLUTION)<500) {
                irCode|=0;
            } else {
                irCode|=1;
            }
        }
}
 