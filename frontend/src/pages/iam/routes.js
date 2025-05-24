export default [
    {
        path: '/auth',
        name: 'auth',
        redirect: { name: 'login' },
        component: () => import('layouts/UnloggedLayout.vue'),
        children: [
          {
            path: 'login',
            name: 'login',
            component: () => import('pages/iam/login/index.vue'),
          },
        ],
    },
];
