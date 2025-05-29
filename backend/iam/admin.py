from django.contrib import admin
from django.contrib.auth.admin import UserAdmin
from django.contrib.auth.forms import UserCreationForm, UserChangeForm
from django.contrib.auth.models import Group

from iam.models import User, Organization, Permission, Role


class UserCreationForm(UserCreationForm):
    class Meta:
        model = User
        fields = ("email",)


class UserChangeForm(UserChangeForm):
    class Meta:
        model = User
        fields = ("email",)


class UserAdmin(UserAdmin):
    add_form = UserCreationForm
    form = UserChangeForm
    model = User
    list_display = ("name", "email", "is_admin", "is_active",)
    list_filter = ("first_name", "last_name", "email", "is_admin", "is_active",)
    filter_horizontal = ()
    fieldsets = (
        (None, {"fields": ("first_name", "last_name", "email", "password")}),
        ("Permissions", {"fields": ("is_admin", "is_active", "organization")}),
    )
    add_fieldsets = ((
        None, {
            "classes": ("wide",),
            "fields": (
                "first_name", "last_name", "email", "password1", "password2",
                "is_admin", "is_active", "roles", "organization"
            )
        }
    ))
    search_fields = ("email", "first_name", "last_name")
    ordering = ("email", "first_name", "last_name")


class OrganizationAdmin(admin.ModelAdmin):
    pass


class PermissionAdmin(admin.ModelAdmin):
    pass


class RoleAdmin(admin.ModelAdmin):
    pass


admin.site.register(Organization, OrganizationAdmin)
admin.site.register(User, UserAdmin)
admin.site.unregister(Group)
admin.site.register(Permission, PermissionAdmin)
admin.site.register(Role, RoleAdmin)
