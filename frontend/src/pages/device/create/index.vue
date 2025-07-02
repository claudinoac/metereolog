<template>
  <q-page class="create-device-page">
    <div class="subheader">
      <div class="subheader--title">
        <span class="text-h4">New Device</span>
      </div>
    </div>
    <q-form @submit.prevent="submitForm">
      <span class="text-h6 q-mb-md">Basic infos:</span>
      <q-input outlined dense v-model="form.name" label="Name" class="q-mb-sm" :rules="[ val => !!val || 'This field is required']"/>
      <q-input outlined dense v-model="form.model" label="Model" class="q-mb-lg"/>
      <q-input outlined dense v-model="form.brand" label="Brand" class="q-mb-lg"/>
      <q-input outlined dense v-model="form.serial" label="Serial Number" class="q-mb-lg"/>
      <q-btn type="submit" label="Save" color="primary"/>
    </q-form>
  </q-page>
</template>
<script>
import { defineComponent } from 'vue';

export default defineComponent({
  name: 'create-device-page',
  data() {
    return {
      form: {
        name: '',
        model: '',
        brand: '',
        serial: '',
      },
    };
  },
  methods: {
    async submitForm() {
      this.$q.loading.show({ message: 'Creating device...' });
      const response = await this.$api.post('/devices', this.form);
      if (response.status === 201) {
        this.$q.loading.hide();
        this.$router.push({ name: 'list-devices' });
      }
      this.$q.loading.hide();
    },
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
