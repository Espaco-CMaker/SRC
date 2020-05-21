//atualizaEEPROM();
//leEEPROM();
//zeraEEPROM();


//quant_total = EEPROMReadInt(add1_EEPROM);
//EEPROMWriteInt(add1_EEPROM, quant_total);
int add_EEPROM = 0; // Primeiro Byte acionamentos
//int add2_EEPROM = 1 * sizeof(int); // Segundo Byte intervalos
//int add3_EEPROM = 2 * sizeof(int); // Terceiro Byte total_acionamento
//int add4_EEPROM = 3*sizeof(int);// Quarto Byte corte_infra

void atualizaEEPROM()
{
  char ltxt[30];

  EEPROM.put(add_EEPROM,dado);
  EEPROM.commit();  
  //EEPROMWriteInt(add1_EEPROM, acionamentos);
  if (DEBUG > 3)
  {
    sprintf(ltxt, "%s -> dado(%d Bytes)", __FUNCTION__, sizeof(dado));
    Serial.println(ltxt);
  }
  if (DEBUG > 1)
    imprimeLog("Dados gravados com sucesso!");
}
void leEEPROM()
{
  char ltxt[30];
  //acionamentos = EEPROMReadInt(add1_EEPROM);
  EEPROM.get(add_EEPROM,dado);
  if (DEBUG > 3)
  {
    sprintf(ltxt, "%s -> dado(%d Bytes)", __FUNCTION__, sizeof(dado));
    Serial.println(ltxt);
  }
  if (DEBUG > 1)
    imprimeLog("Dados lidos com sucesso!");
}

void zeraEEPROM()
{
  char ltxt[30];
  zeraVars();      
  EEPROM.put(add_EEPROM,dado);
  EEPROM.commit();  
  if (DEBUG > 3)
  {
    sprintf(ltxt, "%s -> Dados Zerados na EEPROM\n", __FUNCTION__);
    Serial.println(ltxt);
  }
  //leEEPROM();
  if (DEBUG > 1)
    imprimeLog("Dados Zerados na EEPROM!");
}
/*
void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
*/
