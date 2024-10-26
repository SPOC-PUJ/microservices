@page kubernetes Configuración de Kubernetes para la aplicación SPOK
@tableofcontents

@section requisitos Requisitos

Para configurar Kubernetes y desplegar la aplicación SPOK, se necesitan las siguientes herramientas:

- **kubectl**: Herramienta de línea de comandos para interactuar con el clúster de Kubernetes. Para instalar kubectl, sigue las instrucciones en la [documentación oficial](https://kubernetes.io/es/docs/tasks/tools/).
  
- **istioctl**: Comando de línea para gestionar y configurar Istio. Sigue las instrucciones en la [documentación de instalación de Istio](https://istio.io/latest/docs/setup/getting-started/) para instalarlo.

- **Helm**: Administrador de paquetes para Kubernetes, que permite gestionar aplicaciones en el clúster. Puedes instalarlo siguiendo los pasos en la [documentación de Helm](https://helm.sh/docs/intro/install/).

@section istio Instalación de Istio en el clúster

Para habilitar Istio en el clúster, sigue estos pasos:

1. Instala Istio en el clúster usando el perfil `demo`:
   ```bash
   istioctl install --set profile=demo -y
   ```

2. Habilita la service mesh de Istio en el namespace `default` con el siguiente comando:
   ```bash
   kubectl label namespace default istio-injection=enabled --overwrite
   ```

3. Activa las opciones de observabilidad de Istio aplicando los complementos (addons) en el clúster. Esto se realiza en la carpeta donde se descargó Istio:
   ```bash
   kubectl apply -f samples/addons
   ```

@section keda Instalación de KEDA

Para habilitar el escalado automático con KEDA, sigue estos pasos usando Helm:

1. Agrega el repositorio de KEDA:
   ```bash
   helm repo add kedacore https://kedacore.github.io/charts
   ```

2. Actualiza el repositorio de Helm:
   ```bash
   helm repo update
   ```

3. Instala KEDA en el namespace `keda`:
   ```bash
   helm install keda kedacore/keda --namespace keda --create-namespace
   ```

@note Para obtener más información sobre KEDA, consulta la [documentación oficial](https://keda.sh/docs/2.15/deploy/).

@section desplieguespok Despliegue de la Aplicación

Para desplegar la aplicación SPOK en el clúster de Kubernetes, se utiliza **ArgoCD**. Sigue estos pasos para instalarlo:

1. Crea el namespace para ArgoCD:
   ```bash
   kubectl create ns argocd
   ```

2. Aplica la configuración de instalación de ArgoCD:
   ```bash
   kubectl apply -n argocd -f https://raw.githubusercontent.com/argoproj/argo-cd/stable/manifests/install.yaml
   ```

ArgoCD permite la gestión de aplicaciones y el despliegue automatizado en el clúster de Kubernetes.

@section clon Configuración de Archivos

Para obtener y aplicar las configuraciones necesarias para Istio y ArgoCD, sigue estos pasos:

1. Clona el repositorio completo que contiene los archivos de configuración:
   ```bash
   git clone https://github.com/SPOC-PUJ/microservices.git
   ```

2. Aplica las configuraciones de Istio:
   ```bash
   kubectl apply -f microservices/istio-conf
   ```

3. Aplica las configuraciones de ArgoCD en el namespace `argocd`:
   ```bash
   kubectl apply -f microservices/Argo -n argocd
   ```

Estas configuraciones preparan el entorno para el despliegue y gestión de la aplicación SPOK en el clúster de Kubernetes.

@remark Si después deseas liberar espacio, puedes eliminar el repositorio clonado con el siguiente comando:
   ```bash
   rm -rf microservices
   ```
