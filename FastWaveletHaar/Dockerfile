# Etapa 1: Construcción
FROM ghcr.io/spoc-puj/mygrpc_image:latest AS builder


RUN cd /root/server

# Copiar la carpeta FastWaveletHarr al directorio de trabajo en el contenedor
COPY . /root/server/FastWaveletHarr

# Cambiar al directorio FastWaveletHarr
WORKDIR /root/server/FastWaveletHarr

# Crear el directorio de construcción y compilar el proyecto
RUN mkdir -p build && cd build \
    && cmake .. \
    && make

# Etapa 2: Imagen final
FROM ubuntu:latest

# Establecer el directorio de trabajo en el contenedor final
WORKDIR /app

# Copiar el ejecutable compilado desde la imagen de construcción para disminuir peso
COPY --from=builder /root/server/FastWaveletHarr/build/server /app/server

# Asegurar que el ejecutable tenga los permisos correctos
RUN chmod +x /app/server

# Exponer el puerto en el que el microservicio está corriendo
EXPOSE 5001

# Definir el comando por defecto para ejecutar el servidor
CMD ["/app/server"]
