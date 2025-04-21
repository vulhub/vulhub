FROM node:12

LABEL maintainer="phithon <root@leavesongs.com>"

ADD https://raw.githubusercontent.com/vishnubob/wait-for-it/master/wait-for-it.sh /usr/local/bin/wait-for-it.sh

ENV ME_CONFIG_EDITORTHEME="default" \
    ME_CONFIG_MONGODB_SERVER="mongo" \
    MONGODB_PORT="27017" \
    ME_CONFIG_MONGODB_ENABLE_ADMIN="true" \
    ME_CONFIG_BASICAUTH_USERNAME="" \
    ME_CONFIG_BASICAUTH_PASSWORD="" \
    VCAP_APP_HOST="0.0.0.0" \
    MONGO_EXPRESS="0.53.0"

RUN npm install mongo-express@$MONGO_EXPRESS \
    && chmod +x /usr/local/bin/wait-for-it.sh

WORKDIR /node_modules/mongo-express

COPY docker-entrypoint.sh /
ENTRYPOINT [ "/docker-entrypoint.sh" ]
CMD ["mongo-express"]