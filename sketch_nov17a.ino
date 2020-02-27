#include <Keypad.h>

#include<LiquidCrystal_I2C.h>

#include<EEPROM.h>

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal_I2C lcd(0x26, 16, 2);

char password[4];

char initial_password[4],new_password[4];

int i=0;

int relay_pin = 10;

char key_pressed=0;

const byte rows = 4; 

const byte columns = 4; 

char hexaKeys[rows][columns] = {

{'1','2','3','A'},

{'4','5','6','B'},

{'7','8','9','C'},

{'*','0','#','D'}

};

byte rowPins[rows] = {A5, A4, A3, A2}; //connect to the row pinouts of the keypad
byte colPins[columns] = {7, 8, 9, 10};

Keypad keypad_key = Keypad( makeKeymap(hexaKeys), rowPins, colPins, rows, columns);




void setup()

{

  myservo.attach(0);  // attaches the servo on pin 9 to the servo object

  pinMode(relay_pin, OUTPUT);

  lcd.begin(16,2);

  lcd.print("EGN2020C");

  lcd.setCursor(0,1);

  lcd.print("Pinpass");

  delay(2000);

  lcd.clear();

  lcd.print("Enter Password");

  lcd.setCursor(0,1);

  initialpassword();

}




void loop()

{

  digitalWrite(relay_pin, HIGH);

  key_pressed = keypad_key.getKey();

  if(key_pressed=='#')

    change();

  if (key_pressed)

  {

    password[i++]=key_pressed;

    lcd.print(key_pressed);

      }

  if(i==4)

  {

    delay(200);

    for(int j=0;j<4;j++)

      initial_password[j]=EEPROM.read(j);

    if(!(strncmp(password, initial_password,4)))

    {

      lcd.clear();

      lcd.print("Pass Accepted");

      myservo.write(180);

      delay(2000);

      lcd.setCursor(0,1);

      lcd.print("Pres # to change");

      delay(2000);

      lcd.clear();

      lcd.print("Enter Password:");

      lcd.setCursor(0,1);

      i=0;




    }

    else

    {

      myservo.write(0);




      lcd.clear();

      lcd.print("Wrong Password");

      lcd.setCursor(0,1);

      lcd.print("Pres # to Change");

      delay(2000);

      lcd.clear();

      lcd.print("Enter Password");

      lcd.setCursor(0,1);

      i=0;




    }

  }

}

void change()

{

  int j=0;

  lcd.clear();

  lcd.print("Current Password");

  lcd.setCursor(0,1);

  while(j<4)

  {

    char key=keypad_key.getKey();

    if(key)

    {

      new_password[j++]=key;

      lcd.print(key);

       

    }

    key=0;

  }

  delay(500);




  if((strncmp(new_password, initial_password, 4)))

  {

    lcd.clear();

    lcd.print("Wrong Password");

    lcd.setCursor(0,1);

    lcd.print("Try Again");

    delay(1000);

  }

  else

  {

    j=0;

    lcd.clear();

    lcd.print("New Password:");

    lcd.setCursor(0,1);

    while(j<4)

    {

      char key=keypad_key.getKey();

      if(key)

      {

        initial_password[j]=key;

        lcd.print(key);

        EEPROM.write(j,key);

        j++;

     

      }

    }

    lcd.print("Pass Changed");

    delay(1000);

  }

  lcd.clear();

  lcd.print("Enter Password");

  lcd.setCursor(0,1);

  key_pressed=0;

}




void initialpassword(){

  for(int j=0;j<4;j++)

    EEPROM.write(j, j+49);

  for(int j=0;j<4;j++)

    initial_password[j]=EEPROM.read(j);

}
