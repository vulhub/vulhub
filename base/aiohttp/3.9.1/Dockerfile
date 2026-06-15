FROM python:3.11
LABEL maintainer="phithon <root@leavesongs.com>"
WORKDIR /app
RUN mkdir "static"
RUN pip install aiohttp==3.9.1
COPY main.py main.py
EXPOSE 8080
CMD [ "python" ,"main.py" ]