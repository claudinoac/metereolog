<template>
  <q-input outlined dense :debounce="500" :label="label" :modelValue="displayDate" @update:modelValue="updateInput">
    <template v-slot:prepend>
      <q-icon name="event" class="cursor-pointer">
        <q-popup-proxy cover transition-show="scale" transition-hide="scale">
          <q-date v-model="date" mask="DD/MM/YYYY">
            <div class="row items-center justify-end">
              <q-btn v-close-popup label="Close" color="primary" flat />
            </div>
          </q-date>
        </q-popup-proxy>
      </q-icon>
    </template>

    <template v-slot:append>
      <q-icon name="access_time" class="cursor-pointer">
        <q-popup-proxy cover transition-show="scale" transition-hide="scale">
          <q-time v-model="hour" mask="HH:mm:ss" format24h>
            <div class="row items-center justify-end">
              <q-btn v-close-popup label="Close" color="primary" flat />
            </div>
          </q-time>
        </q-popup-proxy>
      </q-icon>
    </template>
  </q-input>
</template>
<script>
import { defineComponent } from 'vue';

export default defineComponent({
  props: {
    modelValue: {
      type: Number,
    },
    label: {
      type: String,
    },
  },
  emits: ['update:modelValue'],
  data() {
    return {
      date: '',
      hour: '',
      datetime: null
    };
  },
  computed: {
    displayDate() {
      return `${this.date} ${this.hour}`;
    },
  },
  created() {
    if (this.modelValue) {
      this.datetime = new Date(this.modelValue);
    } else {
      this.datetime = new Date();
    }
  },
  watch: {
    modelValue(newVal, oldVal) {
      if (newVal !== oldVal) {
        this.datetime = new Date(newVal);
      }
    },
    datetime(newVal, oldVal) {  // eslint-disable-line
      if (newVal) {
        this.date = this.datetime.toLocaleDateString('en-GB', {day: '2-digit', month: '2-digit', year: 'numeric'});
        this.hour = this.datetime.toLocaleString(undefined, {
          hour12: false, hour: '2-digit', minute:'2-digit', second:'2-digit',
        });
        this.$emit('update:modelValue', this.datetime.toISOString().replace('Z', '+00:00'));
      }
    },
    date(newVal, oldVal) {
      if (newVal && newVal !== oldVal) {
        const [day, month, year] = newVal.split('/');
        const [hours, minutes, seconds] = this.hour.split(':');
        this.datetime = new Date(year, month - 1, day, hours, minutes, seconds);
      }
    },
    hour(newVal, oldVal) {
      if (newVal && newVal !== oldVal) {
        const [day, month, year] = this.date.split('/');
        const [hours, minutes, seconds] = newVal.split(':');
        this.datetime = new Date(year, month - 1, day, hours, minutes, seconds);
      }
    }
  },
  methods: {
    updateInput(datetimeStr) {
      [this.date, this.hour] = datetimeStr.split(' ');
    },
  },
});
</script>
<style lang="scss">
</style>
