/*
 * =====================================================================================
 *
 *       Filename:  main.html.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  18/10/2025 03:29:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef MAIN_HTML_HPP
#define MAIN_HTML_HPP
#endif

const char index_html[] PROGMEM = R"====(
    <!DOCTYPE html>
    <html>
      <head>
        <link href="https://fonts.googleapis.com/css?family=Roboto:100,300,400,500,700,900|Material+Icons|Material+Icons+Outlined" rel="stylesheet" type="text/css">
        <link href="https://cdn.jsdelivr.net/npm/@mdi/font@^7.0.0/css/materialdesignicons.min.css" rel="stylesheet" type="text/css">
        <link href="https://cdn.jsdelivr.net/npm/quasar@2.18.2/dist/quasar.prod.css" rel="stylesheet" type="text/css">
      </head>

      <body>
        <div id="q-app">
            <q-layout view="hHh lpR fFf">
                <q-header bordered class="bg-primary text-white">
                  <q-toolbar>
                    <q-btn dense flat round icon="menu" @click="openSidebar = !openSidebar"></q-btn>

                    <q-toolbar-title>
                      <q-avatar>
                        <img src="https://app.metereolog.site/assets/logo-metereolog-white-D7FqNAoH.png">
                      </q-avatar>
                      Metereolog Station ARA 01
                    </q-toolbar-title>
                  </q-toolbar>
                </q-header>

                <q-drawer show-if-above v-model="openSidebar" side="left" bordered>
                    <q-list>
                        <template v-for="(menuItem, index) in menuList" :key="index">
                            <q-item clickable :href="menuItem.path">
                                <q-item-section avatar>
                                    <q-icon :name="menuItem.icon"></q-icon>
                                </q-item-section>
                                <q-item-section>
                                    {{ menuItem.label }}
                                </q-item-section>
                            </q-item>
                            <q-separator :key="'sep' + index"  v-if="menuItem.separator"></q-separator>
                        </template>
                    </q-list>
                </q-drawer>

                <q-page-container>
                    <q-page class="flex items-center justify-center">
                        <q-card style="max-width: 560px">
                            <q-card-section>
                                <div class="text-h4">Sensor Stats:</div>
                            </q-card-section
                            <q-separator inset></q-separator>
                            <q-card-section>
                                <q-list bordered separator>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-thermometer" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Temperature (BMP): <b>{{ sensors.bmp.temperature }} &deg;C</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-altimeter" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Altitude (BMP): <b>{{ sensors.bmp.altitude }} m</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-gauge" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Pressure (BMP): <b>{{ sensors.bmp.pressure }} hPa</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-thermometer" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Temperature (DHT_11): <b>{{ sensors.dht_11.temperature }} &deg;C</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-cloud-percent" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Humidity (DHT-11): <b>{{ sensors.dht_11.humidity }} %</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-wind-power" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Wind Speed: <b>{{ sensors.anemometer }} km/h</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-sign-direction" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Wind Direction: <b>{{ windDirMapping[sensors.wind_dir] }}</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-meter-electric-outline" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Power Supply Voltage: <b>{{ sensors.voltmeter }} V</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-current-dc" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Power Supply Current: <b>{{ sensors.amperimeter }} A</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                    <q-item>
                                        <q-item-section avatar>
                                            <q-icon color="primary" name="mdi-lightning-bolt-outline" />
                                        </q-item-section>
                                        <q-item-section>
                                            <span style="text-size: 16px">
                                                Consumed Power: <b>{{ sensors.power }} W</b>
                                            </span>
                                        </q-item-section>
                                    </q-item>
                                </q-list>
                            <q-card-section>
                        </q-card
                    </q-page>
                </q-page-container>
            </q-layout>
        </div>

        <script src="https://cdn.jsdelivr.net/npm/vue@3/dist/vue.global.prod.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/quasar@2.18.2/dist/quasar.umd.prod.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/quasar@2.18.2/dist/icon-set/mdi-v7.umd.prod.js"></script>
        
        <script>
          const app = Vue.createApp({
            data() {
              return {
                openSidebar: false,
                menuList: [
                    { label: 'Home', path: '/', separator: true, icon: 'mdi-home' },
                    { label: 'Update Firmware', path: '/update', icon: 'mdi-upload' },
                ],
                sensors: {
                    dht_11: {
                        temperature: 0,
                        humidity: 0,
                    },
                    bmp: {
                        temperature: 0,
                        altitude: 0,
                        pressure: 0,
                    },
                    anemometer: 0,
                    wind_dir: '',
                    amperimeter: 0,
                    voltmeter: 0,
                    power: 0,
                },
                windDirMapping: {
                    N: 'North',
                    S: 'South',
                    W: 'West',
                    SW: 'SouthWest',
                    NW: 'NorthWest',
                    E: 'East',
                    NE: 'NorthEast',
                    SE: 'SouthEast',
                },
                isFetching: false,
              };
            },
            created() {
                setInterval(this.loadStats, 5000);
            },
            methods: {
                loadStats() {
                    if(!this.isFetching) {
                        this.isFetching = true;
                        var that = this;
                        axios.get('/stats').then(
                            function (response) {
                                if (response.data.dht_11.temperature) {
                                    that.sensors.dht_11 = response.data.dht_11;
                                }
                                if (response.data.bmp.temperature) {
                                    that.sensors.bmp = response.data.bmp;
                                }
                                if (response.data.voltmeter) {
                                    that.sensors.voltmeter = response.data.voltmeter;
                                }
                                if (response.data.amperimeter) {
                                    that.sensors.amperimeter = response.data.amperimeter;
                                }
                                that.sensors.power = that.sensors.voltmeter * that.sensors.amperimeter;
                                if (response.data.wind_dir) {
                                    that.sensors.wind_dir = response.data.wind_dir;
                                }
                                if (response.data.anemometer) {
                                    that.sensors.anemometer = response.data.anemometer;
                                }
                                that.isFetching = false;
                            }
                        ).catch(
                            function (error) { 
                                that.isFetching = false;
                            }
                        );
                    }
                },
            },           
          });
          const devtools = window.__VUE_DEVTOOLS_GLOBAL_HOOK__;
          if (devtools) {
              devtools.enabled = true;
              devtools.emit('app:init', app, app.version, {});
          }
          app.use(Quasar, { config: { debug: true } });
          Quasar.IconSet.set(Quasar.IconSet.mdiV7);
          app.mount('#q-app');
        </script>
      </body>
    </html>
)====";
