<template>
  <q-page class="list-devices-page">
    <div class="subheader">
      <div class="subheader--title">
        <span class="text-h4">Devices</span>
        <q-btn color="primary" no-caps label="New Device"/>
      </div>
      <q-input outlined dense :modelValue="filter" @update:modelValue="filterRows" :debounce="300">
        <template v-slot:append>
          <q-icon v-if="filter !== ''" name="close" @click="filter = ''" class="cursor-pointer" />
          <q-icon name="search" />
        </template>
      </q-input>
    </div>
    <q-table
      :rows="devices"
      :columns="columns"
      :pagination="pagination"
      @request="getList"
      @row-click="viewDevice"
    >
      <template v-slot:body-cell-is_active="props">
        <q-td :props="props">
          <q-icon name="mdi-check" v-if="props.row.is_active" color="green" size="24px"/>
          <q-icon name="mdi-close" v-else color="red" size="24px"/>
        </q-td>
      </template>
      <template v-slot:body-cell-actions="props">
        <q-td :props="props">
            <span class="q-mr-sm">
              <q-icon class="edit-btn" name="mdi-pencil-box" size="24px"/>
              <q-tooltip>Edit</q-tooltip>
            </span>
            <span>
              <q-icon class="delete-btn" name="mdi-delete" size="22px" @click="deleteDevice(props.row)"/>
              <q-tooltip>Delete</q-tooltip>
            </span>
        </q-td>
      </template>
    </q-table>
  </q-page>
</template>
<script>
import { defineComponent } from 'vue';

const columns = [
  { name: 'name', label: 'Name', field: 'name', sortable: true, align: 'left' },
  { name: 'identifier', label: 'ID', field: 'identifier', sortable: false, align: 'center' },
  { name: 'organization', label: 'Organization', field: 'organization', sortable: true, align: 'center' },
  { name: 'is_active', label: 'Enabled', field: 'is_active', sortable: true, align: 'center' },
  { name: 'sensors', label: '# Sensors', field: 'sensors', sortable: false, align: 'center' },
  { name: 'actions', label: '', field: 'identifier', sortable: false, align: 'center' },
]

export default defineComponent({
  name: 'list-devices',
  data() {
    return {
      devices: [],
      columns,
      page: 0,
      filter: '',
      pagination: {
        descending: true,
        page: 1,
        rowsPerPage: 100,
        rowsNumber: 0,
      }
    };
  },
  created() {
    this.getList();
  },
  methods: {
    async getList(options) {
      const response = await this.$api.get('devices', {
        params: {
          q: options?.filter || this.filter,
          page: options?.pagination?.page || 1
        }
      });
      if (response.status === 200) {
        this.devices = response.data.results;
        this.pagination.rowsNumber = response.data.count;
      }
    },
    filterRows(filter) {
      this.filter = filter ?? this.filter;
      this.getList();
    },
    async deleteDevice(row) {
      const response = await this.$api.delete(`device/${row.identifier}`);
      if (response.status === 204) {
        this.getList();
        this.$q.notify({
          type: 'positive',
          message: `Device ${row.name} deleted successfully`,
        });
      }
    },
    viewDevice(evt, row) {
      this.$router.push({ name: 'view-device', params: { deviceId: row.identifier } });
    },
  },
});
</script>
<style lang="scss">
.list-devices-page {
  .q-table__container {
    margin-top: 40px;
  }
  .delete-btn {
    color: black;

    &:hover {
      color: $negative;
    }
  }

  .edit-btn {
    color: black;

    &:hover {
      color: $primary;
    }
  }
}
</style>
