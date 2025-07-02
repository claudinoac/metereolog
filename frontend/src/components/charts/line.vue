<template>
  <div class="line-chart">
    <LineChart :data="chartData" :options="chartOptions" v-if="isLoaded" ref="chart"/>
  </div>
</template>
<script>
import { defineComponent } from 'vue';
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  TimeScale,
} from 'chart.js'
import { Line as LineChart } from 'vue-chartjs'
import 'chartjs-adapter-date-fns';
ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  TimeScale,
  Title,
  Tooltip,
  Legend
);

export default defineComponent({
  name: 'line-chart',
  components: {
    LineChart,
  },
  props: {
    data: {
      type: Array,
      required: true,
    },
    isLoaded: {
      type: Boolean,
      default: false,
    },
    color: {
      type: String,
      default: 'rgb(75, 19, 192)',
    },
    label: {
      type: String,
      required: true,
    },
    options: {
      type: Object,
      default: () => ({}),
    },
    dataOptions: {
      type: Object,
      default: () => ({}),
    },
    labels: {
      type: Array,
      default: () => ([]),
    },
  },
  data() {
    return {
      chartOptions: {
        elements: {
          point: {
            pointStyle: false,
          },
        },
        responsive: true,
        maintainAspectRatio: false,
        scales: {
            x: {
                type: 'time',
                ticks: {
                  maxTicksLimit: 10,
                },
            },
            y: {
              max: 0,
              min: 100,
            }
        },
        ...this.options,
      },
    };
  },
  computed: {
    chartData() {
      return {
        labels: this.labels,
        datasets: [{
          borderColor: this.color,
          label: this.label,
          data: this.data,
          tension: 0.1,
          ...this.dataOptions,
        }],
      };
    },
  },
  watch: {
    options: {
      deep: true,
      handler() {
        this.$refs.chart.chart.update();
      },
    },
  },
});
</script>
<style lang="scss">
.line-chart {
  display: flex;
  background-color: white;
  width: 100%;
  height: 100%;
  max-height: 600px;
  align-items: center;
  justify-items: center;
  padding: 10px;
  border-radius: 6px;
}
</style>
