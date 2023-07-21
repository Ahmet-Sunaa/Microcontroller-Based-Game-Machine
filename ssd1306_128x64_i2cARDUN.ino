#include <List.hpp>

#include <Wire.h>
#include<SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#define OLED_RESET 9
#define buttonPin A0
#define mux0 0
#define mux1 1
#define gunmux0 A1
#define gunmux1 A2

int buzer = 10;
const byte konsol = A3;

Adafruit_SSD1306 display(128,64,&Wire,OLED_RESET);



int digit[10] = {0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111};

int digit1, digit2, digit3; // her segmenti kontrol etmek için atama
int general=0,totallife=3,totalgun=1;
int ship_y1=24;
int ship_y2=32;
class engel{
  public:
    int x=0,y=0,z=0;
    engel(int a,int b,int c){
      x=a;
      y=b;
      z=c;
    }
};
List<engel> meteorlist;
List<engel> spacetrashlist;
List<engel> extralifelist;
List<engel> gunlist;
List<engel> untouchablelist;
List<engel> gunshotlist;

void setup(){ 
  for (int i = 2; i < 9; i++){
    pinMode(i, OUTPUT); // 0-9 inci pini çıkış olarak ayarlama(kod kalabağı olmaması için for ile ayarlandı)
  }

  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT); //1. hane output
  pinMode(12, OUTPUT);//2.hane output
  pinMode(buzer,OUTPUT);
  pinMode(mux0, OUTPUT);
  pinMode(mux1, OUTPUT);
  randomSeed(5000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.display();
  display.clearDisplay();
//display.drawBitmap(0, 0, image_data_Saraarray, 128, 64, 1);

}


