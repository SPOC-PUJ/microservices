# Etapa 1: Construcción
FROM ghcr.io/spoc-puj/mygrpc_image:latest AS builder

# Hacer git pull
RUN cd /root/server

# Copiar la carpeta FFT_EIGEN al directorio de trabajo en el contenedor
COPY . /root/server/FFT_EIGEN

# Cambiar al directorio FFT_EIGEN
WORKDIR /root/server/FFT_EIGEN

# Crear el directorio de construcción y compilar el proyecto
RUN mkdir -p build && cd build \
    && cmake .. \
    && make

# Etapa 2: Imagen final
FROM ubuntu:latest

# Establecer el directorio de trabajo en el contenedor final
WORKDIR /app

# Copiar el ejecutable compilado desde la imagen de construcción para disminuir peso
COPY --from=builder /root/server/FFT_EIGEN/build/server /app/server

# Asegurar que el ejecutable tenga los permisos correctos
RUN chmod +x /app/server

# Exponer el puerto en el que el microservicio está corriendo
EXPOSE 5001

# Definir el comando por defecto para ejecutar el servidor
CMD ["/app/server"]
