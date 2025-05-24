from rest_framework.views import APIView
from rest_framework import status
from rest_framework.permissions import AllowAny, IsAuthenticated
from rest_framework.response import Response
from django.contrib.auth import login
from iam.api.serializers import LoginSerializer, UserInfoSerializer
from django.contrib.auth import logout


class LoginView(APIView):
    serializer_class = LoginSerializer
    permission_classes = [AllowAny]

    def post(self, request):
        serializer = self.serializer_class(data=request.data)
        if not serializer.is_valid():
            return Response(data=serializer.errors, status=status.HTTP_401_UNAUTHORIZED)
        user = serializer.validated_data["user"]
        login(request, user)
        return Response(
            status=status.HTTP_201_CREATED,
            data={"detail": "Login success."}
        )


class LogoutView(APIView):
    serializer_class = LoginSerializer
    permission_classes = [IsAuthenticated]

    def post(self, request):
        logout(request)
        return Response(status=status.HTTP_204_NO_CONTENT)


class UserInfoView(APIView):
    serializer_class = UserInfoSerializer

    def get(self, request):
        return Response(
            data=self.serializer_class(instance=request.user).data
        )
