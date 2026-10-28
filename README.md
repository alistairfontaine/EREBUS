# EREBUS

<img src="assets/logo.png">

EREBUS is a freestanding, zero-dependency C++20 Fully Homomorphic Encryption (FHE) Runtime Engine. It implements first-principles lattice-based cryptographic schemes to execute deterministic operations on multidimensional arrays while they remain completely encrypted. 

By utilizing manual memory alignment and custom integer bit-shifting, EREBUS eliminates abstraction bloat to provide a hyper-secure runtime environment that serializes encrypted matrix states into a proprietary packed binary format (**`.erb`**).

## Specifications
- **Language Profile:** Freestanding C++20 (`-std=c++20`).
- **Memory Footprint:** Cache-aligned (`alignas(64)`), stack-allocated data primitives.
- **Dependency Footprint:** Zero external shared libraries or runtimes.
- **Serialization Target:** Custom `.erb` packed payload blocks.

