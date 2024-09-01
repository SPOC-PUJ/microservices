# Etapa 1: Construcción
FROM ghcr.io/spoc-puj/mygrpc_image:latest AS builder

# Cambiar al directorio de trabajo
WORKDIR /root/server

# Clonar el repositorio
RUN git clone https://github.com/SPOC-PUJ/microservices.git

RUN cd /microservices/Moving_Average


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

# Exponer el puerto en el que el microservicio está corriendo
EXPOSE 5000

# Definir el comando por defecto para ejecutar el servidor
CMD ["/app/server"]
