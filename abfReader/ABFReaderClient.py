import grpc
import abf_service_pb2
import abf_service_pb2_grpc

def run():
    with grpc.insecure_channel(
        'localhost:8080', 
                                        options=[
            ('grpc.max_receive_message_length', 100 * 1024 * 1024),  # 100 MB
            ('grpc.max_send_message_length', 100 * 1024 * 1024)  # 100 MB
        ]  # 100 MB
    ) as channel:
        stub = abf_service_pb2_grpc.ABFServiceStub(channel)
        
        # Leer el archivo ABF como bytes
        with open("11361009.ABF", "rb") as f:
            file_content = f.read()
        print(file_content[:10])
        # Enviar el archivo al servidor
        response = stub.readAbf(abf_service_pb2.ABFRequest(file_content=file_content))
        
        # Imprimir la respuesta (arreglo de números)
        
        print(response.numbers[:10])
        print(response.sp)

if __name__ == "__main__":
    run()
