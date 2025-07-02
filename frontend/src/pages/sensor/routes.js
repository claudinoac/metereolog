export default [
  {
    name: 'create-sensor',
    path: '/devices/:deviceId/sensor/new',
    component: () => import("pages/sensor/create/index.vue"),
  },
];