void makemeteor(engel engels){    
  display.fillCircle(engels.x, engels.y , 4, WHITE);
  display.fillCircle(engels.x-8, engels.y , 4, BLACK);
}
void makespacetrash(engel engels){
  display.drawCircle(engels.x, engels.y , 4, WHITE);
  display.drawCircle(engels.x-8, engels.y , 4, BLACK);
}
void makeextralife(engel engels){
  display.drawRoundRect(engels.x, engels.y, 4,4,4,WHITE);
  display.drawRoundRect(engels.x-8, engels.y, 4,4,4,BLACK);
}
void makegun(engel engels){
  display.drawTriangle(engels.x, engels.y-1,engels.x,engels.y+1,engels.x+3,engels.y ,WHITE);
  display.drawTriangle(engels.x-8, engels.y-1,engels.x-8,engels.y+1,engels.x-5,engels.y ,BLACK);
}
void makeuntouchable(engel engels){
  display.drawRect(engels.x, engels.y,2,2,WHITE);
  display.drawRect(engels.x-8, engels.y,2,2,BLACK);
}
void downobject(){
  
  for(int i=0;i<meteorlist.getSize();i++){
    int x=meteorlist.getValue(i).x,y=meteorlist.getValue(i).y;
    if(x>=128){
      if(y-4 < ship_y1 && y+4>ship_y1){
        digitalWrite(buzer,HIGH);
        delay(500);
        digitalWrite(buzer,LOW);
        totallife--;
      }
      else if(y-4<ship_y2 && y+4>ship_y2){
        digitalWrite(buzer,HIGH);
        delay(500);
        digitalWrite(buzer,LOW);
        totallife--;
      }else if((y-4==ship_y2 && y+4 == ship_y1) || (y-4==ship_y1 && y+4 == ship_y2)){
        digitalWrite(buzer,HIGH);
        delay(500);
        digitalWrite(buzer,LOW);
        totallife--;
      }
      makemeteor(meteorlist.getValue(i));
      meteorlist.remove(i);
      continue;
    }
      
    engel a(x+8,y,0);
    meteorlist.addAtIndex(i,a);
    meteorlist.remove(i+1);
    makemeteor(a);      
    
  }
  for(int i=0;i<spacetrashlist.getSize();i++){
    int x=spacetrashlist.getValue(i).x,y=spacetrashlist.getValue(i).y;
    if(x>=128){
      if(y-4 < ship_y1 && y+4>ship_y1){
        digitalWrite(buzer,HIGH);
        delay(500);
        digitalWrite(buzer,LOW);
        totallife--;
      }
      else if(y-4<ship_y2 && y+4>ship_y2){
        digitalWrite(buzer,HIGH);
        delay(500);
        digitalWrite(buzer,LOW);
        totallife--;
      }else if((y-4==ship_y2 && y+4 == ship_y1) || (y-4==ship_y1 && y+4 == ship_y2)){
        digitalWrite(buzer,HIGH);
        delay(500);
        digitalWrite(buzer,LOW);
        totallife--;
      }
      makespacetrash(spacetrashlist.getValue(i));
      spacetrashlist.remove(i);
      continue;
    }
    engel a(x+8,y,spacetrashlist.getValue(i).z);
    spacetrashlist.addAtIndex(i,a);
    spacetrashlist.remove(i+1);
    makespacetrash(a);
  }
  for(int j=0;j<extralifelist.getSize();j++){
    int x=extralifelist.getValue(j).x,y=extralifelist.getValue(j).y;
    if(x>=128){
      if(y-4 < ship_y1 && y+4>ship_y1){
        if (totallife!=3)
          totallife++;
      }
      else if(y-4<ship_y2 && y+4>ship_y2){
        if (totallife!=3)
          totallife++;
      }else if((y-4==ship_y2 && y+4 == ship_y1) || (y-4==ship_y1 && y+4 == ship_y2)){
        if (totallife!=3)
          totallife++;
      }
      makeextralife(extralifelist.getValue(j));
      extralifelist.remove(j);
      continue;
    }
    engel a(x+8,y,2);
    extralifelist.addAtIndex(j,a);
    extralifelist.remove(j+1);
    makeextralife(a);
  }
  for(int i=0;i<gunlist.getSize();i++){
    int x=gunlist.getValue(i).x,y=gunlist.getValue(i).y;
    if(x>=128){
      if(y-4 < ship_y1 && y+4>ship_y1){
        if (totalgun!=3)
          totalgun++;
      }
      else if(y-4<ship_y2 && y+4>ship_y2){
        if (totalgun!=3)
          totalgun++;
      }else if((y-4==ship_y2 && y+4 == ship_y1) || (y-4==ship_y1 && y+4 == ship_y2)){
        if (totalgun!=3)
          totalgun++;
      }
      makegun(gunlist.getValue(i));
      gunlist.remove(i);
      continue;
      }
    engel a(x+8,y,3);
    gunlist.addAtIndex(i,a);
    gunlist.remove(i+1);
    makegun(a);
  }
  for(int i=0;i<untouchablelist.getSize();i++){
    if(untouchablelist.getValue(i).x>=128){
      makeuntouchable(untouchablelist.getValue(i));
      untouchablelist.remove(i);
      continue;
    }
    int x=untouchablelist.getValue(i).x+8,y=untouchablelist.getValue(i).y;
    engel a(x,y,4);
    untouchablelist.addAtIndex(i,a);
    untouchablelist.remove(i+1);
    makeuntouchable(a);
  }
  delay(5);
  display.display();

}
void makebullet(engel engels){
  display.drawTriangle(engels.x, engels.y-2,engels.x,engels.y+2,engels.x-3,engels.y ,WHITE);
  display.drawTriangle(engels.x+8, engels.y-2,engels.x+8,engels.y+2,engels.x+5,engels.y ,BLACK);
}
void gunshot(){
  for(int i=0;i<gunshotlist.getSize();i++){
    int x=gunshotlist.getValue(i).x,y=gunshotlist.getValue(i).y;
    if(x<=0){
      makebullet(gunshotlist.getValue(i));
      gunshotlist.remove(i);
      continue;
    }    
    engel a(x-8,y,0);
    gunshotlist.addAtIndex(i,a);
    gunshotlist.remove(i+1);
    makebullet(a);
  }
  display.display();

}
int n=0;

