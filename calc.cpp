#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

// Fonksiyon: calculateConvolution
// Açıklama: İki vektör arasında konvolüsyon işlemi gerçekleştiren fonksiyon.
std::vector<int> calculateConvolution(const std::vector<int>& input, const std::vector<int>& kernel) {
    int inputSize = input.size();
    int kernelSize = kernel.size();
    int resultSize = inputSize + kernelSize - 1;

    // Sonuç vektörünü sıfırlarla başlat
    std::vector<int> result(resultSize, 0);

    // Konvolüsyon işlemini gerçekleştir
    for (int n = 0; n < resultSize; ++n) {
        for (int k = 0; k <= n; ++k) {
            if (k < inputSize && (n - k) < kernelSize) {
                result[n] += input[k] * kernel[n - k];
            }
        }
    }

    return result;
}

// Fonksiyon: serialConvolution
// Açıklama: Seri konvolüsyon işlemi gerçekleştiren fonksiyon.
void serialConvolution(int inputSize, int kernelSize) {
    // Giriş ve kernel vektörlerini oluştur
    std::vector<int> input(inputSize);
    std::vector<int> kernel(kernelSize);

    // Rastgele değerlerle giriş ve kernel vektörlerini doldur
    for (int i = 0; i < inputSize; ++i) {
        input[i] = std::rand() % 10;
    }

    for (int i = 0; i < kernelSize; ++i) {
        kernel[i] = std::rand() % 5;
    }

    // Seri konvolüsyon işlemini gerçekleştir
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> result = calculateConvolution(input, kernel);
    auto end = std::chrono::high_resolution_clock::now();

    // Geçen süreyi hesapla ve ekrana yazdır
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Seri Konvolüsyon Süresi: " << duration.count() << " ms" << std::endl;
}

// Fonksiyon: parallelConvolution
// Açıklama: Paralel konvolüsyon işlemi gerçekleştiren fonksiyon.
void parallelConvolution(int inputSize, int kernelSize, int threadCount) {
    // Hatalı thread sayısı kontrolü
    if (threadCount <= 0) {
        std::cerr << "Hatalı thread sayısı." << std::endl;
        return;
    }

    // Giriş ve kernel vektörlerini oluştur
    std::vector<int> input(inputSize);
    std::vector<int> kernel(kernelSize);

    // Rastgele değerlerle giriş ve kernel vektörlerini doldur
    for (int i = 0; i < inputSize; ++i) {
        input[i] = std::rand() % 10;
    }

    for (int i = 0; i < kernelSize; ++i) {
        kernel[i] = std::rand() % 5;
    }

    // Sonuç vektörünü boyutlandır
    int resultSize = inputSize + kernelSize - 1;
    std::vector<int> result(resultSize, 0);

    // Paralel konvolüsyon işlemini gerçekleştir
    auto start = std::chrono::high_resolution_clock::now();

    // Thread'leri oluştur ve her biri için parça işlemi gerçekleştir
    std::vector<std::thread> threads;
    int chunkSize = resultSize / threadCount;

    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([&, i, chunkSize]() {
            int startIdx = i * chunkSize;
            int endIdx = (i == threadCount - 1) ? resultSize : (i + 1) * chunkSize;

            for (int n = startIdx; n < endIdx; ++n) {
                for (int k = 0; k <= n; ++k) {
                    if (k < inputSize && (n - k) < kernelSize) {
                        result[n] += input[k] * kernel[n - k];
                    }
                }
            }
        });
    }

    // Thread'lerin bitmesini bekle
    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Geçen süreyi hesapla ve ekrana yazdır
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Paralel Konvolüsyon Süresi (" << threadCount << " thread): " << duration.count() << " ms" << std::endl;
}

int main() {
    // Rastgele sayı üretebilmek için seed değerini ayarla
    std::srand(std::time(0));

    // Farklı giriş boyutları, kernel boyutu ve thread sayıları
    const std::vector<int> inputSizes = {10, 100, 1000, 10000, 100000};
    const int kernelSize = 10;
    const std::vector<int> threadCounts = {2, 4, 8, 16};

    // Tüm kombinasyonları deneyerek işlemleri gerçekleştir
    for (int inputSize : inputSizes) {
        for (int threadCount : threadCounts) {
            std::cout << "\nInput Boyutu = " << inputSize << ", Kernel Boyutu = " << kernelSize << ", Thread Sayısı = " << threadCount << std::endl;
            std::cout << "-------------------------------------\n";

            // Seri konvolüsyon işlemi
            serialConvolution(inputSize, kernelSize);

            // Paralel konvolüsyon işlemi
            parallelConvolution(inputSize, kernelSize, threadCount);
        }
    }

    std::cout << "İşlemler bitti." << std::endl;

    return 0;
}
