export default [
  {
    name: 'list-devices',
    path: '/devices',
    component: () => import("pages/device/list/index.vue"),
  },
  {
    name: 'view-device',
    path: '/devices/:deviceId',
    component: () => import("pages/device/details/index.vue"),
  },
  {
    name: 'create-device',
    path: '/devices/new',
    component: () => import("pages/device/create/index.vue"),
  },
];
