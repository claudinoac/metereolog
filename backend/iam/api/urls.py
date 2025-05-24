from iam.api import views
from django.urls import path


urlpatterns = [
    path("login", views.LoginView.as_view()),
    path("logout", views.LogoutView.as_view()),
    path("user/me", views.UserInfoView.as_view()),
]
