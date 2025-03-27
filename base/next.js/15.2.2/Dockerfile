FROM node:22-slim

WORKDIR /app

COPY . .
RUN yarn \
    && yarn build \
    && rm -rf /usr/local/share/.cache/yarn

EXPOSE 3000

CMD ["yarn", "start"]
