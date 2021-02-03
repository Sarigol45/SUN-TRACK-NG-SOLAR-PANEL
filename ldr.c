
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
   set_tris_d(0b00000000);//d portu çýkýþ olarak tanýmlanýyor
   set_tris_a(0b11111111);//a portu giriþ olarak tanýmlanýyor
   setup_adc_ports(AN0_AN1_AN2_AN3_AN4_AN5);  //Adc pin tanýmlamasý
   int16 servod = 60; //servod konum deðiþkeni
   int16 servoy = 45;//servoy konum deðiþkeni
   int balance = 200;//Ldrlerden alýnan analog deðer arasýnda oluþan fark için
   unsigned int16 ldr0;//ldr den alýnan analog deðer için oluþturulan deðiþken
   unsigned int16 ldr1;
   unsigned int16 ldr2;
   unsigned int16 ldr3;
 
   while(TRUE)
   {        
      set_adc_channel(0); //Adc nin 0. 'A0' kanalýný seçiyoruz 
      delay_ms(2);        //Adc kanalý seçildikten sonra bekleme yapýlmasý daha stabil bir deðer okunmasý içindir.
      ldr0 = read_adc()/64; //Adc den alýnan analog deðer ldr0 deðiþkenine eþitleniyor
      set_adc_channel(1);
      delay_ms(2);
      ldr1 = read_adc()/64; 
      set_adc_channel(2);
      delay_ms(2);
      ldr2 = read_adc()/64;
      set_adc_channel(3);
      delay_ms(2);
      ldr3 = read_adc()/64; 
      if(ldr0+balance<ldr3)//servo konumu için ldrlerden alýnan deðer karþýlaþtýrýlýyor 
      {
         servod--;         //servod deðerini 1 azalt
         if(servod<1)servod=1;//servod deðerinin 1 den aþþaðý inmemesi için
      }
      else if (ldr0>ldr3+balance)//servo konumu için ldrlerden alýnan deðer karþýlaþtýrýlýyor
      {
         servod++;         //servod deðerini 1 arttýr
         if(servod>170)servod=170;//servod deðerinin 170 den yukarý çýkmamasý için
      }
      if(servod>60)//servod 60 tan büyük ise servoy yi kontrol eden ldrlerin konum yönleri fatklý olduðu için aþþaðýdaki kod çalýþýr
      //yani ldr yönleri ters çevrilmiþ olur
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
      else//servod 60 tan küçük ise servoy yi kontrol eden ldrlerin konum yönleri fatklý olduðu için aþþaðýdaki kod çalýþýr
          //yani ldr yönleri ters çevrilmiþ olur
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
      dikey(servod);//dikey servonun çalýþmasý için hesaplanan deðer gönderilir
      yatay(servoy);//yatay servonun çalýþmasý için hesaplanan deðer gönderilir
      delay_ms(18);//18ms gecikme
   }
}

