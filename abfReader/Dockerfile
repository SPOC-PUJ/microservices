FROM python:3.9-alpine

COPY . /app

WORKDIR /app

RUN pip install --upgrade setuptools

RUN pip install -r requirements.txt

EXPOSE 50051

ENTRYPOINT [ "python" ]

CMD [ "ABFReaderServer.py" ]
