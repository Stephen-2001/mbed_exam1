#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;

DigitalIn upbutton(D3); // right hand side
DigitalIn downbutton(D9);   // middle
DigitalIn confirm(D5);    // left hand side

AnalogOut aout(PA_4);
AnalogIn ain(D11);

double ADCdata[480];

int main(){
	uLCD.text_width(1);
	uLCD.text_height(1);
	uLCD.color(RED);

    float display_slew_rate = 0.125;
    float slew_rate = 0.125;
    double T = 240.0;
    double piece = 0.0;
    int sample = 20;
    int index = 0;
    int count = 0;

    while (1) {
        uLCD.locate(1, 2);  
        uLCD.printf("%4f %4f\n", display_slew_rate, slew_rate);
        if (upbutton) {
            if (display_slew_rate == 0.125) {
                display_slew_rate = 0.25;
            }
            else if (display_slew_rate == 0.25) {
                display_slew_rate = 0.5;
            }
            else if (display_slew_rate == 0.5) {
                display_slew_rate = 1.0;
            }
            else {
                display_slew_rate = 1.0;
            }
            uLCD.locate(1, 2);  
            uLCD.printf("%4f %4f\n", display_slew_rate, slew_rate);
        }
        if (downbutton) {
            if (display_slew_rate == 1.0) {
                display_slew_rate = 0.5;
            }
            else if (display_slew_rate == 0.5) {
                display_slew_rate = 0.25;
            }
            else if (display_slew_rate == 0.25) {
                display_slew_rate = 0.125;
            }
            else {
                display_slew_rate = 0.125;
            }
            uLCD.locate(1, 2);  
            uLCD.printf("%4f %4f\n", display_slew_rate, slew_rate);
        }
        if (confirm) {
            slew_rate = display_slew_rate;
            piece = 0;
            sample = int(100.0 / (T/1000.0) * 480.0);
            uLCD.locate(1, 2);  
            uLCD.printf("%4f %4f\n", display_slew_rate, slew_rate);
            for(int i=0; i<480; i++){
				printf("%.3f\r\n", ADCdata[i]);
			}
        }

        if (piece >= T) {
            piece = 0;
        }
        else if (piece <= T/3.0) {
            if (piece <= 80.0*slew_rate) {
                aout = piece / (80.0*slew_rate);
            }
            else {
                aout = 1.0;
            }
        }
        else if ((piece > T/3.0) && (piece <= T * 2.0 / 3.0)) {
            aout = 1.0;
        }
        else if ((piece > T * 2.0 / 3.0) && (piece < T)) {
            if (piece >= (T - 80.0*slew_rate)) {
                aout = 1.0 - (piece - T + 80.0*slew_rate) / (80.0*slew_rate);
            }
            else {
                aout = 1.0;
            }
        }
        aout = aout * 3.0 / 3.3;
        piece += (T / 240.0);

        if(index >= 480){
			count = 0;
			index = 0;
		}
		else if(index<480 && (count % sample == 0)){
			ADCdata[index] = ain;
			index++;
		}
		count++;
    }
}
    
