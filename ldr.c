
#include <16F877A.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)
#use rs232(baud=9600,parity=N,xmit=PIN_c6,bits=8)
void dikey(int16 servo)//Servo fonksiyonu
{
   servo=servo*12+500;           
   output_high(pin_d0);   
   delay_us(servo);       
   output_low(pin_d0);
}
void yatay(int16 servo)//Servo fonksiyonu
{
   servo=servo*12+500;
   output_high(pin_d1);   
   delay_us(servo);       
   output_low(pin_d1);
}
void main()
{
   set_tris_d(0b00000000);//d portu ��k�� olarak tan�mlan�yor
   set_tris_a(0b11111111);//a portu giri� olarak tan�mlan�yor
   setup_adc_ports(AN0_AN1_AN2_AN3_AN4_AN5);  //Adc pin tan�mlamas�
   int16 servod = 60; //servod konum de�i�keni
   int16 servoy = 45;//servoy konum de�i�keni
   int balance = 200;//Ldrlerden al�nan analog de�er aras�nda olu�an fark i�in
   unsigned int16 ldr0;//ldr den al�nan analog de�er i�in olu�turulan de�i�ken
   unsigned int16 ldr1;
   unsigned int16 ldr2;
   unsigned int16 ldr3;
 
   while(TRUE)
   {        
      set_adc_channel(0); //Adc nin 0. 'A0' kanal�n� se�iyoruz 
      delay_ms(2);        //Adc kanal� se�ildikten sonra bekleme yap�lmas� daha stabil bir de�er okunmas� i�indir.
      ldr0 = read_adc()/64; //Adc den al�nan analog de�er ldr0 de�i�kenine e�itleniyor
      set_adc_channel(1);
      delay_ms(2);
      ldr1 = read_adc()/64; 
      set_adc_channel(2);
      delay_ms(2);
      ldr2 = read_adc()/64;
      set_adc_channel(3);
      delay_ms(2);
      ldr3 = read_adc()/64; 
      if(ldr0+balance<ldr3)//servo konumu i�in ldrlerden al�nan de�er kar��la�t�r�l�yor 
      {
         servod--;         //servod de�erini 1 azalt
         if(servod<1)servod=1;//servod de�erinin 1 den a��a�� inmemesi i�in
      }
      else if (ldr0>ldr3+balance)//servo konumu i�in ldrlerden al�nan de�er kar��la�t�r�l�yor
      {
         servod++;         //servod de�erini 1 artt�r
         if(servod>170)servod=170;//servod de�erinin 170 den yukar� ��kmamas� i�in
      }
      if(servod>60)//servod 60 tan b�y�k ise servoy yi kontrol eden ldrlerin konum y�nleri fatkl� oldu�u i�in a��a��daki kod �al���r
      //yani ldr y�nleri ters �evrilmi� olur
      {
         if(ldr1+balance<ldr2)
         {
            servoy--;
            if(servoy<1)servoy=1;
         }
         else if (ldr1>ldr2+balance)
         {
            servoy++;
            if(servoy>160)servoy=160;
         }
         }
      else//servod 60 tan k���k ise servoy yi kontrol eden ldrlerin konum y�nleri fatkl� oldu�u i�in a��a��daki kod �al���r
          //yani ldr y�nleri ters �evrilmi� olur
      {
         if(ldr2+200<ldr1)
         {
            servoy--;
            if(servoy<1)servoy=1;
         }
         else if (ldr2>ldr1+200)
         {
            servoy++;
            if(servoy>160)servoy=160;
         }
      }
      dikey(servod);//dikey servonun �al��mas� i�in hesaplanan de�er g�nderilir
      yatay(servoy);//yatay servonun �al��mas� i�in hesaplanan de�er g�nderilir
      delay_ms(18);//18ms gecikme
   }
}

