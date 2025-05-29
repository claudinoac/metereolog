<template>
  <div class="sensor-card">
    <div class="flex row justify-between">
      <div class="sensor-info">
        <span><b>Name:</b> {{ sensorData.name }}</span>
        <span><b>ID:</b> {{ sensorData.identifier }}</span>
      </div>
      <div class="sensor-actions">
        <q-icon class="edit-btn" name="mdi-pencil-box" size="24px"/>
      </div>
    </div>
    <component
      :is="chartType" :label="chartLabel" :options="chartOptions"
      :dataOptions="chartDataOptions" :labels="chartLabels" :data="chartData"
      :isLoaded="isChartLoaded"
    />
  </div>
</template>
<script>
import { defineComponent } from 'vue';
import LineChart from '@/components/charts/line.vue';

export default defineComponent({
  name: 'sensor-card',
  components: {
    LineChart,
  },
  props: {
    sensorData: {
      type: Object,
      required: true,
    },
    timeRange: {
      type: Object,
      required: false,
    },
  },
  data() {
    return {
      isChartLoaded: false,
      chartData: [],
      chartOptions: {},
      chartDataOptions: {},
      chartLabels: [],
      directionsLegend: {
        N: 0,
        NE: 1,
        E: 2,
        SE: 3,
        S: 4,
        SW: 5,
        W: 6,
        NW: 7,
      },
      directions: ['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW'],
    };
  },
  computed: {
    interval() {
      return this.$route?.query?.interval ?? 900;
    },
    chartLabel() {
      return `${this.sensorData.description} (${this.sensorData.unit})`;
    },
    chartType() {
      if (['int', 'float'].includes(this.sensorData.measuring_type)) {
        return LineChart;
      }
      return LineChart;
    },
    isDirectionChart() {
      return this.sensorData.unit === '-';
    }
  },
  async created() {
    if (this.isDirectionChart) {
      this.chartOptions = {
        plugins: {
          tooltip: {
            callbacks: {
              label: (context) => {
                return `Direction: ${this.directions[context.parsed.y]}`;
              },
            },
          },
        },
        scales: {
          x: {
              type: 'time',
              ticks: {
                maxTicksLimit: 10,
              },
          },
          y: {
            max: 7,
            min: 0,
            ticks: {
              callback: (value, index, ticks) => { // eslint-disable-line
                return this.directions[value];
              },
            },
          },
        },
      };
      this.chartDataOptions = {
        pointRadius: 5,
      };
    } else {
      this.chartOptions = {
        scales: {
            x: {
                type: 'time',
                ticks: {
                  maxTicksLimit: 10,
                },
            },
            y: {
              max: this.sensorData.upper_limit ?? 10,
              min: this.sensorData.lower_limit ?? 0,
            }
        },
      };
    }
    const response = await this.$api.get(
      `/sensor/${this.sensorData.identifier}/chart`,
      {
        params: { interval_sec: this.interval },
      },
    );
    if (response.status === 200) {
      this.chartData = response.data.map((item) => ({x: item.timestamp, y: item.value}));
      if (this.isDirectionChart) {
        this.chartData = this.chartData.map((item) => ({...item, y: this.directionsLegend[item.y] }));
      }
      this.isChartLoaded = true;
    }
  },
});
</script>
<style lang="scss">
.sensor-card {
  padding: 20px;
  border-radius: 12px;
  background-color: white;
  display: flex;
  flex-flow: column;

  .sensor-info {
    display: flex;
    flex-flow: column;
  }
}
</style>
