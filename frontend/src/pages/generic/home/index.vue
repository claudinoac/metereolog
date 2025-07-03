<template>
  <q-page class="home-view flex flex-center column">
    <q-img
        rel="preload" as="image"
        src="~assets/home-icons.png" fit="scale-down"
        class="absolute full-width full-height" async defer
    />
    <q-card class="content">
      <div class="getting-started">
        <div class="title">
          <img src="~assets/logo-metereolog.png"/>
          <span class="text-h4">Welcome to Metereolog</span>
        </div>
        <div class="text-h5">Getting Started</div>
        <div class="flex justify-center">
          <span>
            To get started, view or add your devices, configure sensors and view their data in near real-time.
          </span>
          <div class="actions q-pt-lg">
            <q-btn class="q-mr-md" color="primary" label="Add Device" :to="{ name: 'create-device' }"/>
            <q-btn :to="{ name: 'list-devices' }" color="primary" label="View Devices"/>
          </div>
        </div>
      </div>
    </q-card>
  </q-page>
</template>
<script>
import { defineComponent } from 'vue';

export default defineComponent({
  name: 'home-page',
});
</script>
<style lang="scss">
.home-view {
  display: flex;
  width: 100%;
  height: 100%;

  &.q-page {
    padding: 0;
    margin: 0;
  }

  .content {
    display: flex;
    flex-flow: row;
    justify-content: space-between;
    align-items: center;
    align-self: center;
    justify-self: center;
    max-width: 600px;
    width: 100%;
    background-color: rgba(255,255,255,0.97);
    border-radius: 16px;
    padding: 32px;

    .getting-started {
      z-index: 1000;
      display: flex;
      flex-flow: column;
      gap: 20px;
      justify-content: center;
      align-items: center;
    }
  }

  .title {
    color: $primary;
    display: flex;
    align-items: center;
    flex-flow: column;
    justify-content: center;
    gap: 20px;
    margin-bottom: 30px;

    > img {
      height: 100px;
    }
  }
}
</style>
