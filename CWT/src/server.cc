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
using signal::respcoeffs;

/**
 * \defgroup CWTService Microservicio CWT
 * @{
 * Este grupo contiene la implementación del microservicio para calcular la
 * Transformada Wavelet Continua (CWT) utilizando gRPC.
 */

/**
 * @brief Clase que implementa el servicio de CWT.
 * 
 * Esta clase proporciona el servicio de gRPC para calcular la Transformada
 * Wavelet Continua a partir de una señal de entrada.
 */
class SignalServiceImpl final : public SignalService::Service {
public:
    // Implementación del servicio ComputeCWT
    /**
     * @brief Computes the Continuous Wavelet Transform (CWT) for a given signal.
     *
     * This function receives a signal in the form of complex numbers, computes
     * the CWT using a set of specified scales, and returns the coefficients.
     *
     * @param context Server context for handling requests.
     * @param request The request containing the signal and scale parameters.
     * @param response The response object where the CWT coefficients will be stored.
     * @return gRPC status indicating success or failure of the operation.
     * @ingroup CWT
     */
    Status ComputeCWT(ServerContext* context, const CWTRequest* request, CWTResponse* response) override {
        try {
        // Acceder al mensaje SignalGrpc dentro de la solicitud
        const SignalGrpc& grpc_signal = request->signal();
        //std::cout << grpc_signal.values(0).real() << std::endl;
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
            respcoeffs* coeff_msg = response->add_coeffs();
            for (int i = 0; i < coeff.size(); ++i) {
                // Tomamos la magnitud de cada coeficiente complejo o cualquier otra representación real
                coeff_msg->add_coe(std::abs(coeff[i]));
            }
        }
        
        return Status::OK;
    } catch (const std::exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
        return Status(grpc::INVALID_ARGUMENT, ex.what());
    }
    }

private:
    // Función para generar escalas logarítmicas
    /**
     * @brief Genera escalas logarítmicas.
     * 
     * Esta función genera una serie de escalas logarítmicas entre un
     * rango especificado.
     * 
     * @param start Escala inicial.
     * @param end Escala final.
     * @param numScales Número de escalas a generar.
     * @return Vector de escalas generadas.
     */
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
       /**
     * @brief Calcula la CWT utilizando el Wavelet de Morlet.
     * 
     * Esta función calcula la Transformada Wavelet Continua para la señal
     * proporcionada utilizando un conjunto de escalas.
     * 
     * @param signal La señal de entrada representada como un vector de números complejos.
     * @param scales Un vector de escalas a utilizar para el cálculo de la CWT.
     * @return Un vector de vectores complejos que representan los coeficientes de la CWT.
     */
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
   /**
     * @brief Genera el Wavelet de Morlet.
     * 
     * Esta función genera un vector que representa el Wavelet de Morlet
     * para un tamaño dado y una escala especificada.
     * 
     * @param N Tamaño del wavelet.
     * @param scale Escala del wavelet.
     * @param f0 Frecuencia central (por defecto 1.0).
     * @param fb Ancho de banda (por defecto 1.0).
     * @return Vector de números complejos que representa el Wavelet de Morlet.
     */
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
    /**
     * @brief Realiza la convolución en el dominio de Fourier.
     * 
     * Esta función utiliza la transformada de Fourier para realizar la
     * convolución de dos señales en el dominio de Fourier.
     * 
     * @param x La señal de entrada.
     * @param h El kernel de convolución.
     * @param shift Indica si se debe realizar un desplazamiento en el dominio de la frecuencia.
     * @return Resultado de la convolución.
     */
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
      /**
     * @brief Zero-pads a vector to a specified size.
     * 
     * Esta función rellena el vector de entrada con ceros para asegurar que
     * su tamaño coincida con el tamaño objetivo especificado. Si el vector
     * de entrada ya tiene el tamaño objetivo, se devuelve sin cambios.
     * 
     * @param a El vector de entrada a rellenar.
     * @param m El tamaño objetivo para el vector rellenado.
     * @return Un vector relleno con ceros de tamaño m.
     */
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
