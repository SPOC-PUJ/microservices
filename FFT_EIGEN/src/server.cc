#include <grpcpp/grpcpp.h>
#include "proto/signal_FFT.grpc.pb.h"
#include "./unsupported/Eigen/FFT"
#include <complex>
#include <iostream>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using signal::Complex;
using signal::FFTRequest;
using signal::FFTResponse;
using signal::SignalService;
using signal::SignalGrpc;

/**
 * @defgroup FFT Fourier Transform Microservice
 * @brief This microservice computes the Fast Fourier Transform (FFT) of complex signals.
 * 
 * The FFT microservice is implemented using gRPC and Eigen, handling complex-valued signals
 * and applying the Fast Fourier Transform (FFT) to convert them from the time domain to the frequency domain.
 * 
 * @{
 */

/**
 * @class SignalServiceImpl
 * @brief Implements the SignalService for computing the Fast Fourier Transform (FFT).
 * 
 * This class provides the implementation of the SignalService, specifically the ComputeFFT
 * method, which takes a complex signal as input, applies FFT using the Eigen library, 
 * and returns the result in the form of a complex vector.
 */
class SignalServiceImpl final : public SignalService::Service
{
public:
     /**
     * @brief Computes the FFT of a signal.
     * 
     * This method processes the incoming request by extracting the complex signal from 
     * the gRPC request, converts it to an Eigen vector, applies the Fast Fourier Transform (FFT),
     * and sends the result back in the response.
     * 
     * @param context The gRPC server context.
     * @param request The request containing the input signal.
     * @param reply The response containing the FFT-transformed signal.
     * @return A gRPC Status object indicating success or failure.
     */
    Status ComputeFFT(ServerContext *context, const FFTRequest *request,
                      FFTResponse *reply) override
    {
        try
        {
           const SignalGrpc& grpc_signal = request->signal(); // Ahora accede al nuevo tipo SignalGrpc
            Eigen::VectorXcd signal(grpc_signal.values_size()); // Usa values_size() para el tamaño

            for (int i = 0; i < grpc_signal.values_size(); ++i)
            {
                const Complex &complex_msg = grpc_signal.values(i); // Accede a cada Complex dentro de values
                signal[i] = std::complex<double>(complex_msg.real(), complex_msg.imag());
            }

            // Calcular el IFFT usando Eigen
            Eigen::VectorXcd result = FFTEigen(signal);

            // Convertir el resultado a IFFTResponse
            for (int i = 0; i < result.size(); i++)
            {
                auto complex_number = result[i];
                Complex *complex_message = reply->add_result();
                complex_message->set_real(complex_number.real());
                complex_message->set_imag(complex_number.imag());
            }

            return Status::OK;
        }
        catch (const std::exception &ex)
        {
            return Status(grpc::INVALID_ARGUMENT, ex.what());
        }
    }

private:
      /**
     * @brief Performs the Fast Fourier Transform (FFT) on a complex signal.
     * 
     * This function uses Eigen's FFT functionality to compute the forward FFT of 
     * the input complex signal.
     * 
     * @param a The input signal as an Eigen::VectorXcd (complex vector).
     * @return The FFT-transformed signal as an Eigen::VectorXcd.
     */
    Eigen::VectorXcd FFTEigen(Eigen::VectorXcd &a)
    {

        Eigen::FFT<double> ft;
        Eigen::VectorXcd signalFFT;
        ft.fwd(signalFFT, a);
        return signalFFT;
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
