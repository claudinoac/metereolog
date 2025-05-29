<template>
  <q-page class="device-details-view">
    <div class="subheader">
      <div class="subheader--title">
        <span class="text-h5"><b class="q-pr-xs">Device:</b>{{ device.name }}</span>
      </div>
      <div class="details">
          <span><b class="q-pr-xs">ID:</b>{{ device.identifier }}</span>
          <span><b class="q-pr-xs">Organization:</b>{{ device.organization }}</span>
          <span>
            <b class="q-pr-xs">Active:</b>
            <q-toggle :modelValue="device.is_active" @update:modelValue="toggleDevice"/>
          </span>
          <q-input
            :modelValue="mqttTopic" outlined dense
            readonly class="q-mt-none" label="MQTT Topic"
          >
            <template v-slot:append>
              <q-icon name="mdi-content-copy" size="18px" @click="copyClipboard(mqttTopic)"/>
            </template>
          </q-input>
          <q-input
            :modelValue="device.mqtt_user" outlined dense
            readonly class="q-mt-none" label="MQTT User"
          >
            <template v-slot:append>
              <q-icon name="mdi-content-copy" size="18px" @click="copyClipboard(device.mqtt_user)"/>
            </template>
          </q-input>
          <q-input
            :modelValue="device.mqtt_password" outlined dense type="password"
            readonly class="q-mt-none" label="MQTT Password"
          >
            <template v-slot:append>
              <q-icon name="mdi-content-copy" size="18px" @click="copyClipboard(device.mqtt_user)"/>
            </template>
          </q-input>
        </div>
    </div>
    <SensorsDashboard :deviceId="deviceId" />
  </q-page>
</template>
<script>
import { defineComponent } from 'vue';
import { mapState } from 'pinia';
import { useStore } from 'src/stores/store';
import SensorsDashboard from '@/pages/sensor/components/dashboard.vue';

export default defineComponent({
  name: 'device-details',
  components: {
    SensorsDashboard,
  },
  data() {
    return {
      device: {},
      deviceId: this.$route.params.deviceId,
    };
  },
  computed: {
    ...mapState(useStore, { userData: 'getUserData' }),
    endpoints() {
      return {
        device: `device/${this.$route.params.deviceId}`,
      };
    },
    mqttTopic() {
      return `mqtt/${this.userData.organization.id}/${this.deviceId}`;
    },
  },
  created() {
    this.getDevice();
  },
  methods: {
    async getDevice() {
      const response = await this.$api.get(this.endpoints.device);
      if (response.status === 200) {
        this.device = response.data;
      }
    },
    async toggleDevice(value) {
      const response = await this.$api.patch(this.endpoints.device, { is_active: value });
      if (response.status === 200) {
        this.device.is_active = value;
      }
    },
    async copyClipboard(text) {
      await navigator.clipboard.writeText(text);
      this.$q.notify({ message: 'MQTT Topic copied to clipboard.', type: 'info' });
    },
  },
});
</script>
<style lang="scss">
.device-details-view {
  background-color: $background-secondary;

  .details {
    display: grid;
    grid-template-rows: 50px 50px 50px;
    grid-template-columns: 320px 600px;
    grid-auto-flow: column;
    padding-top: 30px;

    > span {
      display: flex;
      align-items: center;
      height: 40px;
    }
  }
}
</style>
