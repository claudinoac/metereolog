import IAMRoutes from '@/pages/iam/routes';

const routes = [
  {
    path: '/',
    name: 'home',
    component: () => import('layouts/MainLayout.vue'),
    children: [
      { path: '', component: () => import('pages/generic/home/index.vue') }
    ]
  },
  ...IAMRoutes,
  {
    path: '/:catchAll(.*)*',
    component: () => import('pages/ErrorNotFound.vue')
  }
]

export default routes
