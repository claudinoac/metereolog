<template>
  <q-page class="create-device-page">
    <div class="subheader">
      <div class="subheader--title">
        <span class="text-h4">New Sensor</span>
      </div>
    </div>
    <q-form @submit.prevent="submitForm">
      <span class="text-h6 q-mb-md">Basic infos:</span>
      <q-input outlined dense v-model="form.name" label="Name" class="q-mb-sm" :rules="[ val => !!val || 'This field is required']"/>
      <q-input outlined dense v-model="form.unit" label="Unit" class="q-mb-sm" :rules="[ val => !!val || 'This field is required']"/>
      <q-input outlined dense v-model="form.description" label="Description" class="q-mb-sm" type="textarea"/>
      <q-select outlined dense options-dense v-model="form.measuring_type" label="Measuring type" map-options emit-value :options="options.measuring_types"/>
      <div class="flex row q-ml-sm q-mt-md items-center">
        <label>Active</label>
        <q-toggle v-model="form.is_active"/>
      </div>
      <q-input outlined dense v-model="form.upper_limit" label="Upper Limit" class="q-mb-sm" v-if="form.measuring_type !== 'str'"/>
      <q-input outlined dense v-model="form.lower_limit" label="Lower Limit" v-if="form.measuring_type !== 'str'"/>
      <div class="flex row items-center q-mt-md">
        <span class="text-h6 q-mr-md">Additional Info:</span>
        <q-btn label="Add info" no-caps color="secondary" @click="addInfo"/>
      </div>
      <div class="flex column q-mt-lg">
        <div class="flex row items-center q-pb-sm" v-for="(item, idx) in this.form.additional_info" :key="idx">
          <q-input class="q-pr-md" outlined dense v-model="item.label" label="label"/>
          <q-input class="q-pr-md" outlined dense v-model="item.value" label="value"/>
          <q-btn icon="mdi-delete" size="15px" color="negative" @click="deleteInfo(idx)"/>
        </div>
      </div>
      <q-btn type="submit" label="Save" color="primary"/>
    </q-form>
  </q-page>
</template>
<script>
import { defineComponent } from 'vue';

export default defineComponent({
  name: 'create-sensor-page',
  data() {
    return {
      deviceId: this.$route.params.deviceId,
      form: {
        name: '',
        unit: '',
        description: '',
        is_active: true,
        measuring_type: 'str',
        additional_info: [],
      },
      options: {
        measuring_types: [
          { label: "String", value: 'str' },
          { label: "Float", value: 'float' },
          { label: "Integer", value: "int"}
        ]
      },
    };
  },
  methods: {
    async submitForm() {
      this.$q.loading.show({ message: 'Creating device...' });
      const form = { ...this.form, additional_info: this.form.additional_info.reduce((acc, item) => ({...acc, [item.label]: item.value}), {})};
      const response = await this.$api.post(`/device/${this.deviceId}/sensors`, form);
      if (response.status === 201) {
        this.$q.loading.hide();
        this.$router.push({ name: 'view-device', params: { deviceId: this.deviceId } });
      }
      this.$q.loading.hide();
    },
    addInfo() {
      this.form.additional_info.push({label: '', value: ''});
    },
    deleteInfo(index) {
      this.form.additional_info.splice(index, 1);
    }
  },
});
</script>
<style lang="scss">
.create-device-page.q-page {
  .q-form {
    margin-top: 60px;
    max-width: 500px;
    display: flex;
    flex-flow: column;
  }
}
</style>
