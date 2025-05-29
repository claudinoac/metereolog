from rest_framework import serializers
from django.contrib.auth import authenticate
from iam.models import User


class LoginSerializer(serializers.Serializer):
    email = serializers.EmailField()
    password = serializers.CharField(max_length=256)

    def validate(self, data):
        user = authenticate(username=data["email"], password=data["password"])
        if not user:
            raise serializers.ValidationError("Invalid username and/or password.")
        if not user.is_active:
            raise serializers.ValidationError("User not active.")
        data["user"] = user
        return data


class UserInfoSerializer(serializers.ModelSerializer):
    organization = serializers.SerializerMethodField(read_only=True)

    class Meta:
        model = User
        fields = [
            "name",
            "is_admin",
            "organization",
        ]

    def get_organization(self, instance):
        organization = instance.organization
        return {
            "name": organization.name,
            "id": organization.identifier,
        }
