import grpc
import mat_service_pb2_grpc
import mat_service_pb2

def run():
    with grpc.insecure_channel(
        'localhost:50052', 
                                        options=[
            ('grpc.max_receive_message_length', 100 * 1024 * 1024),  # 100 MB
            ('grpc.max_send_message_length', 100 * 1024 * 1024)  # 100 MB
        ]  # 100 MB
    ) as channel:
        stub = mat_service_pb2_grpc.MatServiceStub(channel)
        
        # Leer el archivo ABF como bytes
        with open("m_Trials_selected.mat", "rb") as f:
            file_content = f.read()
        
        # Enviar el archivo al servidor
        response = stub.readMat(mat_service_pb2.MatRequest(file_content=file_content,field = "m_Trials_selected"))
        
        # Imprimir la respuesta (arreglo de números)
        resp = response.matrix
        print(len(resp))
        print(type(resp))


if __name__ == "__main__":
    run()
