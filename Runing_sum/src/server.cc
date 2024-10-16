#include <grpcpp/grpcpp.h>
#include "proto/signal_runing_sum.grpc.pb.h"
#include "Eigen/Dense"
#include <complex>
#include <iostream>
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using signal::Complex;
using signal::RuningSumRequest;
using signal::RuningSumResponse;
using signal::SignalGrpc;
using signal::SignalService;


/**
 * @defgroup RunningSum Running Sum Microservice
 * @brief This microservice computes the running sum of a complex signal.
 * 
 * The Running Sum microservice is implemented using gRPC and Eigen, processing complex-valued signals
 * and calculating their cumulative sum.
 * 
 * @{
 */

/**
 * @class SignalServiceImpl
 * @brief Implements the SignalService for computing the running sum of complex signals.
 * 
 * This class provides the implementation of the SignalService, specifically the ComputeRuningSum method,
 * which processes complex signals, calculates their running sum, and returns the result.
 */
class SignalServiceImpl final : public SignalService::Service
{
public:
      /**
     * @brief Computes the running sum of a signal.
     * 
     * This method receives the complex signal from the gRPC request, calculates the running sum
     * of the input signal, and sends the result in the response.
     * 
     * @param context The gRPC server context.
     * @param request The request containing the input signal.
     * @param reply The response containing the cumulative sum of the signal.
     * @return A gRPC Status object indicating success or failure.
     */
    Status ComputeRuningSum(ServerContext* context, const RuningSumRequest* request,
                            RuningSumResponse* reply) override
    {
        try
        {
            // Obtener la señal encapsulada en SignalGrpc
            const SignalGrpc& grpc_signal = request->signal(); // Acceder a SignalGrpc
            Eigen::VectorXcd signal(grpc_signal.values_size()); // Usar values_size() para el tamaño

            // Convertir la señal de entrada a Eigen::VectorXcd
            for (int i = 0; i < grpc_signal.values_size(); i++)
            {
                const Complex& complex_msg = grpc_signal.values(i); // Obtener cada valor
                signal[i] = std::complex<double>(complex_msg.real(), complex_msg.imag());
            }

            // Calcular el RuningSum
            Eigen::VectorXcd result = RuningSum(signal);

            // Convertir el resultado a RuningSumResponse
            for (int i = 0; i < result.size(); i++)
            {
                auto complex_number = result[i];
                Complex* complex_message = reply->add_result();
                complex_message->set_real(complex_number.real());
                complex_message->set_imag(complex_number.imag());
            }
            return Status::OK;
        }
        catch (const std::exception& ex)
        {
            return Status(grpc::INVALID_ARGUMENT, ex.what());
        }
    }

private:
      /**
     * @brief Calculates the running sum of a complex signal.
     * 
     * This function computes the cumulative sum of the input complex signal using Eigen's complex vector.
     * 
     * @param Input The input signal as an Eigen::VectorXcd (complex vector).
     * @return The running sum of the signal as an Eigen::VectorXcd.
     */
    Eigen::VectorXcd RuningSum(const Eigen::VectorXcd& Input)
    {
        int Size = Input.size();
        Eigen::VectorXcd Output(Size);

        if (Size <= 0) return Output;

        Output[0] = Input[0];
        for (size_t i = 1; i < Size; ++i) {
            Output[i] = Output[i - 1] + Input[i];
        }
        return Output;
    }
};

int main()
{
    std::string server_address("0.0.0.0:5001");
    SignalServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
    return 0;
}
