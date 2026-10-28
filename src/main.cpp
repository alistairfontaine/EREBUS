#include "../include/lattice_core.hpp"
#include <iostream>

int main() {
    std::cout << "[EREBUS]: Initializing first-principles homomorphic runtime context..." << std::endl;

    constexpr uint32_t DEGREE = 4; // N must be a power of 2 for Radix-2 execution
    constexpr uint64_t MODULUS = 17; // Tiny prime for arithmetic verification checking
    constexpr uint64_t ROOT_OF_UNITY = 13; // Primitive 4th root of unity modulo 17

    alignas(64) uint64_t cipher_alpha[] = { 2, 1, 3, 4 };
    alignas(64) uint64_t cipher_beta[]  = { 5, 2, 1, 1 };
    alignas(64) uint64_t cipher_result[DEGREE] = {0};

    std::cout << "[EREBUS]: Computing modular additions directly in encrypted state..." << std::endl;
    Erebus::LatticeEngine::homomorphic_add(cipher_alpha, cipher_beta, cipher_result, DEGREE, MODULUS);

    std::cout << "[EREBUS]: Transforming computation blocks into evaluation space via NTT..." << std::endl;
    Erebus::LatticeEngine::compute_ntt(cipher_result, DEGREE, MODULUS, ROOT_OF_UNITY);

    std::cout << "[EREBUS]: NTT Transformation Target Outputs (Evaluation Space Matrix):" << std::endl;
    for (uint32_t i = 0; i < DEGREE; ++i) {
        std::cout << "  Node [" << i << "]: 0x" << std::hex << cipher_result[i] << std::endl;
    }

    Erebus::ErbHeader config_header;
    config_header.magic_bytes = Erebus::LatticeEngine::ERB_MAGIC;
    config_header.polynomial_degree = DEGREE;
    config_header.ciphertext_modulus = MODULUS;
    config_header.reserved_padding = 0x00;

    if (Erebus::LatticeEngine::serialize_erb("snapshot.erb", cipher_result, DEGREE, config_header)) {
        std::cout << std::dec << "[EREBUS]: Serialized NTT array data safely inside .erb container structure." << std::endl;
    }

    return 0;
}

