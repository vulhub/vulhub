FROM node:22-slim

WORKDIR /app

COPY . .
RUN yarn \
    && yarn build

EXPOSE 3000

CMD ["yarn", "start"]
