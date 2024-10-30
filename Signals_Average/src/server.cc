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


/**
 * @defgroup Signals_Average Signals Average Microservice
 * @brief This microservice computes the average of multiple complex signals.
 * 
 * The Signals_Average microservice is implemented using gRPC and Eigen, allowing it to handle
 * complex-valued signals and compute their averages efficiently. The service accepts input
 * signals as complex vectors and returns the averaged signal.
 * 
 * @{
 */

/**
 * @class SignalServiceImpl
 * @brief Implements the SignalService for computing average signals.
 * 
 * This class provides the implementation of the SignalService, specifically the
 * ComputeAverage method, which takes multiple input signals, computes their average,
 * and returns the result in the form of a complex vector.
 */
class SignalServiceImpl final : public SignalService::Service
{
public:

      /**
     * @brief Computes the average of multiple signals.
     * 
     * This function receives a gRPC request containing multiple signals, processes
     * each signal into an Eigen vector, calculates the average using the SignalsAverge
     * method, and sends the averaged result back in the response.
     * 
     * @param context The gRPC server context.
     * @param request The request containing multiple signals.
     * @param reply The response containing the averaged signal.
     * @return A gRPC Status object indicating success or failure.
     */
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
    /**
     * @brief Computes the average of a vector of Eigen::VectorXcd signals.
     * 
     * This function takes a collection of complex-valued signals (Eigen vectors),
     * ensures they are of the same size, and computes their element-wise average.
     * 
     * @param Input A vector of Eigen::VectorXcd containing the input signals.
     * @return The averaged signal as an Eigen::VectorXcd.
     * 
     * @throws std::runtime_error if the input vectors have different sizes.
     */
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
    builder.SetMaxReceiveMessageSize(100 * 1024 * 1024);  // 20 MB
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
    return 0;
}
