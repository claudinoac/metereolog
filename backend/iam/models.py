from django.db import models
from django.contrib.auth.models import AbstractBaseUser, BaseUserManager
from django.utils.translation import gettext_lazy as _


class Permission(models.Model):
    class Meta:
        unique_together = ["resource_type", "operation"]

    class OperationTypes(models.TextChoices):
        CREATE = "create", _("Create")
        LIST = "list", _("List")
        READ = "read", _("Read")
        UPDATE = "update", _("Update")
        DELETE = "delete", _("Delete")

    name = models.CharField(max_length=255)
    resource_type = models.CharField(max_length=64)
    operation = models.CharField(max_length=32, choices=OperationTypes.choices)

    def __str__(self):
        return f"{self.name} -- {self.resource_type}::{self.operation} ({self.id})"


class Organization(models.Model):
    name = models.CharField(max_length=64)
    is_active = models.BooleanField(default=True)

    @property
    def users(self):
        return self.user_set.all()

    def __str__(self):
        return f"{self.name} ({self.id})"


class Role(models.Model):
    name = models.CharField(max_length=64)
    organization = models.ForeignKey(to=Organization, on_delete=models.CASCADE)
    permissions = models.ManyToManyField(to=Permission)
    is_active = models.BooleanField(default=True)

    def __str__(self):
        return f"{self.name} - Org: {self.organization.name} ({self.id})"


class UserManager(BaseUserManager):
    def create_user(self, email, password=None, **extra_fields):
        if not email:
            raise ValueError('Email is required.')
        email = self.normalize_email(email)
        user = self.model(email=email, **extra_fields)
        user.set_password(password)
        user.save(using=self._db)
        return user

    def create_superuser(self, email, password=None, **extra_fields):
        extra_fields.setdefault('is_admin', True)
        extra_fields.setdefault('is_active', True)

        if extra_fields.get('is_admin') is not True:
            raise ValueError('Superuser must have is_admin=True.')
        return self.create_user(email, password, **extra_fields)


class User(AbstractBaseUser):
    email = models.EmailField(unique=True)
    first_name = models.CharField(max_length=30)
    last_name = models.CharField(max_length=150)
    is_admin = models.BooleanField(default=False)
    is_active = models.BooleanField(default=True)
    ts_created = models.DateTimeField(auto_now_add=True)
    organization = models.ForeignKey(to=Organization, on_delete=models.CASCADE, null=True)
    roles = models.ManyToManyField(to=Role)

    objects = UserManager()

    USERNAME_FIELD = 'email'
    REQUIRED_FIELDS = ['first_name', 'last_name']

    def __str__(self):
        if self.organization:
            return f"{self.name} - Org: {self.organization.name} <self.email>"
        return f"{self.name} - NO ORG <self.email>"


    def has_perm(self, perm, obj=None):
        return self.is_admin

    def has_module_perms(self, app_label):
        return self.is_admin

    @property
    def is_staff(self):
        return self.is_admin

    @property
    def name(self):
        return f"{self.first_name} {self.last_name}"

    def get_short_name(self):
        return self.first_name

    def get_full_name(self):
        return self.name
