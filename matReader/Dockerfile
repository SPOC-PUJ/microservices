FROM python:3

COPY . /app

WORKDIR /app

RUN mkdir -p /temp && chmod 777 /temp

RUN pip install -r requirements.txt

EXPOSE 50052


ENTRYPOINT [ "python" ]

CMD [ "MatReaderServer.py" ]
