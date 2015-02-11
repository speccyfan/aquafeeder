#include <Stepper.h>
#include <EEPROM.h>
#include <MenuSystem.h>
#include <LiquidCrystal595.h>
#include <DS1302.h>
#include <OneWire.h>

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)
#define DAYS_PER_WEEK (7UL)
#define SECS_PER_WEEK (SECS_PER_DAY * DAYS_PER_WEEK)
#define SECS_PER_YEAR (SECS_PER_WEEK * 52UL)
#define SECS_YR_2000  (946684800UL) // the time at the start of y2k
#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
static  const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts

// Инициализация пинов часов
DS1302 rtc(2, 3, 4);

//датчик температуры
OneWire  ds(5);

//Инициализация пинов экрана
LiquidCrystal595 lcd(6,7,8);

//пины шагового двигателя
const int Step1Pin = 9;
const int Step2Pin = 10;
const int Step3Pin = 11;
const int Step4Pin = 12;

Stepper motor(100, Step1Pin, Step2Pin, Step3Pin, Step4Pin);


int key=0;
int show_time=1;
int CurS=0;  //текущий настраиваемый параметр (час/мин/сек)
long timestamp;
long timestamp_eeprom;
char k;
int ArrT[3]; //time
int ArrD[3]; //date
Time t;
int counter1 = 100;
int counter2 = 1;
int counter3 = 300;
int counter4 = 30; //счетчик выключения подсветки
int temp1=0;
int ttemp=0;
byte thermostat=0;
byte i;
byte data[12];
byte present = 0;
float celsius = 0;


//menu
MenuSystem ms;
Menu mm("------Menu------");
MenuItem mm_mi1("Feed Now");
MenuItem mm_mi2("Time Setup");
MenuItem mm_mi3("Date Setup");
MenuItem mm_mi4("Thermostat Setup");

void setup()
{
//кнопки  
  pinMode (A0, INPUT);

//шаговик
  pinMode(Step1Pin, OUTPUT);
  pinMode(Step2Pin, OUTPUT);
  pinMode(Step3Pin, OUTPUT);
  pinMode(Step4Pin, OUTPUT);
  motor.setSpeed(40);

 // Запуск часов
  rtc.halt(false);
  rtc.writeProtect(false);

//читаем установки температуры из EEPROM
  int TEMP_EEPROM = 0;
  TEMP_EEPROM = EEPROM_int_read(4);
  if ((TEMP_EEPROM >= 0) && (TEMP_EEPROM <= 100)){
    temp1 = TEMP_EEPROM; 
  }

// Запуск экрана с указанием количества символов и строк

  lcd.setLED2Pin(HIGH);
  lcd.begin(16, 2);
  lcd.clear();

  mm.add_item(&mm_mi1, &feed_now_selected);
  mm.add_item(&mm_mi2, &time_setup_selected);
  mm.add_item(&mm_mi3, &date_setup_selected);
  mm.add_item(&mm_mi4, &thermostat_setup_selected);
//  mm.add_menu(&mu1);
//  mu1.add_item(&mu1_mi1, &on_item3_selected);
  ms.set_root_menu(&mm);
}


