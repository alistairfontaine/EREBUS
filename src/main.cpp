#include "../include/lattice_core.hpp"
#include <iostream>

int main() {
    std::cout << "[EREBUS]: Initializing first-principles homomorphic runtime context..." << std::endl;

    constexpr uint32_t DEGREE = 4;
    constexpr uint64_t MODULUS = 65537;

    // Simulate two distinct ciphertext streams holding different hidden data payloads
    alignas(64) uint64_t cipher_alpha[] = { 0xAF42D, 0x1B89C, 0xC77E1, 0x3D94A };
    alignas(64) uint64_t cipher_beta[]  = { 0x02C13, 0x5E01A, 0x11A0F, 0x4B223 };
    alignas(64) uint64_t cipher_result[DEGREE] = {0};

    std::cout << "[EREBUS]: Computing modular coefficient additions directly in encrypted state..." << std::endl;
    Erebus::LatticeEngine::homomorphic_add(cipher_alpha, cipher_beta, cipher_result, DEGREE, MODULUS);

    std::cout << "[EREBUS]: Invariant Output Verification Vector:" << std::endl;
    for (uint32_t i = 0; i < DEGREE; ++i) {
        std::cout << "  Coeff [" << i << "]: 0x" << std::hex << cipher_result[i] << " (Encrypted Space)" << std::endl;
    }

    Erebus::ErbHeader config_header;
    config_header.magic_bytes = Erebus::LatticeEngine::ERB_MAGIC;
    config_header.polynomial_degree = DEGREE;
    config_header.ciphertext_modulus = MODULUS;
    config_header.reserved_padding = 0x00;

    const char* target_output_file = "snapshot.erb";
    if (Erebus::LatticeEngine::serialize_erb(target_output_file, cipher_result, DEGREE, config_header)) {
        std::cout << std::dec << "[EREBUS]: Homomorphic computation block written to: " << target_output_file << std::endl;
    }

    return 0;
}

