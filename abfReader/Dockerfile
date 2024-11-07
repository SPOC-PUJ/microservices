FROM python:3

COPY . /app

WORKDIR /app

# Crear un subdirectorio en /app para almacenar archivos temporales y darle permisos
RUN mkdir -p /temp && chmod 777 /temp

RUN pip install --upgrade setuptools

RUN pip install -r requirements.txt

EXPOSE 50051

ENTRYPOINT [ "python" ]

CMD [ "ABFReaderServer.py" ]