void loop(){
  //читаем температуру
  byte addr[8];
  if (ds.search(addr)) {
       ds.reset();
       ds.select(addr);
       ds.write(0x44, 1);        // start conversion, with parasite power on at the end
       delay(1);//1000
       present = ds.reset();
       ds.select(addr);    
       ds.write(0xBE);         // Read Scratchpad
          
       for ( i = 0; i < 9; i++) {           // we need 9 bytes
          data[i] = ds.read();
          }
        int16_t raw = (data[1] << 8) | data[0];
        byte cfg = (data[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
         if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
         else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
         else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
         // default is 12 bit resolution, 750 ms conversion time
         celsius = (float)raw / 16.0;
        }

  
  if (show_time == 1){
    //lcd.clear();
      counter2--;
      if (counter2==0){
        if (counter4>0){
          counter4--;
          lcd.setLED2Pin(HIGH);
        }else{
              lcd.setLED2Pin(LOW);
        }
        
        counter2=10;
        lcd.setCursor(0, 0); // Устанавливаем курсор для печати времени в верхней строчке
        lcd.print(rtc.getTimeStr()); // Печатаем время
        lcd.setCursor(9,0); 
          
        lcd.print(celsius);
        lcd.print((char)223); //celsius simvol
//          lcd.print("C");
        lcd.setCursor(3, 1);

        //timestamp = getEpochTime(rtc.getTime());  
        //lcd.print(timestamp);
        lcd.print(rtc.getDateStr()); // Печатаем дату
      }
  }

//проверяем раз в ... секунд
  counter1--;
  if (counter1==0){
    counter1=100;

  //thermostat
  if (temp1>0 && celsius >0){ //если в настройках не 0 и датчик выдает больше 0
      if (thermostat == 0){
        if (celsius<temp1-1){
          thermostat = 1;
          lcd.setLED1Pin(LOW); //вкл
        }
      }else{
        if (celsius>temp1+1){
          thermostat = 0;
          lcd.setLED1Pin(HIGH); //выкл
        }
      }
  }

  //feeder
  t = rtc.getTime();
  int H = t.hour;
  if ((H == 0) || (H == 8) || (H == 16)){
  
      timestamp_eeprom=EEPROMReadlong(0); 
      timestamp = getEpochTime(t);
      
      if (timestamp_eeprom+3600 < timestamp){
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("Feed Now!");
        EEPROMWritelong(0,timestamp);
        motor.step(100);
        motor_stop();
        lcd.clear();
      }
    }
  }  
  
 
  if (keyboard()!=0){
    show_time=0;
    }

  menu();
  delay (100); // Пауза и все по новой!
}


void motor_stop(){
  digitalWrite(Step1Pin,0);
  digitalWrite(Step2Pin,0);
  digitalWrite(Step3Pin,0);
  digitalWrite(Step4Pin,0);
}
 
//display menu function 
void displayMenu() {
  lcd.clear();
  lcd.setCursor(0,0);
  // Display the menu
  Menu const* cp_menu = ms.get_current_menu();

  //lcd.print("Current menu name: ");
  lcd.print(cp_menu->get_name());
  lcd.setCursor(0,1);
  lcd.print(cp_menu->get_selected()->get_name());
  delay(100);
}


char keyboard(){
  key = analogRead (0);

//debug
  //lcd.setCursor(10, 1);
  //lcd.print(key);
  
  //Esc/Cancel
  if ((key > 350) && (key < 360)){
    return 'c';
  }
  //up
  if ((key > 210) && (key < 230)){
    return 'u';
  }
  //right
  if ((key > 560) && (key < 590)){
    return 'r';
  }
  //down
  if ((key > 510) && (key < 540)){
    return 'd';
  }
  //left
  if ((key > 440) && (key < 470)){
    return 'l';
  }
  //Enter
  if (key < 20){
    return 'e';
  }
  return 0;
}

void menu(){
  k = keyboard();
  if (k == 0){
    counter3--;
    if (counter3 == 0){
       counter3 = 300;
       k='c';
    }
  }else{
    lcd.setLED2Pin(HIGH);
    counter4=30;
  }
  
  switch (k){
    case 'c':
      ms.back();
      lcd.clear();
      show_time=1;
      break;
    case 'u':
      ms.prev();
      displayMenu();
      break;

    case 'd':
      ms.next();
      displayMenu();
      break;
    case 'e':
      ms.select();
      displayMenu();
      break;
 }
}

//feed now
void feed_now_selected(MenuItem* p_menu_item)
{
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Feed Now!");
  motor.step(100);
  motor_stop();
  lcd.clear();
}

void thermostat_setup_selected(MenuItem* p_menu_item)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperature:");
  ttemp=temp1;
  delay(300);
  while (k!='c'){
    delay(1);
    k=keyboard();
    
    if (k=='e'){
     temp1=ttemp;
     EEPROM_int_write(4,temp1);
      k='c'; //выход
    }
    
    if (k=='u'){incrTemp();}
    if (k=='d'){decrTemp();}
      lcd.setCursor(0,1);
      lcd.print(ttemp);
      lcd.print(" ");
  }
}


//функция увеличивает temp1 
void incrTemp(){
  if (ttemp<30){
    ttemp++;
  }
  delay(200);
}
void decrTemp(){
  if (ttemp>0){
    ttemp--;
  }
  delay(200);
}

//Date setup
void date_setup_selected(MenuItem* p_menu_item)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Date setup");
  delay (300);
  int Flash=0; //или рисуем цифры или пробелы, мерцание выбранного элемента
  int FC=0;    //счетчик для flash
  t = rtc.getTime();
  ArrD[0]=t.date;
  ArrD[1]=t.mon;
  ArrD[2]=t.year;
  
  while (k!='c'){
    delay(1);
    k=keyboard();
    //flash
    FC++;
    if (FC == 30){
      if (Flash == 0){
        Flash=1;
      }else{
        Flash=0;
      }
    FC=0;
    }
    if (k!=0){
      Flash=1;
    }

    //end flash
    lcd.setCursor(0,1);    
    flashPrintDate(Flash,0);
    lcd.print('.');
    flashPrintDate(Flash,1);
    lcd.print('.');
    flashPrintDate(Flash,2);
    
    if (k=='u'){incrD();}
    if (k=='d'){decrD();}

    if (k=='r'){
      if (CurS <2){
        CurS++;
      }else{
        CurS=0;
      }
     delay(300);
    }
    if (k=='l'){
      if (CurS>0){
        CurS--;
      }else{
        CurS=2;
      }
     delay(300);
    }
    if (k=='e'){
      rtc.setDate(ArrD[0],ArrD[1],ArrD[2]); // Дата.
      k='c'; //выход
    }
  }  
}

