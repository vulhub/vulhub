FROM node:22-slim

WORKDIR /usr/src

COPY . .

RUN yarn \
    && rm -rf /usr/local/share/.cache/yarn

EXPOSE 3000

CMD ["yarn", "dev", "--host", "0.0.0.0", "--strictPort"]
