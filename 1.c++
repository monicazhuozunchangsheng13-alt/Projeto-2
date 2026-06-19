#include <WiFi.h>// permite que ESP32 conectar-se ao roteador e obter IP
#include <WebServer.h>//Fornece as funções necessárias para criar um servidor HTTP, permitindo que o ESP32 receba comando de navegadore

//O ESP32 precisa deles para se autenticar no roteador.
const char* ssid = "WIFI"; 
const char* password = "79296000";

//Cria o servidor web escutando na porta 80, que é a porta padrão para navegação HTTP.
WebServer server(80);

// Pino do LED configurado para o pino 25
const int ledPin = 25;

// Função da página principal (HTML)
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Controle de LED</title>";
  html += "<style>body { font-family: Arial; text-align: center; margin-top: 50px; }";
  html += ".btn { display: inline-block; padding: 15px 25px; font-size: 24px; margin: 10px; cursor: pointer; text-decoration: none; color: white; border-radius: 5px; }";
  html += ".on { background-color: #4CAF50; }";
  html += ".off { background-color: #f44336; }</style></head>";
  html += "<body><h1>Controle de LED - ESP32</h1>";
  html += "<p><a class='btn on' href='/acender'>Acender LED</a></p>";
  html += "<p><a class='btn off' href='/apagar'>Apagar LED</a></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);//envia a página de volta para o navegador com o código de sucesso
}

// Função para acender o LED
void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  Serial.println("Comando recebido: LED Ligado");
  server.sendHeader("Location", "/");
  server.send(303); 
}

// Função para apagar o LED
void handleLedOff() {
  digitalWrite(ledPin, LOW);
  Serial.println("Comando recebido: LED Desligado");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound() {
  server.send(404, "text/plain", "Pagina nao encontrada");
}

void setup() {
  Serial.begin(115200);
  delay(1000); 
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 

  Serial.println("\n--- Iniciando Sistema ---");
  
  WiFi.disconnect(true);
  delay(1000);
  
  Serial.println("Tentando conectar ao Wi-Fi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 30) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[SUCESSO] Wi-Fi conectado!!!");
    Serial.print("-> Copie este IP e digite no navegador: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, handleRoot);
    server.on("/acender", HTTP_GET, handleLedOn);
    server.on("/apagar", HTTP_GET, handleLedOff);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("Servidor HTTP iniciado com sucesso!");
  } else {
    Serial.println("\n[ERRO] Nao foi possivel conectar ao Wi-Fi.");
    Serial.println("Verifique se o nome da rede e a senha estao corretos e se a rede e de 2.4 GHz.");
  }
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }
}