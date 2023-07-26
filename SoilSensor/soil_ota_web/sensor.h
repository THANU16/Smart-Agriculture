#include <SoftwareSerial.h>

#define RE1 13 //9
#define DE1 14 //10

const byte cond[] = { 0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xce };
const byte temp[] = { 0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0e };
const byte mois[] = { 0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0f };
const byte ph[] = { 0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b };

byte values[11];
SoftwareSerial mod1(33, 32);  //D7 D3,D1

float tem = 0.0;
int con = 0;
float phv = 0;
float moi = 0;

char tem_arr[6];
char con_arr[6];
char phv_arr[6];
char moi_arr[6];

void setup_sensor() {
  mod1.begin(9600);
  pinMode(RE1, OUTPUT);
  pinMode(DE1, OUTPUT);
}

int conductivity() {
  digitalWrite(DE1, HIGH);
  digitalWrite(RE1, HIGH);
  delay(10);
  if (mod1.write(cond, sizeof(cond)) == 8) {
    digitalWrite(DE1, LOW);
    digitalWrite(RE1, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod1.read();
    }
    con = values[4] + 256 * values[3];
    con *= 10;
  }
  for (byte i = 0; i < 9; i++) {
    values[i] = 0;
  }
  String(con).toCharArray(con_arr, 6);
  return con;
}


float phvalue() {
  digitalWrite(DE1, HIGH);
  digitalWrite(RE1, HIGH);
  delay(100);
  if (mod1.write(ph, sizeof(ph)) == 8) {
    digitalWrite(DE1, LOW);
    digitalWrite(RE1, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod1.read();
    }
    phv = values[4] + 256 * values[3];
    phv /= 100;
    for (byte i = 0; i < 9; i++) {
      values[i] = 0;
    }
  }
  String(phv).toCharArray(phv_arr, 6);
  return phv;
}

float moisture() {
  moi = values[4] + 256 * values[3];
  moi /= 10;
  for (byte i = 0; i < 9; i++) {
    values[i] = 0;
  }
  String(moi).toCharArray(moi_arr, 6);
  return moi;
}


float temperature() {
  digitalWrite(DE1, HIGH);
  digitalWrite(RE1, HIGH);
  delay(10);
  if (mod1.write(temp, sizeof(temp)) == 8) {
    digitalWrite(DE1, LOW);
    digitalWrite(RE1, LOW);
    for (byte i = 0; i < 9; i++) {
      values[i] = mod1.read();
    }
    tem = values[6] + 256 * values[5];
    tem /= 10;
  }
  String(tem).toCharArray(tem_arr, 6);
  return tem;
}

void dataRead_print() {
  Serial.print("PH: ");
  phvalue();
  delay(150);
  phvalue();
  phv += off_valPH;
  Serial.print(phv);
  Serial.println(" pH");
  delay(250);

  Serial.print("Conductivity: ");
  conductivity();
  delay(150);
  conductivity();
  con += off_valCON;
  Serial.print(con);
  Serial.println(" us/cm");
  delay(250);

  Serial.print("Temperature: ");
  temperature();
  delay(150);
  temperature();
  moisture();
  Serial.print(tem);
  Serial.println(" °C");
  delay(250);

  Serial.print("Moisture: ");
  moi += off_valMOI;
  Serial.print(moi);
  Serial.println(" %");
  Serial.println("================");
  Serial.println();
}