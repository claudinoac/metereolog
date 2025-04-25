## Sensor de Gás MQ‑6

O sensor MQ‑6 detecta principalmente **GLP (propano/​butano)**, podendo responder ainda a gás natural e fumaça. É muito usado em alarmes de vazamento e sistemas de monitoramento de qualidade do ar.

---

### 1  Dados Técnicos do Sensor MQ‑6

| **Característica**     | **Descrição**                                           |
|------------------------|---------------------------------------------------------|
| Tipo                   | Sensor de gás semicondutor (SnO<sub>2</sub>)           |
| Tensão de operação     | 5 V ± 0,1 V                                             |
| Corrente de operação   | 150 mA (±10 %) para o aquecedor                         |
| Sensibilidade analógica| 0 – 5 V (↓ tensão => ↑ concentração de gás)            |
| Tempo de aquecimento   | ≥ 20 s para estabilidade inicial                        |
| Saída digital          | Alta/Baixa (ajustável via potenciômetro no comparador) |

---

### 2  Pinagem do MQ‑6 

| **Pino** | **Função** | **Descrição**        |
|:-------:|-----------|-----------------------|
| A0      | Saída Analógica | Tensão proporcional à concentração |
| D0      | Saída Digital   | Sinal alto/baixo (limiar ajustável) |
| GND     | Terra           | Referência 0 V                      |
| VCC     | Alimentação 5 V | +5 V (pico de até 200 mA no aquecedor) |

---

### 3  Componentes

- Módulo sensor MQ‑6 
- Resistores de pull‑down (opcional, se ligar só A0)
- Microcontrolador ESP32

---

### 4  Código de Exemplo

```cpp
/****************************************************
 *  MQ‑6 – Implementação 
 ****************************************************/
typedef struct {
    int raw_value;
    float voltage;
} MQ6Info;

class MQ6Sensor {
    public:
        MQ6Sensor(adc1_channel_t channel, float maxVoltage = 5.0f, int maxAdc = 4095);
        MQ6Info read();
        float calibrate(float RL = 4700.0f);
        float readPPM(float Ro, float RL = 4700.0f);

    private:
        adc1_channel_t adc_channel;
        float reference_voltage;
        int adc_resolution;

        float calculateRs(float voltage, float RL);
};



