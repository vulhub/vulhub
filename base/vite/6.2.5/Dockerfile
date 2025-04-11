FROM node:22-slim

WORKDIR /usr/src

COPY . .

RUN yarn \
    && rm -rf /usr/local/share/.cache/yarn

EXPOSE 5173

CMD ["yarn", "dev", "--host", "0.0.0.0", "--strictPort"]
