/*
######################################################
# Этот пример был модифицирован для работы с чипами, #
# объём которых в байтах превышает или равен 2Кб     #
# (24c16, 24c32, 24c64, 24c128, 24c256, 24c512)      #
######################################################
*/

#include <Arduino.h>
#include <EEPROM_24CXX/EEPROM_24cXX.h>
// У ATmega328 И LGT8F328 ОБЪЕМ ОЗУ РАВЕН 2КБ. ПОЭТОМУ МАКСИМАЛЬНАЯ ДЛИНА МАССИВА ТИПА INT СОСТАВЛЯЕТ 1024 ЗНАЧЕНИЯ ИЛИ 2048Б (2048КБ)
// ОДНАКО ПОМИМО МАССИВА В ОПЕРАТИВНОЙ ПАМЯТИ ХРАНИТСЯ САМА ПРОГРАММА, ПОЭТОМУ РАЗУМНО СОЗДАТЬ МАССИВ НА 512 ЗНАЧЕНИЙ (1024Б ИЛИ 1КБ) И ОСТАВИТЬ ЕЩЕ ОДИН КБ НА НУЖДЫ ПРОГРАММЫ

#define vals_size 512 // ДЛИНА В КОТОРЫЙ БУДЕМ СЧИТЫВАТЬ ЧИП.
EEPROM_24cXX eep(0x50,C256);//0x05 - i2c адрес, C256 - тип памяти (C16, C32, C64, C128 etc)

void setup() {
  Serial.begin(115200);
  eep.begin();
  uint16_t value = 0;
  const uint16_t value_initial = value;
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.print("Chip size (bytes) ");
  Serial.println(eep.ByteSize());// ByteSize() возвращает количество байтов (ячеек) в чипе памяти, умножая type на 128.

  // ОБЪЕМ ЧИПА 24С256 СОСТАВЛЯЕТ 32 КБ, ЧТО НЕ ВМЕЩАЕТСЯ В ОЗУ ATmega328 и LGT8F328,
  // ПОЭТОМУ МЫ ЗАДАДИМ МАССИВ НА 512 ЗНАЧЕНИЙ ТИПА uint16_t И ОБЪЕМОМ 1024Б.
  // ТК ОБЪЕМ ЧИПА ПРЕВЫШАЕТ ОБЪЕМ ОЗУ ДАННЫЕ ИЗ НЕГО НЕВОЗМОЖНО ВЫГРУЗИТЬ В ОЗУ ЦЕЛИКОМ, ПОЭТОМУ БУДЕМ РАБОТАТЬ С НИМИ КУСКАМИ.
  uint16_t vals[vals_size];   
  
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


  // далее побайтово считаем чип в заданный ранее массив и выведем данные из него
  // ТК МАССИВ МЕНЬШЕ ЧИПА ТО МЫ БУДЕМ ЕГО ПЕРЕЗАПИСЫВАТЬ ПО ОКОНЧАНИИ МЕСТА И РАБОТАТЬ С ЧИПОМ КУСКАМИ
  uint32_t readtime = 0; 
  for(uint16_t i = 0; i < eep.ByteSize() / vals_size; i++){     
    Serial.println(vals_size*i); // выведем значение сдвига
    start = millis();
    for(uint16_t addr = 0; addr < vals_size / 2; addr++){
    //функция ReadInt() преобразует байт по переданному адресу и байт стоящий за ним в число типа int
      vals[addr] = eep.ReadInt(addr * 2 + vals_size * i);  
      // переменная i хранит номер текущей итерации перезаписи массива,
      // уможив ее на размер массива (sizeof(vals)) мы получим сдвиг необходимый для последовательного считывания. addr необходимо
      // умножить на 2 тк функция ReadInt преобразует текущий байт и байт за ним в одно число.    
    }    
    // по окончании чтения выведем числа
    readtime += millis() - start; // добавим время чтения текущего куска, без учета отправки его в порт
    for(int addr = 0; addr < vals_size / 2; addr++){
      Serial.print(vals[addr]);
      Serial.print(' ');
    }      
    Serial.print('\n');
  }
  
  //в самом конце выведем время затраченное на чтение
  Serial.print("Reading took ");
  Serial.println(readtime);
  //по завершении работы зажжем светодиод
  digitalWrite(LED_BUILTIN,1);
}

void loop() {
  // put your main code here, to run repeatedly:
}