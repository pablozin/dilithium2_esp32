# dilithium2_esp32
**A performance evaluation of the Dilithium-2 post-quantum signature scheme on ESP32-WROOM-32 microcontrollers.**
---
## Overview
This repository contains an implementation of the **Dilithium-2** post-quantum cryptographic algorithm, optimized for the ESP32-WROOM-32 microcontroller using the ESP-IDF framework. The codebase is derived directly from the reference implementation provided by the [pq-crystals/dilithium](https://github.com/pq-crystals/dilithium) project, with adaptations to ensure compatibility and performance on resource-constrained IoT devices. In the project, I've used a ESP-WROOM-32.
---
## Features  
- **Performance Testing**:  
  - Iterative measurement of CPU cycles for:  
    - `crypto_sign_keypair` (Key Generation)  
    - `crypto_sign` (Signing)  
    - `crypto_sign_open` (Verification)  
  - Statistical analysis across multiple test iterations (default: `NTESTS = 10`).
---
- **Parameter Compliance**:  
  - Adheres to NIST Round 3 specifications:  
    - `CRYPTO_PUBLICKEYBYTES = 1312`  
    - `CRYPTO_SECRETKEYBYTES = 2560`  
    - `CRYPTO_BYTES = 2420`
---
- **Perfomance**:
| Operation      | Average Cycles | Time @ 160 MHz |
|----------------|----------------|----------------|
| Key Generation | 3,847,692      | 24.05 ms       |
| Signing        | 6,420,695      | 40.13 ms       |
| Verification   | 4,190,801      | 26.19 ms       |

This project is *entirely dependent* on the [pq-crystals/dilithium](https://github.com/pq-crystals/dilithium)
