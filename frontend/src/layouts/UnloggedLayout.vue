<template>
  <q-layout view="lHh Lpr lFf">
    <q-page-container>
        <q-img
            rel="preload" as="image"
            src="~assets/metereolog-cover.png" fit="cover"
            class="absolute full-width full-height" async defer
        />
      <router-view />
    </q-page-container>
  </q-layout>
</template>