void func(){
  delay(1000);
  n+=1;
  int x=4,y=60;
  // meteorlist.add(a);
  // meteorlist.add(b);
  drawship(ship_y1,ship_y2,0);
  for (int j = 0; j <= 999; j++){
    if(totallife==0){
      digitalWrite(mux0,LOW);
      digitalWrite(mux1,LOW);
      display.display();
      display.clearDisplay();
      display.display();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(15, 20);
      display.print("Game Over");
      display.setCursor(15, 40);
      display.print(j);
      display.display();
      delay(5000);
      break;
    }else if(totallife==1){
      digitalWrite(mux0,HIGH);
      digitalWrite(mux1,LOW);
    }else if(totallife==2){
      digitalWrite(mux0,LOW);
      digitalWrite(mux1,HIGH);
    }else if(totallife==3){
      digitalWrite(mux0,HIGH);
      digitalWrite(mux1,HIGH);
    }
    if(totalgun==0){
      digitalWrite(gunmux0,LOW);
      digitalWrite(gunmux1,LOW);
    }else if(totalgun==1){
      digitalWrite(gunmux0,HIGH);
      digitalWrite(gunmux1,LOW);
    }else if(totalgun==2){
      digitalWrite(gunmux0,LOW);
      digitalWrite(gunmux1,HIGH);
    }else if(totalgun==3){
      digitalWrite(gunmux0,HIGH);
      digitalWrite(gunmux1,HIGH);
    }
    if(digitalRead(buttonPin)==HIGH && totalgun!=0){
      totalgun--;
      engel c(128,(ship_y1+ship_y2)/2,0);
      gunshotlist.add(c);
    }
    if(gunshotlist.getSize()!=0){
      gunshot();
    }
    downobject();
    
    if(j%10==0){
      int b=random(0,7);
      for(int i=0;i<8;i++){
        if(i==b)
          continue;
        int e;
        e=random(0,5);
        if(e==0){
          engel c(x,y-(i*8),2);
          meteorlist.add(c);          
        }
        else if(e==1){
        engel c(x,y-(i*8),1);
        spacetrashlist.add(c);
        }
        else if(e==2){
          engel c(x,y-(i*8),0);
          extralifelist.add(c);

        }
        else if(e==3){
          engel c(x,y-(i*8),0);
          gunlist.add(c);
        }
        else if(e==4){
          engel c(x,y-(i*8),0);
          untouchablelist.add(c);
        }      
      }
      
    }
    int potdeger=analogRead(konsol);
      if (potdeger<500){
        drawship(ship_y1,ship_y2,1);
        if(ship_y1!=0){
          ship_y1-=4;
          ship_y2-=4;
        }
        drawship(ship_y1,ship_y2,0);
      }
      if(potdeger>550){
        drawship(ship_y1,ship_y2,1);
        if(ship_y2!=64){
          ship_y1+=4;
          ship_y2+=4;
        }
        drawship(ship_y1,ship_y2,0);
      }
    digit3=j/100;
    digit2 = (j / 10)%10;
    digit1 = j % 10;
    for ( int k = 0; k < 20; k++){
      digitalWrite(13, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      dis(digit3);
      delay(1);

      digitalWrite(11, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(12, LOW);
      dis(digit2);
      delay(1);
      
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(11, LOW);
      dis(digit1);
      delay(3);

    }
  }

}

int x=0;
void loop() {
  if(digitalRead(buttonPin)==HIGH){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(15, 20);
    display.print("kolay mod icin 10 saniye icinde basiniz");
    display.display();
    delay(1000);
    for(int i=0;i<100;i++){
      if(digitalRead(buttonPin)==HIGH){
        display.clearDisplay();
        display.display();
        func();
        break;
      }
      delay(100);
    }
    meteorlist.removeAll();
    spacetrashlist.removeAll();
    extralifelist.removeAll();
    gunlist.removeAll();
    untouchablelist.removeAll();
    gunshotlist.removeAll();
    display.clearDisplay();
    display.display();
    
  }
  totallife=3;
  totalgun=1;
  meteorlist.removeAll();
  spacetrashlist.removeAll();
  extralifelist.removeAll();
  gunlist.removeAll();
  untouchablelist.removeAll();
  gunshotlist.removeAll();
  display.clearDisplay();
  display.display();

}

void drawship(int y1,int y2,int color){
  int y3=(y1+y2)/2;
  //display.clearDisplay();
  if(color==0){
    display.fillTriangle(128, y1, 128, y2, 120, y3, WHITE);
    ship_y1=y1;
    ship_y2=y2;
  }
  else
    display.fillTriangle(128, y1, 128, y2, 120, y3, BLACK);
  //display.drawPixel(127, 32, WHITE);
  display.display();
}


void dis(int num){
  for (int i = 2; i < 9; i++){
    digitalWrite(i, bitRead(digit[num], i - 2));
  }
}




 
