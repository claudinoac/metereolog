FROM node:22-alpine

COPY ./package.json ./yarn.lock  /code/
WORKDIR /code/
RUN npm update
COPY . /code/
RUN yarn install --update-lockfile --update-checksums
CMD yarn dev
