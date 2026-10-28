#include "../include/lattice_core.hpp"
#include <fstream>

namespace Erebus {

    bool LatticeEngine::serialize_erb(const char* filepath, const uint64_t* ciphertext, size_t element_count, const ErbHeader& header) {
        std::ofstream file(filepath, std::ios::binary | std::ios::out);
        if (!file.is_open()) return false;
        file.write(reinterpret_cast<const char*>(&header), sizeof(ErbHeader));
        file.write(reinterpret_cast<const char*>(ciphertext), element_count * sizeof(uint64_t));
        file.close();
        return true;
    }

    bool LatticeEngine::deserialize_erb(const char* filepath, uint64_t* output_ciphertext, size_t max_elements, ErbHeader& out_header) {
        std::ifstream file(filepath, std::ios::binary | std::ios::in);
        if (!file.is_open()) return false;
        file.read(reinterpret_cast<char*>(&out_header), sizeof(ErbHeader));
        if (out_header.magic_bytes != ERB_MAGIC) {
            file.close();
            return false;
        }
        if (out_header.polynomial_degree > max_elements) {
            file.close();
            return false;
        }
        file.read(reinterpret_cast<char*>(output_ciphertext), out_header.polynomial_degree * sizeof(uint64_t));
        file.close();
        return true;
    }

    void LatticeEngine::homomorphic_add(const uint64_t* poly_a, const uint64_t* poly_b, uint64_t* poly_result, uint32_t degree, uint64_t modulus) {
        for (uint32_t i = 0; i < degree; ++i) {
            uint64_t sum = poly_a[i] + poly_b[i];
            poly_result[i] = (sum >= modulus) ? (sum - modulus) : sum;
        }
    }

    void LatticeEngine::bit_reverse_reorder(uint64_t* data, uint32_t n) {
        uint32_t j = 0;
        for (uint32_t i = 0; i < n; ++i) {
            if (i < j) {
                uint64_t temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
            uint32_t bit = n >> 1;
            while (j & bit) {
                j ^= bit;
                bit >>= 1;
            }
            j ^= bit;
        }
    }

    void LatticeEngine::compute_ntt(uint64_t* poly, uint32_t n, uint64_t modulus, uint64_t root_of_unity) {
        // Step 1: Execute fast cache-friendly bit-reversal sorting index step
        bit_reverse_reorder(poly, n);

        // Step 2: Iterative Cooley-Tukey Radix-2 Integer Butterfly Loops
        for (uint32_t len = 2; len <= n; len <<= 1) {
            // Calculate current layer root step parameters
            uint64_t wlen = root_of_unity;
            for (uint32_t i = n; i > len; i >>= 1) {
                wlen = (wlen * wlen) % modulus;
            }

            for (uint32_t i = 0; i < n; i += len) {
                uint64_t w = 1;
                uint32_t half_len = len >> 1;
                for (uint32_t j = 0; j < half_len; ++j) {
                    uint64_t u = poly[i + j];
                    uint64_t v = (poly[i + j + half_len] * w) % modulus;

                    // Compute clean, fast conditional reductions to avoid CPU branching stalls
                    poly[i + j] = (u + v >= modulus) ? (u + v - modulus) : (u + v);
                    poly[i + j + half_len] = (u < v) ? (u + modulus - v) : (u - v);

                    w = (w * wlen) % modulus;
                }
            }
        }
    }

}

