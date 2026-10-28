#ifndef LATTICE_CORE_HPP
#define LATTICE_CORE_HPP

#include <cstdint>
#include <cstddef>

namespace Erebus {

    struct alignas(64) ErbHeader {
        uint32_t magic_bytes;       
        uint32_t polynomial_degree; 
        uint32_t ciphertext_modulus;
        uint32_t reserved_padding;  
    };

    class alignas(64) LatticeEngine {
    public:
        static constexpr uint32_t ERB_MAGIC = 0x21425245; 

        static bool serialize_erb(const char* filepath, const uint64_t* ciphertext, size_t element_count, const ErbHeader& header);
        static bool deserialize_erb(const char* filepath, uint64_t* output_ciphertext, size_t max_elements, ErbHeader& out_header);
        static void homomorphic_add(const uint64_t* poly_a, const uint64_t* poly_b, uint64_t* poly_result, uint32_t degree, uint64_t modulus);

        // Phase 3: Freestanding Number Theoretic Transform (NTT) for O(N log N) Polynomial Multiplication
        static void bit_reverse_reorder(uint64_t* data, uint32_t n);
        static void compute_ntt(uint64_t* poly, uint32_t n, uint64_t modulus, uint64_t root_of_unity);
    };

}

#endif

