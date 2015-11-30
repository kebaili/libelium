/*
 *  envoi de données de la carte agriculture version 1 à travers xBee
 *
 *  Implementation:         Mohamed Omar Kebaili        CONTOS²
 */


packetXBee* paq_sent; 
char aux[200];
char* macHigh="          ";
char* macLow="           ";
float pression =0;
float temperature=0;
float luminosite=0;
float moisture=0;

#define key_access "LIBELIUM"

uint8_t direccion[8]={0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF};

void setup(){
SensorAgr.setBoardMode(SENS_ON); 
SensorAgr.setSensorMode(SENS_ON,SENS_AGR_PRESSURE);
SensorAgr.setSensorMode(SENS_ON,SENS_AGR_TEMPERATURE);
SensorAgr.setSensorMode(SENS_ON, SENS_AGR_WATERMARK_1);
SensorAgr.setSensorMode(SENS_ON, SENS_AGR_LEAF_WETNESS);
  // Store key access in EEPROM
  for(int i=0;i<8;i++){
    Utils.writeEEPROM(i+107,key_access[i]);
  }

  RTC.ON();
  
  ACC.ON();

  // Inits the XBee 802.15.4 library 
  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL); 
 
  // Powers XBee 
  xbee802.ON();
  
  // Get the XBee MAC address

  delay(500);
  
  int counter = 0;  
  while(xbee802.getOwnMac()==1&&counter<4){
    xbee802.getOwnMac();
    counter++;
  }
  
  Utils.hex2str(xbee802.sourceMacHigh,macHigh,4);
  Utils.hex2str(xbee802.sourceMacLow,macLow,4);
  

}

void loop(){

   pression=SensorAgr.readValue(SENS_AGR_PRESSURE);
   temperature=SensorAgr.readValue(SENS_AGR_TEMPERATURE);
   moisture=SensorAgr.readValue(SENS_AGR_WATERMARK_1);
   luminosite=SensorAgr.readValue(SENS_AGR_LEAF_WETNESS);

   sprintf(aux,"mac:%s%s bat:%d x:%d y:%d z:%d temp:%d pres:%d moist:%d lum:%d\r\n",macHigh,macLow,(int)PWR.getBatteryLevel(),(int)ACC.getX(),(int)ACC.getY(),(int)ACC.getZ(),(int)temperature,(int)pression,(int)moisture,(int)luminosite);
  
  paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
  paq_sent->mode=BROADCAST; 
  paq_sent->MY_known=0; 
  paq_sent->packetID=0x52; 
  paq_sent->opt=0; 
  xbee802.hops=0; 
  xbee802.setOriginParams(paq_sent,MAC_TYPE); 
  xbee802.setDestinationParams(paq_sent, direccion, aux, MAC_TYPE, DATA_ABSOLUTE);
  xbee802.sendXBee(paq_sent); 
  
  free(paq_sent);
  paq_sent = NULL;
  
  delay(10000);
}

