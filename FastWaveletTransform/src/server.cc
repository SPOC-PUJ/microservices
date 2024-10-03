#include <grpcpp/grpcpp.h>
#include "proto/signal_fast_wavelet_transform.grpc.pb.h"
#include "./Eigen/Dense"
#include "./unsupported/Eigen/FFT"
#include <vector>
#include <string>
#include <stdexcept>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using signal::Complex;
using signal::VectorXcd;
using signal::FastWaveletTransformRequest;
using signal::FastWaveletTransformResponse;
using signal::SignalGrpc;
using signal::SignalService;


const double PI = acos(-1);
    // db1 :
    const Eigen::VectorXcd Db1DecLow {{std::complex<double>(0.7071067812, 0.0), std::complex<double>(0.7071067812, 0.0)}};
    
    const Eigen::VectorXcd Db1DecHigh   {{std::complex<double>(-0.7071067812, 0.0), std::complex<double>(0.7071067812, 0.0)}};

    // db2 :
    const Eigen::VectorXcd Db2DecLow  {{std::complex<double>(-0.1294095226, 0.0), std::complex<double>(0.2241438680, 0.0),std::complex<double>(0.8365163037, 0.0), std::complex<double>(0.4829629131, 0.0)}};

    const Eigen::VectorXcd Db2DecHigh {{std::complex<double>(-0.4829629131, 0.0), std::complex<double>(0.8365163037, 0.0),std::complex<double>(-0.2241438680, 0.0), std::complex<double>(-0.1294095226, 0.0)}};


    // db3 :
    const Eigen::VectorXcd Db3DecLow  {{ std::complex<double>(0.0352262919, 0.0), std::complex<double>(-0.0854412739, 0.0),std::complex<double>(-0.1350110200, 0.0), std::complex<double>(0.4598775021, 0.0),std::complex<double>(0.8068915093, 0.0), std::complex<double>(0.3326705530, 0.0)}};

    const Eigen::VectorXcd Db3DecHigh  {{ std::complex<double>(-0.3326705530, 0.0), std::complex<double>(0.8068915093, 0.0),std::complex<double>(-0.4598775021, 0.0), std::complex<double>(-0.1350110200, 0.0),std::complex<double>(0.0854412739, 0.0), std::complex<double>(0.0352262919, 0.0)}};

    // db4 :
    const Eigen::VectorXcd Db4DecLow  {{std::complex<double>(-0.0105974018, 0.0), std::complex<double>(0.0328830117, 0.0),std::complex<double>(0.0308413818, 0.0), std::complex<double>(-0.1870348117, 0.0),std::complex<double>(-0.0279837694, 0.0), std::complex<double>(0.6308807679, 0.0),std::complex<double>(0.7148465706, 0.0), std::complex<double>(0.2303778133, 0.0)}};

    const Eigen::VectorXcd Db4DecHigh  {{std::complex<double>(-0.2303778133, 0.0), std::complex<double>(0.7148465706, 0.0),std::complex<double>(-0.6308807679, 0.0), std::complex<double>(-0.0279837694, 0.0),std::complex<double>(0.1870348117, 0.0), std::complex<double>(0.0308413818, 0.0),std::complex<double>(-0.0328830117, 0.0), std::complex<double>(-0.0105974018, 0.0)}};

    // bior 3.1
    const Eigen::VectorXcd Bio31DecLow  {{std::complex<double>(-0.3535533905932738, 0.0), std::complex<double>(1.0606601717798212, 0.0),std::complex<double>(1.0606601717798212, 0.0), std::complex<double>(-0.3535533905932738, 0.0)}};

    const Eigen::VectorXcd Bio31DecHigh  {{std::complex<double>(-0.1767766952966369, 0.0), std::complex<double>(0.5303300858899106, 0.0),std::complex<double>(-0.5303300858899106, 0.0), std::complex<double>(0.1767766952966369, 0.0)}};

