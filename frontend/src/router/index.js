import { defineRouter } from '#q-app/wrappers';
import { createRouter, createWebHistory } from 'vue-router';
import { Loading } from 'quasar';
import { api } from 'boot/axios';
import routes from './routes';
import { useStore } from '../stores/store';


export default defineRouter(function (/* { store, ssrContext } */) {

  const store = useStore();
  const unloggedRoutes = [
    'login',
    '404',
  ];

  const Router = createRouter({
    scrollBehavior: () => ({ left: 0, top: 0 }),
    routes,
    history: createWebHistory(),
  });

  Router.beforeEach(async (to, from, next) => {
    Loading.show({ message: "Loading page..."});
    if (!unloggedRoutes.includes(to.name)) {
      const response = await api.get("iam/user/me");
      if (response.status === 200) {
        store.updateUserData(response.data);
      };
    }
    next();
  });

  Router.beforeResolve(() => {
    Loading.hide();
  });

  return Router
});
