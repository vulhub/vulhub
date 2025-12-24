FROM node:24-slim

LABEL maintainer="phithon <root@leavesongs.com>"

COPY . /usr/src

RUN set -ex \
    && cd /usr/src \
    # && npx --yes create-next-app@15.5.6 next-playground --typescript --no-tailwind --no-linter --app --src-dir \
    #     --webpack --no-turbopack --use-npm --disable-git --no-import-alias \
    && npm install \
    && npm run build

WORKDIR /usr/src

EXPOSE 3000

CMD ["npm", "run", "start"]
