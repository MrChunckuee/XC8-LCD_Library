/*******************************************************************************
 *
 *      LCD - Creacion de numeros grandes
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sánchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Probando la creacion de numeros grandes utilizando caracteres
 *                  personalizados en la CGRAM de la LCD.
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      21/09/2025      Creación del ejemplo
 ******************************************************************************/
#include <xc.h>
#include <stdbool.h>
#include "FUSES_Config.h"
#include "LiquidCrystalDisplay.h" 
#include "LCD_BIGDigits.h"

#define _XTAL_FREQ  16000000

// Conexiones de la LCD
LCD_t LCD = {
    .TRIS = &TRISB,
    .PORT = &PORTB,
    .RS   = 0,  // Pin RB0 para RS
    .EN   = 1,  // Pin RB1 para EN
    .D4   = 4,  // Pin RB2 para D4
    .D5   = 5,  // Pin RB3 para D5
    .D6   = 6,  // Pin RB4 para D6
    .D7   = 7   // Pin RB5 para D7
};

uint8_t counter = 0;
uint16_t RTCC_Count = 0;
bool LCDUpdate = false;

#define TIME_DELAY 3000

void MCU_Init(void);
void RTCC_Init(void);

void __interrupt() INTERRUPT_InterruptManagerHigh (void){
    if(INTCONbits.TMR0IE && INTCONbits.TMR0IF){
        ++RTCC_Count;
        if (RTCC_Count >= TIME_DELAY){
            LCDUpdate = true;
            RTCC_Count = 0;
        }
        TMR0H = 0xF8;
        TMR0L = 0x30;
        INTCONbits.TMR0IF = 0;
        return;
    }
}

void main(void) {
    MCU_Init();

    if (!LCD_Init(LCD)){ } // LCD Init

    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_puts("MrChunckuee!!");
    LCD_SetCursor(1, 4);
    LCD_puts("BIG Digits test");
    LCD_SetCursor(2, 1);
    LCD_puts("mrchunckuee.blog-");
    LCD_SetCursor(3, 11);
    LCD_puts("spot.com");
    __delay_ms(1000);
    
    while (1) {
        if(LCDUpdate){
            counter++;
            if(counter>3) counter = 1;
            LCD_Clear();
            switch(counter){ //Select and Set FONT
                case 1:
                    LCD_SetFont(FONT_TRON_BOLD);
                    break;
                case 2:
                    LCD_SetFont(FONT_SQUARE_FOUR);
                    break;
                case 3:
                    LCD_SetFont(FONT_BLOCKS);
                    break;
                default:
                    break;
            }
            //Update LCD
            LCD_DrawBigDigit(0, 0, 1);  // draw digit 0
            LCD_DrawBigDigit(1, 0, 4);  // draw digit 1
            LCD_DrawBigDigit(2, 0, 7);  // draw digit 2
            LCD_DrawBigDigit(3, 0, 10); // draw digit 3
            LCD_DrawBigDigit(4, 0, 13); // draw digit 4
            LCD_DrawBigDigit(5, 2, 5);  // draw digit 5
            LCD_DrawBigDigit(6, 2, 8);  // draw digit 6
            LCD_DrawBigDigit(7, 2, 11); // draw digit 7
            LCD_DrawBigDigit(8, 2, 14); // draw digit 8
            LCD_DrawBigDigit(9, 2, 17); // draw digit 9
            LCDUpdate = false;
        }
    }
    
}

void MCU_Init(void){
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    //Configura oscilador interno a 16MHz
    OSCCONbits.IRCF=0b111; //El ciclo de instruccion es de 0.25 us
    RTCC_Init();
}

void RTCC_Init(void){
    //Configuración del TMR0 = 1mS
    T0CONbits.TMR0ON = 1; //TMR0 On
    T0CONbits.T08BIT = 0; //16 bits
    T0CONbits.T0CS = 0;
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0b000; // Prescaler = 2
    TMR0H = 0xF8;
    TMR0L = 0x30;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCON2bits.TMR0IP = 1;
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
}