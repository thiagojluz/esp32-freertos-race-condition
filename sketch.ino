/* * Universidade Federal do Paraná (UFPR) - DELT
 * Disciplina: Sistemas Operacionais Embarcados
 * Prática 2: Multitarefa, Afinidade de Núcleo e Condição de Corrida
 * Professor: Thiago José da Luz, PhD
 * Data: 28/03/2026
 * * OBJETIVO DIDÁTICO:
 * Demonstrar o escalonamento de múltiplas tarefas (Tasks) no ESP32 utilizando 
 * as APIs do FreeRTOS. O código foca na distribuição de carga entre os núcleos 
 * (Core 0 e Core 1) e evidencia o conflito de acesso ao hardware (LCD) compartilhado.
 */


#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <LiquidCrystal.h>
#include <DHTesp.h>

// Definições de Hardware
#define slider 34     // Entrada Analógica (Potenciômetro)
#define temp 15       // Pino de dados DHT22
#define PinLed 32     // LED de status
#define DHT_PIN temp

// Barramento Paralelo do LCD (Modo 4 bits)
#define RS 19
#define E 18
#define D4 5
#define D5 17
#define D6 16
#define D7 4
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

DHTesp dhtSensor;

// Protótipos das Tasks
void TaskBlink(void* pvParameters);
void TaskSlider(void* pvParameters);
void TaskTemp(void* pvParameters);

void setup() {
  Serial.begin(115200);
  
  // Task 1: Criada sem afinidade fixa (escalonador decide o núcleo livre)
  xTaskCreate(TaskBlink, "Blink", 10000, NULL, 2, NULL);

  // Task 2: Pinned ao Core 0 - Foco em leitura de entrada analógica
  xTaskCreatePinnedToCore(TaskSlider, "Slider", 10000, NULL, 1, NULL,0);

  // Task 3: Pinned ao Core 1 - Foco em leitura de sensor digital (DHT22)
  xTaskCreatePinnedToCore(TaskTemp, "Temperatura", 10000, NULL, 1, NULL,1);

  // Inicialização de Periféricos
  lcd.begin(16, 2);
  lcd.clear();
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  // Loop vazio: Toda a lógica é gerenciada pelas Tasks do FreeRTOS
}

/* TASK SLIDER (Core 0): Gerencia a leitura do potenciômetro e escreve na Linha 0 */
void TaskSlider(void* pvParameters) {
  (void) pvParameters;
  vTaskDelay(500/portTICK_PERIOD_MS); // Offset inicial para setup
  lcd.setCursor(0, 0);
  lcd.print("Potenc.:");

  for(;;){
    // leia a entrada analógica no pino A0
    int sensor = analogRead(slider);
    // Converte o valor para percentual
    sensor = map(sensor, 0,4096,0,100);

    // ATENÇÃO: Acesso direto ao LCD sem Mutex pode gerar Condição de Corrida
    lcd.setCursor(8, 0);
    lcd.print(sensor);
    lcd.print("%   "); // Limpa caracteres residuais
    // um atraso de tique (15 ms) entre as leituras para estabilidade
    vTaskDelay(500/portTICK_PERIOD_MS); // Bloqueio da Task por 500ms 
  }
}

/* TASK TEMP (Core 1): Gerencia o sensor DHT22 e escreve na Linha 1 */
void TaskTemp(void* pvParameters) {
  (void) pvParameters;
  vTaskDelay(5000/portTICK_PERIOD_MS); // Atraso proposital para alternar escrita
  lcd.setCursor(0, 1);
  lcd.print("Temp.:");
  int sensor;

  for(;;){
    // ATENÇÃO: Task concorrente tentando acessar o barramento do LCD simultaneamente
    lcd.setCursor(8, 1);
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();
    lcd.print(String(data.temperature, 2));
    lcd.write(0xDF); // Símbolo de grau
    lcd.print("C");
    // um atraso de tique (15 ms) entre as leituras para estabilidade
    vTaskDelay(500/portTICK_PERIOD_MS); // Bloqueio da Task por 500ms
}
}

/* TASK BLINK (Dinâmica): Reporta o Núcleo de Execução via Serial */
void TaskBlink(void* pvParameters) {
  (void) pvParameters;

  pinMode(PinLed, OUTPUT);
  for (;;) 
  {
    digitalWrite(PinLed , HIGH); // Liga LED
    // xPortGetCoreID identifica em qual núcleo o Scheduler posicionou a tarefa
    printf("Taks LED executado no núcleo: %d \n",xPortGetCoreID());

    vTaskDelay(1000/portTICK_PERIOD_MS); // Bloqueio da Task por 1000ms

    digitalWrite(PinLed , LOW); // Desliga LED
    printf("Taks LED executado no núcleo: %d \n",xPortGetCoreID());

    vTaskDelay(1000/portTICK_PERIOD_MS); // Bloqueio da Task por 1000ms
  
  }
}