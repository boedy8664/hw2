#include "mbed.h"
#include "uLCD_4DGL.h"
using namespace std::chrono;


Timer debounce;
uLCD_4DGL uLCD(D1, D0, D2);


// Initialize a pins to perform analog and digital output functions

// Adjust analog output pin name to your board spec.


AnalogOut  aout(PA_4);

//DigitalOut dout(LED1);
InterruptIn sw2(PB_1);
InterruptIn sw1(PB_4);
InterruptIn sw0(PA_3);
EventQueue queue(64 * EVENTS_EVENT_SIZE);
Thread t;

int flag = 0;
int c = 0;

void Confirm_print() {
   if (duration_cast<milliseconds>(debounce.elapsed_time()).count() > 500) {
      uLCD.printf("\nConfirm !\n");
      debounce.reset();
   }
}

void Confirm_Freq() {
   c = 1;
   queue.call(Confirm_print);
}

void uLCD_print() {
   if (flag == -1) {
      uLCD.cls();
      uLCD.printf("\nThe frequency is : 100 Hz\n");
   }
   if (flag == 0) {
      uLCD.cls();
      uLCD.printf("\nThe frequency is : 200 Hz\n");
   }
   if (flag == 1) {
      uLCD.cls();
      uLCD.printf("\nThe frequency is : 1000 Hz\n");
   }
}

void Trig_up() {
   if (duration_cast<milliseconds>(debounce.elapsed_time()).count() > 500) {
      if (flag < 1) flag = flag + 1;
      else flag = 1;

      c = 0;
      queue.call(uLCD_print);
      debounce.reset();
   }
}

void Trig_down() {
   if (duration_cast<milliseconds>(debounce.elapsed_time()).count() > 500) {
      if (flag > -1) flag = flag - 1;
      else flag = -1;
      c = 0;
      queue.call(uLCD_print);
      debounce.reset();
   }
}

int main(void)

{
   debounce.start();

   t.start(callback(&queue, &EventQueue::dispatch_forever));

   sw2.rise(Trig_up);
   sw1.rise(Trig_down);
   sw0.rise(Confirm_Freq);


   while (1) {

      // change the voltage on the digital output pin by 0.1 * VCC

      //  and print what the measured voltage should be (assuming VCC = 3.3v)

      if ((flag == -1) && c) {
         for (float i = 0.0f; i < 0.9; i += 0.02f) {

               aout = i;

               wait_us(10);
         }

         for (float a = 0.9; a > 0.0f; a -= 0.0023f) {

            aout = a;

            wait_us(10);

         }
      }

      if ((flag == 0) && c) {
         for (float b = 0.0f; b < 0.9; b+= 0.0127f) {
            aout = b;
         }
         for (float a = 0.9; a > 0.0f; a -= 0.00143f) {

            aout = a;
         }
      }

      /*for (float b = 0.0f; b < 0.9; b+= 0.032f) {
         aout = b;
      }
      for (float a = 0.9; a > 0.0f; a -= 0.0035f) {

            aout = a;

      }*/

      if((flag == 1) && c) {
         for (float b = 0.0f; b < 0.9; b+= 0.065f) {
            aout = b;
         }
         for (float a = 0.9; a > 0.0f; a -= 0.0067f) {
            aout = a;
         }
      }
   }
}