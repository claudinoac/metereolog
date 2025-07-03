<template>
  <q-page class="device-details-view">
    <div class="subheader">
      <div class="subheader--title">
        <span class="text-h5"><b class="q-pr-xs">Device:</b>{{ device.name }}</span>
      </div>
      <div class="details-grid">
        <div class="basic-details">
            <span class="text-h6">Basic details:</span>
            <span><b class="q-pr-xs">ID:</b>{{ device.identifier }}</span>
            <span><b class="q-pr-xs">Brand:</b>{{ device.brand }}</span>
            <span><b class="q-pr-xs">Model:</b>{{ device.model }}</span>
            <span><b class="q-pr-xs">Serial Number:</b>{{ device.serial_number }}</span>
        </div>
        <div class="mqtt-details">
          <span class="text-h6">MQTT Connection details:</span>
          <q-input
            :modelValue="mqttBroker" outlined dense
            readonly class="q-mt-none" label="Broker Address"
          >
            <template v-slot:append>
              <q-icon name="mdi-content-copy" size="18px" @click="copyClipboard(mqttBroker, 'Broker Address')"/>
            </template>
          </q-input>
          <q-input
            :modelValue="mqttTopic" outlined dense
            readonly class="q-mt-none" label="Topic"
          >
            <template v-slot:append>
              <q-icon name="mdi-content-copy" size="18px" @click="copyClipboard(mqttTopic, 'Topic')"/>
            </template>
          </q-input>
          <q-input
            :modelValue="mqttUser" outlined dense
            readonly class="q-mt-none" label="User"
          >
            <template v-slot:append>
              <q-icon name="mdi-content-copy" size="18px" @click="copyClipboard(mqttUser, 'User')"/>
            </template>
          </q-input>
          <q-input
            :modelValue="device.mqtt_password" outlined dense type="password"
            readonly class="q-mt-none" label="Password"
          >
            <template v-slot:append>
              <q-icon name="mdi-content-copy" size="18px" @click="copyClipboard(device.mqtt_password, 'Password')"/>
            </template>
          </q-input>
        </div>
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
      domainMap: {
        'localhost': 'mqtt://localhost:1883',
        'app.metereolog.site': 'mqtt://mqtt.metereolog.site:1883',
        'app.staging.metereolog.site': 'mqtt://mqtt.staging.metereolog.site:1883',
      }
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
      const orgId = this.userData.organization.id.replaceAll('-', '');
      return `mqtt/${orgId}/${this.mqttUser}`;
    },
    mqttUser() {
      return this.deviceId.replaceAll('-', '');
    },
    mqttBroker() {
      const domain = window.location.hostname;
      return this.domainMap[domain];
    }
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
    async copyClipboard(text, type) {
      await navigator.clipboard.writeText(text);
      this.$q.notify({ message: `MQTT ${type} copied to clipboard.`, type: 'info' });
    },
  },
});
</script>
<style lang="scss">
.device-details-view {
  background-color: $background-secondary;

  .details-grid {
    display: flex;
    flex-flow: row;
    padding-top: 30px;
    gap: 40px;

    span {
      display: flex;
      align-items: center;
      height: 40px;
    }
  }

  .basic-details {
    display: flex;
    flex-flow: column;
    gap: 20px;

  }

  .mqtt-details {
    display: flex;
    flex-flow: column;
    gap: 20px;
    min-width: 600px;
  }
}
</style>
