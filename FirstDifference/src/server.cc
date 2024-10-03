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


class SignalServiceImpl final : public SignalService::Service {
public:
    
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
