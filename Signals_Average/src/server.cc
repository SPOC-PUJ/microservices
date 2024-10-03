#include <grpcpp/grpcpp.h>
#include "proto/signal_average.grpc.pb.h"
#include "Eigen/Dense"
#include <complex>
#include <iostream>
#include <vector>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using signal::AverageRequest;
using signal::AverageResponse;
using signal::Complex;
using signal::SignalService;

class SignalServiceImpl final : public SignalService::Service
{
public:
    Status ComputeAverage(ServerContext *context, const AverageRequest *request,
                          AverageResponse *reply) override
    {
        try
        {
            // Convertir la entrada a un vector de Eigen::VectorXcd
            std::vector<Eigen::VectorXcd> inputSignals;
            for (int i = 0; i < request->signals_size(); i++)
            {
                const auto &signalMsg = request->signals(i);
                Eigen::VectorXcd signal(signalMsg.values_size());
                for (int j = 0; j < signalMsg.values_size(); j++)
                {
                    const Complex &complex_msg = signalMsg.values(j);
                    signal[j] = std::complex<double>(complex_msg.real(), complex_msg.imag());
                }
                inputSignals.push_back(signal);
            }

            // Calcular el promedio de las señales
            Eigen::VectorXcd averageSignal = SignalsAverge(inputSignals);

            // Convertir el resultado a AverageResponse
            for (int i = 0; i < averageSignal.size(); i++)
            {
                auto complex_number = averageSignal[i];
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
    // La función SignalsAverge que ya tienes
    Eigen::VectorXcd SignalsAverge(const std::vector<Eigen::VectorXcd> &Input)
    {
        int numVectors = Input.size();
        if (numVectors == 0)
        {
            return Eigen::VectorXcd(); // Retornar vector vacío si la entrada está vacía
        }

        int size = Input[0].size();

        // Asegurarse que todos los vectores tengan el mismo tamaño
        for (const auto &vec : Input)
        {
            if (vec.size() != size)
            {
                throw std::runtime_error("All input vectors must have the same size.");
            }
        }

        // Inicializar el vector promedio
        Eigen::VectorXcd meanVector = Eigen::VectorXcd::Zero(size);

        // Acumular la suma de todos los vectores
        for (const auto &vec : Input)
        {
            meanVector += vec;
        }

        // Calcular el promedio
        meanVector /= static_cast<double>(numVectors);

        return meanVector;
    }
};

int main()
{
    std::string server_address("0.0.0.0:5001");
    SignalServiceImpl service;

    ServerBuilder builder;
    builder.SetMaxReceiveMessageSize(20 * 1024 * 1024);  // 20 MB
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
    return 0;
}
