# Etapa 1: Construcción
FROM ghcr.io/spoc-puj/mygrpc_image:latest AS builder

# Establecer el directorio de trabajo dentro del contenedor de construcción
WORKDIR /root/server/microservices

# Hacer git pull para obtener la última versión del código
RUN git pull

# Crear el directorio de construcción y compilar el proyecto
RUN mkdir -p build && cd build \
    && cmake .. \
    && make

# Etapa 2: Imagen final
FROM ubuntu:latest

# Establecer el directorio de trabajo en el contenedor final
WORKDIR /app

# Copiar el ejecutable compilado desde la imagen de construcción
COPY --from=builder /root/server/microservices/build/server /app/server

# Asegurar que el ejecutable tenga los permisos correctos
RUN chmod +x /app/server

# Definir el comando por defecto para ejecutar el servidor
CMD ["/app/server"]
