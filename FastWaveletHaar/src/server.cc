#include <grpcpp/grpcpp.h>
#include "proto/signal_wavelet_harr.grpc.pb.h"
#include "Eigen/Dense"
#include <complex>
#include <vector>
#include <iostream>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using signal::Complex;
using signal::FastWaveletTransformHaarRequest;
using signal::FastWaveletTransformHaarResponse;
using signal::SignalGrpc;
using signal::SignalService;

class SignalServiceImpl final : public SignalService::Service
{
public:
    Status ComputeFastWaveletHaar(ServerContext* context, const FastWaveletTransformHaarRequest* request,
                                           FastWaveletTransformHaarResponse* reply) override
    {
        try
        {
            // Acceder al mensaje SignalGrpc dentro de la solicitud
            const SignalGrpc& grpc_signal = request->signal();
            size_t n = grpc_signal.values_size();  // Obtener el tamaño de la señal
            Eigen::VectorXcd input(n);
            for (size_t i = 0; i < n; i++)
            {
                const Complex& complex_msg = grpc_signal.values(i);
                input[i] = std::complex<double>(complex_msg.real(), complex_msg.imag());
            }

            // Calcular la Transformada Rápida de Wavelet de Haar
            auto [approximation, detail] = FastWaveletTransformHaar(input);

            // Convertir los resultados a FastWaveletTransformHaarResponse
            for (size_t i = 0; i < approximation.size(); i++)
            {
                Complex* approx_msg = reply->add_approximation();
                approx_msg->set_real(approximation[i].real());
                approx_msg->set_imag(approximation[i].imag());

                Complex* detail_msg = reply->add_detail();
                detail_msg->set_real(detail[i].real());
                detail_msg->set_imag(detail[i].imag());
            }

            return Status::OK;
        }
        catch (const std::exception& ex)
        {
            std::cout << "Error"<< std::endl;
            return Status(grpc::INVALID_ARGUMENT, ex.what());
        }
    }

private:
    std::pair<Eigen::VectorXcd, Eigen::VectorXcd> FastWaveletTransformHaar(const Eigen::VectorXcd& input)
    {
        size_t n = input.size();
        Eigen::VectorXcd approximation(n / 2);
        Eigen::VectorXcd detail(n / 2);

        // Filtros de Wavelet de Haar
        std::vector<double> low_pass_filter = {0.5, 0.5};
        std::vector<double> high_pass_filter = {0.5, -0.5};

        for (size_t i = 0; i < n / 2; ++i) {
            approximation[i] = low_pass_filter[0] * input[2 * i] + low_pass_filter[1] * input[2 * i + 1];
            detail[i] = high_pass_filter[0] * input[2 * i] + high_pass_filter[1] * input[2 * i + 1];
        }

        return std::make_pair(approximation, detail);
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
