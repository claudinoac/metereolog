### Sensor DHT11

O DHT11 é um sensor digital amplamente utilizado para medir temperatura e umidade relativa do ar em ambientes internos e externos. É comumente empregado em projetos com microcontroladores, como o ESP32, para monitoramento ambiental em sistemas meteorológicos, automação residencial, estufas e outros dispositivos de coleta de dados climáticos.

---

### 1   Dados Técnicos do Sensor DHT11

**Característica** | **Descrição**
Tipo | Sensor digital de temperatura e umidade
Tensão de operação | 3.3V a 5V
Corrente típica | 0.5 mA
Faixa de temperatura | 0 °C a 50 °C (±2 °C)
Faixa de umidade | 20% a 90% UR (±5%)
Frequência de leitura | 1 Hz (1 leitura por segundo)
Saída de sinal | Digital (protocolo próprio via 1 fio)

---

### 2  Pinagem do DHT11

**Pino** |**Função**
VCC | Alimentação (+5 V ou +3.3 V)
GND | Terra
DATA | Sinal digital

---

### 3 Componentes 

- Sensor DHT11
- Resistor de pull-up de 10kΩ entre VCC e DATA
- Microcontrolador ESP32

### 4  Código de Exemplo

```cpp
/****************************************************
 *  DHT11 – Implementação 
 ****************************************************/
typedef struct {
    float temperature;
    float humidity;
} DHTInfo;

class DHTSensor {
    public:
        DHT *sensor;
        DHTSensor(gpio_num_t port, DHTModel_t model = DHT_MODEL_DHT11);
        DHTInfo read();
    private:
        unsigned long ts_read;
        unsigned long delta_time;
        unsigned long sampling_period;
};