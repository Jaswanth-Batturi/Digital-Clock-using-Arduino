// Name : Batturi Jaswanth 
// Roll No : 1901CS13
// Project : Digital Clock 

// Libraries
#include <LiquidCrystal.h>
#include <dht.h> 
// The DHT Library needs to be included as a .ZIP Library 
// The ZIP Library file is attached in the Sketch Folder, along with main code

// DHT11 analog input Pin
#define DHT11_apin A0

dht DHT11;

// LCD pins
const int rs = 2;
const int en = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;

// LCD Object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Symbols
byte Temperature_Symbol[8] = {
  B00100,
  B01011,
  B01010,
  B01011,
  B01110,
  B11111,
  B11111,
  B01110
};

byte Degrees[8] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte Drop1[8] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B01110,
  B00000,
  B00000,
  B00000
};

byte Drop2[8] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B01110
};

byte AlarmOn[8] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};

int hrs = 12,mins = 0,secs = 0,flag = 0,hr_12 = 1;
int ahrs = 12,amins = 0,aflag = 0,buz_time = 60; 
int shrs = 0, smins = 0, ssecs = 0, sstart=0;
int lhrs = 0, lmins = 0, lsecs = 0;
int thrs = 0, tmins = 0, tsecs = 0, tstart = 0;
int mode = 0, settime = 0, setdate = 0, alarm = 0, timer = 0;
int dd = 1, mm = 1, yy = 2021, ImportantDay = 0;
int T_Unit = 0, Lii = 0, Li = 16;

int daysOfMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char* weekDays[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const char* ImportantDays[] = {"New Year's Day", "Republic Day", "Ambedkar Jayanti", "May Day", "Independence Day", "Teachers' Day", "Gandhi Jayanthi", "Children's Day", "Christmas Day"};
const int ImportantDD[] = {1, 26, 14, 1, 15, 5, 2, 14, 25};
const int ImportantMM[] = {1, 1, 4, 5, 8, 9, 10, 11, 12};

bool on = 0, buzzer = 0;
bool adjust = 0;

// Displays the Time
void DispTime()
{
  if(hr_12)
    {
      lcd.setCursor(2,0);
    }
    else
    {
      lcd.setCursor(4,0);
    }

    if(hr_12)
    {
        if(hrs < 10)
        {
            lcd.print("0");
        }
        lcd.print(hrs);
    }
    else
    {
        if(flag < 10)
        {
            lcd.print("0");
        }
        lcd.print(flag);
    }
    lcd.print(":");

    if(mins < 10)
    {
        lcd.print("0");
    }
    lcd.print(mins);
    lcd.print(":");

    if(secs < 10)
    {
        lcd.print("0");
    }
    lcd.print(secs);

    if(hr_12)
    {
        if(flag < 12)
        {
            lcd.print(" AM");
        }
        else
        {
            lcd.print(" PM");
        }
    }
}

bool ImpDay()
{
  for(int i = 0; i < sizeof(ImportantDays)/sizeof(ImportantDays[0]); i++)
    {
      if(dd == ImportantDD[i] && mm == ImportantMM[i])
        {
          return true;
        }
    }
  
    return false;
}

void Scroll_LCD_Left(String Imp_Day)
{
  Imp_Day = "                " + Imp_Day + "                ";
  lcd.print(Imp_Day.substring(Li,Lii));
  Li++;
  Lii++;
  if (Li>Imp_Day.length())
  {
    Li=16;
    Lii=0;
  }
}

void DispImpDay()
{
  for(int i = 0; i < sizeof(ImportantDays)/sizeof(ImportantDays[0]); i++)
  {
    if(dd == ImportantDD[i] && mm == ImportantMM[i])
    {
        for(int j = 0; j < 2; j++)
        {
          lcd.setCursor(0,1);
          Scroll_LCD_Left(ImportantDays[i]);
          delay(500);
        }
        return;
    }
  }
}

void DispDateDay(bool DispDay_ImpDay)
{
    if(adjust || (!ImportantDay))
    {
      lcd.setCursor(3,0);
    }
    else
    {
      lcd.setCursor(0,0);
    }
  
    if(dd<10)
    {
      lcd.print("0");
    }
    lcd.print(dd);
    lcd.print("/");

    if(mm<10)
    {
      lcd.print("0");
    }
    lcd.print(mm);
    lcd.print("/");

    lcd.print(yy);
    lcd.print(" ");
  
    if(ImportantDay)
    {
      lcd.setCursor(11,0);
    }
    else
    {
      lcd.setCursor(7,1);
    }
  
    if(DispDay_ImpDay)
    {
      lcd.print(weekDays[dayFinder(dd,mm,yy)]);
    }
   
    if(ImportantDay)
    {
      lcd.setCursor(0,1);
      if(DispDay_ImpDay)
      {
          DispImpDay();
      }
    }
}

void DispAlarm()
{
  if(hr_12)
    {
      lcd.setCursor(4,0);
    }
    else
    {
      lcd.setCursor(5,0);
    }

    if(hr_12)
    {
        if(ahrs < 10)
        {
            lcd.print("0");
        }
        lcd.print(ahrs);
    }
    else
    {
        if(aflag < 10)
        {
            lcd.print("0");
        }
        lcd.print(aflag);
    }
    lcd.print(":");

    if(amins < 10)
    {
        lcd.print("0");
    }
    lcd.print(amins);

    if(hr_12)
    {
        if(aflag < 12)
        {
            lcd.print(" AM");
        }
        else
        {
            lcd.print(" PM");
        }
    }

    lcd.setCursor(3,1);
    lcd.print("Alarm: ");

    if(on)
    {
      lcd.print("ON ");
    }
    else
    {
      lcd.print("OFF");
    }
}

void DispSTW()
{
  lcd.setCursor(0,0);
    lcd.print(" STW  ");

    if(shrs < 10)
    {
        lcd.print("0");
    }
    lcd.print(shrs);
    lcd.print(":");

    if(smins < 10)
    {
        lcd.print("0");
    }
    lcd.print(smins);
    lcd.print(":");

    if(ssecs < 10)
    {
        lcd.print("0");
    }
    lcd.print(ssecs);
}

void DispTMR()
{
    lcd.setCursor(0,0);
    lcd.print(" TMR  ");

    if(thrs < 10)
    {
        lcd.print("0");
    }
    lcd.print(thrs);
    lcd.print(":");

    if(tmins < 10)
    {
        lcd.print("0");
    }
    lcd.print(tmins);
    lcd.print(":");

    if(tsecs < 10)
    {
        lcd.print("0");
    }
    lcd.print(tsecs);
}

bool LeapYear(int year)
{
  if((year%4 == 0 && year%100 != 0) || year%400 == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int dayFinder(int d, int m, int y)
{
  static int t[12] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 }; 
  y -= m < 3; 
  return ( y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7; 
}

int Celcius_to_Fahrenheit(int C)
{
  return ((C*9)+160)/5;
}

int Celcius_to_Kelvin(int C)
{
  return C+273;
}

void Disp_Temp_RH()
{
    DHT11.read11(DHT11_apin);
    
    int T_C = DHT11.temperature;
    int T_F = Celcius_to_Fahrenheit(T_C);
    int T_K = Celcius_to_Kelvin(T_C);
    int RH = DHT11.humidity;
    lcd.setCursor(0,1);
    lcd.write(1);
    lcd.print(" ");
    if(T_Unit == 0)
    {
      lcd.print(T_C);
      lcd.write(2);
      lcd.print("C ");
    }
    if(T_Unit == 1)
    {
      lcd.print(T_F);
      lcd.write(2);
      lcd.print("F ");
    }
    if(T_Unit == 2)
    {
      lcd.print(T_K);
      lcd.print("K ");
    }
    lcd.setCursor(9,1);
    lcd.write(3);
    lcd.write(4);
    lcd.print(" ");
    lcd.print(RH);
    lcd.print("%");
}


// SetUp Function
void setup()
{
  lcd.begin(16, 2);
  
  lcd.createChar(1,Temperature_Symbol);
  lcd.createChar(2,Degrees);
  lcd.createChar(3,Drop1);
  lcd.createChar(4,Drop2);
  lcd.createChar(5,AlarmOn);
  
  
  // Push Buttons  
  pinMode(9, INPUT_PULLUP);
  digitalWrite(9, HIGH);
  pinMode(10, INPUT_PULLUP);
  digitalWrite(10, HIGH);
  pinMode(11, INPUT_PULLUP);
  digitalWrite(11, HIGH);
  pinMode(12, INPUT_PULLUP);
  digitalWrite(12, HIGH);
  
  // Buzzer and LED
  pinMode(8, OUTPUT);
  
}

// Loop Function
void loop()
{
  if(!buzzer)
  {
    if(!adjust)
    {
      // Switching the modes
      if(digitalRead(12) == LOW)
      {
        mode++;
        mode = mode % 5;
        
        Li = 16;
        Lii = 0;
        lcd.clear();
      }
    }

    lcd.setCursor(15,0);
    if(on)
    {
        lcd.write(5);
    }
    else
    {
        lcd.print(" ");
    }
  
    // Displays Time (12 and 24 hrs format) and Temperature and Humidity
    if(mode == 0)
    {
      if(digitalRead(11) == LOW)
      {
        adjust = !adjust;
        settime = 0;
      }
      
      if(!adjust)
      {
        if(digitalRead(10) == LOW)
        {
            T_Unit++;
            T_Unit = T_Unit % 3;
        }
        Disp_Temp_RH();

        if(digitalRead(9) == LOW)
        {
          hr_12 = !hr_12;
          lcd.setCursor(0,0);
          lcd.print("               ");
        }
      }

      DispTime();

      if(!adjust)
      {
        delay(1000);
      }

      else
      {
        if(digitalRead(12) == LOW)
        {
          settime++;
          if(hr_12)
          {
            settime = settime % 4;
          }
          else
          {
            settime = settime % 3;
          }
        }

        Disp_Temp_RH();

        delay(500);

        int hrs_pos, mins_pos, secs_pos;
        
        if(hr_12)
        {
            hrs_pos = 2;
            mins_pos = 5;
            secs_pos = 8;
        }
        else
        {
            hrs_pos = 4;
            mins_pos = 7;
            secs_pos = 10;
        }
        
        // Hours
        if(settime == 0)
        {
          lcd.setCursor(hrs_pos,0);

          lcd.print("  ");

          delay(500);
          
      // Increment
          if(digitalRead(9) == LOW)
          {
              hrs++;
              flag++;
          }
      // Decrement
          if(digitalRead(10) == LOW)
          {
              hrs--;
              flag--;
          }
        }
    
        // Minutes
        if(settime == 1)
        { 
          lcd.setCursor(mins_pos,0);

          lcd.print("  ");
          delay(500);

          // Increment
          if(digitalRead(9) == LOW)
          {
              mins++;
          }
      // Decrement
          if(digitalRead(10) == LOW)
          {
              mins--;
          }
        }
    
        // Seconds
        if(settime == 2)
        { 
          lcd.setCursor(secs_pos,0);

          lcd.print("  ");
          delay(500);

          if(digitalRead(9) == LOW || digitalRead(10) == LOW)
          {
              secs = -1;
          }
        }
        
        // AM/PM 
        if(settime == 3)
        {
          lcd.setCursor(11,0);

          lcd.print("  ");
          delay(500);
      
          // Toggle
          if(digitalRead(9) == LOW || digitalRead(10) == LOW)
          {
              flag = (flag+12)%24;
          }
        }
      }
    }


    // Displays Date and Day and reminds us important and special dates
    if(mode == 1)
    {
      if(digitalRead(11) == LOW)
      {
        adjust = !adjust;
        setdate = 0;
        Li = 16;
        Lii = 0;
        lcd.clear();
      }
      
      bool dispDay_impDay;
      
      if((!adjust) && ImportantDay != ImpDay())
      {
        ImportantDay = !ImportantDay;
        lcd.clear();
      }

      if((!adjust) && ImportantDay)
      {
        dispDay_impDay = true;
      }
      
      if(!adjust)
      {
        if((dd > daysOfMonth[mm]) || (dd == 29 && mm == 2 && !LeapYear(yy)))
        {
            dd = 1;
            mm++;
        }

        DispDateDay(true);
        
        if(!ImportantDay)
        {
          delay(1000);
        }
      }

      else
      {
        DispDateDay(false);
        
        if(digitalRead(12) == LOW)
        {
          setdate++;
          setdate = setdate % 3;
        }

        delay(500);

        // Days
        if(setdate==0)
        {
          lcd.setCursor(3,0);

          lcd.print("  ");

          delay(500);

          // Increment
          if(digitalRead(9) == LOW)
          {
              dd++;
              if(dd > daysOfMonth[mm])
              {
                  dd = 1;
              }
          }

          // Decrement
          if(digitalRead(10) == LOW)
          {
              dd--;
              if(dd == 0)
              {
                  dd = daysOfMonth[mm];
              }
          }

        }

        // Month
        if(setdate==1)
        {
          
          lcd.setCursor(6,0);

          lcd.print("  ");

          delay(500);

          // Increment
          if(digitalRead(9) == LOW)
          {
              mm++;
              if(mm == 13)
            {
              mm = 1;
            }
          }

          // Decrement
          if(digitalRead(10) == LOW)
          {
              mm--;
              if(mm == 0)
              {
                  mm = 12;
              }
          }

        }

        // Years
        if(setdate==2)
        {
          lcd.setCursor(9,0);

          lcd.print("    ");

          delay(500);

          // Increment
          if(digitalRead(9) == LOW)
          {
              yy++;
          }

          // Decrement
          if(digitalRead(10) == LOW)
          {
              yy--;
          }

          // Leap Year
          if(LeapYear(yy))
          {
            daysOfMonth[2]=29;
          }
          else
          {
            daysOfMonth[2]=28;
          }

        }

      }
    }

    // Set the Alarm to a desired Time
    if(mode == 2)
    {
      if(digitalRead(11) == LOW)
      {
        adjust = !adjust;
        alarm = 0;
      }
      
      DispAlarm();

      if(!adjust)
      {
        delay(1000);

        // Alarm ON/OFF
        if(digitalRead(9) == LOW || digitalRead(10) == LOW)
        {
          on = !on;
        }
      }

      else
      {
        if(digitalRead(12) == LOW)
        {
          alarm++;

          if(hr_12 == 1)
          {
              alarm = alarm % 3;
          }
          else
          {
              alarm = alarm % 2;
          }
        }

        delay(500);

        int ahrs_pos, amins_pos;
        
        if(hr_12 == 0)
        {
          ahrs_pos = 5;
          amins_pos = 8;
        }
        else
        {
          ahrs_pos = 4;
          amins_pos = 7;
        }
        
        // Alarm Hours
        if(alarm == 0)
        {
          lcd.setCursor(ahrs_pos,0);

          lcd.print("  ");

          delay(500);

          // Increment
          if(digitalRead(9) == LOW)
          {
              ahrs++;
              aflag++;
          }

          // Decrement
          if(digitalRead(10) == LOW)
          {
              ahrs--;
              aflag--;
          }
        }

        // Alarm Minutes
        if(alarm == 1)
        { 
          lcd.setCursor(amins_pos,0);

          lcd.print("  ");
          delay(500);

          // Increment
          if(digitalRead(9) == LOW)
          {
              amins++;
          }

          // Decrement
          if(digitalRead(10) == LOW)
          {
              amins--;
          }
        }

        // ALarm AM/PM
        if(alarm == 2)
        {
          lcd.setCursor(10,0);

          lcd.print("  ");
          delay(500);

          // Toggle
          if(digitalRead(9) == LOW || digitalRead(10) == LOW)
          {
              aflag = (aflag+12)%24;
          }
        }
      }
    }

    // Stopwatch with start, stop and reset options along with Lap Time calculator
    if(mode == 3)
    {
      DispSTW();

      // Start/Stop
      if(digitalRead(9) == LOW)
      {
        sstart = !sstart;
      }
      
      // Reset
      if(!sstart)
      {
        if(digitalRead(10) == LOW)
        {
           shrs = smins = ssecs = 0;
           lhrs = lmins = lsecs = 0;
           lcd.clear();
        }
      }

      delay(1000);
    }

    // Timer which notifies us with buzzer and LED after a desired time period
    if(mode == 4)
    { 
      if(digitalRead(11) == LOW)
      {
        adjust = !adjust;
        timer = 0;
      }

      DispTMR();

      if(!adjust)
      { 
        // Start/Stop
        if(digitalRead(9) == LOW)
        {
            tstart = !tstart;
        }

        // Reset
        if(!tstart)
        {
           if(digitalRead(10) == LOW)
           {
               thrs = tmins = tsecs = 0;
           }
        }

        delay(1000);
      }

      else
      {
        if(digitalRead(12) == LOW)
        {
          timer++;
          timer = timer % 3;
        }

        delay(500);

        // Timer Hours
        if(timer == 0)
        {
          lcd.setCursor(6,0);

          lcd.print("  ");

          delay(500);

          // Increment
          if(digitalRead(9) == LOW)
          {
              thrs++;
          }

          // Decrement
          if(digitalRead(10) == LOW)
          {
              thrs--;
          }
        }

        // Timer Minutes
        if(timer == 1)
        { 
          lcd.setCursor(9,0);

          lcd.print("  ");
          delay(500);

          // Increment
          if(digitalRead(9) == LOW)
          {
              tmins++;
          }

          // Decrement
          if(digitalRead(10) == LOW)
          {
              tmins--;
          }
        }

        // Timer Seconds
        if(timer == 2)
        { 
          lcd.setCursor(12,0);

          lcd.print("  ");
          delay(500);

          // Increment
          if(digitalRead(9) == LOW)
          {
              tsecs++;
          }

          // Decrement
          if(digitalRead(10) == LOW)
          {
              tsecs--;
          }
        }
        
        // Timer Sync
        if(tsecs == 60)
        {
          tsecs = 0;
        }

        if(tmins == 60)
        {
          tmins = 0;
        }

        if(thrs == 100)
        {
          thrs = 0;
        }

        if(tsecs == -1)
        {
          tsecs = 59;
        }

        if(tmins == -1)
        {
          tmins = 59;
        }

        if(thrs == -1)
        {
          thrs = 99;
        }
      }
    }
  }
  
  else if(mode == 0)
  {
    DispTime();
  }
  else if(mode == 1)
  {
    DispDateDay(true);
  }
  else if(mode == 2)
  {
    DispAlarm();
  }
  else if(mode == 3)
  {
    DispSTW();
  }
  else
  {
    DispTMR();
  }
  
  // Clock Time Sync
  if(mins == 60)
  {
    mins = 0;
  }
  
  if(hrs == 13)
  {
    hrs = 1;
  }
  
  if(flag == 24)
  {
    flag = 0;
  }
  
  if(mins == -1)
  {
    mins = 59;
  }
  
  if(hrs == 0)
  {
    hrs = 12;
  }
  
  if(flag == -1)
  {
    flag = 23;
  }
  
  // Alarm Time Sync
  if(amins == 60)
  {
    amins = 0;
  }
  
  if(ahrs == 13)
  {
    ahrs = 1;
  }
  
  if(aflag == 24)
  {
    aflag = 0;
  }
  
  if(amins == -1)
  {
    amins = 59;
  }
  
  if(ahrs == 0)
  {
    ahrs = 12;
  }
  
  if(aflag == -1)
  {
    aflag = 23;
  }
  
  // Stopwatch and Lap Time Background run
  if(sstart)
  { 
      if(mode == 3 && digitalRead(11) == LOW)
      {     
          lcd.setCursor(0,1);
          lcd.print(" LAP  ");

          if(lhrs < 10)
          {
              lcd.print("0");
          }
          lcd.print(lhrs);
          lcd.print(":");

          if(lmins < 10)
          {
              lcd.print("0");
          }
          lcd.print(lmins);
          lcd.print(":");

          if(lsecs < 10)
          {
              lcd.print("0");
          }
          lcd.print(lsecs);
          lhrs = lmins = lsecs = 0;
      }

      // Stopwatch Time Sync
      ssecs++;
      if(ssecs == 60)
      {
        ssecs = 0;
        smins++;
      }
      if(smins == 60)
      {
        smins = 0;
        shrs++;
      }

      // Lap Time Sync
      lsecs++;
      if(lsecs == 60)
      {
        lsecs = 0;
        lmins++;
      }
      if(lmins == 60)
      {
        lmins = 0;
        lhrs++;
      }
  }
  
  // Timer Background Run
  if(tstart)
  {
    // Timer Buzzer and LED
    if(thrs == 0 && tmins == 0 && tsecs == 0)
    {
      tone(8, 500, 1000);
      tstart = 0;
    }
    else
    {
      // Timer Sync
      tsecs--;
      if(tsecs == -1)
      {
        tsecs = 59;
        tmins--;
      }
      if(tmins == -1)
      {
        tmins = 59;
        thrs--;
      }
    }
  }
  
  // Clock Time Sync
  secs++;
  if(secs == 60)
  {
    secs = 0;
    mins++;
  }
  
  if(mins == 60)
  {
    mins = 0;
    hrs++;
    flag++;
  }
  
  if(hrs == 13)
  {
    hrs = 1;
  }
  
  if(flag == 24)
  {
    flag = 0;
    dd++;
    if(dd > daysOfMonth[mm])
    {
      dd=1;
      mm++;
    }

    if(mm == 13)
    {
      mm=1;
      yy++;
    }

    if(LeapYear(yy))
    {
      daysOfMonth[2]=29;
    }
    else
    {
      daysOfMonth[2]=28;
    }
  }
  
  // Activate Buzzer
  if((!(mode == 2 && adjust)) && on && aflag==flag && amins==mins && secs == 0)
  {
    buzzer = 1;
  }
  
  // Alarm Buzzer and LED Blinking
  if(buzzer)
  {
    if(buz_time)
    {
      tone(8, 500, 500);
      buz_time--;
      
      // Stop Alarm
      if((!buz_time) || digitalRead(9) == LOW || digitalRead(10) == LOW || digitalRead(11) == LOW || digitalRead(12) == LOW)
      {
          buzzer = 0;
          buz_time = 60;
      }
    }
    
    delay(1000);
  }
}
