<template>
  <div class="sensor-card">
    <div class="flex row justify-between">
      <div class="sensor-info">
        <span><b class="q-pr-sm">Name:</b>{{ sensorData.name }}</span>
        <div class="flex row items-center">
          <span><b class="q-pr-sm">ID:</b> {{ sensorData.identifier }}</span>
          <q-icon class="q-pl-sm copy-icon" size="16px" name="mdi-content-copy" @click="copyClipboard(sensorData.identifier)"/>
        </div>
      </div>
      <div class="sensor-actions">
        <q-icon class="edit-btn" name="mdi-pencil-box" size="24px"/>
        <q-icon class="delete-btn" name="mdi-trash-can" size="24px" @click="deleteSensor"/>
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
  emits: ['deleteSensor'],
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
              min: this.timeRange.start,
              max: this.timeRange.end,
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
                min: this.timeRange.start,
                max: this.timeRange.end,
            },
            y: {
              max: this.sensorData.upper_limit ?? 10,
              min: this.sensorData.lower_limit ?? 0,
            }
        },
      };
    }
    this.getChartData();
  },
  methods: {
    async copyClipboard(text) {
      await navigator.clipboard.writeText(text);
      this.$q.notify({ message: 'Sensor ID copied to clipboard.', type: 'info' });
    },
    async getChartData() {
      const response = await this.$api.get(
        `/sensor/${this.sensorData.identifier}/chart`,
        {
          params: this.timeRange,
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
    deleteSensor() {
      this.$emit('deleteSensor', this.sensorData);
    },
  },
  watch: {
    timeRange: {
      deep: true,
      handler() {
        if (this.timeRange.start && this.timeRange.end) {
          this.getChartData();
        }
      },
    },
  }
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
