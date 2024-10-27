import grpc
from concurrent import futures
import mat_service_pb2_grpc
import mat_service_pb2
import scipy.io
import os

## @package mat_service
#  @defgroup MAT MAT Reader
#  This module implements a gRPC service to read MAT files and return the data for a specified field.
#  It uses the scipy.io library to read the MAT files.
#  @{

## A gRPC service class that handles MAT file processing.
#
#  The class receives MAT files via gRPC, processes them using scipy.io, and returns the requested
#  data field as a matrix of vectors.
class MatServiceServicer(mat_service_pb2_grpc.MatServiceServicer):

    ## Processes the MAT file received in the request and returns the data for the specified field.
    #
    #  The method saves the received MAT file temporarily, processes it using scipy.io.loadmat, and
    #  extracts the matrix corresponding to the requested field. The matrix is then returned as a list of vectors.
    #
    #  @param request The gRPC request containing the MAT file (file_content) and the field name to be extracted.
    #  @param context The gRPC context (used for error handling and metadata).
    #
    #  @return mat_service_pb2.MatResponse The response containing the extracted matrix or an error if the field is not found.
    #
    #  @exception FileNotFoundError Raised if the specified field is not found in the MAT file.
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
            #print(data)
            data = data.T
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
