# Prática 2: Multitarefa e Condição de Corrida com FreeRTOS (ESP32) 🚀
### Este repositório apresenta o desenvolvimento da Prática 2 da disciplina de Sistemas Operacionais Embarcados na UFPR. O projeto explora o escalonamento de tarefas em múltiplos núcleos e os riscos de acesso compartilhado a periféricos sem sincronização.
## 🎯 Objetivos Didáticos
1. **Criação de Tasks:** Implementação de tarefas com diferentes níveis de prioridade e afinidade de núcleo.
2. **Arquitetura Dual-Core:** Demonstração do uso de `xTaskCreatePinnedToCore` para distribuir processamento entre o Core 0 e Core 1 do ESP32.
3. **Identificação de Race Condition:** Evidenciar como o acesso simultâneo de duas tarefas (`TaskSlider` e `TaskTemp`) ao mesmo recurso físico (LCD via barramento paralelo) corrompe a comunicação e a exibição de dados.
## ⚙️ Regras e Implementação
* **Task 1 (Blink):** Criada sem núcleo definido, utilizando `xPortGetCoreID()` para reportar via Serial em qual core o escalonador a posicionou a cada ciclo.
* **Task 2 (Slider - Core 0):** Leitura analógica de um potenciômetro e atualização da linha 0 do LCD.
* **Task 3 (Temperatura - Core 1):** Leitura de um sensor **DHT22** e atualização da linha 1 do LCD.
* **Interface:** Uso de biblioteca `LiquidCrystal` em modo 4 bits (sem I2C), forçando a manipulação direta de pinos de dados e controle.
## 🛠️ Hardware Utilizado (Simulado no Wokwi)
* **Microcontrolador:** ESP32.
* **Sensores:** DHT22 (Temperatura) e Potenciômetro (Slider).
* **Display:** LCD 16x2 em modo paralelo.
## 🚀 Como Executar
1. O código principal está localizado no arquivo sketch.ino.
2. As conexões de hardware estão descritas no arquivo diagram.json.
3. As bibliotecas estão especificadas no arquivo libraries.txt 
4. **Simulação Online:** Você pode testar o comportamento do sistema diretamente no navegador:
    * ▶️ [Link para a Simulação no Wokwi](https://wokwi.com/projects/436002682510007297) 

## ⚠️ Análise Técnica: O Problema da Concorrência
Nesta implementação, ambas as tarefas de núcleo fixo tentam realizar operações de escrita no LCD (`lcd.setCursor` e `lcd.print`) simultaneamente. Como não foram utilizados mecanismos de exclusão mútua (como **Mutexes**), os sinais de controle do display se sobrepõem, resultando em caracteres corrompidos ou falha total na atualização das linhas. Esta prática serve de base para a introdução de **Semáforos e Mutexes** nas próximas etapas.

---
### Prof. Thiago José da Luz, PhD 
