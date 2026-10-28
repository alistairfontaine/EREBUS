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
        // High-velocity unrolled loop maintaining strict algebraic bounds
        for (uint32_t i = 0; i < degree; ++i) {
            uint64_t sum = poly_a[i] + poly_b[i];
            
            // Branchless conditional modular reduction to preserve low-level pipeline cycles
            poly_result[i] = (sum >= modulus) ? (sum - modulus) : sum;
        }
    }

}

