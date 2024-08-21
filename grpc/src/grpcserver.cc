#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <complex>
#include <grpcpp/grpcpp.h>
#include "proto/elproto.grpc.pb.h"
#include <Eigen/Dense>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpcserver::MovingAverageRequest;
using grpcserver::MovingAverageResponse;
using grpcserver::SignalProcessingService;

// Define the SignalData class with the MovingAverage function
class SignalData
{
public:
    static Eigen::VectorXcd MovingAverage(const Eigen::VectorXcd &a, int window_size)
    {
        int n = a.size();
        Eigen::VectorXcd result(n);

        if (window_size <= 0 || window_size > n)
        {
            throw std::invalid_argument("El tamaño de la ventana debe ser mayor que 0 y menor o igual al tamaño de la señal.");
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

// Implement the SignalProcessingService service
class SignalProcessingServiceImpl final : public SignalProcessingService::Service
{
    Status ComputeMovingAverage(ServerContext *context, const MovingAverageRequest *request, MovingAverageResponse *reply) override
    {
        // Convert the input to an Eigen::VectorXcd
        std::vector<std::complex<double>> input_complex;
        for (size_t i = 0; i < request->input_size(); i += 2)
        {
            input_complex.emplace_back(request->input(i), request->input(i + 1));
        }
        Eigen::VectorXcd input(input_complex.size());
        for (int i = 0; i < input.size(); ++i)
        {
            input[i] = input_complex[i];
        }

        // Call the MovingAverage function
        Eigen::VectorXcd result = SignalData::MovingAverage(input, request->window_size());

        // Convert the result back to a repeated field of doubles (interleaved real/imaginary parts)
        for (int i = 0; i < result.size(); ++i)
        {
            reply->add_output(result[i].real());
            reply->add_output(result[i].imag());
        }

        return Status::OK;
    }
};

// Run the server
void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    SignalProcessingServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char **argv)
{
    RunServer();

    return 0;
}
