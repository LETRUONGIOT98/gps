
#include <SoftwareSerial.h> 
#include <TinyGPS.h> 
float lat = 28.5458,lon = 77.1703; // create variable for latitude and longitude object  
TinyGPS gps; // create gps object 
SoftwareSerial gpsSerial(3,2);//rx,tx(Chân D3 nối với RX Module Neo6M, chân 4 nối với TX Module Neo6M
SoftwareSerial sim(11, 10); //Chân 10 nối với RX simL và chân 11 với TX
int interrupt =0; //Chân out Shock sensor nối với chân 2(D2) của Arduino
#define coi 13
int i=0;
long time;
int donhay = 15; //Cỉnh độ nhạy cảm biến, số càng lớn càng ít nhạy
void setup(){ 
  pinMode(coi, OUTPUT);
  pinMode(2, INPUT_PULLUP);
Serial.begin(9600); // connect serial 
//Serial.println("The GPS Received Signal:"); 
//delay(10000); //Delay 10s để chờ các module khác khởi động
  gpsSerial.begin(9600); // connect gps sensor 
  delay(1000);
sim.begin(9600);
delay(2000);
//Các lệnh kiểm tra sim
  sim.println("AT");
  delay(100);
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CNMI=1,2,0,0,0");
  digitalWrite(coi, HIGH);
  delay(1000);
  attachInterrupt(interrupt,dem, RISING);  //Hàm ngắt để đọc tín hiệu từ shock sensor
  digitalWrite(coi, LOW);
  time = millis();
} 
void loop(){ 
  if(millis() - time > 1000){
    i=0;
  }
  while(gpsSerial.available()){ // check for gps data 
  if(gps.encode(gpsSerial.read()))// encode gps data 
  {  
  gps.f_get_position(&lat,&lon); // get latitude and longitude 
  Serial.print("Position: "); 
  Serial.print("Latitude:"); 
  Serial.print(lat,6); 
  Serial.print(";"); 
  Serial.print("Longitude:"); 
  Serial.println(lon,6);  
 } 
} 
String latitude = String(lat,6); 
  String longitude = String(lon,6); 
Serial.println(latitude+";"+longitude); 
delay(1000); 
} 
void phanhoi() {   //Hàm con khi có các lệnh bên trên thực hiện thì hàm này đc gọi lên để gửi tin nhắn về điện thoại
  Serial.println("DANG GUI CANH BAO VE CHO NGUOI THAN");
  delay(100);
  digitalWrite(coi, HIGH);
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"0365801715\"\r");   //Số điện thoại nhận tin nhắn cảnh báo
  delay(1000);
    sim.print("http://maps.google.com/maps?q=loc:");
    sim.print(lat);
    sim.print(",");
    sim.print(lon);
    delay(3000);
    Serial.println("DA GUI CANH BAO XONG");
   delay(100);
  sim.println((char)26);
  delay(1000);
  digitalWrite(coi, LOW);
  }
void dem(){
  time = millis();
  i = i+1;
  delay(1);
  if(i == donhay){
    phanhoi();
  }
}
