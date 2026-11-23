const int BOTAO = 5;
const int RELE = 18;
const int LED_MOTOR = 2;
const int LED_ENERGIA = 13;

bool motorLigado = false;
int leituraAnterior = HIGH;
int estadoBotao = HIGH;
unsigned long tempoDebounce = 0;
const unsigned long intervaloDebounce = 50;

void ligarRele() {
  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, LOW);   // puxa o IN para GND: relé liga
}

void desligarRele() {
  pinMode(RELE, INPUT);      // solta o IN: o módulo desliga sozinho
}

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(LED_MOTOR, OUTPUT);
  pinMode(LED_ENERGIA, OUTPUT);

  desligarRele();                 // começa com o motor desligado
  digitalWrite(LED_MOTOR, LOW);
  digitalWrite(LED_ENERGIA, HIGH);

  Serial.println("Sistema da escada rolante iniciado");
}

void loop() {
  int leitura = digitalRead(BOTAO);

  if (leitura != leituraAnterior) {
    tempoDebounce = millis();
  }

  if (millis() - tempoDebounce > intervaloDebounce) {
    if (leitura != estadoBotao) {
      estadoBotao = leitura;

      if (estadoBotao == LOW) {
        motorLigado = !motorLigado;

        if (motorLigado) {
          ligarRele();
          digitalWrite(LED_MOTOR, HIGH);
          Serial.println("MOTOR LIGADO");
        } else {
          desligarRele();
          digitalWrite(LED_MOTOR, LOW);
          Serial.println("MOTOR DESLIGADO");
        }
      }
    }
  }

  leituraAnterior = leitura;
}
