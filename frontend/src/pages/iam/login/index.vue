<template>
  <q-page class="login-view flex flex-center column">
    <q-card bordered class="login-card shadow-12">
      <q-card-section class="logo-section">
        <div class="logo">
          <img src="~assets/logo-metereolog.png"/>
          <span>Metereolog</span>
        </div>
      </q-card-section>
      <q-form @submit.prevent="submitLogin">
        <div class="text-h4 q-pt-sm text-center q-pb-sm">Login</div>
        <q-card-section class="inputs">
          <q-input
            label="email" outlined dense v-model="form.email"
            :rules="[(val) => !!val || 'This field is required.']"
          />
          <q-input
            label="password" type="password" outlined dense v-model="form.password"
            :rules="[(val) => !!val || 'This field is required.']"
          />
        </q-card-section>
        <q-card-actions>
          <q-btn class="submit-btn" type="submit" color="primary" label="Login"/>
        </q-card-actions>
      </q-form>
    </q-card>
  </q-page>
</template>

<script>
import { defineComponent } from 'vue';

export default defineComponent({
  name: 'login-page',
  data() {
    return {
      form: {
        email: '',
        password: '',
      }
    };
  },
  methods: {
    async submitLogin() {
        const response = await this.$api.post('iam/login', this.form);
        if (response.status === 201) {
          this.$router.push({ name: 'home' });
        }
    },
  },
});
</script>
<style lang="scss">
.login-view {
  .login-card {
      background-color: $background;
      min-width: 500px;
      border-radius: 15px;
      padding-bottom: 10px;
  }
  .logo {
    display: flex;
    flex-flow: row;
    align-items: center;
    justify-self: center;
    gap: 10px;
    color: $primary;
    font-size: 30px;
    font-weight: 500;

    > img {
      width: 80px;
    }
  }

  .logo-section {
    background-color: $background;
    padding-bottom: 0;
  }

  .submit-btn {
    display: flex;
    width: 100%;
    margin: 0 10px;
  }

  .inputs {
    display: flex;
    flex-flow: column;
    gap: 10px;
  }
}
</style>
