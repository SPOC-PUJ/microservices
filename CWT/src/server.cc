#include <grpcpp/grpcpp.h>
#include "proto/signal_CWT.grpc.pb.h"
#include "Eigen/Dense"
#include "./unsupported/Eigen/FFT"
#include <complex>
#include <vector>
#include <iostream>
#include <execution>
#include <algorithm>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using signal::Complex;
using signal::CWTRequest;
using signal::CWTResponse;
using signal::SignalGrpc;
using signal::SignalService;

class SignalServiceImpl final : public SignalService::Service {
public:
    // Implementación del servicio ComputeCWT
    Status ComputeCWT(ServerContext* context, const CWTRequest* request, CWTResponse* response) override {
        try {
            // Acceder al mensaje SignalGrpc dentro de la solicitud
            const SignalGrpc& grpc_signal = request->signal();
            size_t n = grpc_signal.values_size();
            Eigen::VectorXcd input_signal(n);

            for (size_t i = 0; i < n; ++i) {
                const Complex& complex_msg = grpc_signal.values(i);
                input_signal[i] = std::complex<double>(complex_msg.real(), complex_msg.imag());
            }

            // Generar las escalas logarítmicas
            std::vector<double> scales = GenerateLogScales(request->start(), request->end(), request->numscales());

            // Calcular la Transformada Wavelet Continua
            std::vector<Eigen::VectorXcd> coeffs = CWTEigen(input_signal, scales);

            // Convertir los resultados a CWTResponse
            for (const auto& coeff : coeffs) {
                SignalGrpc* signal_msg = response->add_coeffs();
                for (int i = 0; i < coeff.size(); ++i) {
                    Complex* complex_msg = signal_msg->add_values();
                    complex_msg->set_real(coeff[i].real());
                    complex_msg->set_imag(coeff[i].imag());
                }
            }

            return Status::OK;
        }
        catch (const std::exception& ex) {
            std::cout << "Error: " << ex.what() << std::endl;
            return Status(grpc::INVALID_ARGUMENT, ex.what());
        }
    }

private:
    // Función para generar escalas logarítmicas
    std::vector<double> GenerateLogScales(double start, double end, int numScales) {
        std::vector<double> scales(numScales);
        double logStart = std::log10(start);
        double logEnd = std::log10(end);
        double logStep = (logEnd - logStart) / (numScales - 1);

        for (int i = 0; i < numScales; ++i) {
            scales[i] = std::pow(10, logStart + i * logStep);
        }

        return scales;
    }

    // Función para calcular la CWT utilizando el Wavelet de Morlet
    std::vector<Eigen::VectorXcd> CWTEigen(const Eigen::VectorXcd& signal, const std::vector<double>& scales) {
        int n = signal.size();
        std::vector<Eigen::VectorXcd> coeffs(scales.size());

        std::transform(std::execution::par, scales.begin(), scales.end(), coeffs.begin(), [&](double scale) {
            const auto morlet = MorletWavelet(n, scale);
            return FFTconvolveEigen(signal, morlet, true);
        });

        return coeffs;
    }

    // Función que genera el Wavelet de Morlet
    Eigen::VectorXcd MorletWavelet(int N, double scale, double f0 = 1.0, double fb = 1.0) {
        Eigen::VectorXcd wavelet(N);
        double t;
        double normalization = 1 / sqrt(M_PI * fb);
        for (int n = 0; n < N; ++n) {
            t = (n - N / 2.0) / scale;
            wavelet[n] = normalization * std::exp(-(1 / fb) * t * t) * std::exp(std::complex<double>(0, 2 * M_PI * f0 * t));
        }
        return wavelet;
    }

    // Función que realiza la convolución en el dominio de Fourier
    Eigen::VectorXcd FFTconvolveEigen(const Eigen::VectorXcd& x, const Eigen::VectorXcd& h, bool shift) {
        int N = x.size();

        auto h_padded = ZeroPadGivenSize(h, N);
        Eigen::VectorXcd signalFFT;
        Eigen::VectorXcd Kernel;

        Eigen::FFT<double> ft;

        ft.fwd(signalFFT, x);
        ft.fwd(Kernel, h_padded);
        if(shift){
            // Frequency domain shift to center the wavelet
            Eigen::VectorXcd phaseShift(N);
            for (int k = 0; k < N; ++k) {
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

    // Función para hacer el padding a un vector de tamaño N
    Eigen::VectorXcd ZeroPadGivenSize(const Eigen::VectorXcd &a,int m) {
       int n = a.size();

        // If n is already a power of two, return the original array
        if (m == n) {
            return a;
        }
        // Directly initializing the new VectorXcd with zero-padding
        Eigen::VectorXcd padded_a = Eigen::VectorXcd::Zero(m);

        // Copy the input vector into the beginning of the padded vector
        padded_a.head(n) = a;

        return padded_a;
    }
};

// Configuración del servidor gRPC
void RunServer() {
    std::string server_address("0.0.0.0:5001");
    SignalServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Servidor escuchando en " << server_address << std::endl;
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}
