/* 功能：通过按PS2摇杆的“上下左右”移动实现前进 、后退、左转、右转。
 * 调整上一版速度为常量，不随摇杆位置变化的问题；增加读取摇杆值，作为PWM输入，控制速度；
 *日期：2017年8月23日
 * 验证完成：
 * 作者：lcl;
 */




#include <PS2X_lib.h>  //for v1.6

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
//PS2手柄引脚；
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

// 电机控制引脚；
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

  int speedPinA = 8;
  int speedPinB = 9;

int speed;

//speed =200;

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures   true
//#define pressures   false
#define rumble      true
//#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;



 void setup(){
 
   pinMode(IN1, OUTPUT);
   pinMode(IN2, OUTPUT);
   pinMode(IN3, OUTPUT);
   pinMode(IN4, OUTPUT);

   
  
   Serial.begin(57600);
   delay(300) ; //added delay to give wireless ps2 module some time to startup, before configuring it
   //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
 //  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}

 void turnLeft() //左转
 {      
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4,LOW);
  delay(20);
}
 void turnRight()//右转
 {
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, HIGH);
   digitalWrite(IN4, LOW);
  delay(20);      
}

 void forward() // 前进
 {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);   
  delay(20);   
}
void stop() // 停止；
 {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(20);
}

 void back()  //后退
 {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);     
}



 void loop(){
   /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1) //skip loop if no controller found
    return; 
  
  if(type == 2) {//Guitar Hero Controller
  return;
     }
  else  { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed


    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
	    Serial.print("Stick Values:");
	    Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
	    Serial.print(",");
	    Serial.print(ps2x.Analog(PSS_LX), DEC);
	    Serial.print(",");
	    Serial.print(ps2x.Analog(PSS_RY), DEC);
	    Serial.print(",");
	    Serial.println(ps2x.Analog(PSS_RX), DEC);
			
		int LY=ps2x.Analog(PSS_LY);
		int LX=ps2x.Analog(PSS_LX);
		int RY=ps2x.Analog(PSS_RY);
		int RX=ps2x.Analog(PSS_RX);
		
		if (LY<128)  //前进
		{
			
			speed = 2*(127-LY);
			//speed = LY;
			analogWrite(speedPinA, speed);
			analogWrite(speedPinB, speed);
			forward();	
		}

		//后退
		if (LY>128)
		{
			speed=2*(LY-128);
			analogWrite(speedPinA, speed);
			analogWrite(speedPinB, speed);
			back();
		}
		//左转
		if (LX<128)
		{
			speed = 2*(127-LX);
			analogWrite(speedPinA, speed);
			analogWrite(speedPinB, speed);
			turnLeft();
		}
		//右转
		if (LX>128)
		{
			speed=2*(LX -128);
			analogWrite(speedPinA, speed);
			analogWrite(speedPinB, speed);
			turnRight();
		}
		//如果摇杆居中
		if (LY>=128 && LY<=128 && LX>=128 && LX<=128)
		{
             stop();
		}
    }

  }
}










