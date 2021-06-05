FROM vulhub/django:3.0.3

COPY src/ /usr/src/
COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN chmod +x /docker-entrypoint.sh

WORKDIR /usr/src
ENTRYPOINT [ "/docker-entrypoint.sh"]

CMD [ "python", "manage.py", "runserver", "0.0.0.0:8000" ]