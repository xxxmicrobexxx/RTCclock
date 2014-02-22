#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>

//4 Digit Arduino Counter
#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define F 7
#define G 8
#define dot 9
// Pins driving common anodes
#define CA1 13
#define CA2 12
#define CA3 11
#define CA4 10
// Pins for A B C D E F G, in sequence
const int segs[7] = {2, 3, 4, 5, 6, 7, 8};
// Segments that make each number
const byte numbers[10] = {0b1000000,0b1111001,0b0100100,0b0110000,0b0011001,0b0010010,0b0000010,0b1111000,0b0000000,0b0010000};
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

  tmElements_t tm;
void setup() {
  
  bool parse=false;
  bool config=false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {

int digit1;
int digit2;
int digit3;
int digit4;
if (RTC.read(tm)) {
    if(tm.Hour > 10){
      digit1 = 1;
      digit2 = tm.Hour - 10;
    }
    else{
      digit1 = 0;
      digit2 = tm.Hour;
    }
    
   if(tm.Minute > 10){
      digit4 = tm.Minute % 10; 
      digit3 = (tm.Minute - digit4)/10;
          
    }
    else{
      digit3 = 0;
      digit4 = tm.Minute;
    }
  

       lightDigit1(numbers[digit1]);
        delay(5);
       lightDigit2(numbers[digit2]);
       delay(5);
        lightDigit3(numbers[digit3]);
       delay(5);
       lightDigit4(numbers[digit4]);
       delay(5);

}
else{
  //if we don't have the time display horizontal lines
  digitalWrite(CA1, HIGH);
  digitalWrite(CA2, HIGH);
  digitalWrite(CA3, HIGH);
  digitalWrite(CA4, HIGH);

  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, LOW);
}

}  


 void lightDigit1(byte number)      {
  digitalWrite(CA1, LOW);
  digitalWrite(CA2, LOW);
  digitalWrite(CA3, LOW);
  digitalWrite(CA4, HIGH);
  lightSegments(number);
 }
void lightDigit2(byte number) {
  digitalWrite(CA1, LOW);
  digitalWrite(CA2, LOW);
  digitalWrite(CA3, HIGH);
  digitalWrite(CA4, LOW);
  lightSegments(number);
}
void lightDigit3(byte number) {
  digitalWrite(CA1, LOW);
  digitalWrite(CA2, HIGH);
  digitalWrite(CA3, LOW);
  digitalWrite(CA4, LOW);
  lightSegments(number);
}
void lightDigit4(byte number){
  digitalWrite(CA1, HIGH);
  digitalWrite(CA2, LOW);
  digitalWrite(CA3, LOW);
  digitalWrite(CA4, LOW);
  lightSegments(number);

}
void lightSegments(byte number){
  for (int i = 0;i < 7;i++) {
    int bit = bitRead(number, i);
    digitalWrite(segs[i], bit);
  }
}


bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}
bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
