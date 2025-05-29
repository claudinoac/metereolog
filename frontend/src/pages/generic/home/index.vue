<template>
  <q-page class="home-view flex flex-center column">
    <div class="content">
      <div class="getting-started">
        <div class="title">
          <img src="~assets/logo-metereolog.png"/>
          <span class="text-h4">Welcome to Metereolog</span>
        </div>
        <div class="text-h5">Getting Started</div>
        <div>
          <span>
            To get started, view or add your devices, configure sensors and view their data in near real-time.
          </span>
          <div class="actions q-pt-lg">
            <q-btn class="q-mr-md" color="primary" label="Add Device"/>
            <q-btn :to="{ name: 'list-devices' }" color="primary" label="View Devices"/>
          </div>
        </div>
      </div>
      <div class="icons">
        <q-img src="~assets/home-icons.png"/>
      </div>
    </div>
    <div class="bottom-wave">
      <svg width="100%" height="100%" id="svg" viewBox="0 0 1440 450" xmlns="http://www.w3.org/2000/svg" class="transition duration-300 ease-in-out delay-150"><path d="M 0,600 L 0,225 C 127.06666666666666,201.93333333333334 254.13333333333333,178.86666666666665 420,197 C 585.8666666666667,215.13333333333335 790.5333333333333,274.46666666666664 967,286 C 1143.4666666666667,297.53333333333336 1291.7333333333333,261.26666666666665 1440,225 L 1440,600 L 0,600 Z" stroke="none" stroke-width="0" fill="#bbdefb" fill-opacity="1" class="transition-all duration-300 ease-in-out delay-150 path-0"></path></svg>
    </div>
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
  justify-content: space-between;
  align-items: baseline;

  &.q-page {
    padding: 0;
  }

  .title {
    color: $primary;
    display: flex;
    align-items: center;
    gap: 20px;
    margin-bottom: 30px;

    > img {
      height: 100px;
    }
  }

  .content {
    padding: 80px 60px 80px 60px;
    display: flex;
    flex-flow: row;
    justify-content: space-between;
    align-items: center;
    width: 100%;

    .getting-started {
      display: flex;
      flex-flow: column;
      gap: 20px;
    }

    .icons {
      display: flex;
      justify-content: end;
      max-width: 780px;
      width: calc(100% - 550px);
      min-width: 400px;
      z-index: 1000;
    }
  }
  .bottom-wave {
    position: sticky;
    bottom: 0;
    right: 0;
    width: 100%;
    margin-right: 40px;
  }
}
</style>
