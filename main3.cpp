#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif

class PasswordBruteForcer {
private:
    volatile bool found;
    volatile long long attempts;
    std::string targetPassword;
    
    const std::string charset = "0123456789";
    
public:
    PasswordBruteForcer(const std::string& target) 
        : found(false), attempts(0), targetPassword(target) {}
    
    // Simple single-threaded brute force
    void simpleBruteForce() {
        std::string current(12, '0');
        auto startTime = std::chrono::steady_clock::now();
        
        std::cout << "Starting brute force..." << std::endl;
        std::cout << "Target: " << targetPassword << std::endl;
        
        while (!found) {
            attempts++;
            
            // Check if current password matches target
            if (current == targetPassword) {
                found = true;
                auto endTime = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
                
                std::cout << "\n\n[SUCCESS] Password found: " << current << std::endl;
                std::cout << "Total attempts: " << attempts << std::endl;
                std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
                break;
            }
            
            // Display progress every 1 million attempts
            if (attempts % 1000000 == 0) {
                auto currentTime = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
                double attemptsPerSecond = (elapsed.count() > 0) ? 
                    static_cast<double>(attempts) / elapsed.count() : 0;
                
                std::cout << "\rAttempts: " << attempts 
                          << " | Current: " << current
                          << " | Speed: " << std::setprecision(2) << attemptsPerSecond << "/sec" << std::flush;
            }
            
            // Increment password (like counting)
            incrementPassword(current);
            
            // Safety check - stop if we've tried all combinations
            if (current == "999999999999") {
                std::cout << "\n[FAILURE] Password not found in search space!" << std::endl;
                break;
            }
        }
    }
    
private:
    void incrementPassword(std::string& password) {
        for (int i = 11; i >= 0; i--) {
            if (password[i] < '9') {
                password[i]++;
                return;
            } else {
                password[i] = '0';
            }
        }
    }
};

// Optimized version for faster searching
void optimizedBruteForce(const std::string& target) {
    std::cout << "Starting optimized brute force..." << std::endl;
    
    char current[13] = "000000000000"; // 12 chars + null terminator
    long long attempts = 0;
    auto start = std::chrono::steady_clock::now();
    
    while (true) {
        attempts++;
        
        // Compare passwords directly
        bool match = true;
        for (int i = 0; i < 12; i++) {
            if (current[i] != target[i]) {
                match = false;
                break;
            }
        }
        
        if (match) {
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            std::cout << "\n\n[SUCCESS] Password found: " << current << std::endl;
            std::cout << "Total attempts: " << attempts << std::endl;
            std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
            break;
        }
        
        // Progress display
        if (attempts % 5000000 == 0) {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - start);
            double speed = static_cast<double>(attempts) / elapsed.count();
            std::cout << "\rAttempts: " << attempts 
                      << " | Current: " << current
                      << " | Speed: " << std::setprecision(2) << speed << "/sec" << std::flush;
        }
        
        // Increment password efficiently
        for (int i = 11; i >= 0; i--) {
            if (current[i] < '9') {
                current[i]++;
                break;
            } else {
                current[i] = '0';
            }
        }
        
        // Check if we've searched everything
        if (attempts >= 1000000000000LL) { // 1 trillion
            std::cout << "\n[INFO] Reached maximum attempts for demo" << std::endl;
            break;
        }
    }
}

// Demo with smaller search space
void demoBruteForce() {
    std::cout << "\n=== DEMO MODE ===" << std::endl;
    std::cout << "Searching for password: 123456789012" << std::endl;
    
    std::string target = "123456789012";
    char current[13] = "000000000000";
    long long attempts = 0;
    auto start = std::chrono::steady_clock::now();
    
    // Search only up to a reasonable limit for demo
    while (attempts < 10000000) { // 10 million max for demo
        attempts++;
        
        // Check if found
        bool match = true;
        for (int i = 0; i < 12; i++) {
            if (current[i] != target[i]) {
                match = false;
                break;
            }
        }
        
        if (match) {
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "\n[SUCCESS] Password found: " << current << std::endl;
            std::cout << "Attempts: " << attempts << std::endl;
            std::cout << "Time: " << duration.count() << " ms" << std::endl;
            return;
        }
        
        // Show progress
        if (attempts % 100000 == 0) {
            std::cout << "\rAttempts: " << attempts << " | Testing: " << current << std::flush;
        }
        
        // Increment
        for (int i = 11; i >= 0; i--) {
            if (current[i] < '9') {
                current[i]++;
                break;
            } else {
                current[i] = '0';
            }
        }
    }
    
    std::cout << "\n[DEMO] Search completed without finding password in demo range" << std::endl;
}

bool isValidPassword(const std::string& password) {
    if (password.length() != 12) return false;
    for (char c : password) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

int main() {
    std::cout << "=== 12-Digit Password Brute Force Demo ===" << std::endl;
    std::cout << "WARNING: For educational purposes only!" << std::endl;
    std::cout << "Use only on systems you own." << std::endl << std::endl;
    
    std::cout << "Choose mode:" << std::endl;
    std::cout << "1. Test with demo password" << std::endl;
    std::cout << "2. Enter custom 12-digit password" << std::endl;
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        demoBruteForce();
    } else if (choice == 2) {
        std::string targetPassword;
        std::cout << "Enter 12-digit password: ";
        std::cin >> targetPassword;
        
        if (!isValidPassword(targetPassword)) {
            std::cout << "Error: Must be exactly 12 digits (0-9)" << std::endl;
            return 1;
        }
        
        std::cout << "\nChoose algorithm:" << std::endl;
        std::cout << "1. Standard brute force" << std::endl;
        std::cout << "2. Optimized brute force" << std::endl;
        std::cout << "Choice: ";
        
        int algo;
        std::cin >> algo;
        
        if (algo == 1) {
            PasswordBruteForcer bruteForcer(targetPassword);
            bruteForcer.simpleBruteForce();
        } else {
            optimizedBruteForce(targetPassword);
        }
    } else {
        std::cout << "Invalid choice!" << std::endl;
        return 1;
    }
    
    return 0;
}