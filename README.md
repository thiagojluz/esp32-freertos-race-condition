# Prática 2: Multitarefa e Condição de Corrida com FreeRTOS (ESP32) 🚀

Este repositório apresenta o desenvolvimento da **Prática 2** da disciplina de **Sistemas Operacionais Embarcados** na **Universidade Federal do Paraná (UFPR)**.  
O projeto explora o **escalonamento de tarefas em arquitetura dual-core** no ESP32 e os **riscos associados ao acesso concorrente a recursos compartilhados sem mecanismos de sincronização**.

---

## 🎯 Objetivos Didáticos

1. **Criação de Tasks:** Implementação de tarefas com diferentes níveis de prioridade e afinidade de núcleo.
2. **Arquitetura Dual-Core:** Demonstração do uso da função `xTaskCreatePinnedToCore` para distribuir tarefas entre o **Core 0** e o **Core 1** do ESP32.
3. **Identificação de Race Condition:** Evidenciar como o acesso simultâneo de múltiplas tarefas ao mesmo recurso físico, sem sincronização, pode causar corrupção de dados e comportamento indefinido do sistema.

---

## 🧠 Competências Demonstradas

- Programação de sistemas embarcados utilizando **FreeRTOS**.
- Criação e gerenciamento de **tasks concorrentes** com prioridades distintas.
- Utilização de **afinidade de núcleo (core affinity)** em arquiteturas multicore.
- Análise prática de **condição de corrida (race condition)**.
- Avaliação do impacto do acesso concorrente a periféricos **não thread-safe**.
- Compreensão de problemas de **sincronização** em sistemas de tempo real.
- Escrita de código estruturado com foco didático e engenharia de sistemas.

---

## ⚙️ Regras e Implementação

- **Task 1 (Blink):**  
  Criada sem núcleo definido, utiliza a função `xPortGetCoreID()` para reportar via **Serial** em qual núcleo foi alocada pelo escalonador do FreeRTOS.

- **Task 2 (Slider – Core 0):**  
  Leitura analógica de um potenciômetro e atualização da **linha 0** do display LCD.

- **Task 3 (Temperatura – Core 1):**  
  Leitura de um sensor **DHT22** e atualização da **linha 1** do display LCD.

- **Interface:**  
  Utilização da biblioteca `LiquidCrystal` em modo **paralelo (4 bits)**, sem comunicação I²C, exigindo a manipulação direta dos pinos de dados e controle do display.

---

## 🛠️ Hardware Utilizado (Simulado no Wokwi)

- **Microcontrolador:** ESP32  
- **Sensores:**  
  - DHT22 (Temperatura)  
  - Potenciômetro (Slider)  
- **Display:** LCD 16x2 em modo paralelo

---

## 💡 Relevância Técnica

Condições de corrida representam um problema crítico em **sistemas embarcados reais**, especialmente em arquiteturas **multicore**, onde múltiplas tarefas podem acessar simultaneamente recursos compartilhados.  
Esta prática reproduz um cenário típico encontrado em aplicações **industriais, automotivas e de IoT**, evidenciando como a ausência de mecanismos de exclusão mútua pode comprometer a integridade dos dados e a confiabilidade do sistema.  
O experimento estabelece a base conceitual para o uso de **mutexes e semáforos**, fundamentais em projetos baseados em **RTOS**.

---

## 🚀 Como Executar

1. O código principal está localizado no arquivo `sketch.ino`.
2. As conexões de hardware estão descritas no arquivo `diagram.json`.
3. As bibliotecas necessárias estão listadas no arquivo `libraries.txt`.
4. **Simulação Online:**  
   É possível testar o comportamento do sistema diretamente no navegador através do link abaixo:  
   ▶️ [Simulação no Wokwi](https://wokwi.com/projects/436002682510007297)

---

## 🧪 Validação e Testes

- Execução simultânea das tasks fixadas em núcleos distintos.
- Observação de **corrompimento de caracteres** no display LCD.
- Avaliação do comportamento instável ao aumentar a frequência de acesso ao display.
- Confirmação de que o problema ocorre independentemente da ordem de criação das tasks.
- Identificação clara da ausência de sincronização como causa principal do problema.

---

## ⚠️ Análise Técnica: Condição de Corrida e Ausência de Sincronização

Nesta implementação, ambas as tarefas com núcleo fixo tentam realizar operações de escrita no LCD (`lcd.setCursor` e `lcd.print`) de forma simultânea.  
Como não são utilizados mecanismos de exclusão mútua (como **Mutexes**), os sinais de controle e dados do display se sobrepõem, resultando em exibição corrompida ou falhas completas na atualização das informações.  

Esta prática serve como ponto de partida para a introdução de **Semáforos** e **Mutexes** nas próximas etapas do portfólio, demonstrando como o FreeRTOS oferece ferramentas para garantir acesso seguro a recursos compartilhados.

---

📚 **Evolução do Projeto**

Este projeto faz parte de um portfólio progressivo em **Sistemas Operacionais Embarcados**.  
As próximas práticas incluem:

- Implementação de mecanismos de sincronização utilizando **Mutexes**.
- Comparação entre acesso concorrente protegido e não protegido.
- Avaliação do impacto da sincronização no desempenho do sistema.
- Extensão para aplicações mais próximas de sistemas críticos reais.

---

**Prof. Thiago José da Luz, PhD**  
Departamento de Engenharia Elétrica – UFPR
