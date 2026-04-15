FROM node:22-alpine as builder

COPY frontend/package.json frontend/yarn.lock  /code/
WORKDIR /code/
RUN npm update
COPY frontend /code/
RUN yarn install --update-lockfile --update-checksums
RUN yarn build 

FROM nginx as server
COPY --from=builder /code/dist /usr/share/nginx/
RUN mkdir -p /usr/share/nginx/django
COPY ./backend/static /usr/share/nginx/django/static
COPY ./frontend/nginx/nginx.prod.conf /etc/nginx/conf.d/ 
