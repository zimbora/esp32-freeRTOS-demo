
#include "../../../package.h"
#ifdef APP_TEST

#include "app.h"

extern CALLS call;
extern SENSORS sensors;
extern DynamicJsonDocument doc; // json

app_settings app_s = {
  .fw = {
    /* version */   APP_VERSION,
    /* md5 */       ""
  },
};

uint32_t last_data_send = 0;

void APP::init(){

  Serial.println("Init app test module");

  load_settings();
  log_settings();

}

void APP::loop(){

}

/*
* if topic ends with '/get', it should be removed from topic avoiding loops
* if topic ends with '/set', an unpublish should be sent
*/
void APP::parse_mqtt_messages(uint8_t clientID, String topic, String payload){

  DBGLOG(Debug,"app topic: "+topic);
  DBGLOG(Debug,"payload: "+payload);
  String subtopic = "";
  bool set = false;
  bool get = false;
  bool store = false;

  if(topic.endsWith("/set")){
    set = true;
    if(payload == "")
      return;
  }

  if(topic.endsWith("/get")){
    get = true;
    uint16_t index = topic.lastIndexOf("/");
    subtopic = topic.substring(0,index); // filter get
  }

  switch(resolveOption(appTopics,topic)){
    case settings_reset_set_:
      reset_settings();
  }

  if(store){
    store_settings();
    log_settings();
  }

  if(set)
    core_send_mqtt_message(clientID,topic,"",0,true); // unpublish

}

appTopics_ APP::resolveOption(std::map<long, appTopics_> map, String topic) {

  std::string topic_ = std::string(topic.c_str());
  long str_hash = (long)std::hash<std::string>{}(topic_);
  std::map<long,appTopics_>::iterator it;

  it = map.find(str_hash);
  if(it != map.end())
    return it->second;

  return app_not_found;
}

bool APP::load_settings(){
  uint16_t len = sizeof(app_s);
  //TickType_t xBlockTime = pdMS_TO_TICKS(100);  // Specify a timeout
  //char* data = (char*)pvPortMalloc(len);
  char data[len];
  //if(data != nullptr){
    call.read_file(APP_SETTINGS_FILENAME,data,&len);
    memcpy(app_s.fw.version,data,sizeof(app_s.fw.version));
    String version = String(app_s.fw.version);
    DBGLOG(Info,"fw version: "+version);
    if(version.startsWith("0.") || version.startsWith("1.") || version.startsWith("2."))
      memcpy(app_s.fw.version,data,sizeof(app_s));
    else{
      String version = APP_VERSION;
      memcpy(app_s.fw.version,version.c_str(),version.length());
      store_settings();
    }
    //vPortFree(data);
  //}
  return true;
}

bool APP::store_settings(){

  Serial.printf("filename %s \n",APP_SETTINGS_FILENAME);
  if(!call.write_file(APP_SETTINGS_FILENAME,app_s.fw.version,sizeof(app_s))){
    DBGLOG(Error,"failing writing file: "+String(APP_SETTINGS_FILENAME));
    return false;
  }
  return true;
}

bool APP::reset_settings(){
  memset(app_s.fw.version,0,sizeof(app_s.fw.version));
  if(call.write_file(APP_SETTINGS_FILENAME,app_s.fw.version,sizeof(app_s)))
    call.fw_reboot();
}

void APP::log_settings(){

  DBGLOG(Debug,"fw.version: "+String(app_s.fw.version));
  DBGLOG(Debug,"fw.md5: "+String(app_s.fw.md5));
}

bool APP::checkStringIsNumerical(String myString){

  uint16_t Numbers = 0;

  for(uint16_t i = 0; i < myString.length(); i++)
  {
     if (myString[i] == '0' || myString[i] == '1' || myString[i] == '2' || myString[i] == '3' || myString[i] == '4' || myString[i] == '5' ||
         myString[i] == '6' || myString[i] == '7' || myString[i] == '8' || myString[i] == '9')
    {
        Numbers ++;
    }
  }

  return Numbers == myString.length();
}

#endif
