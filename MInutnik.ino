#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>
LiquidCrystal_I2C lcd(0x27,16,2);
RotaryEncoder encoder(A2, A3);

volatile unsigned int minuty=0;
volatile unsigned long czas=0;
volatile int ozon=0;
volatile unsigned long czas_start=0;
volatile unsigned long czas_pozostalo=0;
const int ozon_pin=13;
const int przycisk=2;

void setup()
{
  lcd.init();                     
  lcd.backlight();
  pinMode(przycisk, INPUT_PULLUP); 
  pinMode(ozon_pin, OUTPUT);
  digitalWrite(ozon_pin, LOW);
  attachInterrupt(digitalPinToInterrupt(przycisk), przycisk_przerwanie, RISING);
}

void update_lcd(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Poz:");
  lcd.setCursor(4,0);
  if (czas_start > 0 && minuty > 0){
    lcd.print((minuty - ((millis()/60000) - (czas_start/60000)))-1);
    lcd.setCursor(6,0);
    
    if (((minuty - ((millis()/60000) - (czas_start/60000)))-1)>9){
     lcd.setCursor(7,0);
    }
    
    lcd.print((((minuty*60) - ((millis()/1000) - (czas_start/1000))))%60);
  }
  else {
    lcd.print("0");
  }
  lcd.setCursor(0,1);
  lcd.print("Czas: ");
  lcd.setCursor(6,1);
  lcd.print(minuty);
  lcd.setCursor(10,1);
  lcd.print("minut");
  
  if (ozon == 0) {
    lcd.setCursor(15,0);
    lcd.print("Z");
  }
  else{
    lcd.setCursor(15,0);
    lcd.print("A");
  }
  czas = millis();
}

void przycisk_przerwanie()
{
    //detachInterrupt(przycisk_przerwanie);
    //delay(5000);
    digitalWrite(ozon_pin, HIGH);
    ozon=1;
    czas_start = millis();
} 

void loop(){
  encoder.tick();
  int minuta_enkoder = encoder.getPosition();

  if (minuta_enkoder < 0){
    encoder.setPosition(0);
    minuta_enkoder = 0;
  }
  
  if (minuty != minuta_enkoder) {
    minuty = minuta_enkoder;
    update_lcd();
  }
  
  if ((millis() - czas) >= 1000 ){
    update_lcd();
  }

  if ((millis() - czas_start >= minuty *60000)){
    digitalWrite(ozon_pin, LOW);
    ozon = 0;
    czas_start = 0;
    czas_pozostalo = 0;
    //lcd.noBacklight();
  }
}
