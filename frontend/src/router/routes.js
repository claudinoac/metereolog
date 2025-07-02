import { unLoggedRoutes, loggedRoutes as IAMRoutes }from '@/pages/iam/routes';
import DeviceRoutes from '@/pages/device/routes';
import SensorRoutes from '@/pages/sensor/routes';

const routes = [
  ...unLoggedRoutes,
  {
    path: '',
    redirect: '/home',
  },
  {
    path: '/',
    component: () => import('layouts/MainLayout.vue'),
    name: 'root',
    children: [
      {  name: 'home', path: 'home', component: () => import('pages/generic/home/index.vue') },
      ...IAMRoutes,
      ...DeviceRoutes,
      ...SensorRoutes,
    ]
  },
  {
    path: '/:catchAll(.*)*',
    component: () => import('pages/ErrorNotFound.vue')
  }
]

export default routes
