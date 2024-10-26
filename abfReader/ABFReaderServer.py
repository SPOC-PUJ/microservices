import grpc
from concurrent import futures
import abf_service_pb2
import abf_service_pb2_grpc
import pyabf  # Librería para procesar archivos ABF
import os

## @package abf_service
#  @defgroup ABF ABF Reader
#  This module implements a gRPC service to process ABF files.
#  It uses the pyABF library to read the file and extract the signal data and sample rate.
#  @{

## A gRPC service class that handles ABF file processing.
#
#  The class receives ABF files via gRPC, processes them using the pyABF library, and returns the
#  extracted signal data and sample rate.
class ABFServiceServicer(abf_service_pb2_grpc.ABFServiceServicer):
    ## Processes the ABF file received in the request and returns its data and sampling rate.
    #
    #  The method saves the received ABF file temporarily, processes it using pyABF, and extracts
    #  the signal data and sampling rate.
    #
    #  @param request The gRPC request containing the ABF file as bytes (file_content).
    #  @param context The gRPC context (used for error handling and metadata).
    #
    #  @return abf_service_pb2.ABFResponse The response containing the ABF data and sample rate.
    #
    #  @exception IOError Raised if an error occurs when reading or writing the file.
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
