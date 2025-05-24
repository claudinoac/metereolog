import { defineStore, acceptHMRUpdate } from 'pinia'

export const useStore = defineStore('store', {
  state: () => ({
    userData: {
      name: '',
    },
  }),
  getters: {
    getUserData(state) {
      return state.userData;
    },
  },
  actions: {
    updateUserData(userData) {
      this.userData = userData;
    },
  },
})

if (import.meta.hot) {
  import.meta.hot.accept(acceptHMRUpdate(useStore, import.meta.hot))
}
