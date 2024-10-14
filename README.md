# SPOK microservices Documentation

## Descripción Breve 🤗

SPOK es una aplicación web accesible, de código abierto, diseñada para el procesamiento de señales médicas, específicamente electroencefalografía (EEG). Acepta archivos en formatos `.abd`, `.edf` y `.mat`, ofreciendo herramientas similares a EEGLAB de manera gratuita. SPOK aprovecha la potencia de clústeres de Kubernetes para manejar procesos de alto costo computacional.

## Características 🧠

- **Procesamiento de señales EEG**: Incluye funcionalidades como Convolución, Transformada rápida de Fourier (FFT), y Transformada Wavelet Continua (CWT).
- **Formatos soportados**: `.abd`, `.edf`, `.mat`.
- **Escalabilidad**: Basado en microservicios desplegados en Kubernetes.
- **Accesibilidad**: Compatible con navegadores como Chromium, Safari y Mozilla.
- **Código abierto y gratuito**: No se almacena información identificable del usuario.

## Tabla de Contenidos 🧾

- [Descripción](#descripción-breve)
- [Uso](#uso)
- [Arquitectura](#arquitectura)
- [Contacto](#contacto)


## Uso 🔬

### 1. Subir un archivo EEG

Desde la interfaz web, puedes subir un archivo en formato `.abd`, `.edf`, o `.mat` para comenzar el procesamiento.

### 2. Seleccionar un análisis

Elige entre las diferentes herramientas de procesamiento disponibles, como la Transformada rápida de Fourier (FFT) o la Transformada Wavelet Continua (CWT).

### 3. Visualizar los resultados

Una vez procesada la señal, podrás visualizar los resultados en forma de gráficos interactivos y descargar los datos procesados.

## Arquitectura 🌉

SPOC está compuesto por múltiples microservicios, cada uno encargado de realizar un tipo específico de procesamiento de señal. Estos microservicios incluyen:

- **FFT Service**: Realiza la transformada rápida de Fourier.
- **CWT Service**: Implementa la transformada wavelet continua.
- **Signals Averge** y otros microservicios especializados.

Cada microservicio está desplegado en un clúster de Kubernetes, lo que permite una escalabilidad y manejo eficiente de las cargas computacionales. La comunicación entre los microservicios se realiza utilizando gRPC.
