#include <Arduino.h>
#include <EEPROM_24CXX/EEPROM_24cXX.h>
EEPROM_24cXX eep(0x50,C64);//0x05 - i2c адрес, C02 - тип памяти (C01,C02,C04,C08, etc)

void setup() {
  Serial.begin(115200);
  eep.begin();
  uint16_t value = 0;
  const uint16_t value_initial = value;
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.print("Chip size (bytes) ");
  Serial.println(eep.ByteSize());// ByteSize() возвращает количество байтов (ячеек) в чипе памяти, умножая type на 128.

  // создаем массив чисел типа int длинной равной половине количества байтов в чипе, тк числа типа int имеют объем 2 байта
  // то массив займет в памяти МК объем равный объему чипа EEPROM
  uint16_t vals[eep.ByteSize() / 2];

  //Запишем 1 байт памяти и выведем время затраченное на это
  static uint32_t start = millis();
  eep.WriteByte(0,200);// функция WriteByte() записывает однобайтовое число в данную ячейку
  Serial.print("Writing Byte took ");
  Serial.println(millis() - start);
  
  //Запишем 2 байта (при помощи функции WriteInt() запишем int значение) памяти и выведем время затраченное на это
  start = millis();
  eep.WriteInt(0,12345); 
  // функция WriteInt() разбивает число типа int на два байта 
  // и записывает их в ячейки по данному адресу, начиная со старшего.
  Serial.print("Writing Int took ");
  Serial.println(millis() - start);

  start = millis();
  eep.WriteFloat(0,123.45); 
  // функция WriteFloat() разбивает число типа float на четыре байта 
  // и записывает их в ячейки по данному адресу, начиная со старшего.
  Serial.print("Writing Float took ");
  Serial.println(millis() - start);

  //заполняем чип памяти числами типа int, тк int занимает 2 байта,
  //то в памяти чипа поместится количество чисел типа int равное числу байтов (ячеек) поделенное на 2
  Serial.println("Writing full start");
  start = millis();
  for(uint16_t addr = 0; addr < eep.ByteSize(); addr += 2){
    eep.WriteInt(addr,value);
    value += 1;
    // digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN)); // во время записи будем мигать светодиодом
    // Serial.println(addr);
  }
  // digitalWrite(LED_BUILTIN,0);
  Serial.print('\n');
  //по окончании записи выведем время, затраченное на запись, первое и последнее записанное число
  Serial.print("Writing took ");
  Serial.println(millis() - start);
  Serial.print("Initial value ");
  Serial.println(value_initial);
  Serial.print("Final value ");
  Serial.println(value - 10);  


  //далее побайтово считаем чип в заданный ранее массив
  start = millis();
  for(uint16_t addr = 0; addr < eep.ByteSize() / 2; addr ++){
    //функция ReadInt() преобразует байт по переданному адресу и байт стоящий за ним в число типа int
    vals[addr] = eep.ReadInt(addr * 2);    
  }
 
  //в самом конце выведем сами прочитанные числа и время затраченное на чтение
    for(uint16_t addr = 0; addr < sizeof(vals) / 2; addr ++){
    Serial.print(vals[addr]);
    Serial.print(' ');
  }  
  Serial.print('\n');
  Serial.print("Reading took ");
  Serial.println(millis() - start);
  //по завершении работы зажжем светодиод
  digitalWrite(LED_BUILTIN,1);
}

void loop() {
  // put your main code here, to run repeatedly:
}