//Time setup
void time_setup_selected(MenuItem* p_menu_item)
{

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time setup");
  delay (300);
  int Flash=0; //или рисуем цифры или пробелы, мерцание выбранного элемента
  int FC=0;    //счетчик для flash

  t = rtc.getTime();
  ArrT[0]=t.hour;
  ArrT[1]=t.min;
  ArrT[2]=t.sec;
  
  while (k!='c'){
    delay(1);
    k=keyboard();
    
    //flash
    FC++;
    if (FC == 30){
      if (Flash == 0){
        Flash=1;
      }else{
        Flash=0;
      }
    FC=0;
    }
    if (k!=0){
      Flash=1;
    }
    //end flash
    
    lcd.setCursor(0,1);    
    flashPrint(Flash,0);
    lcd.print(':');
    flashPrint(Flash,1);
    lcd.print(':');
    flashPrint(Flash,2);

    //меняем значение
    if (k=='u'){incr();}
    if (k=='d'){decr();}
    if (k=='r'){
      if (CurS <2){
        CurS++;
      }else{
        CurS=0;
      }
     delay(300);
    }
    if (k=='l'){
      if (CurS>0){
        CurS--;
      }else{
        CurS=2;
      }
     delay(300);
    }
    
    if (k=='e'){
      rtc.setTime(ArrT[0], ArrT[1], ArrT[2]); // Часы, минуты, секунды 24-часовой формат.
      k='c'; //выход
    }
    
  }
}

//функция выводит чч или мм или cc
void flashPrint(int Flash, int CT){
  if ((CurS == CT) && (Flash==0)){
      lcd.print("  ");      
    }else{
      if (ArrT[CT]<10){
        lcd.print('0');
      }
      lcd.print(ArrT[CT]);
    }
}
//функция выводит день, месяц или год
void flashPrintDate(int Flash, int CT){
  if ((CurS == CT) && (Flash==0)){
      if (CurS == 2){
        lcd.print("    ");
      }else{
        lcd.print("  ");      
      }
    }else{
      if (ArrD[CT]<10){
        lcd.print('0');
      }
      lcd.print(ArrD[CT]);
    }
}

//функция увеличивает выбранный элемент на 1
void incr(){
  int limit=59;
  if (CurS==0){
      limit=23;
  }
  
  if (ArrT[CurS] < limit){
    ArrT[CurS]++;
  }else{
    ArrT[CurS]=0;
  }
  delay(200);
}
//функция уменьшает выбранный элемент на 1 для даты
void decr(){
  int limit=59;
  if (CurS==0){
   limit=23;
  }  
  if (ArrT[CurS] > 0){
    ArrT[CurS]--;
  }else{
    ArrT[CurS]=limit;
  }
  delay(200);
}

//функция увеличивает выбранный элемент на 1 для даты
void incrD(){
  int limit=31;
  if (CurS==1){
      limit=12;
  }
  if (CurS==2){
      limit=5079;
  }
  
  if (ArrD[CurS] < limit){
    ArrD[CurS]++;
  }else{
    ArrD[CurS]=1;
  }
  delay(200);
}
//функция уменьшает выбранный элемент на 1 для даты
void decrD(){
  int limit=31;
  if (CurS==1){
      limit=12;
  }
  if (CurS==2){
      limit=5079;
  }
  if (ArrD[CurS] > 1){
    ArrD[CurS]--;
  }else{
    ArrD[CurS]=limit;
  }
  delay(200);
}


//timestamp
uint32_t getEpochTime(Time tm){  
  int i;
  uint32_t seconds;
  int year = tm.year - 1970;
 
  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= year*(SECS_PER_DAY * 365);
  for (i = 0; i < year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
 
  // add days for this year, months start from 1
  for (i = 1; i < tm.mon; i++) {
    if ( (i == 2) && LEAP_YEAR(year)) {
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (tm.date-1) * SECS_PER_DAY;
  seconds+= tm.hour * SECS_PER_HOUR;
  seconds+= tm.min * SECS_PER_MIN;
  seconds+= tm.sec;
  return seconds;
}

//eeprom int
void EEPROM_int_write(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}

unsigned int EEPROM_int_read(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);
  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}


//eeprom long int
void EEPROMWritelong(int address, long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }
      
long EEPROMReadlong(long address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }
      
      
