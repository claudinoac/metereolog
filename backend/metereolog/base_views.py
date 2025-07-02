from rest_framework.permissions import IsAuthenticated
from rest_framework.generics import ListCreateAPIView
from django.db.models import Q


class BaseListView(ListCreateAPIView):
    serializer_class = None
    write_serializer_class = None
    model_class = None
    permission_classes = [IsAuthenticated]
    filterable_params = []
    sortable_params = []
    default_order_field = "-id"

    def __init__(self, *args, **kwargs):
        if not self.write_serializer_class:
            self.write_serializer_class = self.serializer_class
        super(BaseListView, self).__init__(*args, **kwargs)

    def filter_queryset(self, queryset):
        query_filter = self.request.GET.get('q')
        model_filter = Q()
        if query_filter:
            for filter_param in self.filterable_params:
                model_filter |= Q(**{f"{filter_param}__icontains": query_filter})
        return queryset.filter(model_filter).distinct()

    def order_queryset(self, queryset):
        default_order_field = self.default_order_field
        order_by = self.request.GET.get('order_by', default_order_field)
        if order_by and order_by.replace('-', '') in self.sortable_params:
            queryset = queryset.order_by(order_by)
        return queryset

    def get_base_queryset(self):
        return self.model_class.objects.filter(organization=self.request.user.organization)

    def get_queryset(self):
        return self.order_queryset(
            self.filter_queryset(
                self.get_base_queryset()
            )
        )

