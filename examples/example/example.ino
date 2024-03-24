#include <Arduino.h>
#include <EEPROM_24CXX/EEPROM_24cXX.h>
EEPROM_24cXX eep(0x50,C02);//0x05 - i2c адрес, C02 - тип памяти (C01,C02,C04,C08, etc)

void setup() {
  Serial.begin(115200);
  eep.begin();
  uint16_t value = 10000;
  pinMode(LED_BUILTIN,OUTPUT);

  //ByteSize() возвращает количество байтов (ячеек) в чипе памяти, умножая type на 128
  uint16_t vals[eep.ByteSize()];

  //Запишем 1 байт памяти и выведем время затраченное на это
  static uint32_t start = millis();
  eep.WriteByte(0,200);// функция WriteByte() записывает однобайтовое число в данную ячейку
  Serial.print("Writing Byte took ");
  Serial.println(millis() - start);
  
  //Запишем 2 байта (при помощи функции WriteInt() запишем int значение) памяти и выведем время затраченное на это
  start = millis();
  eep.WriteInt(0,value); 
  // функция WriteInt() разбивает число типа int на два байта 
  // и записывает их в ячейки по данному адресу, начиная со старшего.
  Serial.print("Writing Int took ");
  Serial.println(millis() - start);

  //заполняем чип памяти числами типа int, тк int занимает 2 байта,
  //то в памяти чипа поместится количество чисел типа int равное числу байтов (ячеек) поделенное на 2
  start = millis();
  for(int addr = 0; addr < eep.ByteSize() - 1; addr += 2){
    eep.WriteInt(addr,value);
    value += 10;
  }
  //по окончании записи выведем время, затраченное на запись и последнее записанное число
  Serial.print("Writing took ");
  Serial.println(millis() - start);
  Serial.print("Final value ");
  Serial.println(value - 10);
  start = millis();

  //далее побайтово считаем чип в заданный рание массив
  for(int addr = 0; addr < 255; addr += 2){
    //функция ReadInt() преобразует байт по переданному адресу и байт стоящий за ним в число типа int
    vals[addr / 2] = eep.ReadInt(addr);    
  }
  //в самом конце выведем время затраченное на чтение и сами прочитанные числа
  Serial.print("Reading took ");
  Serial.println(millis() - start);
  start = millis();
  for(int addr = 0; addr < 127; addr += 1){
    Serial.print(vals[addr]);
    Serial.print(' ');
  }
  Serial.print('\n');
  //по завершении работы зажжем светодиод
  digitalWrite(LED_BUILTIN,1);
}

void loop() {
  // put your main code here, to run repeatedly:
}
