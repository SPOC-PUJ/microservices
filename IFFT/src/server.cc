#include <grpcpp/grpcpp.h>
#include "proto/signal_IFFT.grpc.pb.h"
#include "./unsupported/Eigen/FFT"
#include <complex>
#include <iostream>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using signal::Complex;
using signal::IFFTRequest;
using signal::IFFTResponse;
using signal::SignalService;

// Implementación del servicio SignalService
class SignalServiceImpl final : public SignalService::Service
{
public:
    Status ComputeIFFT(ServerContext* context, const IFFTRequest* request,
                       IFFTResponse* reply) override
    {
        try
        {
            // Convertir la señal de entrada a Eigen::VectorXcd
            Eigen::VectorXcd signal(request->signal_size());
            for (int i = 0; i < request->signal_size(); i++)
            {
                const Complex& complex_msg = request->signal(i);
                signal[i] = std::complex<double>(complex_msg.real(), complex_msg.imag());
            }

            // Calcular el IFFT usando Eigen
            Eigen::VectorXcd result = IFFTEigen(signal);

            // Convertir el resultado a IFFTResponse
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
    // Función para calcular el IFFT usando Eigen FFT
    Eigen::VectorXcd IFFTEigen(Eigen::VectorXcd& a)
    {
        Eigen::FFT<double> ft;
        Eigen::VectorXcd signalFFT;
        ft.inv(signalFFT, a);
        return signalFFT;
    }
};

int main()
{
    std::string server_address("0.0.0.0:5000");
    SignalServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
    return 0;
}