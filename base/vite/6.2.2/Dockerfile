FROM node:22-slim

WORKDIR /usr/src

COPY . .

RUN yarn

EXPOSE 5173

CMD ["yarn", "dev", "--host", "0.0.0.0", "--strictPort"]
