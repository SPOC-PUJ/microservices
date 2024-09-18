import grpc
from concurrent import futures
import mat_service_pb2_grpc
import mat_service_pb2
import scipy.io
import os

class MatServiceServicer(mat_service_pb2_grpc.MatServiceServicer):
    def readMat(self, request, context):
        temp_file = "temp.mat"

        try:
            with open(temp_file, "wb") as f:
                f.write(request.file_content)
            mat_data = scipy.io.loadmat(temp_file)
            field = request.field

            if field not in mat_data:
                context.set_code(grpc.StatusCode.NOT_FOUND)
                context.set_details(f"Variable {field} not found in the MAT file.")
                return mat_service_pb2.MatResponse()

            data = mat_data[field]
            print(data)
            matrix_list = data.tolist()

            vectors = [mat_service_pb2.Vector(numbers=row) for row in matrix_list]

            return mat_service_pb2.MatResponse(matrix=vectors)

        finally:
            if os.path.exists(temp_file):
                os.remove(temp_file)

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10),
                         options=[
                             ('grpc.max_receive_message_length', 100 * 1024 * 1024),
                             ('grpc.max_send_message_length', 100 * 1024 * 1024)
                         ])
    mat_service_pb2_grpc.add_MatServiceServicer_to_server(MatServiceServicer(), server)
    server.add_insecure_port('[::]:50052')
    server.start()
    server.wait_for_termination()

if __name__ == "__main__":
    serve()
