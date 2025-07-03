<template>
  <div class="sensors-dashboard">
    <div class="flex row items-center justify-between">
      <span class="text-h5">Sensors Dashboard</span>
      <q-btn label="New Sensor" no-caps color="primary" @click="newSensor"/>
    </div>
    <div class="column">
      <b class="q-mb-sm q-mt-md">Time Range:</b>
      <q-form class="column" @submit.prevent="updateTimeRange">
        <div class="row items-center">
          <q-select outlined dense options-dense label="Preset" :options="timePresets" map-options emit-value v-model="timeFilterType"/>
          <q-icon name="mdi-refresh" class="refresh-icon" size="28px" @click="updateTimeRange"/>
        </div>
        <div class="row q-mt-md" v-if="timeFilterType === 'custom'">
          <DateTimePicker class="time-picker q-mr-md" label="From" v-model="timeRangeSelection.start"/>
          <DateTimePicker class="time-picker" label="To" v-model="timeRangeSelection.end"/>
          <q-btn label="Apply Time Range" color="primary" class="q-ml-md" type="submit"/>
        </div>
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
import { is } from 'quasar';


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
      },
      timeFilterType: 'last_half_hour',
      timePresets: [
        { label: 'Last minute', value: 'last_min' },
        { label: 'Last 5 min', value: 'last_five_min' },
        { label: 'Last 15 min', value: 'last_quarter_hour' },
        { label: 'Last 30 min', value: 'last_half_hour' },
        { label: 'Last 1 hour', value: 'last_hour' },
        { label: 'Last 6 hours', value: 'last_six_hours' },
        { label: 'Last 12 hours', value: 'last_twelve_hours' },
        { label: 'Last 24 hours', value: 'last_day' },
        { label: 'Last week', value: 'last_week' },
        { label: 'Last month', value: 'last_month' },
        { label: 'Last year', value: 'last_year' },
        { label: 'Custom', value: 'custom' },
      ],
      timeMap: {
        'last_min': { minutes: 1 },
        'last_five_min': { minutes: 5 },
        'last_quarter_hour': { minutes: 15 },
        'last_half_hour': { minutes: 30 },
        'last_hour': { hours: 1 },
        'last_six_hours': { hours: 6 },
        'last_twelve_hours': { hours: 12 },
        'last_day': { hours: 24 },
        'last_week': { days: 7 },
        'last_month': { months: 1 },
        'last_year': { months: 12 },
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
      if (this.timeFilterType !== 'custom') {
        const currentTime = new Date();
        this.timeRange.end = currentTime.toISOString();
        this.timeRange.start = subDate(currentTime, this.timeMap[this.timeFilterType]).toISOString();
        this.updateTimeRange();
      }
      if (is.deepEqual(this.$route.query, this.timeRangeSelection)) {
        this.loadSensors();
      } else {
        this.$router.push({ query: this.timeRangeSelection });
      }
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
    },
    timeFilterType () {
      this.updateTimeRange();
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

  .refresh-icon {
    margin-left: 10px;
    &:hover {
      color: $info;
    }
  }
}
</style>
