<template>
  <div class="sensors-dashboard">
    <div class="flex row items-center justify-between">
      <span class="text-h5">Sensors Dashboard</span>
      <q-btn label="New Sensor" no-caps color="primary" @click="newSensor"/>
    </div>
    <div class="column">
      <b class="q-mb-sm q-mt-md">Date Range:</b>
      <q-form class="row" @submit.prevent="updateTimeRange">
        <DateTimePicker class="time-picker q-mr-md" label="From" v-model="timeRangeSelection.start"/>
        <DateTimePicker class="time-picker" label="To" v-model="timeRangeSelection.end"/>
        <q-btn label="Apply Time Range" color="primary" class="q-ml-md" type="submit"/>
      </q-form>
    </div>
    <div class="charts">
      <template v-for="sensor in sensors" :key="sensor.identifier">
        <SensorCard :sensorData="sensor" :timeRange="timeRange" @deleteSensor="deleteSensor"/>
      </template>
    </div>
  </div>
</template>
<script>
import { defineComponent } from 'vue';
import SensorCard from '@/pages/sensor/components/sensor-card.vue';
import DateTimePicker from '@/components/datetime-picker.vue';
import { sub as subDate } from 'date-fns';

export default defineComponent({
  name: 'sensors-dashboard',
  components: {
    SensorCard,
    DateTimePicker,
  },
  props: {
    deviceId: {
      type: String,
      required: true,
    },
  },
  data() {
    return {
      isChartLoaded: false,
      chartData: [],
      sensors: [],
      timeRange: {
        start: null,
        end: null,
      },
      timeRangeSelection: {
        start: null,
        end: null,
      }
    };
  },
  async created() {
    const currentTime = new Date();
    this.timeRange.end = this.$route.query?.end || currentTime.toISOString();
    this.timeRange.start = this.$route.query?.start || subDate(currentTime, { minutes: 30 }).toISOString();
    this.timeRangeSelection = { ...this.timeRange };
    this.loadSensors();
  },
  methods: {
    async loadSensors() {
      const response = await this.$api.get(`/device/${this.deviceId}/sensors`);
      if (response.status === 200) {
        this.sensors = response.data.results.map((item) => ({ ...item, chartLabel: `${item.description} (${item.unit})` }));
      }
    },
    newSensor() {
      this.$router.push({ name: 'create-sensor', params: { deviceId: this.deviceId } });
    },
    updateTimeRange() {
      this.$router.push({ query: this.timeRangeSelection });
    },
    async deleteSensor(sensorData) {
      this.$q.loading.show({ message: `Deleting sensor ${sensorData.name}...`});
      const response = await this.$api.delete(`/sensor/${sensorData.identifier}`);
      if (response.status === 204) {
        this.$q.notify({ type: 'positive', message: `Sensor ${sensorData.name} successfully deleted` });
      }
      await this.loadSensors();
      this.$q.loading.hide();
    },
  },
  watch: {
    '$route': {
      deep: true,
      handler() {
        this.timeRange = { ...this.timeRangeSelection };
      }
    }
  },
});
</script>
<style lang="scss">
.sensors-dashboard {
  margin-top: 30px;
  margin-left: 10px;

  .charts {
    margin-top: 40px;
    display: grid;
    grid-template-columns: 1fr 1fr;
    grid-auto-rows: 600px;
    gap: 40px;

  }

  .time-picker {
    max-width: 300px;
  }
}
</style>
