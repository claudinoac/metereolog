<template>
  <div class="sensors-dashboard">
    <div class="flex row items-center justify-between">
      <span class="text-h5">Sensors</span>
      <q-btn label="New Sensor" no-caps color="primary" @click="newSensor"/>
    </div>
    <div class="charts">
      <template v-for="sensor in sensors" :key="sensor.identifier">
        <SensorCard :sensorData="sensor"/>
      </template>
    </div>
  </div>
</template>
<script>
import { defineComponent } from 'vue';
import SensorCard from '@/pages/sensor/components/sensor-card.vue';

export default defineComponent({
  name: 'sensors-dashboard',
  components: {
    SensorCard,
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
    };
  },
  async created() {
      const response = await this.$api.get(`/device/${this.deviceId}/sensors`);
      if (response.status === 200) {
        this.sensors = response.data.results.map((item) => ({ ...item, chartLabel: `${item.description} (${item.unit})` }));
      }
  },
  computed: {
    interval() {
      return this.$route?.query?.interval ?? 900;
    },
  },
  methods: {
    newSensor() {
      this.$router.push({ name: 'new-sensor', params: { deviceId: this.deviceId } });
    },
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
}
</style>
