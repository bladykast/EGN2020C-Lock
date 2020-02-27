#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>

Servo myservo;                              // create servo object to control a servo

LiquidCrystal_I2C lcd(0x26, 16, 2);         // create LCD object to control LCD


char password[4];
char initial_password[4],new_password[4];
int i=0;

char key_pressed=0;

const byte rows = 4; 
const byte columns = 4; 

char hexaKeys[rows][columns] = {

{'1','2','3','A'},

{'4','5','6','B'},

{'7','8','9','C'},

{'*','0','#','D'}

};

byte rowPins[rows] = {A3, A2, A1, A0};            //connect to the row pinouts of the keypad
byte colPins[columns] = {10, 9, 8, 7};            //...and the columns

Keypad keypad_key = Keypad( makeKeymap(hexaKeys), rowPins, colPins, rows, columns);




void setup()

{

  myservo.attach(3);                                // initializes the lock servo on pin 3

  lcd.init();                                        //initializes the LCD
  lcd.backlight();                                
  lcd.begin(16,2);
  lcd.print("EGN2020C");
  lcd.setCursor(0,1);
  lcd.print("Pinpass");

  delay(2000);

  lcd.clear();          
  lcd.print("Enter Password");                      // Asks for password
  lcd.setCursor(0,1);

  initialpassword();

}


void loop()                                     // Constantly checks the keypad for inputs

{
  key_pressed = keypad_key.getKey();
  
  if(key_pressed=='#')
    change();
    
  if (key_pressed)
  {
    password[i++]=key_pressed;
    lcd.print(key_pressed);
      }

  if(i==4)                                        //When 4 digits are entered...

  {

    delay(200);

    for(int j=0;j<4;j++)
      initial_password[j]=EEPROM.read(j);

    if(!(strncmp(password, initial_password,4))) //Check entered password against real password

    {

      lcd.clear();                              //If password correct... 
      lcd.print("Pass Accepted");
      myservo.write(180);                       //opens door and displays "Pass Accepted

      delay(2000);
      lcd.setCursor(0,1);
      lcd.print("Pres # to change");            //Allows for a password change
      delay(2000);
      lcd.clear();
      lcd.print("Enter Password:");
      lcd.setCursor(0,1);
      
      i=0;

    }

    else                                        //If password incorrect...
    
    {
      myservo.write(0);                          //displays "Wrong password" and keeps door shut
      lcd.clear();
      lcd.print("Wrong Password");
      delay(2000);

      lcd.clear();                              //Asks again for password
      lcd.print("Enter Password");
      lcd.setCursor(0,1);

      i=0;

    }

  }

}

void change()                                   //Logic to change password

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

void initialpassword(){                            //Writes the initial password to memory

  for(int j=0;j<4;j++)                             //sets the password as, creatively, 1234
    EEPROM.write(j, j+49);

  for(int j=0;j<4;j++)
    initial_password[j]=EEPROM.read(j);

}
