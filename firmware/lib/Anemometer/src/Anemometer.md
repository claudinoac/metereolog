### Anemômetro SV10

O anemômetro mede a velocidade do vento em ambientes abertos. É utilizado em sistemas meteorológicos, estações climáticas e projetos com microcontroladores para estimar intensidade e variações do vento em tempo real.

---

### 1  Dados Técnicos do Sensor SV10

| **Característica**     | **Descrição**                                           |
|------------------------|---------------------------------------------------------|
| Tipo                   | Sensor de vento tipo copo com sensor magnético lacrado  |
| Tensão de operação     | 5 V - 24 V                                              |
| Corrente típica        | <10mA                                                   |
| Velocidade de medição  | 0,3 a 32,4 m/s (±0,3 m/s)                               |
| Frequencia de saída    | 1 Hz ≈ 2,4 km/h (a cada giro completo das pás)          |
| Sensibilidade analógica| 0 – 5 V (↓ tensão => ↑ concentração de gás)             |
| Temperatura de operação| –30 °C a +70 °C                                         |
| Saída de sinal         | Pulso digital (TTL)                                     |

---

### 2 Pinagem do Anemômetro

| **Pino** | **Função** | **Descrição**        |
|:-------:|-----------|-----------------------|
| SIG     | Saída Digital   | Pulso TTL proporcional á velocidade    |
| VCC     | Alimentação     | 5 V                                    |

---

### 3 Componentes 

- Sensor Anemômetro SV10
- Resistores de pull-up (opcional,dependendo da interface)
- Microcontrolador ESP32

### 4  Código de Exemplo

```cpp
/****************************************************
 *  Anemômetro – Implementação 
 ****************************************************/
typedef struct {
    float windSpeed;
    float rotations;
} WindInfo;

class Anemometer {
public:
    Anemometer(gpio_num_t pin, float radius = 14.7);
    WindInfo read();

private:
    gpio_num_t inputPin;
    float radius;
};