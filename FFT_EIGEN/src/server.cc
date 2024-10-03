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

// Implementación del servicio SignalService
class SignalServiceImpl final : public SignalService::Service
{
public:
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
