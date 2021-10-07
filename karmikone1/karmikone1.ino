#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 0
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE,15);

const char* ssid = "GalaxyA313";
const char* password = "1234567891";


WiFiServer server(80);
int led = 2;
int brightness = 0;

void setup()
{
  Serial.begin(115200);
  delay(100);
  
  dht.begin();
  delay(100);

  pinMode(led,OUTPUT);
  digitalWrite(led,0);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected.");

  server.begin();
  Serial.println("Server Started");

  Serial.println(WiFi.localIP());
}

void loop() 
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  Serial.println("new client");

  while (!client.available())
  {
    delay(1);
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  Serial.println("humadity");
  Serial.println(h);
  Serial.println("temperature");
  Serial.println(t);
  
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  //-----------------------------------------
  if (req.indexOf("/on") != -1)
  {
    brightness = 255;  
  }
  else if (req.indexOf("/off") != -1)
  {
    brightness = 0;
  }
  
  if (req.indexOf("/increase") != -1)
  {
    brightness += 20;  
  }
  else if (req.indexOf("/decrease") != -1)
  {
    brightness -= 20;
  }
  
  if(brightness > 255)
  {
    brightness = 255;
  }
  else if(brightness < 0)
  {
    brightness = 0;
  }
  
  delay(25);
  analogWrite(led,brightness);
  //-----------------------------------------
  client.flush();
  //-----------------------------------------

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<head>";
  s += "<meta charset=\"utf-8\">";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
  s += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
  s += "</head>";
  
  s += "<body>";
  s += "<div>";
  s += "<div style=\"text-align:center;width:100%;padding:20px;background-color:#3EECAC\">";
  s += "<div class=\"row\" style=\"text-align:center\">";
  s += "<h1 class=\"col-md-12\">Embeded Systems Project</h1>";
  s += "<div class=\"col-md-3\"></div>";
  s += "<h3 class=\"col-md-3\">Sadra Heidari Moghadam</h3>";
  s += "<h3 class=\"col-md-3\">Mobina Kashanian</h3>";
  s += "<div class=\"col-md-3\"></div>";
  s += "</div>";
  s += "</div>";
  s += "<div style=\"text-align:center\">";
  
  s += "<h3 style=\"margin-top:40px\">ON/OFF</h3>";
  s += "<div class=\"row\">";
  s += "<div class=\"col-md-3\"></div>";
  s += "<div class=\"col-md-3\">";
  s += "<input class=\"btn btn-block btn-lg btn-success\" style=\"margin:auto;margin-top:30px;font-size:28px;border-radius:100%;height:100px;width:100px;\"type=\"button\" value=\"ON\" onclick=\"on()\">";
  s += "</div>";
  s += "<div class=\"col-md-3\">";
  s += "<input class=\"btn btn-block btn-lg btn-danger\" style=\"margin:auto;margin-top:30px;font-size:28px;border-radius:100%;height:100px;width:100px;\" type=\"button\" value=\"OFF\" onclick=\"off()\">";
  s += "</div>";
  s += "<div class=\"col-md-3\"></div>";
  s += "</div>";
  s += "<div style=\"margin-top:40px; height:3px; width:100%; background-color:#3EECAC\"></div>";
  s += "<h3 style=\"margin-top:40px\">Brightness Control</h3>";
  s += "<div class=\"row\">";
  s += "<div class=\"col-md-3\"></div>";
  s += "<div class=\"col-md-3\">";
  s += "<input class=\"btn btn-block btn-lg btn-success\" style=\"margin:auto;margin-top:30px;font-size:28px;border-radius:100%;height:100px;width:100px;\"type=\"button\" value=\"+\" onclick=\"increase()\">";
  s += "</div>";
  s += "<div class=\"col-md-3\">";
  s += "<input class=\"btn btn-block btn-lg btn-danger\" style=\"margin:auto;margin-top:30px;font-size:28px;border-radius:100%;height:100px;width:100px;\" type=\"button\" value=\"-\" onclick=\"decrease()\">";
  s += "</div>";
  s += "<div class=\"col-md-3\"></div>";
  s += "</div>";
  s += "<div style=\"margin-top:40px; height:3px; width:100%; background-color:#3EECAC\"></div>";
  s += "<h3 style=\"margin-top:40px\"> Wireless Meteorology</h3>";
  s += "<div class=\"row voffset\" style=\"margin-top:40px\">";
  s += "<h4 class=\"col-md-3\">Temperature: </h4><h4 class=\"col-md-3\">" + String(t) + "</h4>";
  s += "<h4 class=\"col-md-3\">Humidity: </h4><h4 class=\"col-md-3\">" + String(h) + "</h4>";
  s += "</div>";
  s += "</div>";
  s += "</div>";
  s += "<script>function on() {$.get(\"/on\");}</script>";
  s += "<script>function off() {$.get(\"/off\");}</script>";
  s += "<script>function increase() {$.get(\"/increase\");}</script>";
  s += "<script>function decrease() {$.get(\"/decrease\");}</script>";
  s += "</body>";
  client.print(s);
  delay(1);

  Serial.println("disconnected");

  
}
