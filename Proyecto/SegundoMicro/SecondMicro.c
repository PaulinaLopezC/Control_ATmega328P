/*
 * Joystick.c
 *
 * Created: 10/06/2024 10:39:43 a. m.
 * Author: Samsung
 */
 
   #asm
    .equ __lcd_port=0x05
    .equ __lcd_EN=1
    .equ __lcd_RS=0
    .equ __lcd_D4=2
    .equ __lcd_D5=3
    .equ __lcd_D6=4
    .equ __lcd_D7=5
   #endasm

#include <mega328p.h>
#include <delay.h>
#include <display.h> 
#include "cancioninicio.h"

unsigned char Corazon[8]= {0x00,0x00,0x1B,0x1F,0x1F,0x0E,0x04,0x00};
unsigned char Equis[8]= {0x00,0x0E,0x1F,0x15,0x1F,0x0E,0x00,0x0E};

// -- Inicilizacion Funcion ADC -- //
// ADC Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

// Read the AD conversion result
// Read Voltage=read_adc*(Vref/1024.0)
unsigned int read_adc(unsigned char adc_input)
{
    ADMUX=adc_input | ADC_VREF_TYPE;
    // Delay needed for the stabilization of the ADC input voltage
    delay_us(10);
    // Start the AD conversion
    ADCSRA|=(1<<ADSC);
    // Wait for the AD conversion to complete
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA|=(1<<ADIF);
    return ADCW;
}


unsigned int xpos, ypos;

int thisNoteA, thisNoteB;

void main(void)
{
    // -- Ajustes de ADC -- //
    // ADC initialization
    // ADC Clock frequency: 125.000 kHz
    // ADC Voltage Reference: AVCC pin
    // ADC Auto Trigger Source: ADC Stopped
    // Digital input buffers on ADC0: Off, ADC1: Off, ADC2: On, ADC3: On
    // ADC4: On, ADC5: On
    DIDR0=(0<<ADC5D) | (0<<ADC4D) | (0<<ADC3D) | (0<<ADC2D) | (1<<ADC1D) | (1<<ADC0D);
    ADMUX=ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    ADCSRB=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
        
    // -- Salidas para los LEDs del Joystick -- //
    DDRD = 0x0F;     
    DDRD.5 = 1;
    
    SetupLCD();
    MoveCursor(1,0);
    StringLCD("Vidas Actuales");  
    
    // -- Entradas que indican las vidas -- // 
    PORTC.2 = 1;
    PORTC.3 = 1;
    PORTC.4 = 1;
    PORTC.5 = 1;  
    PORTD.4 = 1;
    CreateChar(0,Equis);
    CreateChar(1,Corazon);

    for (thisNoteA = 0; thisNoteA < notesPacMan * 2; thisNoteA = thisNoteA + 2) {
        // calculates the duration of each note
        dividerPacMan = melodyPacMan[thisNoteA + 1];
        if (dividerPacMan > 0) {
          // regular note, just proceed
          noteDurationPacMan = (wholenotePacMan) / dividerPacMan;
        } else if (dividerPacMan < 0) {
          // dotted notes are represented with negative durations!!
          dividerPacMan = (-1)*dividerPacMan;
          noteDurationPacMan = (wholenotePacMan) / dividerPacMan;
          noteDurationPacMan = 1.5*noteDurationPacMan; // increases the duration in half for dotted notes
        }
        tono(melodyPacMan[thisNoteA]);
        delay_ms(noteDurationPacMan * 0.9);
        noTono();
        delay_ms(noteDurationPacMan * 0.1);
    }    

    while (1){
        xpos = read_adc(1);
        ypos = read_adc(0);    
                                 
        if (xpos<200){
            PORTD.3 = 1;
            PORTD.1 = 0;
        }
        else if(xpos>800){
            PORTD.3 = 0;
            PORTD.1 = 1;
        }
        else{
            PORTD.3 = 0;
            PORTD.1 = 0;   
        }   
        
        if (ypos<200){
            PORTD.0 = 1;
            PORTD.2 = 0;
        }
        else if(ypos>800){
            PORTD.0 = 0;
            PORTD.2 = 1;
        }
        else{
            PORTD.0 = 0;
            PORTD.2 = 0;
        }
        
        
    // Display
        if(PINC.5 == 0){
            MoveCursor(6,1);
            CharLCD(0);    
        }
        else{
            MoveCursor(6,1);
            CharLCD(1);    
        }
        if(PINC.4 == 0){
            MoveCursor(7,1);
            CharLCD(0);    
        }
        else{
            MoveCursor(7,1);
            CharLCD(1);    
        }
        if(PINC.3 == 0){
            MoveCursor(8,1);
            CharLCD(0);    
        }
        else{
            MoveCursor(8,1);
            CharLCD(1);    
        }
        if(PINC.2 == 0){
            MoveCursor(9,1);
            CharLCD(0);    
        }
        else{
            MoveCursor(9,1);
            CharLCD(1);    
        }

        if (PIND.4 == 0){
            for (thisNoteB = 0; thisNoteB < notesShoot * 2; thisNoteB = thisNoteB + 2) {
                // calculates the duration of each note
                dividerShoot = melodyShoot[thisNoteB + 1];
                if (dividerShoot > 0) {
                // regular note, just proceed
                noteDurationShoot = (wholenoteShoot) / dividerShoot;
                } else if (dividerShoot < 0) {
                // dotted notes are represented with negative durations!!
                dividerShoot = (-1)*dividerShoot;
                noteDurationShoot = (wholenoteShoot) / dividerShoot;
                noteDurationShoot = 1.5*noteDurationShoot; // increases the duration in half for dotted notes
                }
                tono(melodyShoot[thisNoteB]);
                delay_ms(noteDurationShoot * 0.9);
                noTono();
                delay_ms(noteDurationShoot * 0.1);
            } 
        }
        
    }
}
