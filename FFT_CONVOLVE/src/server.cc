#include <grpcpp/grpcpp.h>
#include "proto/signal_FFT_convolve.grpc.pb.h"

#include "unsupported/Eigen/FFT"
#include <complex>
#include <stdexcept>
#include <vector>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using signal::Complex;
using signal::FftConvolveRequest;
using signal::FftConvolveResponse;
using signal::SignalGrpc;
using signal::SignalService;

class SignalServiceImpl final : public SignalService::Service
{
public:
    Status ComputeFftConvolve(ServerContext *context, const FftConvolveRequest *request,
                              FftConvolveResponse *reply) override
    {
        try
        {
            // Convertir los datos de la solicitud a Eigen::VectorXcd
            Eigen::VectorXcd signalx(request->signalx().values_size());
            Eigen::VectorXcd signalh(request->signalh().values_size());

            for (int i = 0; i < request->signalx().values_size(); ++i)
            {
                const Complex &complex_msg = request->signalx().values(i);
                signalx[i] = std::complex<double>(complex_msg.real(), complex_msg.imag());
            }

            for (int i = 0; i < request->signalh().values_size(); ++i)
            {
                const Complex &complex_msg = request->signalh().values(i);
                signalh[i] = std::complex<double>(complex_msg.real(), complex_msg.imag());
            }

            bool shift = request->shift();

            // Realizar la convolución FFT usando Eigen
            Eigen::VectorXcd result = FFTconvolveEigen(signalx, signalh, shift);

            // Llenar la respuesta
            for (int i = 0; i < result.size(); ++i)
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
    Eigen::VectorXcd FFTconvolveEigen(const Eigen::VectorXcd &x, const Eigen::VectorXcd &h, bool shift)
    {
        int N = x.size();

        auto h_padded = ZeroPadGivenSize(h, N);
        Eigen::VectorXcd signalFFT;
        Eigen::VectorXcd Kernel;

        Eigen::FFT<double> ft;

        ft.fwd(signalFFT, x);
        ft.fwd(Kernel, h_padded);

        if (shift)
        {
            Eigen::VectorXcd phaseShift(N);
            for (int k = 0; k < N; ++k)
            {
                double freq = 2.0 * M_PI * k / N;
                phaseShift[k] = std::exp(std::complex<double>(0, -freq * (static_cast<double>(N) / 2)));
            }
            Kernel = Kernel.cwiseProduct(phaseShift);
        }

        Eigen::VectorXcd convFFT = signalFFT.cwiseProduct(Kernel);
        Eigen::VectorXcd convolutionResult;
        ft.inv(convolutionResult, convFFT);

        return convolutionResult.head(N);
    }

    Eigen::VectorXcd ZeroPadGivenSize(const Eigen::VectorXcd &h, int size)
    {
        Eigen::VectorXcd h_padded(size);
        h_padded.setZero();
        int h_size = h.size();
        h_padded.head(h_size) = h;
        return h_padded;
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