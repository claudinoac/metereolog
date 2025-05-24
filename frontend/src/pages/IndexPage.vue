<template>
  <q-page class="dashboard-view flex flex-center column">
    <div class="subheader full-width">
      <div class="q-pl-md">
        <h4>Device: {{ device?.name }}</h4>
      </div>
    </div>
    <div class="charts">
      <div class="line-chart">
        <LineChart :data="tempChart" :options="tempChartOptions" v-if="isLoaded"/>
      </div>
      <div class="line-chart">
        <LineChart :data="humChart" :options="humChartOptions" v-if="isLoaded"/>
      </div>
      <div class="rose-chart">
        <RadarChart :data="windDirChart" :options="windDirChartOptions" v-if="isLoaded"/>
      </div>
    </div>
  </q-page>
</template>

<script>
import { defineComponent } from 'vue';
import 'chartjs-adapter-date-fns';
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
  RadialLinearScale,
} from 'chart.js'
import { Line as LineChart, Radar as RadarChart } from 'vue-chartjs'

ChartJS.register(
  CategoryScale,
  LinearScale,
  RadialLinearScale,
  PointElement,
  LineElement,
  TimeScale,
  Title,
  Tooltip,
  Legend
);

const baseChartOptions = {
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
          max: 50,
          min: -10,
        }
    },
}

export default defineComponent({
  name: 'IndexPage',
  components: {
    LineChart,
    RadarChart,
  },
  async created() {
    const that = this;
    const response = await this.$api.get(`/device/6d2673ca-0408-4556-8605-c9a738a8bae3`);
    if (response.status === 200) {
      this.device = response.data;
    }
    async function load() {
      await Promise.allSettled(Object.entries(that.sensors).map(async ([name, uuid]) => {
        const response = await that.$api.get(
          `/sensor/${uuid}/chart`,
          {
            params: { interval_sec: that.interval },
          },
        );
        if (response.status === 200) {
          const formattedData = response.data.map((item) => ({x: item.timestamp, y: item.value}));
          that[`${name}Chart`].datasets[0].data = formattedData;
        }
      }));

      const response = await that.$api.get(
        `/sensor/${that.windDir}/chart`,
        {
          params: { interval_sec: that.interval },
        },
      );
      if (response.status === 200) {
        const order = ['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW'];
        const dirMap = {
          N: 0,
          W: 0,
          E: 0,
          S: 0,
          NE: 0,
          NW: 0,
          SE: 0,
          SW: 0,
        };
        let innerCounter = 0;
        const dataPoints = response.data.map((item, idx) => {
          if (idx % 8 == 0) {
            innerCounter = 0;
          }
          dirMap[item.value]++;
          const value = dirMap[order[innerCounter]];
          innerCounter++;
          return value;
        });
        that[`windDirChart`].datasets[0].data = dataPoints;
      }
      that.isLoaded = true;
    };
    load();
    that.interval_runner = setInterval(load, 10000);
  },
  computed: {
    interval() {
      return this.$route?.query?.interval ?? 900;
    },
  },
  data() {
    return {
      device: {},
      sensors: {
        temp: '47f4bc3a-a817-49e9-af06-a408a5ee602d',
        hum: 'a1cb7516-c98e-4339-ab13-6f358aa7e38f',
      },
      windDir: '4f531ada-394e-46a4-98f6-38d8bd45a3a3',
      isLoaded: false,
      interval_runner: null,
      tempChartOptions: {
        ...baseChartOptions,
      },
      humChartOptions: {
        ...baseChartOptions,
        scales: {
            x: {
                type: 'time',
                ticks: {
                  maxTicksLimit: 10,
                },
            },
            y: {
              max: 100,
              min: 0,
            }
        },
      },
      windDirChartOptions: {
        scales: {
          r: {
          },
        },
        elements: {
          line: {
            borderWidth: 3
          }
        }
      },
      humChart: {
        datasets: [{
          borderColor: 'rgb(75, 19, 192)',
          label: 'Humidity (%)',
          data: [],
          tension: 0.1,
        }],
      },
      tempChart: {
        datasets: [{
          borderColor: 'rgb(75, 192, 192)',
          label: 'Temperature (ºC)',
          data: [],
          tension: 0.1,
        }],
      },
      windDirChart: {
        labels: [
          'North',
          'NorthEast',
          'East',
          'SouthEast',
          'South',
          'SouthWest',
          'West',
          'NorthWest',
        ],
        datasets: [{
          borderColor: 'rgb(75, 192, 192)',
          label: 'Wind Direction',
          fill: true,
          data: [],
          tension: 0.1,
        }],
      },
    };
  },
  beforeUnmount() {
    clearInterval(this.interval_runner);
  },
});
</script>
<style lang="scss">
.dashboard-view {
    background-color: $grey-2;

  .charts {
      display: grid;
      grid-template-columns: 1fr 1fr;
      width: 100%;
      padding: 0 20px;
      gap: 20px;

      .line-chart,.rose-chart{
        background-color: white;
        width: 100%;
        height: 600px;
        align-items: center;
        justify-items: center;
        padding: 20px;
        border-radius: 6px;
      }
  }
}
</style>