class SignalServiceImpl final : public SignalService::Service {
public:
    Status ComputeFastWaveletTransform(ServerContext* context, const FastWaveletTransformRequest* request,
                                        FastWaveletTransformResponse* reply) override {
        try {
            const SignalGrpc& grpc_signal = request->signal();
            size_t n = grpc_signal.values_size();  // Obtener el tamaño de la señal
            // Convertir la señal de entrada a Eigen::VectorXcd
            Eigen::VectorXcd signal(n);
            for (int i = 0; i < n; i++) {
                const Complex& complex_msg = grpc_signal.values(i);
                signal[i] = std::complex<double>(complex_msg.real(), complex_msg.imag());
            }

            // Obtener el nivel de descomposición y el nombre del wavelet
            int dec_level = request->dec_level();
            std::string wave_name = request->wave_name();
            std::cout<< "level:" << dec_level << std::endl;
            std::cout<< "nombre:" << wave_name << std::endl;
            // Calcular la Transformada de Wavelet
            auto [approximations, details] = FastWaveletTransform(signal, dec_level, wave_name);
            
            // Convertir el resultado a FastWaveletTransformResponse
            for (const auto& approx : approximations) {
                auto* vec = reply->add_approximations();
                for (const auto& value : approx) {
                    auto* complex_msg = vec->add_values();
                    complex_msg->set_real(value.real());
                    complex_msg->set_imag(value.imag());
                }
            }
            
            for (const auto& detail : details) {
                auto* vec = reply->add_details();
                for (const auto& value : detail) {
                    auto* complex_msg = vec->add_values();
                    complex_msg->set_real(value.real());
                    complex_msg->set_imag(value.imag());
                }
            }

            return Status::OK;
        } catch (const std::exception& ex) {
            return Status(grpc::INVALID_ARGUMENT, ex.what());
        }
    }

private:
        
        std::pair< std::vector< Eigen::VectorXcd >, std::vector< Eigen::VectorXcd > > FastWaveletTransform(const Eigen::VectorXcd& input,int DecLevel,std::string WaveName){

        std::vector<Eigen::VectorXcd>Aproximations;
        std::vector<Eigen::VectorXcd>Details;
        const auto getWaveletFilters = [](const std::string& waveName) -> std::pair<Eigen::VectorXcd, Eigen::VectorXcd> {
            if (waveName == "db1") {
                return std::make_pair(Db1DecLow, Db1DecHigh);
            } else if (waveName == "db2") {
                return std::make_pair(Db2DecLow, Db2DecHigh);
            } else if (waveName == "db3") {
                return std::make_pair(Db3DecLow, Db3DecHigh);
            } else if (waveName == "db4") {
                return std::make_pair(Db4DecLow, Db4DecHigh);
            } else if (waveName == "bior3.1") {
                return std::make_pair(Bio31DecLow, Bio31DecHigh);
            } else {
                throw std::invalid_argument("Unknown wavelet name");
            }
        };

        auto [Lo_d, Hi_d] = getWaveletFilters(WaveName);

        
        auto [aprox ,detail ] = FastWaveletTransformAux(input,Lo_d,Hi_d );
        
        Aproximations.push_back(aprox);
        Details.push_back(detail);
        if(DecLevel > 1){
            
        for( int i = 1 ; i < DecLevel ; i++ ){

            auto descompocision = FastWaveletTransformAux(aprox,Lo_d,Hi_d );
            aprox = descompocision.first;
            detail = descompocision.second;
            Aproximations.push_back(aprox);
            Details.push_back(detail);
        }


        }
        
        return std::make_pair(Aproximations, Details);
    }

    std::pair<Eigen::VectorXcd,Eigen::VectorXcd> FastWaveletTransformAux(const Eigen::VectorXcd& input,Eigen::VectorXcd Lo_d , Eigen::VectorXcd Hi_d){

    // conseguir filtros segun el nombre
    // Tipos de filtros

    // conseguir aproximation FFTConvolve con low pass 
    
        auto aprox = FFTconvolveEigen(input, Lo_d );
        
    // conseguir detail FFTConvolve con High pass
    
        auto detail = FFTconvolveEigen(input, Hi_d );
        
    // hacer down sample de aproximation y de detail
        int n = aprox.size();
        int m = detail.size();
        auto aproxi = aprox(Eigen::seq(0,n-1,2));
        auto detaili = detail(Eigen::seq(0,m-1,2));

        // llamar la descompocision denuevo hata que se acben los niveles

        return std::make_pair(aproxi, detaili);
    }

    Eigen::VectorXcd FFTconvolveEigen(const Eigen::VectorXcd& x, const Eigen::VectorXcd& h, bool shift = false) {
        int N = x.size();
        auto h_padded = ZeroPadGivenSize(h, N);
        Eigen::VectorXcd signalFFT;
        Eigen::VectorXcd Kernel;

        Eigen::FFT<double> ft;
        ft.fwd(signalFFT, x);
        ft.fwd(Kernel, h_padded);

        if (shift) {
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
        return convolutionResult.head(x.size());
    }


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

int main() {
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
