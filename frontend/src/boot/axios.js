import { defineBoot } from '#q-app/wrappers'
import axios from 'axios'
import { Notify } from 'quasar';

// Be careful when using SSR for cross-request state pollution
// due to creating a Singleton instance here;
// If any client changes this (global) instance, it might be a
// good idea to move this instance creation inside of the
// "export default () => {}" function below (which runs individually
// for each client)
const api = axios.create({ baseURL: '/api' })
const loginRoute = '/auth/login';

api.interceptors.response.use(
    (response) => {
        const message = response.data?.detail || response.data?.message;
        if (message) {
            Notify.create({
                type: 'positive',
                message,
            });
        }
        return response;
    },
    async (error) => {
        if (
          error.response && error.response.status >= 400
        ) {
            if (error.response.status === 401 && window.location.pathname !== loginRoute) {
              Notify.create({
                type: 'negative',
                message: 'You session has expired. Please login again.',
              });
              window.location = loginRoute;
            }
            let responseData = error.response.data;
            let messages = [];
            const singleMessage = responseData?.detail || responseData?.error;
            if (singleMessage) messages.push(singleMessage);
            if (messages.length === 0 && error.response.status === 404) {
                messages = ['Requested resource not found'];
            } else if (typeof responseData === 'object' && Object.keys(responseData).length > 0) {
                if (responseData['non_field_errors']) {
                  messages.push(responseData['non_field_errors'][0]);
                } else {
                  Object.entries(responseData).forEach(([key, value]) => {
                      messages.push(`${key}: ${value}`);
                  });
                }
            } else {
                messages = ['There was an unexpected error. Please, try again later.'];
            }
            messages.forEach((message) => {
                Notify.create({
                    type: 'negative',
                    message,
                });
            });
        }
        return error?.response || {};
    },
);

export default defineBoot(({ app }) => {
  // for use inside Vue files (Options API) through this.$axios and this.$api

  app.config.globalProperties.$axios = axios
  // ^ ^ ^ this will allow you to use this.$axios (for Vue Options API form)
  //       so you won't necessarily have to import axios in each vue file

  app.config.globalProperties.$api = api
  // ^ ^ ^ this will allow you to use this.$api (for Vue Options API form)
  //       so you can easily perform requests against your app's API
})

export { api }
