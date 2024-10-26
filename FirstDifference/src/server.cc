#include <grpcpp/grpcpp.h>
#include "proto/signal_first_difference.grpc.pb.h"  
#include "Eigen/Dense"  
#include <complex>
#include <iostream>


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using signal::Complex;
using signal::FirstDifferenceRequest;
using signal::FirstDifferenceResponse;
using signal::SignalGrpc;
using signal::SignalService;

/**
 * @defgroup FirstDifference First Difference Microservice
 * @brief This microservice computes the first difference of a complex signal.
 * 
 * The First Difference microservice is built using gRPC and Eigen. It processes complex-valued signals
 * and computes the first difference between consecutive elements of the signal.
 * 
 * @{
 */

/**
 * @class SignalServiceImpl
 * @brief Implements the SignalService for computing the first difference of complex signals.
 * 
 * This class provides the implementation of the SignalService, specifically the `ComputeFirstDifference` method,
 * which processes a signal, calculates its first difference (i.e., the difference between consecutive elements),
 * and returns the result.
 */

class SignalServiceImpl final : public SignalService::Service {
public:
        /**
     * @brief Computes the first difference of a signal.
     * 
     * This method receives the input signal as a gRPC request, processes it, computes the first difference
     * between consecutive elements of the complex signal, and sends the result in the response.
     * 
     * @param context The gRPC server context.
     * @param request The request containing the input complex signal.
     * @param reply The response containing the computed first difference of the signal.
     * @return A gRPC Status object indicating success or failure.
     */
    Status ComputeFirstDifference(ServerContext* context, const FirstDifferenceRequest* request,
                                  FirstDifferenceResponse* reply) override {
        try {

            const SignalGrpc& grpc_signal = request->signal(); // Ahora accede al nuevo tipo SignalGrpc
            Eigen::VectorXcd signal(grpc_signal.values_size()); // Usa values_size() para el tamaño

            for (int i = 0; i < grpc_signal.values_size(); ++i)
            {
                const Complex &complex_msg = grpc_signal.values(i); // Accede a cada Complex dentro de values
                signal[i] = std::complex<double>(complex_msg.real(), complex_msg.imag());
            }
            
            Eigen::VectorXcd result = FirstDifference(signal);

            
            for (int i = 0; i < result.size(); i++) {
                auto complex_number = result[i];
                Complex* complex_message = reply->add_result();
                complex_message->set_real(complex_number.real());
                complex_message->set_imag(complex_number.imag());
            }
            return Status::OK;
        } catch (const std::exception& ex) {
            
            return Status(grpc::INVALID_ARGUMENT, ex.what());
        }
    }

private:
        /**
     * @brief Computes the first difference of a complex signal.
     * 
     * This function calculates the first difference of an input signal, where each element in the result
     * is the difference between consecutive elements of the input signal.
     * 
     * @param input The input signal as an Eigen::VectorXcd (complex vector).
     * @return The first difference of the signal as an Eigen::VectorXcd.
     */
    Eigen::VectorXcd FirstDifference(const Eigen::VectorXcd& input) {
        size_t size = input.size();
        Eigen::VectorXcd output(size);

        if (size <= 0) return output;

        output[0] = input[0];  // El primer elemento se mantiene igual
        for (size_t i = 1; i < size; ++i) {
            output[i] = input[i] - input[i - 1];  // Resta elemento actual con el anterior
        }
        return output;
    }
};

int main() {
    
    std::string server_address("0.0.0.0:5001");
    SignalServiceImpl service;

    // Construcción y configuración del servidor
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    // Iniciar el servidor
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Esperar hasta que el servidor sea detenido
    server->Wait();
    return 0;
}
