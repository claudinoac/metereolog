<template>
  <q-layout view="lHh Lpr lFf">
    <q-header elevated>
      <q-toolbar>
        <q-btn
          flat
          dense
          round
          icon="menu"
          aria-label="Menu"
          @click="toggleSidebar"
        />
        <q-img width="30px" src="~assets/logo-metereolog-white.png"/>
        <q-toolbar-title>
          Metereolog
        </q-toolbar-title>
        <q-space/>
        <q-btn-dropdown stretch flat no-caps icon="mdi-account" :label="userData.name">
          <q-list>
            <q-item clickable v-ripple @click="logout">
              <q-item-section>
                <div class="flex row items-center">
                  <q-icon name="mdi-logout" size="24px" class="q-pr-sm"/>
                  <q-item-label>Logout</q-item-label>
                </div>
              </q-item-section>
            </q-item>
            <q-item clickable v-ripple>
              <q-item-section>
                <div class="flex row items-center">
                  <q-icon name="mdi-account-details" size="24px" class="q-pr-sm"/>
                  <q-item-label>User Profile</q-item-label>
                </div>
              </q-item-section>
            </q-item>
          </q-list>
        </q-btn-dropdown>
      </q-toolbar>
    </q-header>

   <q-drawer
      v-model="sidebarOpen"
      show-if-above
      :width="200"
      :breakpoint="500"
      bordered
      :class="$q.dark.isActive ? 'bg-grey-4' : 'bg-grey-2'"
    >
      <q-scroll-area class="fit">
        <q-list>

          <template v-for="(menuItem, index) in menuList" :key="index">
            <q-item clickable v-ripple :to="menuItem.to" linkActiveClass="link-active">
              <q-item-section>
                <div class="flex row">
                  <q-icon :name="menuItem.icon" size="24px" class="text-orage-7 q-pr-sm"/>
                  {{ menuItem.label }}
                </div>
              </q-item-section>
            </q-item>
            <q-separator :key="'sep' + index"  v-if="menuItem.separator" />
          </template>

        </q-list>
      </q-scroll-area>
    </q-drawer>

    <q-page-container>
      <router-view />
    </q-page-container>
  </q-layout>
</template>

<script>
import { defineComponent } from 'vue';
import { mapState } from 'pinia';
import { useStore } from 'src/stores/store';

const menuList = [
  {
    icon: 'home',
    label: 'Home',
    separator: true,
    to: { name: 'home' },
  },
  {
    icon: 'mdi-account-multiple',
    label: 'Users',
    separator: false,
    color: 'primary',
  },
  {
    icon: 'mdi-account-lock',
    label: 'Roles',
    separator: true
  },
  {
    icon: 'settings',
    label: 'Organization Settings',
    separator: false
  },
  {
    icon: 'mdi-account-details',
    label: 'User Profile',
    separator: true
  },
  {
    icon: 'mdi-memory',
    label: 'Devices',
    to: { name: 'list-devices' },
    separator: false
  },
  {
    icon: 'mdi-domain',
    label: 'Organizations',
    separator: false,
    admin: true,
  },
];

export default defineComponent({
  name: 'MainLayout',
  data () {
    return {
      menuList: [],
      sidebarOpen: false,
    }
  },
  created() {
    menuList.forEach((item) => {
      if (!item.admin || this.userData.is_admin) {
        this.menuList.push(item);
      }
    });
  },
  computed: {
      currentPath() {
          return window.location.pathname;
      },
    ...mapState(useStore, { userData: 'getUserData' }),
  },
  methods: {
    toggleSidebar () {
      this.sidebarOpen = !this.sidebarOpen
    },
    async logout() {
      const response = await this.$api.post('iam/logout');
      if (response.status === 204) {
        this.$router.push({ name: 'login'});
      }
    },
  },
});
</script>
<style lang="scss">
.link-active {
  color: $primary;
  font-weight: 700 !important;
}
</style>
