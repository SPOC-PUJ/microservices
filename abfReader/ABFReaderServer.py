import grpc
from concurrent import futures
import abf_service_pb2
import abf_service_pb2_grpc
import pyabf  # Librería para procesar archivos ABF
import os
class ABFServiceServicer(abf_service_pb2_grpc.ABFServiceServicer):
    def readAbf(self, request, context):
        # Guardar temporalmente el archivo ABF recibido
        print("mensaje recibido")
        temp_file = "temp.abf"
        
        try:
            with open(temp_file, "wb") as f:
                f.write(request.file_content)

            # Leer el archivo ABF usando pyabf
            abf = pyabf.ABF(temp_file)

            # Extraer los datos y la tasa de muestreo
            data = abf.sweepY
            print(data)
            
            # Devolver los datos como un arreglo de floats y la tasa de muestreo
            return abf_service_pb2.ABFResponse(numbers=data, sp=abf.sampleRate)

        finally:
            # Eliminar el archivo temporal después de su uso
            if os.path.exists(temp_file):
                os.remove(temp_file)
                print(f"{temp_file} eliminado.")
def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10),
                                options=[
            ('grpc.max_receive_message_length', 100 * 1024 * 1024),  # 100 MB
            ('grpc.max_send_message_length', 100 * 1024 * 1024)  # 100 MB
        ])
    abf_service_pb2_grpc.add_ABFServiceServicer_to_server(ABFServiceServicer(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    server.wait_for_termination()

if __name__ == "__main__":
    serve()
