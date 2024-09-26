#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include "proto/signal.grpc.pb.h"
#include "Eigen/Dense"
#include <complex>
#include <stdexcept>
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using signal::Complex;
using signal::MovingAverageRequest;
using signal::MovingAverageResponse;
using signal::SignalGrpc;
using signal::SignalService;

class SignalServiceImpl final : public SignalService::Service
{
public:
    Status ComputeMovingAverage(ServerContext *context, const MovingAverageRequest *request,
                                MovingAverageResponse *reply) override
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

            int window_size = request->window_size();
            std::cout << window_size << std::endl;
            Eigen::VectorXcd result = MovingAverage(signal, window_size);

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
    Eigen::VectorXcd MovingAverage(const Eigen::VectorXcd &a, int window_size)
    {
        int n = a.size();
        std::cout << n << std::endl;
        Eigen::VectorXcd result(n);

        if (window_size <= 0 || window_size > n)
        {
            throw std::invalid_argument("Window size must be greater than 0 and less than or equal to the size of the signal.");
        }

        std::complex<double> sum = 0.0;
        for (int i = 0; i < window_size; ++i)
        {
            sum += a[i];
        }
        result[0] = sum / static_cast<double>(window_size);

        for (int i = 1; i <= n - window_size; ++i)
        {
            sum += a[i + window_size - 1];
            sum -= a[i - 1];
            result[i] = sum / static_cast<double>(window_size);
        }

        for (int i = n - window_size + 1; i < n; ++i)
        {
            result[i] = result[i - 1];
        }

        return result;
    }
};

int main()
{
    std::string server_address("0.0.0.0:5000");
    SignalServiceImpl service;

    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
    return 0;
}