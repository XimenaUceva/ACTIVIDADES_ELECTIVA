#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>


#define DHTPIN 13
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
int temp;
int hum;

/*
INICIALIZA EL DISPOSITIVO
PUERTO SRIAL VELOCIDAD
DIRECCIONES DE PINES DE ENTRADA Y SALIDA
CREAR OBJETOS QUE SE REPRESENTAN SENSORES, DISPOSITIVOS WIFI Y OBJETOS DE LIBRERIAS EXTERNAS
*/

const char* ssid = "ZANTY";
const char* pass = "santi0804";
const char* host = "dweet.io";
const char* pagina  = "unidadcentral" ;
const char* contenido = "Temperatura";
const char* contenidoH = "&Humedad";
const int port = 80; // puerto por defecto del servicio web

void setup() {
  Serial.begin(115200); //configurar 115200 bits por segundo velocidad
  WiFi.begin(ssid, pass); // para vincular esp a la red asignada
  Serial.print("Se está conectando a la red WiFi denominada ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
  Serial.print("conectado exitosamente!!");
  Serial.print("direccion IP: ");
  Serial.println(WiFi.localIP());

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {

  delay(2000);
  WiFiClient cliente; //objeto que realiza peticiones a un socket servidor

  if(!cliente.connect(host, port)){
    Serial.println("conexion fallida");
    delay(1000);
    return;
  }

  float hum = dht.readHumidity(); // Lea la humedad relativa
  float temp = dht.readTemperature(); // Leo la temperatura en grados celsius

  if (isnan (hum)||isnan (temp)) {
    Serial.println("Error al leer el sensor DTH22");
    return;
  }

  //si llega a esta linea es porque si hubo conexion con el servidor
  //ahora tenemros un flujo de transmision y otro de recepcion.
   //temp = random (20,100);
    //hum = random (10,50);
    

  //Serial.println(http);
  String request = String("GET /dweet/for/unidadcentral") + "?" + contenido + "=" + temp + contenidoH + "=" + hum + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "connection: keep alive\r\n\r\n"; //parametro de cabcera
  cliente.print(request);

  //enviamos la peticion en protocolo http al servidor

 //cliente.print(String("GET ") + http + " HTTP/1.1\r\n" +
               //"Host: " + host + "\r\n" + 
               //"Connection: keep alive\r\n\r\n");
  
  unsigned long inicio = millis();
  while(cliente.available() == 0){
    if(millis() - inicio > 2000){
      Serial.println("tiempo de espera del servidor agotado");
      cliente.stop();
      return;
    }
  }


  while (cliente.available()) {
    String linea= cliente.readStringUntil('\r'); //lea una linea hasta que encuentre el caracter enter \r
    Serial.println(linea);
      }
  
    Serial.println("fin de conexion al servidor");
    cliente.stop();
    delay(2000); 
}